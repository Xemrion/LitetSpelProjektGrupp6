#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(vec3 StartPos, vec3 endPos)
{
	this->startPos = StartPos;
	this->pos = StartPos;
	this->endPos = endPos;
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
