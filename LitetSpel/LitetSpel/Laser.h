#pragma once

#include "Button.h"


class Laser : public CollisionObject
{
public:
	Laser();
	Laser(vec3 start, vec3 end, vec3 color, int index);
	virtual ~Laser();
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void move(float dt);

	Line visual;
	Box hitbox;
	std::vector<Button*> buttons;
	bool isMoved;
	int index;

};

