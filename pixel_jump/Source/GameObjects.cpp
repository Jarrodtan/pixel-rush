/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

//------------------------------------------------------------------------------

/******************************************************************************/
/*!
	External Variables
*/
/******************************************************************************/
extern GameObject* p_GameObjectList;
extern GameObjectInstance* p_GameObjectInstanceList;
//------------------------------------------------------------------------------

// constructor for the game object instance class
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

