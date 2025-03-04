#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "AEEngine.h"
#include "Collision.h"
#include "Sequence.h"

namespace Objects {
	/******************************************************************************/
	/*!
		Class/Enum Definitions
	*/
	/******************************************************************************/

	// Object Type
	enum TYPE_OBJECT {
		TYPE_OBJECT_EMPTY,	// used in the map grid to indicate no object
		TYPE_OBJECT_WALL,	// used in the map grid to indicate a wall or floor
		TYPE_OBJECT_LIGHTPLAYER, // used to indicate the player object
		TYPE_OBJECT_SHADOWPLAYER,
		TYPE_OBJECT_MOVEABLE_SPIKE,	// used to indicate the enemy object
		TYPE_OBJECT_SPIKE,	// used to indicate the enemy object
		TYPE_OBJECT_LIGHTSHARD,
		TYPE_OBJECT_SHADOWFRAGMENT,
		TYPE_PRESSURE_PLATE,
		TYPE_OBJECT_MOVINGPLATFORM,
		TYPE_OBJECT_LIGHTSPIKE,
		TYPE_OBJECT_SHADOWSPIKE,
		TYPE_OBJECT_MOVEABLE_LIGHTSPIKE,
		TYPE_OBJECT_MOVEABLE_SHADOWSPIKE,
		TYPE_OBJECT_PRESSURE_PLATE_LIGHT,
		TYPE_OBJECT_PRESSURE_PLATE_DARK,

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

	struct GameObject {
		// game object's type
		u32 u_Type;
		// pointer to the mesh that represents the object
		AEGfxVertexList* p_Mesh;
	};

	class GameObjectInstance {
	public:
		// pointer to the game object that this instance is based on
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
		Collision::AABB boundingBox;

		// used to hold the collision flag to the map grid
		s32 i_gridCollisionFlag;
		// pointer to custom data specific for each object type
		void* p_userData;

		// state of the object instance
		enum STATE state;
		enum INNER_STATE innerState;

		// general purpose counter (This variable can be used for the enemy state machine later)
		f64 f_Counter;
		SequenceCollection sequenceCollection;

	};
	/******************************************************************************/
	/*!
		Non-member functions for GameObjectInstance
	*/
	/******************************************************************************/
	GameObjectInstance* gameObjectInstanceCreate(GameObject* p_GameObjectList, GameObjectInstance* p_GameObjectInstanceList, u32 type, AEVec2* scale, AEVec2* position, AEVec2* velocity, f32 direction, STATE startState);
	void gameObjectInstanceDestroy(GameObjectInstance* p_Instance);
	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT);
	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, GameObjectInstance*& r_Instance, f32& f_Time);
	void PressurePlateResolution(GameObjectInstance* p_Instance, GameObjectInstance* p_Player, float f_Time, bool interactable);
}



#endif // GAMEOBJECT_HEADER