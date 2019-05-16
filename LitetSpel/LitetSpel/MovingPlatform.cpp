#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(vec3 startPos, vec3 endPos, vec4 center, vec4 halfLength,vec4 color)
{
	this->startPos = startPos;
	this->pos = startPos;
	this->endPos = endPos;
	this->period = glm::length(endPos - startPos) / 20; // move 20 units per second by default
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLength;
	this->hitbox.color = color;
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::move(double physicsTime)
{
	pos = moveFunction(physicsTime);
	this->hitbox.center = vec4(pos, 0);
}

vec3 MovingPlatform::moveFunction(double time) const
{
	return startPos + abs(glm::mod((float)time / period, 2.0f) - 1.0f) * (endPos - startPos);
}
