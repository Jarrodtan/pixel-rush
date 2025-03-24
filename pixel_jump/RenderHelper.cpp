#include "RenderHelper.h"
#include "GameObjects.h"
#include "main.h"
#include <string>

namespace Render {
	/******************************************************************************/
	/*!
		Load function to load all the necessary resources for rendering.
		Call this function in main.cpp before the game loop.

		\param
			none

		\return
			none
	*/
	/******************************************************************************/
	void Load() {
		// load the font
		g_Font = AEGfxCreateFont("Assets/Fonts/Arial Italic.ttf", 30);

		// load the meshes of basic shapes
		g_Shapes = new AEGfxVertexList * [SHAPES::MAX_SHAPES];
		for (u32 i{}; i < SHAPES::MAX_SHAPES; ++i) {
			g_Shapes[i] = nullptr;
		}

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
			0.5f, -0.5f, 0xFF000000, 1.0f, 0.0f,
			0.f, 0.5f, 0xFF000000, 0.0f, 1.0f);
		g_Shapes[SHAPES::TRIANGLE] = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
		g_Shapes[SHAPES::SQUARE] = AEGfxMeshEnd();

		{
			AEGfxMeshStart();
			s32 numSlices = 32;
			AEVec2 vertex[3]{ 0.f,0.5f,0.f,0.5f,0.f,0.5f };
			for (s32 i = 1; i < numSlices + 1; ++i) {
				AEMtx33 ts{};
				AEMtx33Rot(&ts, 2 * PI / numSlices * i);
				AEMtx33MultVec(&vertex[1], &ts, &vertex[2]);
				AEGfxTriAdd(
					0.0f, 0.0f, 0xFF000000, 0.5f, 0.5f,
					vertex[0].x, vertex[0].y, 0xFF000000, vertex[0].x + 0.5f, vertex[0].y + 0.5f,
					vertex[1].x, vertex[1].y, 0xFF000000, vertex[1].x + 0.5f, vertex[1].y + 0.5f);
				memcpy(&vertex[0], &vertex[1], sizeof(vertex[0]));
			}
			g_Shapes[SHAPES::CIRCLE] = AEGfxMeshEnd();
		}

		g_Texture = new AEGfxTexture * [Render::MAX_TEXTURE];
		for (u32 i{}; i < TEXTURE::MAX_TEXTURE; ++i) {
			g_Texture[i] = nullptr;
		}

