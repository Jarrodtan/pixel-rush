/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

//------------------------------------------------------------------------------

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
												 f32 direction, STATE startState) {
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
				p_Instance->innerState = INNER_STATE_ON_ENTER;
				p_Instance->f_Counter = 0;
				p_Instance->sequenceCollection.sequence = nullptr;
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


