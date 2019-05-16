#pragma once
#include "Button.h"

class Gate : public CollisionObject
{
public:
	Gate(glm::vec4 center, glm::vec4 halfLengths, glm::vec4 color, float timerAdd, vec4 buttonCenter, vec4 buttonHalflengths);
	virtual ~Gate();
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void move(float dt);

	Box hitbox;
	Button button;
	float timer;
	float timerAdd;
	bool isMoved;

};

