#include "RenderHelper.h"
#include "GameObjects.h"
#include "main.h"
#include <string>

namespace Render {

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
			0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
		g_Shapes[SHAPES::TRIANGLE] = AEGfxMeshEnd();

		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
			0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

		AEGfxTriAdd(
			-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f,
			0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
			0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
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
		g_Texture[TEXTURE::COIN] = AEGfxTextureLoad("Assets/Art/Coin.png");

	}

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

	void InitRender(Objects::GameObject*& p_GameObjectList, u32 max) {


		for (u32 i{}; i < max; ++i) {
			Objects::GameObject* p_Object = p_GameObjectList + i;

			switch (p_Object->u_Type) {
			case Objects::TYPE_OBJECT_EMPTY:
			case Objects::TYPE_OBJECT_WALL:
			case Objects::TYPE_OBJECT_PLAYER:
			case Objects::TYPE_OBJECT_ENEMY:
			case Objects::TYPE_OBJECT_POWERUP:
			case Objects::TYPE_OBJECT_POWERUPEFFECT:
				p_Object->p_Mesh = g_Shapes[SHAPES::SQUARE];
				break;

			case Objects::TYPE_OBJECT_COIN:
				p_Object->p_Mesh = g_Shapes[SHAPES::CIRCLE];
				break;
			}
		}
	}

	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(transform.m);

		switch (p_Object->p_GameObject->u_Type) {
		case Objects::TYPE_OBJECT_PLAYER:
			AEGfxSetColorToAdd(0.f, 0.f, 1.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_COIN:
			AEGfxSetColorToAdd(1.f, 1.f, 0.f, 1.f);
			break;

		case Objects::TYPE_OBJECT_ENEMY:
			AEGfxSetColorToAdd(1.f, 0.f, 0.f, 1.f);
			break;
		}

		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
	}

	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, f32 r, f32 g, f32 b, f32 a) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
	}

	void Render::RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, TEXTURE tex, f32 r, f32 g, f32 b, f32 a){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
		AEGfxSetColorToAdd(r, g, b, a);
		AEGfxTextureSet(g_Texture[tex], 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_Object->p_GameObject->p_Mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxTextureSet(NULL, 0, 0);
	}

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

	void RenderFont(std::string const& text, f32 size, f32 x, f32 y, f32 r, f32 g, f32 b, f32 a, FONT_ALIGN alignment) {
		f32 f_TextWidth, f_TextHeight;
		AEGfxGetPrintSize(g_Font, text.c_str(), size, &f_TextWidth, &f_TextHeight);
		switch (alignment) {
		case LEFT:
			break;
		case CENTER:
			x = -f_TextWidth / 2.f;
			y = -f_TextHeight / 2.f;
			break;
		case RIGHT:
			x = -f_TextWidth;
			break;
		}

		AEGfxPrint(g_Font, text.c_str(), x, y, size, r, g, b, a);
	}
}
