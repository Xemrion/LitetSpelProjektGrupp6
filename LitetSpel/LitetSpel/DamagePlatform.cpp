#include"DamagePlatform.h"

DamagePlatform::DamagePlatform() :
	CollisionObject()
{
	this->hitbox.center;
	this->hitbox.halfLengths;

}

DamagePlatform::DamagePlatform(glm::vec4 center, glm::vec4 halfLengths, glm::vec4 color)
{
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLengths;
	this->hitbox.color = color;
}

DamagePlatform::~DamagePlatform() {}
