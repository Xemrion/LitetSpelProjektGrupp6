#pragma once

#include"Geometry.h"
#include "Collisions.h"

class DamagePlatform : public CollisionObject {
public:
	DamagePlatform();
	DamagePlatform(glm::vec4 center, glm::vec4 halfLengths, glm::vec4 color);
	virtual ~DamagePlatform() noexcept;
	//Box& getPlatformBox() const { return this->hitbox; };
	void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override {}
	Box hitbox;

private:
	//Hitbox
};
