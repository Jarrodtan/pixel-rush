#include "RenderHelper.h"
#include "GameObjects.h"
#include <string>

namespace Render {
	void InitRender(Objects::GameObject*& p_GameObjectList, u32 max) {
		for (u32 i{}; i < max; ++i) {
			Objects::GameObject* p_Object = p_GameObjectList + i;

			switch (p_Object->u_Type) {
			case Objects::TYPE_OBJECT_EMPTY:
			case Objects::TYPE_OBJECT_WALL:
			case Objects::TYPE_OBJECT_PLAYER:
			case Objects::TYPE_OBJECT_ENEMY:
				AEGfxMeshStart();
				AEGfxTriAdd(
					-0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
					0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
					-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

				AEGfxTriAdd(
					-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f,
					0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
					0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
				p_Object->p_Mesh = AEGfxMeshEnd();
				break;

			case Objects::TYPE_OBJECT_COIN:
				{
					AEGfxMeshStart();
					s32 numSlices = 32;
					AEVec2 vertex[3]{ 0.f,0.5f,0.f,0.5f,0.f,0.5f };
					for (s32 i = 1; i < numSlices; ++i) {
						AEMtx33 ts{};
						AEMtx33Rot(&ts, 2 * PI / numSlices * i);
						AEMtx33MultVec(&vertex[1], &ts, &vertex[2]);
						AEGfxTriAdd(
							0.0f, 0.0f, 0xFF000000, 0.5f, 0.5f,
							vertex[0].x, vertex[0].y, 0xFF000000, vertex[0].x + 0.5f, vertex[0].y + 0.5f,
							vertex[1].x, vertex[1].y, 0xFF000000, vertex[1].x + 0.5f, vertex[1].y + 0.5f);
						memcpy(&vertex[0], &vertex[1], sizeof(vertex[0]));
					}
					p_Object->p_Mesh = AEGfxMeshEnd();
				}
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
}
