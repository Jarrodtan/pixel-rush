/*******************************************************************/
/*!
	File:		Collision.h
	Project:	Pixel Rush
	Author(s):

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef COLLISION_HEADER
#define COLLISION_HEADER

namespace Collision {
	class AABB {
	public:
		AEVec2 min;
		AEVec2 max;
	};

	// Collision Functions
	s32 StaticCollision(const AABB& aabb1, const AABB& aabb2);
	bool CollisionAABB(const AABB& aabb1, const AEVec2& vel1, const AABB& aabb2, const AEVec2& vel2, f32& firstTimeOfCollision);

	// Map Collision Functions
	s32 GetCellValue(s32**& r_BinaryCollisionArray, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, s32 i_X, s32 i_Y);
	s32 CheckInstanceBinaryMapCollision(s32**& r_BinaryCollisionArray, f32 f_PosX, f32 f_PosY, f32 f_ScaleX, f32 f_ScaleY);
	void SnapToCell(f32* p_Coordinate);
} // Collision


#endif // COLLISION_HEADER