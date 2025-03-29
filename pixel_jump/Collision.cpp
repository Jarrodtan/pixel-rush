/*******************************************************************/
/*!
	File:		Collision.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		Game Collision Function to use such as AABB collision
				and dynamic collision system.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

namespace Collision {
	/**************************************************************************/
	/*!
		This function check for static collision between 2 rectangles.
	*/
	/**************************************************************************/
	s32 StaticCollision(const AABB& aabb1, const AABB& aabb2) {
		if (aabb2.max.x < aabb1.min.x)return 0;
		if (aabb2.min.x > aabb1.max.x)return 0;
		if (aabb2.max.y < aabb1.min.y)return 0;
		if (aabb2.min.y > aabb1.max.y)return 0;

		return 1;
	}

	void SnapToCell(f32* p_Coordinate) {
		s32 temp = static_cast<s32>(*p_Coordinate);
		*p_Coordinate = static_cast<f32>(temp);
		*p_Coordinate += 0.5f;
	}

	s32 GetCellValue(s32**& r_BinaryCollisionArray, 
					 s32& r_BINARY_MAP_WIDTH,
					 s32& r_BINARY_MAP_HEIGHT, 
					 s32 i_X,
					 s32 i_Y) {

		if (i_X >= r_BINARY_MAP_WIDTH || i_X < 0) {
			return 0;
		}

		if (i_Y >= r_BINARY_MAP_HEIGHT || i_Y < 0) {
			return 0;
		}
	
		return r_BinaryCollisionArray[i_X][i_Y];
	}

	s32 CheckInstanceBinaryMapCollision(s32**& r_BinaryCollisionArray,
										f32 f_PosX,
										f32 f_PosY,
										f32 f_ScaleX,
										f32 f_ScaleY) {

		//RIGHT hotspot 1
		f32 f_Rx1 = f_PosX + f_ScaleX / 2;	//To reach the right side
		f32 f_Ry1 = f_PosY + f_ScaleY / 4;	//To go up 1/4 of the height

		//RIGHT hotspot 2
		f32 f_Rx2 = f_PosX + f_ScaleX / 2;	//To reach the right side
		f32 f_Ry2 = f_PosY - f_ScaleY / 4;	//To go down 1/4 of the height

		//LEFT hotspot 1
		f32 f_Lx1 = f_PosX - f_ScaleX / 2;	//To reach the left side
		f32 f_Ly1 = f_PosY + f_ScaleY / 4;	//To go up 1/4 of the height

		//LEFT hotspot 2
		f32 f_Lx2 = f_PosX - f_ScaleX / 2;	//To reach the left side
		f32 f_Ly2 = f_PosY - f_ScaleY / 4;	//To go down 1/4 of the height

		//TOP hotspot 1
		f32 f_Tx1 = f_PosX - f_ScaleX / 4;	//To go left 1/4 of the center
		f32 f_Ty1 = f_PosY + f_ScaleY / 2;	//To reach the top side

		//TOP hotspot 2
		f32 f_Tx2 = f_PosX + f_ScaleX / 4;	//To go right 1/4 of the center
		f32 f_Ty2 = f_PosY + f_ScaleY / 2;	//To reach the top side

		//BOTTOM hotspot 1
		f32 f_Bx1 = f_PosX - f_ScaleX / 4;	//To go left 1/4 of the center
		f32 f_By1 = f_PosY - f_ScaleY / 2;	//To reach the bottom side

		//BOTTOM hotspot 2
		f32 f_Bx2 = f_PosX + f_ScaleX / 4;	//To go right 1/4 of the center
		f32 f_By2 = f_PosY - f_ScaleY / 2;	//To reach the bottom side

		s32 FLAG{};

		// RIGHT
		if (r_BinaryCollisionArray[static_cast<s32>(f_Rx1)][static_cast<s32>(f_Ry1)] == 1) {
			FLAG += COLLISION_RIGHT;
		}
		else if (r_BinaryCollisionArray[static_cast<s32>(f_Rx2)][static_cast<s32>(f_Ry2)] == 1) {
			FLAG += COLLISION_RIGHT;
		}

		// LEFT
		if (r_BinaryCollisionArray[static_cast<s32>(f_Lx1)][static_cast<s32>(f_Ly1)] == 1) {
			FLAG += COLLISION_LEFT;
		}
		else if (r_BinaryCollisionArray[static_cast<s32>(f_Lx2)][static_cast<s32>(f_Ly2)] == 1) {
			FLAG += COLLISION_LEFT;
		}

		// TOP
		if (r_BinaryCollisionArray[static_cast<s32>(f_Tx1)][static_cast<s32>(f_Ty1)] == 1) {
			FLAG += COLLISION_TOP;
		}
		else if (r_BinaryCollisionArray[static_cast<s32>(f_Tx2)][static_cast<s32>(f_Ty2)] == 1) {
			FLAG += COLLISION_TOP;
		}

		// BOTTOM
		if (r_BinaryCollisionArray[static_cast<s32>(f_Bx1)][static_cast<s32>(f_By1)] == 1) {
			FLAG += COLLISION_BOTTOM;
		}
		else if (r_BinaryCollisionArray[static_cast<s32>(f_Bx2)][static_cast<s32>(f_By2)] == 1) {
			FLAG += COLLISION_BOTTOM;
		}

		// If there is a collision, return the flag
		if (FLAG > 0) {
			return FLAG;
		}

		// No Collision
		return 0;
	}

	bool CollisionAABB(const AABB& aabb1,
					   const AEVec2& vel1, 
					   const AABB& aabb2,
					   const AEVec2& vel2, 
					   f32& firstTimeOfCollision) {

		if (StaticCollision(aabb1, aabb2) == 1) {
			return true;
		}

		AEVec2 RelativeVelocity{};
		f32* p_Tfirst{}, p_Tlast{};

		RelativeVelocity.x = vel1.x - vel2.x;
		RelativeVelocity.y = vel1.y - vel2.y;

		p_Tfirst = &firstTimeOfCollision;
		*p_Tfirst = 0.f;
		p_Tlast = static_cast<f32>(AEFrameRateControllerGetFrameTime());

		if (RelativeVelocity.x < 0) {
			if (aabb2.min.x > aabb1.max.x) return 0;

			if (aabb2.max.x < aabb1.min.x) {
				*p_Tfirst = max((aabb2.max.x - aabb1.min.x) / RelativeVelocity.x, *p_Tfirst);

			}
			if (aabb2.min.x < aabb1.max.x) {
				p_Tlast = min((aabb2.min.x - aabb1.max.x) / RelativeVelocity.x, p_Tlast);
			}

		}
		else if (RelativeVelocity.x > 0) {
			if (aabb2.min.x > aabb1.max.x) {
				*p_Tfirst = max((aabb2.min.x - aabb1.max.x) / RelativeVelocity.x, *p_Tfirst);
			}
			if (aabb2.max.x > aabb1.min.x) {
				p_Tlast = min((aabb2.max.x - aabb1.min.x) / RelativeVelocity.x, p_Tlast);
			}
			if (aabb2.max.x < aabb1.min.x)return 0;
		}
		else if (RelativeVelocity.x == 0) {
			if (aabb2.max.x < aabb1.min.x) return 0;
			else if (aabb2.min.x > aabb1.max.x) return 0;
		}

		if (*p_Tfirst > p_Tlast)return 0;

		/*Dynamic Collision for y axis*/
		if (RelativeVelocity.y < 0) {
			if (aabb2.min.y > aabb1.max.y) return 0;
			if (aabb2.max.y < aabb1.min.y) {
				*p_Tfirst = max((aabb2.max.y - aabb1.min.y) / RelativeVelocity.y, *p_Tfirst);
			}
			if (aabb2.min.y < aabb1.max.y) {
				p_Tlast = min((aabb2.min.y - aabb1.max.y) / RelativeVelocity.y, p_Tlast);
			}
		}
		else if (RelativeVelocity.y > 0) {
			if (aabb2.min.y > aabb1.max.y) {
				*p_Tfirst = max((aabb2.min.y - aabb1.max.y) / RelativeVelocity.y, *p_Tfirst);
			}
			if (aabb2.max.y > aabb1.min.y) {
				p_Tlast = min((aabb2.max.y - aabb1.min.y) / RelativeVelocity.y, p_Tlast);
			}
			if (aabb2.max.y < aabb1.min.y)return 0;
		}
		else if (RelativeVelocity.y == 0) {
			if (aabb2.max.y < aabb1.min.y) return 0;
			else if (aabb2.min.y > aabb1.max.y) return 0;
		}

		if (*p_Tfirst > p_Tlast)return 0;

		return 1;

	}

} // namespace Collision