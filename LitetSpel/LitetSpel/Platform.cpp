#include"Platform.h"

Platform::Platform()
{
	this->platformBox.center;
	this->platformBox.halfLengths;
}

Platform::Platform(glm::vec4 center, glm::vec4 halfLengths)
{
	this->platformBox.center = center;
	this->platformBox.halfLengths = halfLengths;
}

Platform::~Platform()
{
}