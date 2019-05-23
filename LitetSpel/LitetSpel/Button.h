#pragma once
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Globals.h"
#include "Collisions.h"

using namespace glm;

class Button : public CollisionObject
{
public:
	Button();
	Button(vec4 buttonCenter, vec2 halfLength,vec4 color, float timerAdd ,int index);
	virtual ~Button();
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void move(float dt);

	int index;
	Box hitbox;
	bool isPressed;
	bool isMoved;
	float timer;
	float timerAdd;
};

