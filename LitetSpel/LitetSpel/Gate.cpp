#include "Gate.h"

Gate::Gate()
{
}

Gate::Gate(glm::vec4 center, glm::vec2 halfLength, vec4 color, int index)
{
	this->index = index;
	this->hitbox.center = center;
	this->hitbox.halfLengths = vec4(halfLength, 10, 0);
	this->hitbox.color = color;
	isMoved = false;
}

Gate::~Gate()
{
}

void Gate::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{

}

void Gate::move(float dt)
{
	if(button->isMoved && !isMoved)
	{
		this->hitbox.center.y -= hitbox.halfLengths.y * 2 + 0.01;
		isMoved = true;
	}
	else if(!button->isMoved && isMoved)
	{
		this->hitbox.center.y += hitbox.halfLengths.y * 2 + 0.01;
		isMoved = false;
	}
	button->move(dt);
}
