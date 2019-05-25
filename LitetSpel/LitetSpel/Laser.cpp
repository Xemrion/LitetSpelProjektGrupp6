#include "Laser.h"



Laser::Laser()
{
}

Laser::Laser(vec3 start, vec3 end, vec3 color, int index)
{
	visual.start = start;
	visual.end = end;
	visual.color = color;

	//hitbox.center = vec4(start,0);
	//hitbox.center = vec4(start + end/vec3(2), 0);
	hitbox.center = vec4((start.x + end.x) / 2, (start.y + end.y) / 2, 0, 0);
	//hitbox.halfLengths = vec4(0.1, 20, 5 ,0);
	//hitbox.halfLengths = vec4((start-end)/vec3(2),0);
	hitbox.halfLengths = vec4(vec3(
		(start.x - end.x) / 2 + 0.1,
		(start.y - end.y) / 2 + 0.1,
		0.1),
		0);
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
	bool activateLaser = false;
	bool deactivateLaser = false;
	for (int i = 0; i < buttons.size(); i++)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons.at(i)->isMoved && !isMoved)
			{
				deactivateLaser = true;
			}
			else if (!buttons.at(i)->isMoved && isMoved)
			{
				activateLaser = true;
			}
			buttons.at(i)->move(dt);

		}
		if (deactivateLaser)
		{
			hitbox.center = vec4(0, -1000, 0, 0);
			isMoved = true;

		}
		else if (activateLaser)
		{
			hitbox.center = vec4(visual.start - (visual.end / vec3(2)), 0);
			isMoved = false;
		}
		buttons.at(i)->move(dt);
	}

}
