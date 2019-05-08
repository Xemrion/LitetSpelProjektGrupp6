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
	//if (pos == endPos or pos == startPos) 
	//{
	//	Speed = -Speed;
	//}
	//pos -= (endPos-startPos)*(Speed/5);

	if (pos.x <= endPos.x or pos.x >= startPos.x) 
	{
		Speed.x = -Speed.x;
	}
	if (pos.y <= endPos.y or pos.y >= startPos.y) 
	{
		Speed.y = -Speed.y;
	}

	pos.x -= (endPos.x - startPos.x)*(Speed.x / 5.0);
	pos.y -= (endPos.y - startPos.y)*(Speed.y / 5.0);
}
