#ifndef PLATFORM_H
#define PLATFORM_H
#include"Geometry.h"

class Platform
{
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths);
	~Platform();

	Box getPlatformBox() const { return this->platformBox; };
private:

	Box platformBox;
	//Hitbox

	
};


#endif // !PLATFORM_H