		// TODO: Load all textures here
		g_Texture[TEXTURE::SPLASHSCREEN] = AEGfxTextureLoad("Assets/Art/DigiPen_Singapore_WEB_WHITE.png");
		g_Texture[TEXTURE::FMODSCREEN] = AEGfxTextureLoad("Assets/Art/FMOD-Logo.png");
		g_Texture[TEXTURE::LIGHT_PLAYER] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Characters/tile_0002.png");
		g_Texture[TEXTURE::SHADOW_PLAYER] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Characters/tile_0002.png");
		g_Texture[TEXTURE::TILE_FLOOR] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0000.png"); // 0026
		g_Texture[TEXTURE::TILE_FOREGROUND] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0006.png");
		g_Texture[TEXTURE::TILE_MIDDLEGROUND] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0014.png");
		g_Texture[TEXTURE::TILE_BACKGROUND] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0022.png");
		g_Texture[TEXTURE::SPIKE] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0068.png");
		g_Texture[TEXTURE::MOVEABLE_SPIKE] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0180.png");
		g_Texture[TEXTURE::TOKEN]= AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0067.png");
		g_Texture[TEXTURE::PLATFORM] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0146.png");
		g_Texture[TEXTURE::PRESSURE_PLATE] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0107.png");
		g_Texture[TEXTURE::DOOR] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0095.png");
		g_Texture[TEXTURE::PORTAL] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0181.png");
		g_Texture[TEXTURE::GRAVITY_TOKEN] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0007.png");
		g_Texture[TEXTURE::TILE_FLOOR2] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/tile_0029.png");
		g_Texture[TEXTURE::TILE_FOREGROUND2] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0002.png");
		g_Texture[TEXTURE::TILE_MIDDLEGROUND2] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0009.png");
		g_Texture[TEXTURE::TILE_BACKGROUND2] = AEGfxTextureLoad("Assets/Art/kenney_pixel-platformer/Tiles/Backgrounds/tile_0018.png");
		g_Texture[TEXTURE::LEVEL_BASIC2] = AEGfxTextureLoad("Assets/Art/levelbasicbg6.png");
		g_Texture[TEXTURE::TITLE_MAINMENU] = AEGfxTextureLoad("Assets/Art/mainmenubg.png");
		g_Texture[TEXTURE::TITLE_CREDIT] = AEGfxTextureLoad("Assets/Art/creditbg.png");
		g_Texture[TEXTURE::TITLE_WIN] = AEGfxTextureLoad("Assets/Art/winbg.png");
		g_Texture[TEXTURE::TITLE_LOSE] = AEGfxTextureLoad("Assets/Art/losebg.png");
		g_Texture[TEXTURE::LEVEL_BASIC] = AEGfxTextureLoad("Assets/Art/levelbasicbg5.png");
		g_Texture[TEXTURE::UI_BASIC] = AEGfxTextureLoad("Assets/Art/uibasicbg3.png");
		g_Texture[TEXTURE::UI_PLives] = AEGfxTextureLoad("Assets/Art/uiplives.png");
		g_Texture[TEXTURE::UI_SFrag] = AEGfxTextureLoad("Assets/Art/uisfrag.png");
		g_Texture[TEXTURE::UI_LShard] = AEGfxTextureLoad("Assets/Art/uilshard.png");
		g_Texture[TEXTURE::TUTORIAL_INTRO] = AEGfxTextureLoad("Assets/Art/Tutorial_Intro.png");
		g_Texture[TEXTURE::TUTORIAL_TIPS] = AEGfxTextureLoad("Assets/Art/Tutorial_Tip.png");
		g_Texture[TEXTURE::UI_TIPS] = AEGfxTextureLoad("Assets/Art/uitips5.png");
		g_Texture[TEXTURE::UI_TIMER] = AEGfxTextureLoad("Assets/Art/uitimer.png");
		g_Texture[TEXTURE::UI_BASIC2] = AEGfxTextureLoad("Assets/Art/uibasicbg5.png");
	}

	/******************************************************************************/
	/*!
		Exit function to unload all the resources for rendering.
		Call this function in main.cpp after the game loop.

		\param
			none

		\return
			none
	*/
	/******************************************************************************/
	void Exit() {
		for (u32 i{}; i < SHAPES::MAX_SHAPES; ++i) {
			if (g_Shapes[i] == nullptr) continue;
			AEGfxMeshFree(g_Shapes[i]);
		}
		delete[] g_Shapes;

		AEGfxDestroyFont(g_Font);

		for (u32 i{}; i < TEXTURE::MAX_TEXTURE; ++i) {
			if (g_Texture[i] == nullptr) continue;
			AEGfxTextureUnload(g_Texture[i]);
		}
		delete[] g_Texture;
	}

	/******************************************************************************/
	/*!
		Function to initialize the rendering of the game objects.
		Assigns the appropriate mesh to the game objects.
		Call this function in the game state's init function.

		\param p_GameObjectList
			pointer to the game object list
		\param max
			maximum number of game objects

		\return
			none
	*/
	/******************************************************************************/
	void InitRender(Objects::GameObject*& p_GameObjectList, u32 max) {


		for (u32 i{}; i < max; ++i) {
			Objects::GameObject* p_Object = p_GameObjectList + i;

			switch (p_Object->u_Type) {
			case Objects::TYPE_OBJECT_EMPTY:
			case Objects::TYPE_OBJECT_WALL:
			case Objects::TYPE_OBJECT_LIGHTPLAYER:
			case Objects::TYPE_OBJECT_SHADOWPLAYER:
			case Objects::TYPE_OBJECT_GATE:
			case Objects::TYPE_OBJECT_SHADOW_DOOR:
			case Objects::TYPE_OBJECT_LIGHT_DOOR:
			case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			case Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT:
			case Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK:
			case Objects::TYPE_OBJECT_DOOR:
			case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
			case Objects::TYPE_OBJECT_SHADOWSPIKE:
			case Objects::TYPE_OBJECT_LIGHTSPIKE:
			case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
			case Objects::TYPE_OBJECT_SPIKE:
			case Objects::TYPE_OBJECT_LIGHTSHARD:
			case Objects::TYPE_OBJECT_TELEPORTOR:
			case Objects::TYPE_OBJECT_GRAVITYSWITCHER:
			case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
				p_Object->p_Mesh = g_Shapes[SHAPES::SQUARE];
				break;
			}
		}
	}

	/******************************************************************************/
	/*!
		Function to render the game objects.
		Call this function in the game state's draw function.
		Use this function if you want to render the game objects with the default colours.

		\param p_GameObjectInstanceList
			pointer to the game object instance list

		\param max
			maximum number of game object instances

		\return
			none
	*/
	/******************************************************************************/
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(transform.m);

		switch (p_Object->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_LIGHTPLAYER:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_SHADOWPLAYER:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);
			break;
		
		case Objects::TYPE_OBJECT_LIGHTSHARD:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_SHADOWFRAGMENT:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_PRESSURE_PLATE_LIGHT:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);
			break;
		case Objects::TYPE_OBJECT_PRESSURE_PLATE_DARK:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_GATE:
			AEGfxSetColorToAdd(0.5f, 0.5f, 0.95f, 1.f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SPIKE:
			AEGfxSetColorToAdd(1.f, 0.f, 0.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_SPIKE:
			AEGfxSetColorToAdd(0.75f, 0.75f, 0.75f, 1.f);
			break;

		case Objects::TYPE_OBJECT_MOVINGPLATFORM:
			AEGfxSetColorToAdd(0.2f, 0.3f, 0.8f, 1.f);
			break;

		case Objects::TYPE_OBJECT_LIGHTSPIKE:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_SHADOWSPIKE:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_LIGHT_DOOR:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_SHADOW_DOOR:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_TELEPORTOR:
			AEGfxSetColorToAdd(0.6f, 0.25f, 0.5f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_GRAVITYSWITCHER:
			AEGfxSetColorToAdd(0.1f, 0.8f, 0.7f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_LIGHTSPIKE:
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 0.6f);
			break;

		case Objects::TYPE_OBJECT_MOVEABLE_SHADOWSPIKE:
			AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.6f);
			break;
		case Objects::TYPE_OBJECT_DOOR:
			AEGfxSetColorToAdd(0.6f, 0.6f, 0.6f, 1.0f);
			break;
		}

		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
	}

	/******************************************************************************/
	/*!
		Function to render the game objects.
		Call this function in the game state's draw function.
		Use this function if you want to render the game objects with custom colours.

		\param p_GameObjectInstanceList
			pointer to the game object instance list

		\param max
			maximum number of game object instances

		\param r
			red colour component

		\param g
			green colour component

		\param b
			blue colour component

		\param a
			alpha colour component

		\return
			none
	*/
	/******************************************************************************/
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, f32 r, f32 g, f32 b, f32 a) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
	}

	/******************************************************************************/
	/*!
	Function to render the game objects.
	Call this function in the game state's draw function.
	Use this function if you want to render the game objects with textures.

	\param shape
		the shape of the mesh to render

	\param transform
		the transformation matrix to apply to the mesh

	\param tex
		texture to render the game object with

	\param r
		red colour component

	\param g
		green colour component

	\param b
		blue colour component

	\param a
		alpha colour component

	\return
		none
	*/
	/******************************************************************************/
	void RenderShapes(SHAPES shape, AEMtx33 trans, f32 r, f32 g, f32 b, f32 a) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxSetTransform(trans.m);
		AEGfxMeshDraw(g_Shapes[shape], AE_GFX_MDM_TRIANGLES);
	}

	/******************************************************************************/
	/*!
		Function to render the game objects.
		Call this function in the game state's draw function.
		Use this function if you want to render the game objects with textures.

		\param p_GameObjectInstanceList
			pointer to the game object instance list

		\param max
			maximum number of game object instances

		\param tex
			texture to render the game object with

		\return
			none
	*/
	/******************************************************************************/
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, TEXTURE tex, f32 r, f32 g, f32 b, f32 a){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxTextureSet(g_Texture[tex], 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxTextureSet(NULL, 0, 0);
	}

	/******************************************************************************/
	/*!
		Function to render the game objects.
		Call this function in the game state's draw function.
		Use this function if you want to render the game objects with textures.

		\param shape
			the shape of the mesh to render

		\param transform
			the transformation matrix to apply to the mesh

		\param tex
			texture to render the game object with

		\param r
			red colour component

		\param g
			green colour component

		\param b
			blue colour component

		\param a
			alpha colour component

		\return
			none
	*/
	/******************************************************************************/
	void RenderShapes(SHAPES shape, AEMtx33 transform, TEXTURE tex, f32 r, f32 g, f32 b, f32 a) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxTextureSet(g_Texture[tex], 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(g_Shapes[shape], AE_GFX_MDM_TRIANGLES);
		AEGfxTextureSet(NULL, 0, 0);
	}

	/******************************************************************************/
	/*!
		Function to render fonts for the game.
		Call this function to render text on the screen with the default colour.

		\param text
			text to render

		\param size
			size of the text

		\param x
			x position of the text

		\param y
			y position of the text

		\param colour
			colour of the text

		\param alignment
			align the text to the left, center, or right

		\return
			none
	*/
	/******************************************************************************/
	void RenderFont(std::string const& text, f32 size, f32 x, f32 y, COLOUR colour, FONT_ALIGN alignment) {
		f32 r{}, g{}, b{}, a = 0.75f;

		switch (colour) {
		case WHITE:
			r = g = b = 1.f;
			break;

		case RED:
			r = 1.f;
			b = g = 0.f;
			break;

		case LIGHT_BLUE:
			r = 0.5f;
			g = 0.8f;
			b = 1.f;
			break;

		case BLUE:
			r = g = 0.f;
			b = 1.f;
			break;

		case YELLOW:
			r = g = 0.75f;
			b = 0.f;
			break;

		case BLACK:
			r = g = b = 0.f;
			break;
		}

		RenderFont(text, size, x, y, r, g, b, a, alignment);
	}

	/******************************************************************************/
	/*!
		Function to render fonts for the game.
		Call this function to render text on the screen with custom colours.

		\param text
			text to render

		\param size
			size of the text

		\param x
			x position of the text

		\param y
			y position of the text

		\param r
			red colour component

		\param g
			green colour component

		\param b
			blue colour component

		\param a
			alpha colour component

		\param alignment
			align the text to the left, center, or right

		\return
			none
	*/
	/******************************************************************************/
	void RenderFont(std::string const& text, f32 size, f32 x, f32 y, f32 r, f32 g, f32 b, f32 a, FONT_ALIGN alignment) {
		f32 f_TextWidth, f_TextHeight;
		AEGfxGetPrintSize(g_Font, text.c_str(), size, &f_TextWidth, &f_TextHeight);
		x = (-g_WindowWidth + (x * 2)) / g_WindowWidth;
		y = (-g_WindowHeight + (y * 2)) / g_WindowHeight;
		switch (alignment) {
		case LEFT:
			break;
		case CENTER:
			x = x -f_TextWidth / 2.f;
			y = y -f_TextHeight / 2.f;
			break;
		case RIGHT:
			x = x -f_TextWidth;
			break;
		}

		AEGfxPrint(g_Font, text.c_str(), x, y, size, r, g, b, a);
	}
}
