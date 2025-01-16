#ifndef COLLISION_HEADER
#define COLLISION_HEADER

class AABB {
public:
	AEVec2 min;
	AEVec2 max;
};

bool CollisionAABB(const AABB& aabb1, const AEVec2& vel1, const AABB& aabb2, const AEVec2& vel2, f32& firstTimeOfCollision);

#endif // COLLISION_HEADER