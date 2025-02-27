/******************************************************************************/
/*!
	Include Files and static variables
*/
/******************************************************************************/
#include "main.h"
#include <random>


namespace {
	s32 i_LightPlayerLives;
	s32 i_LightPlayerInitialX;
	s32 i_LightPlayerInitialY;
	s32 i_ShadowPlayerLives;
	s32 i_ShadowPlayerInitialX;
	s32 i_ShadowPlayerInitialY;
	s32 i_TotalLightShard;
	s32 i_TotalShadowFragment;

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
	Objects::GameObjectInstance* p_LightPlayer;
	Objects::GameObjectInstance* p_ShadowPlayer;
	s32 i_Jump;

	//CAMERA VARIABLES
	s32 deltaWheel{};
	s32 currentDeltaWheel{};
	Camera camera;

	//POWERUP VARIABLES - TODO, PUT IN AGGREGATE DATA TYPE
	int numOfPowerUps{};
	u32 powerUpFlag{};
	Objects::GameObject* p_Effect;

	//CAMERA ZOOM IN/OUT - BASED ON DISTANCE BETWEEN PLAYER 1 & PLAYER 2
	f32 zoomOffset;  // Calculate the distance for camera zoom-in and zoom-out, also to define min/max distances for zoom control.
	// Define a constant for inverse scaling
	f32 k = 200.0f;  // Adjust this to control the rate of decrease
	f32 c = 1.0f;    // Small constant to avoid division by zero
	f32 inverseValue; // Value to inverse zoomOffset when it increases, this value decreases
	s32 zoomRangeMultiplier = 32; // To lock the range to zoom in/out
	f32 camMidpoint_X, camMidpoint_Y;
	bool haveCamZoomFeature = true;
	bool dynamicCamFeature = true;

	//DOUBLE JUMP FEATURES
	s32 doubleJumpCounter_S = 0;
	bool haveDoubleJump_S = true;
	s32 doubleJumpCounter_L = 0;
	bool haveDoubleJump_L = true;
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

	// create the black cell
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_EMPTY;

	// create the white cell
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_WALL;

	// create the player object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHTPLAYER;

	// create the player object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOWPLAYER;

	// create the enemy object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_MOVEABLE_SPIKE;

	// create the light collectable object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHTSHARD;

	// create the shadow collectable object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOWFRAGMENT;

	// create the pressure plate
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_PRESSURE_PLATE;

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
	
	::p_LightPlayer = 0;
	::p_ShadowPlayer = 0;
	::p_BlackCell = 0;
	::p_WhiteCell = 0;
	::doubleJumpCounter_S = 0;
	::doubleJumpCounter_L = 0;

