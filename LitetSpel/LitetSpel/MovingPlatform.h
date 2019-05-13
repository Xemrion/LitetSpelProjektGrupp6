#pragma once

#include"Geometry.h"
#include "Collisions.h"
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

using namespace std; // läs: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rs-using-directive
using namespace glm;

class MovingPlatform : public CollisionObject
{
public:
	MovingPlatform(vec3 StartPos, vec3 endPos, vec3 center, vec3 halfLength);
	virtual ~MovingPlatform();
	void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept override {};
	void move();

	vec2 Speed;
	vec3 pos;
	vec3 startPos;
	vec3 endPos;

	Box hitbox;
};

