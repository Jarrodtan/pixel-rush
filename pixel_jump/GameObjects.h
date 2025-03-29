/*******************************************************************/
/*!
	File:		GameObjects.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang		(85% of Code)
				Mira Tristan Dumlao (5% of Code)
				Tan Wei Leong		(5% of Code)
				Ng Wei Zhen			(5% of Code)
	
	Brief:		All Functional Declaration to use for GameObjects.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER

#include "main.h"

namespace Objects {

	/******************************************************************************/
	/*!
		Class/Enum Definitions
	*/
	/******************************************************************************/
	// Object Type
	enum TYPE_OBJECT {
		TYPE_OBJECT_EMPTY,					// used in the map grid to indicate no object
		TYPE_OBJECT_WALL,					// used in the map grid to indicate a wall or floor
		TYPE_OBJECT_LIGHTPLAYER,			// used to indicate the player object
		TYPE_OBJECT_SHADOWPLAYER,			// used to indicate the player object
		TYPE_OBJECT_MOVEABLE_SPIKE,			// used to indicate the enemy object
		TYPE_OBJECT_SPIKE,					// used to indicate the enemy object
		TYPE_OBJECT_LIGHTSHARD,				// used to indicate the collectible object
		TYPE_OBJECT_SHADOWFRAGMENT,			// used to indicate the collectible object
		TYPE_OBJECT_MOVINGPLATFORM,			// used in the map grid to indicate a moving floor
		TYPE_OBJECT_LIGHTSPIKE,				// used to indicate the enemy object
		TYPE_OBJECT_SHADOWSPIKE,			// used to indicate the enemy object
		TYPE_OBJECT_MOVEABLE_LIGHTSPIKE,	// used to indicate the enemy object
		TYPE_OBJECT_MOVEABLE_SHADOWSPIKE,	// used to indicate the enemy object
		TYPE_OBJECT_PRESSURE_PLATE_LIGHT,	// used to indicate the power up/switch
		TYPE_OBJECT_PRESSURE_PLATE_DARK,	// used to indicate the power up/switch
		TYPE_OBJECT_DOOR,					// used to indicate the door object
		TYPE_OBJECT_GATE,					// used to indicate the gate object
		TYPE_OBJECT_SHADOW_DOOR,			// used to indicate the door object
		TYPE_OBJECT_LIGHT_DOOR,				// used to indicate the door object
		TYPE_OBJECT_TELEPORTOR,				// used to indicate the power up/switch
		TYPE_OBJECT_GRAVITYSWITCHER,		// used to indicate the power up/switch
		TYPE_OBJECT_SPIKE_DOWN,				// used to indicate the enemy object
		TYPE_OBJECT_LIGHTSPIKE_DOWN,		// used to indicate the enemy object
		TYPE_OBJECT_SHADOWSPIKE_DOWN,		// used to indicate the enemy object
		TYPE_OBJECT_CHECKPOINT,
		TYPE_MAX_OBJECT
	};

	// State machine states
	enum STATE {
		STATE_NONE,
		STATE_GOING_LEFT,
		STATE_GOING_RIGHT,
		STATE_GOING_UP,
		STATE_GOING_DOWN,

		STATE_MAX
	};

	// Inner state machine states
	enum INNER_STATE {
		INNER_STATE_ON_ENTER,
		INNER_STATE_ON_UPDATE,
		INNER_STATE_ON_EXIT,

		INNER_STATE_MAX
	};

	struct Index {
		s32 x;
		s32 y;

		bool operator==(Index const& secondIdx) {
			return x == secondIdx.x && y == secondIdx.y;

		}
	};

	struct GameObject {
		// Game object's type
		u32 u_Type;

		// Pointer to the mesh that represents the object
		AEGfxVertexList* p_Mesh;
	};

	class GameObjectInstance {
	public:
		// Pointer to the game object that this instance is based on
		GameObject* p_GameObject;

		// Determines if the object is active or not
		u32 u_Flag;
		// Scaling value of the object instance
		AEVec2 scale;
		// Object's current position
		AEVec2 currPosition;
		// Object's previous position
		AEVec2 prevPosition;
		// Object's current velocity
		AEVec2 currVelocity;

		// Object's current direction
		f32 f_currDirection;

		// Object's drawing/transformation matrix
		AEMtx33 transform;
		// Bounding box that encapsulates the object for AABB collision detection
		Collision::AABB boundingBox;

		// Used to hold the collision flag to the map grid
		s32 i_gridCollisionFlag;
		// Pointer to custom data specific for each object type
		void* p_userData;

		// State of the object instance
		enum STATE state;
		enum INNER_STATE innerState;

		// General purpose counter (This variable can be used for the enemy state machine later)
		f64 f_Counter;
		SequenceCollection sequenceCollection;

		// FOR GAMEOBJECTS THAT DEPEND ON EACH OTHER
		GameObjectInstance* otherGameObj;
		Index idx;
	};


	/******************************************************************************/
	/*!
		Non-member functions for GameObjectInstance
	*/
	/******************************************************************************/
	GameObjectInstance* gameObjectInstanceCreate(GameObject* p_GameObjectList, GameObjectInstance* p_GameObjectInstanceList, u32 type, AEVec2* scale, AEVec2* position, AEVec2* velocity, f32 direction, STATE startState, Index idx = {0, 0});
	void gameObjectInstanceDestroy(GameObjectInstance* p_Instance);
	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT);
	void EnemyStateMachine(GameObjectInstance* p_Instance, s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, GameObjectInstance*& r_Instance, f32& f_Time);
	void MovingPlatformStateMachine(GameObjectInstance* p_Instance);
	void pressurePlateResolution(GameObjectInstance* p_Instance, GameObjectInstance* p_Player, float f_Time, bool interactable);
	void doorResolution(GameObjectInstance* p_Instance, GameObjectInstance* p_Player);
	void doorExecuteSequence(GameObjectInstance* p_Instance, GameObjectInstance* p_Player1, GameObjectInstance* p_Player2);
}

#endif // GAMEOBJECT_HEADER