	AEVec2 Scale = { 1.0f, 1.0f };
	::p_BlackCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_EMPTY, &Scale, 0, 0, 0.0f, Objects::STATE_NONE); 
	::p_BlackCell->u_Flag ^= FLAG_VISIBLE;
	::p_BlackCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::p_WhiteCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_WALL, &Scale, 0, 0, 0.f, Objects::STATE_NONE);
	::p_WhiteCell->u_Flag ^= FLAG_VISIBLE;
	::p_WhiteCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::i_LightPlayerLives = PLAYER_LIVES;
	::i_ShadowPlayerLives = PLAYER_LIVES;

	AEVec2 Position{}, Velocity{};

	AudioManager::PlayBGM("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav");

	for (s32 i{}; i < ::i_BINARY_MAP_WIDTH; ++i) {
		for (s32 j{}; j < ::i_BINARY_MAP_HEIGHT; ++j) {
			if (::p_MapData[i][j] == 0 || ::p_MapData[i][j] == 1) continue;

			AEVec2Set(&Position, static_cast<f32>(i + 0.5f), static_cast<f32>(j + 0.5f));
			AEVec2Set(&Velocity, static_cast<f32>(0), static_cast<f32>(0));

			switch (::p_MapData[i][j]) {
			case Objects::TYPE_OBJECT_LIGHTPLAYER:
				::p_LightPlayer = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_LIGHTPLAYER, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				::i_LightPlayerInitialX = i;
				::i_LightPlayerInitialY = j;
				break;

			case Objects::TYPE_OBJECT_SHADOWPLAYER:
				::p_ShadowPlayer = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SHADOWPLAYER, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				::i_ShadowPlayerInitialX = i;
				::i_ShadowPlayerInitialY = j;
				break;

			case Objects::TYPE_OBJECT_LIGHTSHARD:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_LIGHTSHARD, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				++::i_TotalLightShard;
				break;

			case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SHADOWFRAGMENT, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				++::i_TotalShadowFragment;
				break;

			case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_MOVEABLE_SPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_LEFT);
				break;
			case Objects::TYPE_PRESSURE_PLATE:
				Objects::GameObjectInstance* p_Instance = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_PRESSURE_PLATE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				int count = p_Instance->sequenceCollection.sequenceCount = 2;

				p_Instance->sequenceCollection.sequence = new Sequence[count];

				p_Instance->sequenceCollection.sequence[0] = Sequence{
					false,
					false,
					Position.y,
					0.0f,
					0.0f,
					SpecialSequence::PRESSURE_PLATE_RELEASED
				};

				p_Instance->sequenceCollection.sequence[1] = Sequence{
					false,
					false,
					Position.y - Scale.y,
					0.0f,
					0.0f,
					SpecialSequence::PRESSURE_PLATE_STEPPED
				};

				break;
			}
		}
	}

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

	/*AEInputMouseWheelDelta(&currentDeltaWheel);
	if (currentDeltaWheel != deltaWheel) {
		deltaWheel = currentDeltaWheel;
		camera.changeZoomVal(deltaWheel);

	}*/

	s32 i{}, j{};
	Objects::GameObjectInstance* p_Instance{};

	if (AEInputCheckCurr(AEVK_D)) {
		::p_LightPlayer->currVelocity.x = (powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY;
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		::p_LightPlayer->currVelocity.x = -((powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY);
	}
	else {
		::p_LightPlayer->currVelocity.x = 0;
	}

	if (AEInputCheckCurr(AEVK_RIGHT)) {
		::p_ShadowPlayer->currVelocity.x = (powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY;
	}
	else if (AEInputCheckCurr(AEVK_LEFT)) {
		::p_ShadowPlayer->currVelocity.x = -((powerUpFlag & POWER_ACTIVE) > 0 ? PLAYER_MOVE_VELOCITY + 4.0f : PLAYER_MOVE_VELOCITY);
	}
	else {
		::p_ShadowPlayer->currVelocity.x = 0;
	}

	//Power up [increase velocity]
	if (AEInputCheckTriggered(AEVK_J) && (powerUpFlag & POWER_PICKEDUP) > 0) {
		powerUpFlag ^= POWER_ACTIVE;
		AEGfxVertexList* staging = p_LightPlayer->p_GameObject->p_Mesh;
		p_LightPlayer->p_GameObject->p_Mesh = p_Effect->p_Mesh;
		p_Effect->p_Mesh = staging;

	}

	// basic jump
	/*if (AEInputCheckTriggered(AEVK_W)) {
		if (p_LightPlayer->currVelocity.y == 0) {
			p_LightPlayer->currVelocity.y = JUMP_VELOCITY;
		}
	}*/

	
	if (AEInputCheckTriggered(AEVK_W)) {

		// double jump
		if (haveDoubleJump_L) {
			::doubleJumpCounter_L++;

			if (p_LightPlayer->i_gridCollisionFlag && ::doubleJumpCounter_L >= 2)
			{
				::doubleJumpCounter_L = 0;
			}

			if (::doubleJumpCounter_L < 2) {
				p_LightPlayer->currVelocity.y = JUMP_VELOCITY;
				// Sound effect
				AudioManager::PlayAudio("Assets/AudioSFX/Jump_retro_up.wav", AudioManager::f_JumpVolume);
			}

		}
		else { // basic jump
			if (p_LightPlayer->currVelocity.y == 0) {
				p_LightPlayer->currVelocity.y = JUMP_VELOCITY;
				// Sound effect
				AudioManager::PlayAudio("Assets/AudioSFX/Jump_retro_up.wav", AudioManager::f_JumpVolume);
			}
		}
	}

	if (AEInputCheckTriggered(AEVK_UP)) {

		// double jump
		if (haveDoubleJump_S) {
			::doubleJumpCounter_S++;

			if (p_ShadowPlayer->i_gridCollisionFlag && ::doubleJumpCounter_S >= 2)
			{
				::doubleJumpCounter_S = 0;
			}

			if (::doubleJumpCounter_S < 2) {
				p_ShadowPlayer->currVelocity.y = JUMP_VELOCITY;

				// Sound effect
				AudioManager::PlayAudio("Assets/AudioSFX/Jump_retro_up.wav", AudioManager::f_JumpVolume);
			}
		}
		else { // basic jump
			if (p_ShadowPlayer->currVelocity.y == 0) {
				p_ShadowPlayer->currVelocity.y = JUMP_VELOCITY;

				// Sound effect
				AudioManager::PlayAudio("Assets/AudioSFX/Jump_retro_up.wav", AudioManager::f_JumpVolume);
			}
		}

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

	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_LIGHTPLAYER) {
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
		}
		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_SHADOWPLAYER) {
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
		}
		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_MOVEABLE_SPIKE) {
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
			Objects::EnemyStateMachine(p_Instance, ::p_BinaryCollisionArray, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT);
		}
	}
			
	if (haveCamZoomFeature)
	{
		// Calculate zoomOffset (distance between player and enemy)
		::zoomOffset = static_cast<f32>(sqrt(pow(::p_LightPlayer->currPosition.x - p_ShadowPlayer->currPosition.x, 2) +
			pow(::p_LightPlayer->currPosition.y - p_ShadowPlayer->currPosition.y, 2)));

		// Make another variable decrease when zoomOffset increases
		::inverseValue = ::k / (::zoomOffset + ::c);

		camera.changeZoomVal(static_cast<s32>(::inverseValue));

		// Apply zoom change
		if (::inverseValue > 0)
		{
			::inverseValue *= ::zoomRangeMultiplier;
		}
		else if (::inverseValue <= 0) {
			::inverseValue = 0;
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

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0 || p_Instance->p_GameObject->u_Type == Objects::TYPE_PRESSURE_PLATE) continue;

		p_Instance->i_gridCollisionFlag = Collision::CheckInstanceBinaryMapCollision(::p_BinaryCollisionArray, p_Instance->currPosition.x, p_Instance->currPosition.y, p_Instance->scale.x, p_Instance->scale.y);

		if (p_Instance->i_gridCollisionFlag & COLLISION_BOTTOM) { 
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;
		}
		if (p_Instance->i_gridCollisionFlag & COLLISION_TOP) {
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;
		}
		if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_MOVEABLE_SPIKE) {
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
		case Objects::TYPE_OBJECT_LIGHTSHARD:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_TotalLightShard;
				if (::i_TotalLightShard == 0) {
					// check if all shadow fragments are collected
					if (::i_TotalShadowFragment == 0) {
						// TODO: Go to the next level
						GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
					}
				}
			}
			break;

		case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_TotalShadowFragment;
				if (::i_TotalShadowFragment == 0) {
					if (::i_TotalLightShard == 0) {
						// TODO: Go to the next level
						GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
					}
				}
			}
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				--::i_LightPlayerLives;
				if (::i_LightPlayerLives == 0) {
					// TODO: Go to lose state
				}
			}

			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				--i_ShadowPlayerLives;
				if (i_ShadowPlayerLives == 0) {
					// TODO: Go to lose state
				}
			}
			break;

		case Objects::TYPE_PRESSURE_PLATE:

			if (Collision::CollisionAABB(p_Instance->boundingBox, p_Instance->currVelocity, p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, f_Time)) {
				if (p_Instance->boundingBox.min.x >= p_LightPlayer->boundingBox.max.x && p_LightPlayer->currVelocity.x > 0.0) {
					p_LightPlayer->currPosition.x = p_LightPlayer->currVelocity.x * f_Time + p_LightPlayer->prevPosition.x; //SNAP PLAYER BACK VIA X AXIS
				}
				if (p_Instance->boundingBox.max.x <= p_LightPlayer->boundingBox.min.x && p_LightPlayer->currVelocity.x < 0.0) {
					p_LightPlayer->currPosition.x = p_LightPlayer->currVelocity.x * f_Time + p_LightPlayer->prevPosition.x; //SNAP PLAYER BACK VIA X AXIS
				}if (p_Instance->boundingBox.max.y <= p_LightPlayer->boundingBox.min.y) {

					//p_LightPlayer->currVelocity.y = p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
					if (p_LightPlayer->currVelocity.y <= 0.0f) {
						p_LightPlayer->currVelocity.y = 0.0f;
						p_LightPlayer->currPosition.y = p_LightPlayer->prevPosition.y; //SNAP PLAYER BACK VIA X AXIS
					}




					for (int i{}; i < p_Instance->sequenceCollection.sequenceCount; ++i) {
						Sequence& sequence = *(p_Instance->sequenceCollection.sequence + i);
						if (sequence.getSequenceType() == SpecialSequence::PRESSURE_PLATE_STEPPED) {

							if (sequence.targetValue < p_Instance->currPosition.y) {
								f32 yVel = g_dt * 5.0f;//HARDCODED VEL;
								p_Instance->currPosition.y -= yVel;
								p_LightPlayer->currPosition.y = p_Instance->currPosition.y + 1.0f;

							}
							if (sequence.targetValue >= p_Instance->currPosition.y) {
								Collision::SnapToCell(&p_Instance->currPosition.y);
							}


						}
					}

				}


			}
			else {
				for (int i{}; i < p_Instance->sequenceCollection.sequenceCount; ++i) {
					Sequence& sequence = *(p_Instance->sequenceCollection.sequence + i);

					if (sequence.getSequenceType() == SpecialSequence::PRESSURE_PLATE_RELEASED) {
						Sequence& prevSeq = *(p_Instance->sequenceCollection.sequence + 1);

						if (p_Instance->currPosition.y < sequence.targetValue) {
							f32 yVel = g_dt * 4.0f;//HARDCODED VEL;
							sequence.currentValue += yVel;
							p_Instance->currPosition.y += yVel;
							p_LightPlayer->currPosition.y += yVel;
						}
						else {
							p_Instance->currPosition.y = sequence.targetValue;
						}

					}
				}
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

	if (dynamicCamFeature) {
		camMidpoint_X = (p_LightPlayer->currPosition.x + p_ShadowPlayer->currPosition.x) / 2; // find X midpoint for camera Transform between two players X positions
		camMidpoint_Y = (p_LightPlayer->currPosition.y + p_ShadowPlayer->currPosition.y) / 2; // find Y midpoint for camera Transform between two players Y positions
	}


	for (s32 i{}; i < ::i_BINARY_MAP_WIDTH; ++i) {
		for (s32 j{}; j < i_BINARY_MAP_HEIGHT; ++j) {
			AEMtx33Trans(&CellTranslation, static_cast<f32>(i + 0.5f), static_cast<f32>(j + 0.5f));
			AEMtx33Concat(&FinalCellTransform, &::MapTransform, &CellTranslation);
			if (dynamicCamFeature) {
				camera.applyCameraTransform(FinalCellTransform, camMidpoint_X, camMidpoint_Y);
			}
			AEGfxSetTransform(FinalCellTransform.m);
			switch (::p_BinaryCollisionArray[i][j]) {
			case Objects::TYPE_OBJECT_EMPTY:
				Render::RenderShapes(::p_BlackCell, FinalCellTransform, 0.68f, 0.85f, 0.90f, 1.f);
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
		if (dynamicCamFeature) {
			camera.applyCameraTransform(FinalTransform, camMidpoint_X, camMidpoint_Y);
		}
		AEGfxSetTransform(FinalTransform.m);

		Render::RenderShapes(p_Instance, FinalTransform);
	}


	for (s32 i{}; i < ::i_BINARY_MAP_WIDTH; ++i) {
		for (s32 j{}; j < i_BINARY_MAP_HEIGHT; ++j) {
			AEMtx33Trans(&CellTranslation, static_cast<f32>(i + 0.5f), static_cast<f32>(j + 0.5f));
			AEMtx33Concat(&FinalCellTransform, &::MapTransform, &CellTranslation);
			if (dynamicCamFeature) {
				camera.applyCameraTransform(FinalCellTransform, camMidpoint_X, camMidpoint_Y);
			}
			AEGfxSetTransform(FinalCellTransform.m);
			switch (::p_BinaryCollisionArray[i][j]) {

			case Objects::TYPE_OBJECT_WALL:
				Render::RenderShapes(::p_WhiteCell, FinalCellTransform, 0.4f, 0.9f, 0.4f, 1.f);
				break;

			default:
				break;
			}
		}
	}


	const std::string s_LightShards = "Light Shards: " + std::to_string(::i_TotalLightShard);
	const std::string s_ShadowFragments = "Shadow Fragments: " + std::to_string(::i_TotalShadowFragment);

	Render::RenderFont(s_LightShards, 0.8f, 0.7f, 0.8f, Render::COLOUR::LIGHT_BLUE);
	Render::RenderFont(s_ShadowFragments, 0.8f, -0.7f, 0.8f, Render::COLOUR::YELLOW);

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

