#ifndef PLATFORM_H
#define PLATFORM_H
#include"Geometry.h"
#include "Collisions.h"

class Platform : public CollisionObject  {
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths);
	~Platform();

	//Box& getPlatformBox() const { return this->hitbox; };

    void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box other) {}
    Box hitbox;

private:


	//Hitbox

	
};


#endif // !PLATFORM_H

