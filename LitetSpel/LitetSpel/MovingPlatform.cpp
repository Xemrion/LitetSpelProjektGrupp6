#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(vec3 StartPos, vec3 endPos, vec3 center, vec3 halfLength)
{
	this->startPos = StartPos;
	this->pos = StartPos;
	this->endPos = endPos;
	this->Hitbox.center = vec4(center,0);
	this->Hitbox.halfLengths = vec4(halfLength, 0);
	this->Hitbox.color = vec4(0, 1, 1, 0);
	Speed = vec2(0.05, 0.05);
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::move()
{
	if (endPos.x < startPos.x) 
	{
		if (pos.x <= endPos.x or pos.x >= startPos.x)
		{
			Speed.x = -Speed.x;
		}
	}
	else 
	{
		if (pos.x >= endPos.x or pos.x<= startPos.x)
		{
			Speed.x = -Speed.x;
		}
	}
	if (endPos.y < startPos.y)
	{
		if (pos.y <= endPos.y or pos.y >= startPos.y)
		{
			Speed.y = -Speed.y;
		}
	}
	else
	{
		if (pos.y >= endPos.y or pos.y <= startPos.y)
		{
			Speed.y = -Speed.y;
		}
	}

	pos.x -= (endPos.x - startPos.x)*(Speed.x / 5.0);
	pos.y -= (endPos.y - startPos.y)*(Speed.y / 5.0);
}
