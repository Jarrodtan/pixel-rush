/*******************************************************************/
/*!
	File:		GameStateLevel.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang			(40% of Code)
				Mira Tristan Dumlao		(20% of Code)
				Tan Wei Leong			(20% of Code)
				Ng Wei Zhen				(20% of Code)

	Brief:		This is the long list of code to generate levels based
				off the binary file reader and does all the collision
				phyics implementation and gravity implementation,
				bascially everything needed for the whole game to run
				in between levels to levels and scene to scene.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

namespace {
	// Player related variables
	s32 i_LightPlayerLives;
	s32 i_LightPlayerInitialX;
	s32 i_LightPlayerInitialY;
	s32 i_ShadowPlayerLives;
	s32 i_ShadowPlayerInitialX;
	s32 i_ShadowPlayerInitialY;
	s32 i_TotalLightShard;
	s32 i_PlayerLives;
	s32 i_TotalShadowFragment;
	bool deduct_PlayerLives = false;
	bool b_IsLightPlayerGrounded;
	bool b_IsShadowPlayerGrounded;
	const f32 PLAYER_BOUNDINGBOX_TOLERANCE = 0.1f;

	// Timer Features
	s32 i_Time;
	bool haveTiming = true;

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

	// TRACK SPECIAL GAMEOBJECTS
	s32 i_ID{};
	std::vector<std::pair<Objects::Index, Objects::Index>> metaContainer;
	std::string s_metaLoc;

	//CAMERA VARIABLES
	s32 deltaWheel{};
	s32 currentDeltaWheel{};
	Camera camera;

	//POWERUP VARIABLES
	int numOfPowerUps{};
	u32 powerUpFlag{};
	Objects::GameObject* p_Effect;

	//CAMERA ZOOM IN/OUT - BASED ON DISTANCE BETWEEN PLAYER 1 & PLAYER 2
	f32 zoomOffset;  // Calculate the distance for camera zoom-in and zoom-out, also to define min/max distances for zoom control.
	
	// Define a constant for inverse scaling
	f32 k = 200.0f;               // Adjust this to control the rate of decrease
	f32 c = 1.0f;                 // Small constant to avoid division by zero
	f32 inverseValue;			  // Value to inverse zoomOffset when it increases, this value decreases
	s32 zoomRangeMultiplier = 32; // To lock the range to zoom in/out
	f32 camMidpoint_X, camMidpoint_Y;
	bool haveCamZoomFeature = true;
	bool dynamicCamFeature = true;

	//DOUBLE JUMP FEATURES
	s32 doubleJumpCounter_S = 0;
	bool haveDoubleJump_S = true;
	s32 doubleJumpCounter_L = 0;
	bool haveDoubleJump_L = true;
	bool jumpEnabled_S, jumpEnabled_L;

	//PUZZLE FEATURE 1 *(LIGHTPLAYER CANNOT TOUCH SHADOWFRAGMENT, VIA VISA)
	bool puzzle1 = false;

	//PRESSURE PLATES
	bool unlockDoor;

	//COLLISION CHECK
	bool isCollideGate_S;
	bool isCollideGate_L;
	bool isCollideDoor_S;
	bool isCollideDoor_L;
	bool isCollideSpikes_S;
	bool isCollideSpikes_L;
	bool isCollideMovingPlatform_L, isCollideMovingPlatform_S;

	//GRAVITY SWITCHER
	bool isGravitySwitch_S, isGravitySwitch_L;

	struct TipsText {
		std::string s_Text;
		bool b_IsActive;
		f32 f_Alpha;	
	};

	std::vector<TipsText> Tips{};
	std::vector<TipsText> TipsTutorial1{};
	std::vector<TipsText> TipsTutorial2{};
	std::vector<TipsText> TipsTutorial3{};
	static f32 f_TipsScreenTime;
	static const f32 f_TipsScreenUnchangedTime = 2.f;
	static const f32 f_MaxTipsSreenTime = 1.5f;
	bool b_TipActive;

	// UI Elements (HP, Coins Collected, etc.)
	bool b_ToggleUI;
	AEMtx33 scaleBGUI[999] = { 0 };
	AEMtx33 rotateBGUI[999] = { 0 };
	AEMtx33 translateBGUI[999] = { 0 };
	AEMtx33 transformBGUI[999] = { 0 };

	// health bar
	int hp_num;

	// Bool True for testing levels or switching scenes using button 0 to 9 
	bool enterDebugMode = true;

	//cheat codes 
	bool stoptimer = false;
	bool unlimitedhp = false;
}

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevelLoad(void) {
	// allocate memory for the object type and mesh
	::p_GameObjectList = (Objects::GameObject*)calloc(GAME_OBJECT_NUM_MAX, sizeof(Objects::GameObject));
	// allocate memory for the object instance
	::p_GameObjectInstanceList = (Objects::GameObjectInstance*)calloc(GAME_OBJECT_INST_NUM_MAX, sizeof(Objects::GameObjectInstance));

	AE_ASSERT_MESG(::p_GameObjectList, "Fail to calloc");
	::u_GameObjectNum = 0;

	numOfPowerUps = 1;
	powerUpFlag = 0;

	Objects::GameObject* p_Object{};

	metaContainer = std::vector<std::pair<Objects::Index, Objects::Index>>{};

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

	// create the moveable spike object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_MOVEABLE_SPIKE;

	// create the stationary spike object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SPIKE;

	// create the light collectable object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHTSHARD;

	// create the shadow collectable object
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOWFRAGMENT;

	// create the moving platform
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_MOVINGPLATFORM;

	// create the light spike
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHTSPIKE;

	// create the shadow spike
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOWSPIKE;

	// create the moveable light spike
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE;

	// create the moveable shadow spike
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE;

	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT;

	// create the dark pressure plate
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK;

	// create the door
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_DOOR;

	// create the gate
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_GATE;

	// create the shadow door
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOW_DOOR;

	// create the light door
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHT_DOOR;

	// create the teleporter
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_TELEPORTOR;

	// create the gravity switcher
	p_Object = ::p_GameObjectList + ::u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_GRAVITYSWITCHER;

	p_Object = p_GameObjectList + u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SPIKE_DOWN;

	p_Object = p_GameObjectList + u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_LIGHTSPIKE_DOWN;

	p_Object = p_GameObjectList + u_GameObjectNum++;
	p_Object->u_Type = Objects::TYPE_OBJECT_SHADOWSPIKE_DOWN;

	Render::InitRender(::p_GameObjectList, u_GameObjectNum);

	// setting initial binary map values
	::p_MapData = 0;
	::p_BinaryCollisionArray = 0;
	::i_BINARY_MAP_WIDTH = 0;
	::i_BINARY_MAP_HEIGHT = 0;

	// search for file path for binary maps or metamaps and load them in real time
	std::string s_FilePath{};
	switch (GameStateManager::g_GameStateCurr) {
	case States::GS_TUTORIAL_LVL1:
		::i_Time = -100;
		haveTiming = false;
		s_FilePath = "Assets/Maps/Tutorial1.txt";
		s_metaLoc = "";
		break;

	case States::GS_TUTORIAL_LVL2:
		::i_Time = -100;
		haveTiming = false;
		s_FilePath = "Assets/Maps/Tutorial2.txt";
		s_metaLoc = "";
		break;
	case States::GS_TUTORIAL_LVL3:
		::i_Time = -100;
		haveTiming = false;
		s_FilePath = "Assets/Maps/Tutorial3.txt";
		s_metaLoc = "Assets/MapMetaData/Tutorial3_Meta.txt";
		break;

	case States::GS_PLATFORM_LVL1:
		::i_Time = 160;
		haveTiming = true;
		s_FilePath = "Assets/Maps/Level1.txt";
		s_metaLoc = "";
		break;

	case States::GS_PLATFORM_LVL2:
		::i_Time = 100;
		haveTiming = true;
		s_FilePath = "Assets/Maps/Level2.txt";
		s_metaLoc = "";
		break;
		
	case States::GS_PLATFORM_LVL3:
		::i_Time = 80;
		haveTiming = true;
		s_FilePath = "Assets/Maps/Level3.txt";
		s_metaLoc = "Assets/MapMetaData/Level3_Meta.txt";
		break;
	}

	if (!FileReader::ImportMapDataFromFile(s_FilePath, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT, ::p_MapData, ::p_BinaryCollisionArray)) {
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
void GameStateLevelInit(void) {
	AESysSetWindowTitle("Pixel Rush");
	// loop through UI elements to render it on screen based off different xy position on screen
	for (int n = 0; n < 10; n++) {

		f32 posX, posY, sizeX, sizeY, rotation = 0.f;

		switch (n) {
	    default:
		case 0: sizeX = sizeY = 50.f; posX = -650.f; posY = 355.f; break;
		case 1: sizeX = sizeY = 50.f; posX = -590.f; posY = 355.f; break;
		case 2: sizeX = sizeY = 50.f; posX = -530.f; posY = 355.f; break;
		case 3: sizeX = g_WindowWidth; sizeY = g_WindowHeight; posX = posY = 0.f; break;
		case 4: sizeX = 300.f; sizeY = 250.f; posX = -600.f; posY = 300.f; break;
		case 5: sizeX = sizeY = 50.f; posX = -650.f; posY = 295.f; break;
		case 6: sizeX = sizeY = 50.f; posX = -650.f; posY = 235.f; break;
		case 7: sizeX = g_WindowWidth * 0.75f; sizeY = g_WindowHeight * 0.15f; posX = 0.f; posY = -370.f; break;
		case 8: sizeX = 200.f; sizeY = 100.f; posX = 0.f; posY = 370.f; break;
		case 9: sizeX = sizeY = 50.f; posX = -50.f; posY = 370.f; break;
		}
		renderUI_transform(posX, posY, sizeX, sizeY, rotation, scaleBGUI[n], rotateBGUI[n], translateBGUI[n], transformBGUI[n]);
	}
	
	f_TipsScreenTime = 0.f;
	b_TipActive = false;
	b_ToggleUI = true;

	::p_LightPlayer = 0;
	::p_ShadowPlayer = 0;
	::p_BlackCell = 0;
	::p_WhiteCell = 0;
	::doubleJumpCounter_S = 0;
	::doubleJumpCounter_L = 0;

	unlockDoor = false;
	jumpEnabled_S = false;
	jumpEnabled_L = false;
	isCollideGate_S = false;
	isCollideGate_L = false;
	isCollideSpikes_S = false;
	isCollideSpikes_L = false;
	isCollideMovingPlatform_L = false;
	isGravitySwitch_S = false;
	isGravitySwitch_L = false;

	AEVec2 Scale = { 1.0f, 1.0f };
	::p_BlackCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_EMPTY, &Scale, 0, 0, 0.0f, Objects::STATE_NONE); 
	::p_BlackCell->u_Flag ^= FLAG_VISIBLE;
	::p_BlackCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::p_WhiteCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_WALL, &Scale, 0, 0, 0.f, Objects::STATE_NONE);
	::p_WhiteCell->u_Flag ^= FLAG_VISIBLE;
	::p_WhiteCell->u_Flag |= FLAG_NON_COLLIDABLE;

	::i_PlayerLives = PLAYER_LIVES;

	// reset the total values for both to avoid double count for each of them
	::i_TotalLightShard = 0;
	::i_TotalShadowFragment = 0;

	AEVec2 Position{}, Velocity{};

	AudioManager::PlayBGM("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav");
	// create all game objects instance based off the binary map text file
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
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_MOVEABLE_SPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_RIGHT);
				break;

			case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_LEFT);
				break;

			case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_RIGHT);
				break;

			case Objects::TYPE_OBJECT_SPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;
			
			case Objects::TYPE_OBJECT_SPIKE_DOWN:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SPIKE_DOWN, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_LIGHTSPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_LIGHTSPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_LIGHTSPIKE_DOWN:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_LIGHTSPIKE_DOWN, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_SHADOWSPIKE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SHADOWSPIKE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_SHADOWSPIKE_DOWN:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SHADOWSPIKE_DOWN, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_GATE:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_GATE, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_SHADOW_DOOR:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_SHADOW_DOOR, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_LIGHT_DOOR:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_LIGHT_DOOR, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_TELEPORTOR:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_TELEPORTOR, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_GRAVITYSWITCHER:
				gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_GRAVITYSWITCHER, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE);
				break;

			case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			{
				Objects::GameObjectInstance* platform = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_MOVINGPLATFORM, &Scale, &Position, &Velocity, 0.f, Objects::STATE_GOING_UP);
				// Add movement sequence
				platform->sequenceCollection.sequenceCount = 2;
				platform->sequenceCollection.sequence = new Sequence[2]{
					{false, false, Position.x - 5.0f, 0.0f, 2.0f, SpecialSequence::MOVE_LEFT},
					{false, false, Position.x + 5.0f, 0.0f, 2.0f, SpecialSequence::MOVE_RIGHT}
				};
			}
			break;

			case Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT: 
			{
				Objects::GameObjectInstance* p_Instance = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE, Objects::Index{ i, j });
				p_Instance->u_Flag |= FLAG_NON_COLLIDABLE;
				int count = p_Instance->sequenceCollection.sequenceCount = 2;

				p_Instance->sequenceCollection.sequence = new Sequence[count]{
					Sequence {
						false,
						false,
						Position.y,
						0.0f,
						0.0f,
						SpecialSequence::PRESSURE_PLATE_RELEASED
					},

					Sequence {
						false,
						false,
						Position.y - (Scale.y - PLAYER_BOUNDINGBOX_TOLERANCE),
						0.0f,
						0.0f,
						SpecialSequence::PRESSURE_PLATE_STEPPED
					}
				};	
			}
			break;

			case Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK: 
			{
				Objects::GameObjectInstance* p_Instance = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE, Objects::Index{ i, j });
				p_Instance->u_Flag |= FLAG_NON_COLLIDABLE;
					
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
					Position.y - (Scale.y - PLAYER_BOUNDINGBOX_TOLERANCE),
					0.0f,
					0.0f,
					SpecialSequence::PRESSURE_PLATE_STEPPED
				};
			}
			break;

			case Objects::TYPE_OBJECT_DOOR:
			{
				Objects::GameObjectInstance* p_Instance = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, Objects::TYPE_OBJECT_DOOR, &Scale, &Position, &Velocity, 0.f, Objects::STATE_NONE, Objects::Index{i, j});
				p_Instance->u_Flag |= FLAG_NON_COLLIDABLE;
				p_Instance->sequenceCollection.sequenceCount = 2;
				p_Instance->sequenceCollection.sequence = new Sequence[2];
				p_Instance->sequenceCollection.sequence[0] = Sequence{
						false,
						false,
						Position.y - Scale.y,
						0.0f,
						0.0f,
						SpecialSequence::OPEN
				};

				p_Instance->sequenceCollection.sequence[1] = Sequence{
					false,
					false,
					Position.y,
					0.0f,
					0.0f,
					SpecialSequence::CLOSE
				};
			}
			break;
			}
		}		
	}

	FileReader::readMeta(s_metaLoc, metaContainer);

	for (FileReader::MetaData& data : metaContainer) {

		for (int i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
			Objects::GameObjectInstance* p_Instance = ::p_GameObjectInstanceList + i;
			if ((p_Instance->u_Flag & FLAG_ACTIVE) != 0 && p_Instance->p_GameObject->u_Type != Objects::TYPE_OBJECT_DOOR) continue;

			if (data.second == p_Instance->idx) {
				for (int x{}; x < GAME_OBJECT_INST_NUM_MAX; ++x) {
					Objects::GameObjectInstance* p_secondInst = ::p_GameObjectInstanceList + x;
					if ((p_secondInst->u_Flag & FLAG_ACTIVE) != 0 && (p_secondInst->p_GameObject->u_Type != Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK && p_secondInst->p_GameObject->u_Type != Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT)) continue;

					if (data.first == p_secondInst->idx) {
						p_Instance->otherGameObj = p_secondInst;
						break;
					}
				}
			}
		}
	}

	TipsTutorial1.push_back({ "Tip: Light player can only touch light fragments", true, 1.0f });
	TipsTutorial1.push_back({ "Tip: Shadow player can only touch shadow fragments", false, 1.0f });
	TipsTutorial1.push_back({ "Tip: Light player can safely go through light spikes", false, 1.0f });
	TipsTutorial1.push_back({ "Tip: Shadow player can safely go through shadow spikes", false, 1.0f });
	TipsTutorial1.push_back({ "Tip: Light Door can only be access by Light Player", false, 1.0f });
	TipsTutorial1.push_back({ "Tip: Shadow Door can only be access by Shadow Player", false, 1.0f });

	TipsTutorial2.push_back({ "Tip: You can block your coloured blade", true, 1.f });
	TipsTutorial2.push_back({ "Tip: Avoid the Red Spike!", false, 1.0f });
	TipsTutorial2.push_back({ "Tip: The portal will teleport you to the other player's position", false, 1.0f });

	TipsTutorial3.push_back({ "Tip: You can jump on the pressure plate to open doors", true, 1.f });
	TipsTutorial3.push_back({ "Tip: Invert gravity with gravity token", true, 1.f });

	Tips.push_back({ "Tip: Light player can only touch light fragments", true, 1.0f });
	Tips.push_back({ "Tip: Shadow player can only touch shadow fragments", false, 1.0f });
	Tips.push_back({ "Tip: Light player can safely go through light spikes", false, 1.0f });
	Tips.push_back({ "Tip: Shadow player can safely go through shadow spikes", false, 1.0f });
	Tips.push_back({ "Tip: Light Door can only be access by Light Player", false, 1.0f });
	Tips.push_back({ "Tip: Shadow Door can only be access by Shadow Player", false, 1.0f });
	Tips.push_back({ "Tip: You can block your coloured blade", false, 1.f });
	Tips.push_back({ "Tip: Avoid the Red Spike!", false, 1.0f });
	Tips.push_back({ "Tip: The portal will teleport you to the other player's position", false, 1.0f });
	Tips.push_back({ "Tip: You can jump on the pressure plate to open doors", false, 1.f });
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevelUpdate(void) {
	// check current scene state to save it for save scene feature
	switch (GameStateManager::g_GameStateCurr) {
	default:
	case States::GS_PLATFORM_LVL1:
		saveprevlevel = 1;
		break;
	case States::GS_PLATFORM_LVL2:
		saveprevlevel = 2;
		break;
	case States::GS_PLATFORM_LVL3:
		saveprevlevel = 3;
		break;
	case States::GS_TUTORIAL_LVL1:
		saveprevtut = -1;
		break;
	case States::GS_TUTORIAL_LVL2:
		saveprevtut = -2;
		break;
	case States::GS_TUTORIAL_LVL3:
		saveprevtut = -3;
		break;
	}
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		GameStateManager::g_GameStateNext = States::GS_MAINMENU;
	}

	if (AEInputCheckTriggered(AEVK_H)) {
		b_TipActive = !b_TipActive;
	}

	if (AEInputCheckTriggered(AEVK_T)) b_ToggleUI = !b_ToggleUI;

	// debugging purposes
	if (enterDebugMode) {
		if (AEInputCheckTriggered(AEVK_1)) {
			GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL1;
		}
		else if (AEInputCheckCurr(AEVK_2)) {
			GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL2;
		}
		else if (AEInputCheckCurr(AEVK_3)) {
			GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL3;
		}
		else if (AEInputCheckCurr(AEVK_4)) {
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
		}
		else if (AEInputCheckCurr(AEVK_5)) {
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
		}
		else if (AEInputCheckCurr(AEVK_6)) {
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL3;
		}		
		else if (AEInputCheckCurr(AEVK_O)) {
			GameStateManager::g_GameStateNext = States::GS_LOSE;
		}
		else if (AEInputCheckCurr(AEVK_P)) {
			GameStateManager::g_GameStateNext = States::GS_WIN;
		}
	}
	
	// cheat code for stop/resume timer
	if (AEInputCheckTriggered(AEVK_0)) {
		stoptimer = !stoptimer;
	}

	// cheat code for unlimited health
	if (AEInputCheckTriggered(AEVK_9)) {
		unlimitedhp = !unlimitedhp;
	}

	// unlimited health 
	if (unlimitedhp)
	{
		::i_PlayerLives = 3;
	}


	// Time counter
	if (haveTiming && !stoptimer) {
		static f32 f_TimeCounter{};
		f_TimeCounter += g_dt;
		if (f_TimeCounter >= 1.0f) {
			--::i_Time;
			f_TimeCounter = 0.0f;
		}
	}

	s32 i{};
	Objects::GameObjectInstance* p_Instance{};
	// powerups collection for both player speed increases, had sinced remove the feature so not used 
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

	// double jump for shadow player
	if (AEInputCheckTriggered(AEVK_UP)) {
		if (!isGravitySwitch_S) {
			if (p_ShadowPlayer->currVelocity.y == 0 || doubleJumpCounter_S < 2 || jumpEnabled_S) {
				p_ShadowPlayer->currVelocity.y = JUMP_VELOCITY;
				++doubleJumpCounter_S;
				b_IsShadowPlayerGrounded = false;
			}
		}
		else {
			if (p_ShadowPlayer->currVelocity.y == 0 || doubleJumpCounter_S < 2 || jumpEnabled_S) {
				p_ShadowPlayer->currVelocity.y = -JUMP_VELOCITY;
				++doubleJumpCounter_S;
				b_IsShadowPlayerGrounded = false;
			}
		}
	}

	// double jump for light player
	if (AEInputCheckTriggered(AEVK_W)) {
		if (!isGravitySwitch_L) {
			if (p_LightPlayer->currVelocity.y == 0 || doubleJumpCounter_L < 2 || jumpEnabled_L) {
				p_LightPlayer->currVelocity.y = JUMP_VELOCITY;
				++doubleJumpCounter_L;
				b_IsLightPlayerGrounded = false;
			}
		}
		else {
			if (p_LightPlayer->currVelocity.y == 0 || doubleJumpCounter_L < 2 || jumpEnabled_L) {
				p_LightPlayer->currVelocity.y = -JUMP_VELOCITY;
				++doubleJumpCounter_L;
				b_IsLightPlayerGrounded = false;
			}
		}
	}


	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;
		f32 f_Time{};

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		// check for gravity for both players
		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_LIGHTPLAYER:
			if (!isCollideMovingPlatform_L) {
				if (!isGravitySwitch_L) {
					p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
				}
				else {
					p_Instance->currVelocity.y = -GRAVITY * g_dt + p_Instance->currVelocity.y;
				}
			}
			break;

		case Objects::TYPE_OBJECT_SHADOWPLAYER:
			if (!isCollideMovingPlatform_S) {
				if (!isGravitySwitch_S) {
					p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
				}
				else {
					p_Instance->currVelocity.y = -GRAVITY * g_dt + p_Instance->currVelocity.y;
				}
			}
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
			Objects::EnemyStateMachine(p_Instance, ::p_BinaryCollisionArray, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT, ::p_ShadowPlayer, f_Time);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
			Objects::EnemyStateMachine(p_Instance, ::p_BinaryCollisionArray, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT, ::p_LightPlayer, f_Time);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
			p_Instance->currVelocity.y = GRAVITY * g_dt + p_Instance->currVelocity.y;
			Objects::EnemyStateMachine(p_Instance, ::p_BinaryCollisionArray, ::i_BINARY_MAP_WIDTH, ::i_BINARY_MAP_HEIGHT);
			break;

		case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			Objects::MovingPlatformStateMachine(p_Instance);
			break;
		}
	}

	// camera zoom feature		
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

	// check for bounding box for colllision
	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0) continue;

		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_LIGHTPLAYER:
		case Objects::TYPE_OBJECT_SHADOWPLAYER:
			p_Instance->boundingBox.min.x = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x + PLAYER_BOUNDINGBOX_TOLERANCE;
			p_Instance->boundingBox.min.y = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y + PLAYER_BOUNDINGBOX_TOLERANCE;

			p_Instance->boundingBox.max.x = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x - PLAYER_BOUNDINGBOX_TOLERANCE;
			p_Instance->boundingBox.max.y = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y - PLAYER_BOUNDINGBOX_TOLERANCE;
			break;

		default:
			p_Instance->boundingBox.min.x = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x;
			p_Instance->boundingBox.min.y = -BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y;

			p_Instance->boundingBox.max.x = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.x + p_Instance->prevPosition.x;
			p_Instance->boundingBox.max.y = BOUNDING_RECT_SIZE / 2 * p_Instance->scale.y + p_Instance->prevPosition.y;
			break;
		}

		AEVec2 displacement{};
		AEVec2Scale(&displacement, &p_Instance->currVelocity, g_dt);
		AEVec2Add(&p_Instance->currPosition, &p_Instance->currPosition, &displacement);
	}

	// collision check for two moving game objects dynamically
	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0 || (p_Instance->u_Flag & FLAG_NON_COLLIDABLE) > 0) continue;

		p_Instance->i_gridCollisionFlag = Collision::CheckInstanceBinaryMapCollision(::p_BinaryCollisionArray, p_Instance->currPosition.x, p_Instance->currPosition.y, p_Instance->scale.x, p_Instance->scale.y);

		if (p_Instance->i_gridCollisionFlag & COLLISION_BOTTOM) { 
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;

			if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_LIGHTPLAYER && !isGravitySwitch_L) {
				doubleJumpCounter_L = 0;
				b_IsLightPlayerGrounded = true;
			}
			
			if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_SHADOWPLAYER && !isGravitySwitch_S) {
				doubleJumpCounter_S = 0;
				b_IsShadowPlayerGrounded = true;
			}
		}

		if (p_Instance->i_gridCollisionFlag & COLLISION_TOP) {
			Collision::SnapToCell(&p_Instance->currPosition.y);
			p_Instance->currVelocity.y = 0;
			if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_LIGHTPLAYER && isGravitySwitch_L) {
				doubleJumpCounter_L = 0;
				b_IsLightPlayerGrounded = true;
			}

			if (p_Instance->p_GameObject->u_Type == Objects::TYPE_OBJECT_SHADOWPLAYER && isGravitySwitch_S) {
				doubleJumpCounter_S = 0;
				b_IsShadowPlayerGrounded = true;
			}
		}

		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
		case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
		case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			if (p_Instance->i_gridCollisionFlag & COLLISION_LEFT && p_Instance->currVelocity.x < 0) {
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			if (p_Instance->i_gridCollisionFlag & COLLISION_RIGHT && p_Instance->currVelocity.x > 0) {
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			break;

		default:
			if (p_Instance->i_gridCollisionFlag & COLLISION_LEFT) {
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			if (p_Instance->i_gridCollisionFlag & COLLISION_RIGHT) {
				Collision::SnapToCell(&p_Instance->currPosition.x);
				p_Instance->currVelocity.x = 0;
			}
			break;
		}
	}
	
	// use bool instead cuz worried it will double deduct lives sometimes
	if (deduct_PlayerLives) {
		--::i_PlayerLives;
		deduct_PlayerLives = false;
	}

	// check if all shadow fragments & light shards are collected
	if (::i_TotalLightShard == 0 && ::i_TotalShadowFragment == 0) {
		// TODO: Go to the next level
		GameStateManager::g_GameStateNext = States::GS_WIN;
	}

	// check player health is less than 0 or game timer ends
	if (::i_PlayerLives < 0 || ::i_Time == 0) {
		// TODO: Go to lose state
		GameStateManager::g_GameStateNext = States::GS_LOSE;
	}

	// collision mapping for jump and double jump features
	if (!::isCollideGate_L && !::isCollideDoor_L && !::isCollideSpikes_L && !::isCollideMovingPlatform_L) {
		::jumpEnabled_L = false;
	}
	else {
		::jumpEnabled_L = true;
	}
	if (!::isCollideGate_S && !::isCollideDoor_S && !::isCollideSpikes_S && !::isCollideMovingPlatform_S) {
		::jumpEnabled_S = false;
	}
	else {
		::jumpEnabled_S = true;
	}

	// collision resolution for objects to objects on screen
	for (i = 0; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		f32 f_Time = 0.f;
		p_Instance = ::p_GameObjectInstanceList + i;

		if ((p_Instance->u_Flag & FLAG_ACTIVE) == 0 || (p_Instance->u_Flag & FLAG_VISIBLE) == 0) continue;

		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_LIGHTSHARD:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_TotalLightShard;
				++::i_Time;
				
			}
			break;

		case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				--::i_TotalShadowFragment;
				++::i_Time;
			}
			break;

		case Objects::TYPE_OBJECT_SHADOWSPIKE_DOWN:
		case Objects::TYPE_OBJECT_SHADOWSPIKE:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_LightPlayer->currPosition.x = static_cast<f32>(i_LightPlayerInitialX + 0.5f);
				p_LightPlayer->currPosition.y = static_cast<f32>(i_LightPlayerInitialY + 0.5f);
				deduct_PlayerLives = true; 

			}
			break;

		case Objects::TYPE_OBJECT_SHADOW_DOOR:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) ) {

				if (p_LightPlayer->currPosition.y == p_Instance->currPosition.y) {
					if (p_LightPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x > 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
					else if (p_LightPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x < 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
				}
				if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y) {
					if (p_LightPlayer->currVelocity.y < 0) {
						if (p_LightPlayer->currPosition.x < p_Instance->currPosition.x) {
							p_LightPlayer->currPosition.x -= .01f;
						}
						else if (p_LightPlayer->currPosition.x > p_Instance->currPosition.x) {
							p_LightPlayer->currPosition.x += .01f;
						}

						p_LightPlayer->currVelocity.y = 0;
						Collision::SnapToCell(&p_LightPlayer->currPosition.y);
					}
				}
				if (p_LightPlayer->currPosition.y < p_Instance->currPosition.y) {
					if (p_LightPlayer->currVelocity.y > 0) {
						p_LightPlayer->currVelocity.y = 0;
						Collision::SnapToCell(&p_LightPlayer->currPosition.y);
					}
				}
				isCollideDoor_L = true;
			}
			else {
				isCollideDoor_L = false;
			}
			break;

		case Objects::TYPE_OBJECT_LIGHT_DOOR:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) ) {

				if (p_ShadowPlayer->currPosition.y == p_Instance->currPosition.y) {
					if (p_ShadowPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x > 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
					else if (p_ShadowPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x < 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
				}

				if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y) {
					if (p_ShadowPlayer->currVelocity.y < 0) {
						if (p_ShadowPlayer->currPosition.x < p_Instance->currPosition.x) {
							p_ShadowPlayer->currPosition.x -= .01f;
						}
						else if (p_ShadowPlayer->currPosition.x > p_Instance->currPosition.x) {
							p_ShadowPlayer->currPosition.x += .01f;
						}
						p_ShadowPlayer->currVelocity.y = 0;
						Collision::SnapToCell(&p_ShadowPlayer->currPosition.y);
					}
				}
				isCollideDoor_S = true;
			}
			else {
				isCollideDoor_S = false;
			}
			break;

		case Objects::TYPE_OBJECT_GATE:
			if (!unlockDoor) {
				if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {

					if (p_ShadowPlayer->currPosition.y == p_Instance->currPosition.y) {
						if (p_ShadowPlayer->currPosition.x < p_Instance->currPosition.x) {
							if (p_ShadowPlayer->currVelocity.x > 0) {
								p_ShadowPlayer->currVelocity.x = 0;
								Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
							}
						}
						else if (p_ShadowPlayer->currPosition.x > p_Instance->currPosition.x) {
							if (p_ShadowPlayer->currVelocity.x < 0) {
								p_ShadowPlayer->currVelocity.x = 0;
								Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
							}
						}
					}

					if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y) {
						if (p_ShadowPlayer->currVelocity.y < 0) {
							p_ShadowPlayer->currVelocity.y = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.y);
						}
					}
					isCollideGate_S = true;
				}
				else {
					isCollideGate_S = false;
				}
				if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) && (p_Instance->currVelocity.x == 0.f)) {

					if (p_LightPlayer->currPosition.y == p_Instance->currPosition.y) {
						if (p_LightPlayer->currPosition.x < p_Instance->currPosition.x) {
							if (p_LightPlayer->currVelocity.x > 0) {
								p_LightPlayer->currVelocity.x = 0;
								Collision::SnapToCell(&p_LightPlayer->currPosition.x);
							}
						}
						else if (p_LightPlayer->currPosition.x > p_Instance->currPosition.x) {
							if (p_LightPlayer->currVelocity.x < 0) {
								p_LightPlayer->currVelocity.x = 0;
								Collision::SnapToCell(&p_LightPlayer->currPosition.x);
							}
						}
					}
					if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y) {
						if (p_LightPlayer->currVelocity.y < 0) {
							p_LightPlayer->currVelocity.y = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.y);
						}
					}
					isCollideGate_L = true;
				}
				else {
					isCollideGate_L = false;
				}
			}
			break;
			
		case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_ShadowPlayer->currPosition.x = static_cast<f32>(i_ShadowPlayerInitialX + 0.5f);
				p_ShadowPlayer->currPosition.y = static_cast<f32>(i_ShadowPlayerInitialY + 0.5f);
				deduct_PlayerLives = true;

			}

			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) && (p_Instance->currVelocity.x == 0.f)) {
				if (p_LightPlayer->currPosition.y == p_Instance->currPosition.y) {
					if (p_LightPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x > 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
					else if (p_LightPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x < 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
				}

				if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y) {
					if (p_LightPlayer->currVelocity.y < 0) {
						p_LightPlayer->currVelocity.y = 0;
						Collision::SnapToCell(&p_LightPlayer->currPosition.y);
					}
				}
				//::jumpEnabled_L = true;
				::isCollideSpikes_L = true;
			}
			else {
				//::jumpEnabled_L = false;
				::isCollideSpikes_L = false;
			}
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_LightPlayer->currPosition.x = static_cast<f32>(i_LightPlayerInitialX + 0.5f);
				p_LightPlayer->currPosition.y = static_cast<f32>(i_LightPlayerInitialY + 0.5f);
				deduct_PlayerLives = true;

			}

			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) && (p_Instance->currVelocity.x == 0.f)) {
				if (p_ShadowPlayer->currPosition.y == p_Instance->currPosition.y) {
					if (p_ShadowPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x > 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
					else if (p_ShadowPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x < 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
				}

				if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y) {
					if (p_ShadowPlayer->currVelocity.y < 0) {
						p_ShadowPlayer->currVelocity.y = 0;
						Collision::SnapToCell(&p_ShadowPlayer->currPosition.y);
					}
				}

				//::jumpEnabled_S = true;
				::isCollideSpikes_S = true;
			}
			else {
				//::jumpEnabled_S = false;
				::isCollideSpikes_S = false;
			}
			break;

		case Objects::TYPE_OBJECT_LIGHTSPIKE_DOWN:
		case Objects::TYPE_OBJECT_LIGHTSPIKE:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_ShadowPlayer->currPosition.x = static_cast<f32>(i_ShadowPlayerInitialX + 0.5f);
				p_ShadowPlayer->currPosition.y = static_cast<f32>(i_ShadowPlayerInitialY + 0.5f);
				deduct_PlayerLives = true;

			}
			break;

		// teleport the respective LIGHT / SHADOW player to be together once again
		case Objects::TYPE_OBJECT_TELEPORTOR:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_ShadowPlayer->currPosition.x = p_LightPlayer->currPosition.x;
				p_ShadowPlayer->currPosition.y = p_LightPlayer->currPosition.y;
			}
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				p_LightPlayer->currPosition.x = p_ShadowPlayer->currPosition.x;
				p_LightPlayer->currPosition.y = p_ShadowPlayer->currPosition.y;
			}
			break;

		case Objects::TYPE_OBJECT_GRAVITYSWITCHER:
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				isGravitySwitch_S = !isGravitySwitch_S;
				
			}
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				gameObjectInstanceDestroy(p_Instance);
				isGravitySwitch_L = !isGravitySwitch_L;
				
			}
			break;

		case Objects::TYPE_OBJECT_SPIKE_DOWN:
		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
		case Objects::TYPE_OBJECT_SPIKE:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				deduct_PlayerLives = true;
				p_LightPlayer->currPosition.x = static_cast<f32>(i_LightPlayerInitialX + 0.5f);
				p_LightPlayer->currPosition.y = static_cast<f32>(i_LightPlayerInitialY + 0.5f);
			}
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time)) {
				deduct_PlayerLives = true;
				p_ShadowPlayer->currPosition.x = static_cast<f32>(i_ShadowPlayerInitialX + 0.5f);
				p_ShadowPlayer->currPosition.y = static_cast<f32>(i_ShadowPlayerInitialY + 0.5f);

			}
			break;

		case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			if (Collision::CollisionAABB(p_LightPlayer->boundingBox, p_LightPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) && (p_Instance->currVelocity.x == 0.f)) {
				isCollideMovingPlatform_L = true;
				if (fabs(p_LightPlayer->currPosition.y - p_Instance->currPosition.y) <= TOLERANCE) {
					if (p_LightPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x > 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
					else if (p_LightPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_LightPlayer->currVelocity.x < 0) {
							p_LightPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_LightPlayer->currPosition.x);
						}
					}
				}

				if (!isGravitySwitch_L) {
					if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y && fabs(p_LightPlayer->prevPosition.y - p_Instance->prevPosition.y) >= PLATFORM_SNAPPING_TOLERANCE) {
						p_LightPlayer->currVelocity.y = PLATFORM_MOVE_SPEED;

						if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y) {
							b_IsLightPlayerGrounded = true;
							doubleJumpCounter_L = 0;
							if (p_LightPlayer->currVelocity.y < 0) {
								p_LightPlayer->currVelocity.y = 0;
							}
						}

						if (p_LightPlayer->currPosition.y < p_Instance->currPosition.y - CRUSH_TOLERANCE) {
							if (p_LightPlayer->currVelocity.y > 0) {
								p_LightPlayer->currVelocity.y = -PLATFORM_MOVE_SPEED;
								
							}
						}
					}

				}
				else {
					if (p_LightPlayer->currPosition.y < p_Instance->currPosition.y && fabs(p_LightPlayer->prevPosition.y - p_Instance->prevPosition.y) >= (1.f - PLATFORM_SNAPPING_TOLERANCE)) {
						p_LightPlayer->currVelocity.y = -PLATFORM_MOVE_SPEED;

						if (p_LightPlayer->currPosition.y < p_Instance->currPosition.y) {
							b_IsLightPlayerGrounded = true;
							doubleJumpCounter_L = 0;

							if (p_LightPlayer->currVelocity.y > 0) {
								p_LightPlayer->currVelocity.y = 0;
							}
						}

						if (p_LightPlayer->currPosition.y > p_Instance->currPosition.y + CRUSH_TOLERANCE) {
							if (p_LightPlayer->currVelocity.y < 0) {
								p_LightPlayer->currVelocity.y = PLATFORM_MOVE_SPEED;
							}
						}
					}
				}
			}
			else {
				isCollideMovingPlatform_L = false;
			}
			if (Collision::CollisionAABB(p_ShadowPlayer->boundingBox, p_ShadowPlayer->currVelocity, p_Instance->boundingBox, p_Instance->currVelocity, f_Time) && (p_Instance->currVelocity.x == 0.f)) {
				isCollideMovingPlatform_S = true;
				if (fabs(p_ShadowPlayer->currPosition.y - p_Instance->currPosition.y) <= TOLERANCE) {
					if (p_ShadowPlayer->currPosition.x < p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x > 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
					else if (p_ShadowPlayer->currPosition.x > p_Instance->currPosition.x) {
						if (p_ShadowPlayer->currVelocity.x < 0) {
							p_ShadowPlayer->currVelocity.x = 0;
							Collision::SnapToCell(&p_ShadowPlayer->currPosition.x);
						}
					}
				}
				
				if (!isGravitySwitch_S) {
					if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y && fabs(p_ShadowPlayer->prevPosition.y - p_Instance->prevPosition.y) >= PLATFORM_SNAPPING_TOLERANCE) {
						p_ShadowPlayer->currVelocity.y = PLATFORM_MOVE_SPEED;


						if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y) {
							b_IsShadowPlayerGrounded = true;
							doubleJumpCounter_S = 0;

							if (p_ShadowPlayer->currVelocity.y < 0) {
								p_ShadowPlayer->currVelocity.y = 0;
							}
						}

						if (p_ShadowPlayer->currPosition.y < p_Instance->currPosition.y - CRUSH_TOLERANCE) {
							if (p_ShadowPlayer->currVelocity.y > 0) {
								p_ShadowPlayer->currVelocity.y = -PLATFORM_MOVE_SPEED;
							}
						}
					}
				}
				else {
					if (p_ShadowPlayer->currPosition.y < p_Instance->currPosition.y && fabs(p_ShadowPlayer->prevPosition.y - p_Instance->prevPosition.y) >= (1.f - PLATFORM_SNAPPING_TOLERANCE)) {
						p_ShadowPlayer->currVelocity.y = -PLATFORM_MOVE_SPEED;

						if (p_ShadowPlayer->currPosition.y < p_Instance->currPosition.y) {
							b_IsShadowPlayerGrounded = true;
							doubleJumpCounter_S = 0;

							if (p_ShadowPlayer->currVelocity.y > 0) {
								p_ShadowPlayer->currVelocity.y = 0;
							}
						}

						if (p_ShadowPlayer->currPosition.y > p_Instance->currPosition.y + CRUSH_TOLERANCE) {
							if (p_ShadowPlayer->currVelocity.y < 0) {
								p_ShadowPlayer->currVelocity.y = PLATFORM_MOVE_SPEED;
							}
						}
					}
				}
			}
			else {
				isCollideMovingPlatform_S = false;
			}
			break;

		case Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK:
			Objects::pressurePlateResolution(p_Instance, p_ShadowPlayer, f_Time, true);
			Objects::pressurePlateResolution(p_Instance, p_LightPlayer, f_Time, false);
			break;

		case Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT:
			Objects::pressurePlateResolution(p_Instance, p_LightPlayer, f_Time, true);
			Objects::pressurePlateResolution(p_Instance, p_ShadowPlayer, f_Time, false);
			break;

		case Objects::TYPE_OBJECT_DOOR:
			Objects::doorResolution(p_Instance, p_LightPlayer);
			Objects::doorResolution(p_Instance, p_ShadowPlayer);
			Objects::doorExecuteSequence(p_Instance, p_LightPlayer, p_ShadowPlayer);
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
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevelDraw(void) {

	AEMtx33 CellTranslation{}, FinalCellTransform{};

	AEGfxSetBackgroundColor(0.1f, 0.1f, 0.4f);

	Render::TEXTURE bgMain;

	switch (GameStateManager::g_GameStateCurr) {
	default:
		bgMain = Render::TEXTURE::LEVEL_BASIC;
		break;

	case States::GS_TUTORIAL_LVL3:
	case States::GS_PLATFORM_LVL1:
		bgMain = Render::TEXTURE::LEVEL_BASIC2;
		break;
	}

	Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[3], bgMain, 0, 0, 0, 1);

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

				Render::TEXTURE Mid, Back, Fore;

				switch (GameStateManager::g_GameStateCurr) {
				default:
					Mid = Render::TEXTURE::TILE_MIDDLEGROUND;
					Back = Render::TEXTURE::TILE_BACKGROUND;
					Fore = Render::TEXTURE::TILE_FOREGROUND;
				break;
				case States::GS_TUTORIAL_LVL3:
				case States::GS_PLATFORM_LVL1:
				  Mid = Render::TEXTURE::TILE_MIDDLEGROUND2;
				  Back = Render::TEXTURE::TILE_BACKGROUND2;
				  Fore = Render::TEXTURE::TILE_FOREGROUND2;
				  break;
				}

				if (static_cast<s32>(i_BINARY_MAP_HEIGHT * 0.5f) == j)
					Render::RenderShapes(::p_BlackCell, FinalCellTransform, Mid);
				else if (static_cast<s32>(i_BINARY_MAP_HEIGHT * 0.5f) > j)
					Render::RenderShapes(::p_BlackCell, FinalCellTransform, Back);
				else
					Render::RenderShapes(::p_BlackCell, FinalCellTransform, Fore);
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

		switch (p_Instance->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_LIGHTPLAYER:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::LIGHT_PLAYER, 0.5f, 0.5f, 0.5f);
			break;

		case Objects::TYPE_OBJECT_SHADOWPLAYER:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::SHADOW_PLAYER, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_SPIKE:
		case Objects::TYPE_OBJECT_SPIKE_DOWN:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::SPIKE, 1.f, 0.f, 0.f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::MOVEABLE_SPIKE, 1.f, 0.f, 0.f);
			break;

		case Objects::TYPE_OBJECT_LIGHTSPIKE_DOWN:
		case Objects::TYPE_OBJECT_LIGHTSPIKE:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::SPIKE, 0.5f, 0.5f, 0.5f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::MOVEABLE_SPIKE, 0.5f, 0.5f, 0.5f);
			break;

		case Objects::TYPE_OBJECT_SHADOWSPIKE_DOWN:
		case Objects::TYPE_OBJECT_SHADOWSPIKE:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::SPIKE, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::MOVEABLE_SPIKE, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_LIGHTSHARD:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::TOKEN, 0.8f, 0.8f, 0.8f);
			break;

		case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::TOKEN, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::PLATFORM);
			break;

		case Objects::TYPE_OBJECT_DOOR:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::DOOR);
			break;

		case Objects::TYPE_OBJECT_LIGHT_DOOR:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::DOOR, 0.5f, 0.5f, 0.5f);
			break;

		case Objects::TYPE_OBJECT_SHADOW_DOOR:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::DOOR, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::PRESSURE_PLATE, 0.5f, 0.5f, 0.5f);
			break;

		case Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::PRESSURE_PLATE, -0.6f, -0.6f, -0.6f);
			break;

		case Objects::TYPE_OBJECT_TELEPORTOR:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::PORTAL);
			break;

		case Objects::TYPE_OBJECT_GRAVITYSWITCHER:
			Render::RenderShapes(p_Instance, FinalTransform, Render::TEXTURE::GRAVITY_TOKEN);
			break;

		default:
			Render::RenderShapes(p_Instance, FinalTransform);
			break;
		}
	}

	//Render wall last to hide objects in the scene
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
				Render::TEXTURE Floor;

				switch (GameStateManager::g_GameStateCurr) {
				default:
					Floor = Render::TEXTURE::TILE_FLOOR;
					break;

				case States::GS_TUTORIAL_LVL3:
				case States::GS_PLATFORM_LVL1:
					Floor = Render::TEXTURE::TILE_FLOOR2;
					break;
				}

				Render::RenderShapes(::p_WhiteCell, FinalCellTransform, Floor);
				break;

			default:
				break;
			}
		}
	}
	// game tips for tutorial levels
	switch (GameStateManager::g_GameStateCurr) {
	case States::GS_TUTORIAL_LVL1:
	case States::GS_TUTORIAL_LVL2:
	case States::GS_TUTORIAL_LVL3:
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[7], Render::TEXTURE::UI_TIPS, 0, 0, 0, 1);
		break;
	}

	f_TipsScreenTime += g_dt;
	f32 f_TipsProgress = max(0, f_TipsScreenTime - f_TipsScreenUnchangedTime);
	
	// Render text for each levels
	switch (GameStateManager::g_GameStateCurr) {
	case States::GS_TUTORIAL_LVL1:
		for (s32 i = 0; i < static_cast<s32>(TipsTutorial1.size()); ++i) {
			if (!TipsTutorial1[i].b_IsActive) continue;

			TipsTutorial1[i].f_Alpha = TipsTutorial1[i].f_Alpha - f_TipsProgress / f_MaxTipsSreenTime;
			Render::RenderFont(TipsTutorial1[i].s_Text, 1.25f, g_WindowWidth * 0.5f, g_WindowHeight * 0.1f, 1.f, 0.f, 0.f, TipsTutorial1[i].f_Alpha, Render::FONT_ALIGN::CENTER);

			if (TipsTutorial1[i].f_Alpha <= 0.f) {
				TipsTutorial1[i].f_Alpha = 1.f;
				TipsTutorial1[i].b_IsActive = false;
				f_TipsScreenTime = 0.f;

				TipsTutorial1[(i + 1) % TipsTutorial1.size()].b_IsActive = true;
			}
		}
		break;

	case States::GS_TUTORIAL_LVL2:
		for (s32 i = 0; i < static_cast<s32>(TipsTutorial2.size()); ++i) {
			if (!TipsTutorial2[i].b_IsActive) continue;

			TipsTutorial2[i].f_Alpha = TipsTutorial2[i].f_Alpha - f_TipsProgress / f_MaxTipsSreenTime;
			Render::RenderFont(TipsTutorial2[i].s_Text, 1.25f, g_WindowWidth * 0.5f, g_WindowHeight * 0.1f, 1.f, 0.f, 0.f, TipsTutorial2[i].f_Alpha, Render::FONT_ALIGN::CENTER);

			if (TipsTutorial2[i].f_Alpha <= 0.f) {
				TipsTutorial2[i].f_Alpha = 1.f;
				TipsTutorial2[i].b_IsActive = false;
				f_TipsScreenTime = 0.f;

				TipsTutorial2[(i + 1) % TipsTutorial2.size()].b_IsActive = true;
			}
		}
		break;

	case States::GS_TUTORIAL_LVL3:
		for (s32 i = 0; i < static_cast<s32>(TipsTutorial3.size()); ++i) {
			if (!TipsTutorial3[i].b_IsActive) continue;

			TipsTutorial3[i].f_Alpha = TipsTutorial3[i].f_Alpha - f_TipsProgress / f_MaxTipsSreenTime;
			Render::RenderFont(TipsTutorial3[i].s_Text, 1.25f, g_WindowWidth * 0.5f, g_WindowHeight * 0.1f, 1.f, 0.f, 0.f, TipsTutorial3[i].f_Alpha, Render::FONT_ALIGN::CENTER);

			if (TipsTutorial3[i].f_Alpha <= 0.f) {
				TipsTutorial3[i].f_Alpha = 1.f;
				TipsTutorial3[i].b_IsActive = false;
				f_TipsScreenTime = 0.f;

				TipsTutorial3[(i + 1) % TipsTutorial3.size()].b_IsActive = true;
			}
		}
		break;

	case States::GS_PLATFORM_LVL1:
	case States::GS_PLATFORM_LVL2:
	case States::GS_PLATFORM_LVL3:

		//const std::string s_Time = "Time Remaining: " + std::to_string(::i_Time);
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[8], Render::TEXTURE::UI_BASIC2, 0, 0, 0, 1);
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[9], Render::TEXTURE::UI_TIMER, 0, 0, 0, 1);
		const std::string s_Time = std::to_string(::i_Time);
		Render::RenderFont(s_Time, 1.8f, g_WindowWidth * 0.52f, g_WindowHeight * 0.9125f, Render::COLOUR::RED, Render::CENTER);

		if (b_TipActive) {
			for (s32 i = 0; i < static_cast<s32>(Tips.size()); ++i) {
				if (!Tips[i].b_IsActive) continue;

				Tips[i].f_Alpha = Tips[i].f_Alpha - f_TipsProgress / f_MaxTipsSreenTime;
				Render::RenderFont(Tips[i].s_Text, 1.25f, g_WindowWidth * 0.5f, g_WindowHeight * 0.1f, 1.f, 0.f, 0.f, Tips[i].f_Alpha, Render::FONT_ALIGN::CENTER);

				if (Tips[i].f_Alpha <= 0.f) {
					Tips[i].f_Alpha = 1.f;
					Tips[i].b_IsActive = false;
					f_TipsScreenTime = 0.f;

					Tips[(i + 1) % Tips.size()].b_IsActive = true;
				}
			}
		}
		break;
	}

	const std::string s_PlayersLives = std::to_string(::i_PlayerLives);
	const std::string s_LightShards = std::to_string(::i_TotalLightShard);
	const std::string s_ShadowFragments = std::to_string(::i_TotalShadowFragment);

	if (b_ToggleUI) {
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[2], Render::TEXTURE::UI_PLives, 0, 0, 0, 1);
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[4], Render::TEXTURE::UI_BASIC, 0, 0, 0, 1);
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[5], Render::TEXTURE::UI_SFrag, 0, 0, 0, 1);
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[6], Render::TEXTURE::UI_LShard, 0, 0, 0, 1);


		for (int b = 0; b < ::i_PlayerLives; b++) {
			Render::RenderShapes(Render::SHAPES::SQUARE, transformBGUI[b], Render::TEXTURE::UI_PLives, 0, 0, 0, 1);
		}

		Render::RenderFont(s_LightShards, 1.8f, g_WindowWidth * 0.125f, g_WindowHeight * 0.740f, Render::COLOUR::WHITE);
		Render::RenderFont(s_ShadowFragments, 1.8f, g_WindowWidth * 0.125f, g_WindowHeight * 0.81f, Render::COLOUR::BLACK);
	}
}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevelFree(void) {
	// kill all objects in the list
	for (u32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		Objects::GameObjectInstance* p_Instance = ::p_GameObjectInstanceList + i;
		if ((p_Instance->u_Flag & FLAG_ACTIVE) != 0) { // Check if instance is active
			if (p_Instance->sequenceCollection.sequence != nullptr) {
				delete[] p_Instance->sequenceCollection.sequence;
				p_Instance->sequenceCollection.sequence = nullptr; // Important: set to nullptr
			}
			gameObjectInstanceDestroy(p_Instance);
		}
	}

	for (auto& tip : Tips) {
		tip.b_IsActive = false;
	}

	for (auto& tip : TipsTutorial1) {
		tip.b_IsActive = false;
	}

	for (auto& tip : TipsTutorial2) {
		tip.b_IsActive = false;
	}

	for (auto& tip : TipsTutorial3) {
		tip.b_IsActive = false;
	}
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevelUnload(void) {
	delete[] ::p_GameObjectList;
	delete[] ::p_GameObjectInstanceList;

	FileReader::FreeMapData(::i_BINARY_MAP_WIDTH, ::p_MapData, ::p_BinaryCollisionArray);
}