#pragma once

#include"Geometry.h"
#include "Collisions.h"
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

using namespace glm;

class MovingPlatform : public CollisionObject
{
public:
	MovingPlatform(vec3 StartPos, vec3 endPos, vec4 center, vec4 halfLength);
	virtual ~MovingPlatform();
	void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override {};
	void move(double time);
	vec3 moveFunction(double time) const;

	vec3 pos;
	float period; // time in seconds that the full path takes
	vec3 startPos;
	vec3 endPos;

	Box hitbox;
};

