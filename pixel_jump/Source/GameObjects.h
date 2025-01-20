#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "AEEngine.h"
#include "Collision.h"

/******************************************************************************/
/*!
	Class/Enum Definitions
*/
/******************************************************************************/

// Object Type
enum TYPE_OBJECT {
	TYPE_OBJECT_EMPTY,	// used in the map grid to indicate no object
	TYPE_OBJECT_WALL,	// used in the map grid to indicate a wall or floor
	TYPE_OBJECT_PLAYER, // used to indicate the player object
	TYPE_OBJECT_ENEMY,	// used to indicate the enemy object
	TYPE_OBJECT_COIN,	// used to indicate the coin object

	TYPE_MAX_OBJECT
};


// state machine states
enum STATE {
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT,

	STATE_MAX
};

// inner state machine states
enum INNER_STATE {
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT,

	INNER_STATE_MAX
};

class GameObject {
public:
	u32 u_Type;		// object type
	AEGfxVertexList* p_Mesh;	// mesh used to draw the object
};

class GameObjectInstance {
public:
	// pointer to the game object
	GameObject* p_GameObject;

	// determines if the object is active or not
	u32 u_Flag;
	// scaling value of the object instance
	AEVec2 scale;
	// object's current position
	AEVec2 currPosition;
	// object's previous position
	AEVec2 prevPosition;
	// object's current velocity
	AEVec2 currVelocity;

	// object's current direction
	f32 f_currDirection;

	// object's drawing/transformation matrix
	AEMtx33 transform;
	// bounding box that encapsulates the object for AABB collision detection
	AABB boundingBox;

	// used to hold the collision flag to the map grid
	s32 i_gridCollisionFlag;
	// pointer to custom data specific for each object type
	void* p_userData;

	// state of the object instance
	enum STATE state;
	enum INNER_STATE innerState;

	// general purpose counter (This variable can be used for the enemy state machine later)
	f64 f_Counter;
};

#endif // GAMEOBJECT_HEADER