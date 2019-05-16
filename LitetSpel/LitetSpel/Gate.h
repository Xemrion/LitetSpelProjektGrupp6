#pragma once
#include "Button.h"

class Gate : public CollisionObject
{
public:
	Gate(glm::vec4 center,glm::vec2 halfLength, float timerAdd);
	virtual ~Gate();
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void move(float dt);

	Box hitbox;
	Button* button;
	float timer;
	float timerAdd;
	bool isMoved;

};

