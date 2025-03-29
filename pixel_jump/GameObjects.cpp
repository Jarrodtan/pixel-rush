/*******************************************************************/
/*!
	File:		GameObjects.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		Game Object instance creation and pointer functions
				for gameObject use for destroy objects, transform object,
				position of objects. etc.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

namespace Objects {
	/******************************************************************************/
	/*!
		Factory function to create a game object instance

		\param type
			the type of the game object to create an instance
		\param scale
			the scale of the game object instance
		\param position
			the position of the game object instance
		\param velocity
			the velocity of the game object instance
		\param direction
			the direction of the game object instance
		\param startState
			the state of the game object instance

		\return
			the pointer to the game object instance

		\assumptions
			the caller will need to be responsible in making sure that there is a valid array for
			the game object instance. Namely, p_GameObjectInstanceList and p_GameObjectList.
	*/
	/******************************************************************************/
	GameObjectInstance* gameObjectInstanceCreate(GameObject* p_GameObjectList, 
												 GameObjectInstance* p_GameObjectInstanceList, 
												 u32 type,
												 AEVec2* scale,
												 AEVec2* position,
												 AEVec2* velocity,
												 f32 direction, STATE startState, Index idx) {
		AEVec2 zero;
		AEVec2Zero(&zero);

		// loop through the game object instance list to find a free slot
		for (u32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
			GameObjectInstance* p_Instance = p_GameObjectInstanceList + i;

			// check if the current instance is not active
			if (p_Instance->u_Flag == 0) {

				// use it to create the new instance
				p_Instance->p_GameObject = p_GameObjectList + type;
				p_Instance->u_Flag = FLAG_ACTIVE | FLAG_VISIBLE;
				p_Instance->scale = *scale;
				p_Instance->currPosition = position ? *position : zero;
				p_Instance->currVelocity = velocity ? *velocity : zero;
				p_Instance->f_currDirection = direction;
				p_Instance->p_userData = 0;
				p_Instance->i_gridCollisionFlag = 0;
				p_Instance->state = startState;
				p_Instance->innerState = INNER_STATE_ON_UPDATE;
				p_Instance->f_Counter = 0;
				p_Instance->sequenceCollection.sequence = nullptr;
				p_Instance->otherGameObj = nullptr;
				p_Instance->idx = idx;

				// return the new instance
				return p_Instance;
			}
		}
		return 0;
	}

	/******************************************************************************/
	/*!
		Function to destroy a game object instance

		\param p_Instance
			the pointer to the game object instance to destroy

		\return
			none
	*/
	/******************************************************************************/
	void gameObjectInstanceDestroy(GameObjectInstance* p_Instance) {
		if (p_Instance->u_Flag == 0) {
			return;
		}

		p_Instance->u_Flag = 0;
	}

	void MovingPlatformStateMachine(GameObjectInstance* p_Instance) {
		static const f32 INITIAL_Y = p_Instance->currPosition.y;
		static const f32 FINAL_Y = p_Instance->currPosition.y + PLATFORM_MOVE_DISTANCE;

		switch (p_Instance->state) {
		case STATE_GOING_UP:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_UPDATE:
				if (p_Instance->currPosition.y >= FINAL_Y) {
					p_Instance->f_Counter = PLATFORM_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.y = 0;
					Collision::SnapToCell(&p_Instance->currPosition.y);
				}
				else {
					p_Instance->currVelocity.y = PLATFORM_MOVE_SPEED;
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_DOWN;
					p_Instance->innerState = INNER_STATE_ON_UPDATE;
				}
				break;
			}
			break;

		case STATE_GOING_DOWN:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_UPDATE:
				if (p_Instance->currPosition.y <= INITIAL_Y) {
					p_Instance->f_Counter = PLATFORM_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.y = 0;
					Collision::SnapToCell(&p_Instance->currPosition.y);
				}
				else {
					p_Instance->currVelocity.y = -PLATFORM_MOVE_SPEED;
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_UP;
					p_Instance->innerState = INNER_STATE_ON_UPDATE;
				}
				break;
			}
			break;
		}
	}

	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT) {
		switch (p_Instance->state) {
		case STATE_GOING_LEFT:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_ENTER:
				p_Instance->currVelocity.x = -ENEMY_MOVE_VELOCITY;
				p_Instance->innerState = INNER_STATE_ON_UPDATE;
				break;

			case INNER_STATE_ON_UPDATE:
				if ((p_Instance->i_gridCollisionFlag & COLLISION_LEFT) ||
					Collision::GetCellValue(r_BinaryCollisionArray, r_BINARY_MAP_WIDTH, r_BINARY_MAP_HEIGHT,
						static_cast<s32>(p_Instance->currPosition.x - 1 + 0.5), static_cast<s32>(p_Instance->currPosition.y - 1 - 0.5)) == TYPE_OBJECT_EMPTY) {
					p_Instance->f_Counter = ENEMY_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					Collision::SnapToCell(&p_Instance->currPosition.x);
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_RIGHT;
					p_Instance->innerState = INNER_STATE_ON_ENTER;
				}
				break;
			}
			break;

		case STATE_GOING_RIGHT:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_ENTER:
				p_Instance->currVelocity.x = ENEMY_MOVE_VELOCITY;
				p_Instance->innerState = INNER_STATE_ON_UPDATE;
				break;

			case INNER_STATE_ON_UPDATE:
				if ((p_Instance->i_gridCollisionFlag & COLLISION_RIGHT) ||
					Collision::GetCellValue(r_BinaryCollisionArray, r_BINARY_MAP_WIDTH, r_BINARY_MAP_HEIGHT,
						static_cast<s32>(p_Instance->currPosition.x + 1 - 0.5), static_cast<s32>(p_Instance->currPosition.y - 1 - 0.5)) == TYPE_OBJECT_EMPTY) {
					p_Instance->f_Counter = ENEMY_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					Collision::SnapToCell(&p_Instance->currPosition.x);
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_LEFT;
					p_Instance->innerState = INNER_STATE_ON_ENTER;
				}
				break;
			}
			break;
		}
	}

	void doorResolution(GameObjectInstance* p_Instance, GameObjectInstance* p_Player) {
		float f_Time{};
		if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_Player->boundingBox, p_Player->currVelocity, f_Time)) {
			Sequence& sequence = *(p_Instance->sequenceCollection.getSequence(SpecialSequence::OPEN));

			if (!sequence.isFinished) {
				if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_Player->boundingBox, p_Player->currVelocity, f_Time)) {
					p_Player->currVelocity.x = 0.0f;
					Collision::SnapToCell(&p_Player->currPosition.x);
				}
			}
		}
	}

	void doorExecuteSequence(GameObjectInstance* p_Instance, GameObjectInstance* p_Player1, GameObjectInstance* p_Player2) {
		Sequence& sequence = *(p_Instance->sequenceCollection.getSequence(SpecialSequence::OPEN));
		Sequence& released = *(p_Instance->sequenceCollection.getSequence(SpecialSequence::CLOSE));
		
		if (p_Instance->otherGameObj) {
			if (p_Instance->otherGameObj->sequenceCollection.finished) { //CHECK IF PRESSURE PLATE IS FULLY STEPPED ON
				if (sequence.targetValue < p_Instance->currPosition.y) {
					sequence.isRunning = true;
					f32 yVel = g_dt * 5.0f;//HARDCODED VEL;
					p_Instance->currPosition.y -= yVel;

				}
				if (sequence.targetValue >= p_Instance->currPosition.y && sequence.isRunning) {
					sequence.isRunning = false;
					sequence.isFinished = true;
					Collision::SnapToCell(&p_Instance->currPosition.y);
				}
			} else {

				f32 f_Time{};
				if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_Player2->boundingBox, p_Player2->currVelocity, f_Time)) { 
					if (p_Instance->boundingBox.max.y <= p_Player2->boundingBox.min.y + 0.1f && p_Instance->boundingBox.max.x > p_Player2->boundingBox.min.x+0.1f && p_Instance->boundingBox.min.x < p_Player2->boundingBox.max.x + 0.1f) { //CHECK TOP
						return;
					}
				}
				
				// f32 f_Time2{};
				if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_Player1->boundingBox, p_Player1->currVelocity, f_Time)) { 
					if (p_Instance->boundingBox.max.y <= p_Player1->boundingBox.min.y +0.1f && p_Instance->boundingBox.max.x > p_Player1->boundingBox.min.x+0.1f && p_Instance->boundingBox.min.x < p_Player1->boundingBox.max.x + 0.1f) { //CHECK TOP
						return;
					}
				}

				if (p_Instance->currPosition.y < released.targetValue) {
					f32 yVel = g_dt * 4.0f;//HARDCODED VEL;
					sequence.isFinished = false;

					p_Instance->currPosition.y += yVel;
					released.isRunning = true;
				}
				if (p_Instance->currPosition.y >= released.targetValue && released.isRunning) {
					p_Instance->currPosition.y = released.targetValue;

					released.isRunning = false;
					released.isFinished = true;
				}
			}
		}
	}

	void pressurePlateResolution(GameObjectInstance *p_Instance, GameObjectInstance *p_Player, float f_Time, bool interactable) {

		if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_Player->boundingBox, p_Player->currVelocity, f_Time)) {
			Sequence& sequence = *(p_Instance->sequenceCollection.getSequence(SpecialSequence::PRESSURE_PLATE_STEPPED));
			Sequence& released = *(p_Instance->sequenceCollection.getSequence(SpecialSequence::PRESSURE_PLATE_RELEASED));


			if (p_Instance->boundingBox.min.y >= p_Player->boundingBox.max.y) {
				return;
			}


			if (p_Instance->boundingBox.min.x >= p_Player->boundingBox.max.x && p_Player->currVelocity.x > 0.0) { //CHECK LEFT
				p_Player->currPosition.x = p_Player->currVelocity.x * f_Time + p_Player->prevPosition.x; //SNAP PLAYER BACK VIA X AXIS
			} if (p_Instance->boundingBox.max.x <= p_Player->boundingBox.min.x && p_Player->currVelocity.x < 0.0) { //CHECK RIGHT
				p_Player->currPosition.x = p_Player->currVelocity.x * f_Time + p_Player->prevPosition.x; //SNAP PLAYER BACK VIA X AXIS
			}if ((p_Instance->boundingBox.max.y <= p_Player->boundingBox.min.y && p_Instance->boundingBox.max.x > p_Player->boundingBox.min.x && p_Instance->boundingBox.min.x < p_Player->boundingBox.max.x)) { //CHECK TOP
				if (p_Player->currVelocity.y < 0.0f) {
					p_Player->currVelocity.y = 0.0f;
					p_Player->currPosition.y = p_Instance->currPosition.y + p_Instance->scale.y;
				}

				if (interactable) sequence.isRunning = true;

			}

			if (!interactable) {
				if (released.isRunning && p_Instance->currPosition.y < released.targetValue) {
					p_Player->currPosition.y = p_Instance->currPosition.y + p_Instance->scale.y;
				} else if(released.isFinished) {
					p_Player->currPosition.y = p_Instance->currPosition.y + p_Instance->scale.y;
					released.isFinished = false;
				}
				return;
			}
			
			if (sequence.targetValue < p_Instance->currPosition.y && sequence.isRunning && interactable) {
				
				f32 yVel = g_dt * 5.0f; //HARDCODED VEL;
				p_Instance->currPosition.y -= yVel;
				p_Player->currPosition.y = p_Instance->currPosition.y + 0.9f;
				released.isRunning = false;

			}
			if (sequence.targetValue >= p_Instance->currPosition.y && sequence.isRunning && interactable) {
				sequence.isRunning = false;
				p_Instance->sequenceCollection.finished = true;
				p_Instance->currPosition.y = sequence.targetValue;
			}
		}
		else {

			Sequence* sequence = p_Instance->sequenceCollection.getSequence(SpecialSequence::PRESSURE_PLATE_RELEASED);
			
			if (p_Instance->currPosition.y < sequence->targetValue && interactable) {
				f32 yVel = g_dt * 4.0f; //HARDCODED VEL;
				
				p_Instance->currPosition.y += yVel;
				p_Player->currPosition.y += yVel;
				sequence->isRunning = true;
			}
			if (p_Instance->currPosition.y >= sequence->targetValue && interactable && sequence->isRunning) {
					p_Instance->sequenceCollection.finished = false;
					p_Instance->currPosition.y = sequence->targetValue;

					sequence->isRunning = false;
					sequence->isFinished = true;
			}

		}
	}

	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, GameObjectInstance*& r_Instance, f32& f_Time) {
		switch (p_Instance->state) {
		case STATE_GOING_LEFT:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_ENTER:
				p_Instance->currVelocity.x = -ENEMY_MOVE_VELOCITY;
				p_Instance->innerState = INNER_STATE_ON_UPDATE;
				break;

			case INNER_STATE_ON_UPDATE:
				if ((p_Instance->i_gridCollisionFlag & COLLISION_LEFT) ||
					Collision::GetCellValue(r_BinaryCollisionArray, r_BINARY_MAP_WIDTH, r_BINARY_MAP_HEIGHT,
						static_cast<s32>(p_Instance->currPosition.x - 1 + 0.5), static_cast<s32>(p_Instance->currPosition.y - 1 - 0.5)) == TYPE_OBJECT_EMPTY) {
					p_Instance->f_Counter = ENEMY_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					Collision::SnapToCell(&p_Instance->currPosition.x);
				}
				
				if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, r_Instance->boundingBox, r_Instance->currVelocity, f_Time)) {
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					//Collision::SnapToCell(&p_Instance->currPosition.x);
					//Collision::SnapToCell(&r_Instance->currPosition.x);	
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_RIGHT;
					p_Instance->innerState = INNER_STATE_ON_ENTER;
				}
				break;
			}
			break;

		case STATE_GOING_RIGHT:
			switch (p_Instance->innerState) {
			case INNER_STATE_ON_ENTER:
				p_Instance->currVelocity.x = ENEMY_MOVE_VELOCITY;
				p_Instance->innerState = INNER_STATE_ON_UPDATE;
				break;

			case INNER_STATE_ON_UPDATE:
				if ((p_Instance->i_gridCollisionFlag & COLLISION_RIGHT) ||
					Collision::GetCellValue(r_BinaryCollisionArray, r_BINARY_MAP_WIDTH, r_BINARY_MAP_HEIGHT,
						static_cast<s32>(p_Instance->currPosition.x + 1 - 0.5), static_cast<s32>(p_Instance->currPosition.y - 1 - 0.5)) == TYPE_OBJECT_EMPTY) {
					p_Instance->f_Counter = ENEMY_IDLE_TIME;
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					Collision::SnapToCell(&p_Instance->currPosition.x);
				}
				
				if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, r_Instance->boundingBox, r_Instance->currVelocity, f_Time)) {
					p_Instance->innerState = INNER_STATE_ON_EXIT;
					p_Instance->currVelocity.x = 0;
					//Collision::SnapToCell(&p_Instance->currPosition.x);
					//Collision::SnapToCell(&r_Instance->currPosition.x);
				}
				break;

			case INNER_STATE_ON_EXIT:
				p_Instance->f_Counter -= g_dt;
				if (p_Instance->f_Counter < 0) {
					p_Instance->state = STATE_GOING_LEFT;
					p_Instance->innerState = INNER_STATE_ON_ENTER;
				}
				break;
			}
			break;
		}
	}
}