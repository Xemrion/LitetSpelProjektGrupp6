#ifndef PLATFORM_H
#define PLATFORM_H
#include"Geometry.h"
#include "Collisions.h"

class Platform : public IObject  {
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths);
	~Platform();

	//Box& getPlatformBox() const { return this->hitbox; };

    void collide( CollisionId             ownHitbox,
                  CollisionId             otherHitbox,
                  IObject                &other,
                  CollisionManager const &collisionManager ) {}
    Box hitbox;

private:


	//Hitbox

	
};


#endif // !PLATFORM_H

