#include "main.h"

/**************************************************************************/
/*!
	This function check for static collision between 2 rectangles.
*/
/**************************************************************************/
s32 static_collision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb2.max.x < aabb1.min.x)return 0;
	if (aabb2.min.x > aabb1.max.x)return 0;
	if (aabb2.max.y < aabb1.min.y)return 0;
	if (aabb2.min.y > aabb1.max.y)return 0;

	return 1;
}

