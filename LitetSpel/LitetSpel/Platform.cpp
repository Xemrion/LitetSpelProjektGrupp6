#include"Platform.h"

Platform::Platform():
    IObject()
{
	this->hitbox.center;
	this->hitbox.halfLengths;
}

Platform::Platform(glm::vec4 center, glm::vec4 halfLengths)
{
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLengths;
}

Platform::~Platform()
{
}