/******************************************************************************/
/*!
	Include Files and external variables
*/
/******************************************************************************/
#include "main.h"

GameObject* p_GameObjectList = nullptr;
GameObjectInstance* p_GameObjectInstanceList = nullptr;

//------------------------------------------------------------------------------

/******************************************************************************/
/*!
	Constructor of the game object instance
*/
/******************************************************************************/
GameObjectInstance::GameObjectInstance() {
	// initialize the game object instance
	p_GameObject = nullptr;
	u_Flag = 0;
	AEVec2Zero(&scale);
	AEVec2Zero(&currPosition);
	AEVec2Zero(&prevPosition);
	AEVec2Zero(&currVelocity);
	f_currDirection = 0.f;
	transform = AEMtx33();
	boundingBox = AABB();
	i_gridCollisionFlag = 0;
	p_userData = nullptr;
	state = STATE_NONE;
	innerState = INNER_STATE_ON_ENTER;
	f_Counter = 0.0;
}

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
GameObjectInstance* GameObjectInstance::gameObjectInstanceCreate(u32 type, AEVec2* scale, AEVec2* position, AEVec2* velocity, f32 direction, STATE startState) {
	AEVec2 zero;
	AEVec2Zero(&zero);

	// loop through the game object instance list to find a free slot
	for (u32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		GameObjectInstance* p_Instance = p_GameObjectInstanceList + i;

		// check if the current instance is not active
		if (p_Instance->u_Flag == 0) {
			// use it to create the new instance
			p_Instance->p_GameObject = p_GameObjectList + type;
			p_Instance->i_gridCollisionFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			p_Instance->scale = *scale;
			p_Instance->currPosition = position ? *position : zero;
			p_Instance->currVelocity = velocity ? *velocity : zero;
			p_Instance->f_currDirection = direction;
			p_Instance->p_userData = 0;
			p_Instance->i_gridCollisionFlag = 0;
			p_Instance->state = startState;
			p_Instance->innerState = INNER_STATE_ON_ENTER;
			p_Instance->f_Counter = 0.0;

			// return the new instance
			return p_Instance;
		}
	}
	return 0;
}
