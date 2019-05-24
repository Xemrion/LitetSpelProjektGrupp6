#include "Laser.h"



Laser::Laser()
{
}

Laser::Laser(vec3 start, vec3 end, vec3 color, int index)
{
	visual.start = start;
	visual.end = end;
	visual.color = color;

	hitbox.center = vec4(start,0);
	hitbox.halfLengths = vec4(3, 20, 5 ,0);
	hitbox.color = vec4(color, 0);

	this->index = index;
	isMoved = false;
}


Laser::~Laser()
{
}

void Laser::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{
}

void Laser::move(float dt)
{
	if (button->isMoved && !isMoved)
	{
		//hitbox.center = vec4(0, -1000, 0, 0);
		//isMoved = true;
	}
	else if (!button->isMoved && isMoved)
	{
		//hitbox.center = vec4(visual.start - (visual.end / vec3(2)), 0);
		//isMoved = false;
	}
	button->move(dt);
}
