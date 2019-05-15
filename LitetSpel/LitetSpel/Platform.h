#pragma once

#include"Geometry.h"
#include "Collisions.h"

class Platform : public CollisionObject  {
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths);
    virtual ~Platform() noexcept;
	//Box& getPlatformBox() const { return this->hitbox; };
    void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override {}
    Box hitbox;

private:
	//Hitbox
};
