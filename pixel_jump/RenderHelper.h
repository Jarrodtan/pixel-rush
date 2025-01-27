#ifndef RENDER_HELPER_HEADER
#define RENDER_HELPER_HEADER
#include <string>
#include "GameObjects.h"
#include <AEEngine.h>

namespace Render {

	void InitRender(Objects::GameObject*& p_ObjectList, u32 max);
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform);
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, f32 r, f32 g, f32 b, f32 a);
}

#endif // RENDER_HELPER_HEADER