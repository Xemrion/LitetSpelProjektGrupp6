#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(vec3 startPos, vec3 endPos, vec4 center, vec4 halfLength)
{
	this->startPos = startPos;
	this->pos = startPos;
	this->endPos = endPos;
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLength;
	this->hitbox.color = vec4(0, 1, 1, 0);
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::move(double gameTime)
{
	pos = startPos + abs(glm::mod((float)gameTime, period) - period * 0.5f) * (endPos - startPos);
	this->hitbox.center = vec4(pos, 0);
	this->hitbox.color = vec4(1.0, 1.0, 0.0, 0.0);
}
