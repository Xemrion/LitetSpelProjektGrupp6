#pragma once
#include "Button.h"

class Gate : public CollisionObject
{
public:
	Gate();
	Gate(glm::vec4 center,glm::vec2 halfLength,vec4 color, int index);
	virtual ~Gate();
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void move(float dt);

	int index;
	Box hitbox;
	std::vector<Button*> buttons;
	bool isMoved;
	                                                       
};