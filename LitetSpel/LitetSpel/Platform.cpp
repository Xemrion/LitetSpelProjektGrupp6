#include"Platform.h"

Platform::Platform():
    IObject()
{
	this->hitbox.center;
	this->hitbox.halfLengths;
}

Platform::Platform(glm::vec4 center, glm::vec4 halfLengths,glm::vec3 color)
{
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLengths;
	this->hitbox.color =glm::vec4(color,0);
}

Platform::~Platform()
{
}