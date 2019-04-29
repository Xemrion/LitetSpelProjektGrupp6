#ifndef PLATFORM_H
#define PLATFORM_H
#include"Geometry.h"
#include "Collisions.h"

class Platform : public IObject  {
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths, glm::vec4 color = glm::vec4(0,0.5f,0.5f,0));
	~Platform();

	//Box& getPlatformBox() const { return this->hitbox; };

    void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) {}
    Box hitbox;

private:


	//Hitbox

	
};


#endif // !PLATFORM_H

