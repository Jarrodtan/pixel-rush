/******************************************************************************/
/*!
	Include Files and static variables
*/
/******************************************************************************/
#include "main.h"
#include <random>


namespace {
	s32 i_PlayerLives;
	s32 i_PlayerInitialX;
	s32 i_PlayerInitialY;
	s32 i_TotalCoins;

	// List of original objects
	Objects::GameObject* p_GameObjectList = nullptr;
	u32 u_GameObjectNum;

	// List of object instances
	Objects::GameObjectInstance* p_GameObjectInstanceList = nullptr;
	u32 u_GameObjectInstanceNum;

	// Binary map data
	s32** p_MapData;
	s32** p_BinaryCollisionArray;
	s32 i_BINARY_MAP_WIDTH;
	s32 i_BINARY_MAP_HEIGHT;

	Objects::GameObjectInstance* p_BlackCell;
	Objects::GameObjectInstance* p_WhiteCell;
	AEMtx33 MapTransform;

	// Pointer to player object
	Objects::GameObjectInstance* p_Player;
	s32 i_Jump;

	//CAMERA VARIABLES
	s32 deltaWheel{};
	s32 currentDeltaWheel{};
	Camera camera;

	//POWERUP VARIABLES - TODO, PUT IN AGGREGATE DATA TYPE
	int numOfPowerUps{};
	u32 powerUpFlag{};
	Objects::GameObject* p_Effect;

}

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel1PlatformLoad(void) {
	// allocate memory for the object type and mesh
	::p_GameObjectList = (Objects::GameObject*)calloc(GAME_OBJECT_NUM_MAX, sizeof(Objects::GameObject));
	// allocate memory for the object instance
	::p_GameObjectInstanceList = (Objects::GameObjectInstance*)calloc(GAME_OBJECT_INST_NUM_MAX, sizeof(Objects::GameObjectInstance));

	AE_ASSERT_MESG(::p_GameObjectList, "Fail to calloc");
	::u_GameObjectNum = 0;

	numOfPowerUps = 1;
	powerUpFlag = 0;

	Objects::GameObject* p_Object{};

	// create the back cell
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_EMPTY;

	// create the white cell
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_WALL;

	// create the player object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_PLAYER;

	// create the enemy object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_ENEMY;

	// create the coin object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_COIN;

	// create the powerup object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_POWERUP;

	// create the powerup effect
	p_Effect = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Effect->u_Type = Objects::TYPE_OBJECT_POWERUPEFFECT;


	Render::InitRender(::p_GameObjectList, u_GameObjectNum);

	// setting initial binary map values
	::p_MapData = 0;
	::p_BinaryCollisionArray = 0;
	::i_BINARY_MAP_WIDTH = 0;
	::i_BINARY_MAP_HEIGHT = 0;

	std::string s_FilePath = "Assets/Maps/Test1.txt";
	if (!FileReader::ImportMapDataFromFile(s_FilePath, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT, ::p_MapData, ::p_BinaryCollisionArray)) {
		AE_ASSERT_MESG(0, "Failed to import map data");
		GameStateManager::g_GameStateNext = States::GS_QUIT;
		return;
	}

	AEMtx33 Scale{}, Translate{};

	AEMtx33Trans(&Translate, static_cast<f32>(-i_BINARY_MAP_WIDTH / 2.0), static_cast<f32>(-i_BINARY_MAP_HEIGHT / 2.0));
	AEMtx33Scale(&Scale, RECTANGLE_WIDTH, RECTANGLE_HEIGHT);
	AEMtx33Concat(&::MapTransform, &Scale, &Translate);
	camera.changeLevelSettings(static_cast<f32>(i_BINARY_MAP_WIDTH), static_cast<f32>(i_BINARY_MAP_HEIGHT));
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel1PlatformInit(void) {
	AESysSetWindowTitle("Level 1");
	
	::p_Player = 0;
	::p_BlackCell = 0;
	::p_WhiteCell = 0;
	::i_TotalCoins = 0;

	AEVec2 Scale = { 1.0f, 1.0f };
	::p_BlackCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_EMPTY, &Scale, 0, 0, 0.0f, Objects::STATE_NONE); 
	::p_BlackCell->u_Flag ^= FLAG_VISIBLE;
	::p_BlackCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::p_WhiteCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_WALL, &Scale, 0, 0, 0.f, Objects::STATE_NONE);
	::p_WhiteCell->u_Flag ^= FLAG_VISIBLE;
	::p_WhiteCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::i_PlayerLives = PLAYER_LIVES;

	AEVec2 Position{}, Velocity{};

	

	

	for (s32 i{}; i < ::i_BINARY_MAP_WIDTH; ++i) {
		for (s32 j{}; j < ::i_BINARY_MAP_HEIGHT; ++j) {
			if (::p_MapData[i][j] == 0 || ::p_MapData[i][j] == 1) continue;

			AEVec2Set(&Position, static_cast<f32>(i + 0.5f), static_cast<f32>(j + 0.5f));
			AEVec2Set(&Velocity, static_cast<f32>(0), static_cast<f32>(0));

			switch (::p_MapData[i][j]) {
			case Objects::TYPE_OBJECT_PLAYER:
				::p_Player = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_PLAYER, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				::i_PlayerInitialX = i;
				::i_PlayerInitialY = j;
				break;

			case Objects::TYPE_OBJECT_COIN:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_COIN, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				++::i_TotalCoins;
				break;

			case Objects::TYPE_OBJECT_ENEMY:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_ENEMY, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_LEFT);
				break;
			}
		}
	}

	//TODO REFACTOR -- RNG POWERUP SPAWNER-----------------
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randX(1, i_BINARY_MAP_WIDTH-1);
	std::uniform_int_distribution<std::mt19937::result_type> randY(3, i_BINARY_MAP_HEIGHT - 15);

	u32 u_powerUpXLoc = randX(rng);
	u32 u_powerUpYLoc = randY(rng);
	for (int i{}; i < numOfPowerUps; ++i) {
		while (p_MapData[u_powerUpXLoc][u_powerUpYLoc] != Objects::TYPE_OBJECT_EMPTY ||
			(
				(u_powerUpXLoc == i_PlayerInitialX) ? true : ((p_MapData[u_powerUpXLoc][u_powerUpYLoc - 1] == Objects::TYPE_OBJECT_WALL ? 0 : 1))
			)
		) {
			u_powerUpXLoc = randX(rng);
			u_powerUpYLoc = randY(rng);

		}
		AEVec2Set(&Position, static_cast<f32>(u_powerUpXLoc + 0.5f), static_cast<f32>(u_powerUpYLoc + 0.5f));
		gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_POWERUP, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
		u_powerUpXLoc = randX(rng);
		u_powerUpYLoc = randY(rng);
	}
	//------------------------------------------------------

}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel1PlatformUpdate(void) {
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		GameStateManager::g_GameStateNext = States::GS_QUIT;
	}
	else if (AEInputCheckTriggered(AEVK_1)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
	}
	else if (AEInputCheckTriggered(AEVK_3)) {
		GameStateManager::g_GameStateNext = States::GS_WIN;
	}
	else if (AEInputCheckTriggered(AEVK_4)) {
		GameStateManager::g_GameStateNext = States::GS_LOSE;
	}

	AEInputMouseWheelDelta(&currentDeltaWheel);
	if (currentDeltaWheel != deltaWheel) {
		deltaWheel = currentDeltaWheel;
		camera.changeZoomVal(deltaWheel);

	}

	s32 i{}, j{};
	Objects::GameObjectInstance* p_Instance{};

	if (AEInputCheckCurr(AEVK_D)) {
		::p_Player->currVelocity.x = (powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY;
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		::p_Player->currVelocity.x = -((powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY);
	}
	else {
		::p_Player->currVelocity.x = 0;
	}

	if (AEInputCheckTriggered(AEVK_J) && (powerUpFlag & POWER_PICKEDUP) > 0) {
		powerUpFlag ^= POWER_ACTIVE;
		AEGfxVertexList* staging = p_Player->p_GameObject->p_Mesh;
		p_Player->p_GameObject->p_Mesh = p_Effect->p_Mesh;
		p_Effect->p_Mesh = staging;

	}

	if (AEInputCheckTriggered(AEVK_SPACE)) {
		if (p_Player->currVelocity.y == 0) {
			p_Player->currVelocity.y = JUMP_VELOCITY;
		}



		//else if ((p_Player->i_gridCollisionFlag & COLLISION_LEFT || p_Player->i_gridCollisionFlag & COLLISION_RIGHT)) {  
		//	p_Player->currVelocity.y = JUMP_VELOCITY;
		//}
		//else if (p_Player->i_gridCollisionFlag & COLLISION_LEFT) { 
		//	p_Player->currVelocity.y = JUMP_VELOCITY;
		//}
		//else if (p_Player->i_gridCollisionFlag & COLLISION_RIGHT) { 
		//	p_Player->currVelocity.y = JUMP_VELOCITY;
		//}
		//else if (p_Player->i_gridCollisionFlag & COLLISION_LEFT) { 
		//	p_Player->currVelocity.y = JUMP_VELOCITY;
		//}
		//else if (p_Player->i_gridCollisionFlag & COLLISION_RIGHT) { 
		//	p_Player->currVelocity.y = JUMP_VELOCITY;
		//}
	}

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_PLAYER) {
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
		}
		else if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_ENEMY) {
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
			// TODO: Call the enemy state machine

			if (p_Instance->currPosition.x < 10)
			{
				p_Instance->currVelocity.x += 0.01f;
			}
			else
			{
				p_Instance->currVelocity.x -= 0.01f;
			}

		}
	}

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		p_Instance->prevPosition.x = p_Instance->currPosition.x;
		p_Instance->prevPosition.y = p_Instance->currPosition.y;
	}

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		p_Instance->boundingBox.min.x = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x; 
		p_Instance->boundingBox.min.y = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y; 

		p_Instance->boundingBox.max.x = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x; 
		p_Instance->boundingBox.max.y = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y; 
		
		AEVec2 displacement{};
		AEVec2Scale(&displacement, &p_Instance->currVelocity, g_dt);
		AEVec2Add(&p_Instance->currPosition, &p_Instance->currPosition, &displacement);
	}

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		p_Instance->i_gridCollisionFlag = Collision::CheckInstanceBinaryMapCollision(::p_BinaryCollisionArray, p_Instance->currPosition.x, p_Instance->currPosition.y, p_Instance->scale.x, p_Instance->scale.y);

		if (p_Instance->i_gridCollisionFlag & COLLISION_BOTTOM) { 
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;
		}
		if (p_Instance->i_gridCollisionFlag & COLLISION_TOP) {
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;
		}
		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_ENEMY) {
			if (p_Instance->i_gridCollisionFlag & COLLISION_LEFT && p_Instance->currVelocity.x < 0) { 
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			if (p_Instance->i_gridCollisionFlag & COLLISION_RIGHT && p_Instance->currVelocity.x > 0) { 
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
		}
		else {
			if (p_Instance->i_gridCollisionFlag & COLLISION_LEFT) { 
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			if (p_Instance->i_gridCollisionFlag & COLLISION_RIGHT) { 
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
		}
	}

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		f32 f_Time = 0.f;
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0 || (p_Instance->u_Flag & FLAG_VISIBLE) == 0) continue;

		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_COIN:
			if (Collision::CollisionAABB(p_Player->boundingBox, p_Player->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_TotalCoins;
				if (::i_TotalCoins == 0) {
					GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
					// TODO: Go to the next level or something else
				}
			}
			break;

		case Objects::TYPE_OBJECT_ENEMY:
			if (Collision::CollisionAABB(p_Player->boundingBox, p_Player->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_PlayerLives;
				if (::i_PlayerLives == 0) {
					GameStateManager::g_GameStateNext = States::GS_LOSE;
					// TODO: Go to lose state
				}
			}
			break;

		case Objects::TYPE_OBJECT_POWERUP:
			if (Collision::CollisionAABB(p_Player->boundingBox, p_Player->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				powerUpFlag |= POWER_PICKEDUP;
			}
			break;
		}
	}


	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		AEMtx33 Scale{}, Rotate{}, Translate{};
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		AEMtx33Scale(&Scale, p_Instance->scale.x, p_Instance->scale.y);
		AEMtx33Rot(&Rotate, p_Instance->f_currDirection);
		AEMtx33Trans(&Translate, p_Instance->currPosition.x, p_Instance->currPosition.y);
		AEMtx33Concat(&p_Instance->transform, &Rotate, &Scale);
		AEMtx33Concat(&p_Instance->transform, &Translate, &p_Instance->transform);
	}

	//AEGfxSetCamPosition(1, 1);
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevel1PlatformDraw(void) {

	AEMtx33 CellTranslation{}, FinalCellTransform{};

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);

	for (s32 i{}; i < ::i_BINARY_MAP_WIDTH; ++i) {
		for (s32 j{}; j < i_BINARY_MAP_HEIGHT; ++j) {
			AEMtx33Trans(&CellTranslation, static_cast<f32>(i + 0.5f), static_cast<f32>(j + 0.5f));
			AEMtx33Concat(&FinalCellTransform, &::MapTransform, &CellTranslation);
			camera.applyCameraTransform(FinalCellTransform, p_Player->currPosition.x, p_Player->currPosition.y);
			AEGfxSetTransform(FinalCellTransform.m);
			switch (::p_BinaryCollisionArray[i][j]) {
			case Objects::TYPE_OBJECT_EMPTY:
				Render::RenderShapes(::p_BlackCell, FinalCellTransform, 0.f, 0.f, 0.f, 1.f);
				break;

			case Objects::TYPE_OBJECT_WALL:
				Render::RenderShapes(::p_WhiteCell, FinalCellTransform, 1.f, 1.f, 1.f, 1.f);
				break;

			default:
				break;
			}
		}
	}

	for (s32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		Objects::GameObjectInstance* p_Instance = ::p_GameObjectInstanceList + i;

		if (0 == (p_Instance->u_Flag & FLAG_ACTIVE) || 0 == (p_Instance->u_Flag & FLAG_VISIBLE)) {
			continue;
		}

		AEMtx33 FinalTransform{};
		AEMtx33Concat(&FinalTransform, &::MapTransform, &p_Instance->transform);
		camera.applyCameraTransform(FinalTransform, p_Player->currPosition.x, p_Player->currPosition.y);
		AEGfxSetTransform(FinalTransform.m);

		Render::RenderShapes(p_Instance, FinalTransform);
	}

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformFree(void) {
	// kill all objects in the list
	for (u32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		gameObjectInstanceDestroy(::p_GameObjectInstanceList + i);
	}
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformUnload(void) {
	delete[] ::p_GameObjectList;
	delete[] ::p_GameObjectInstanceList;

	FileReader::FreeMapData(::i_BINARY_MAP_WIDTH, ::p_MapData, ::p_BinaryCollisionArray);
}

