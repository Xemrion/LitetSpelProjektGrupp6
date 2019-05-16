#include "Gate.h"

Gate::Gate(glm::vec4 center,glm::vec2 halfLength, float timerAdd)
{
	this->hitbox.center = center;
	this->hitbox.halfLengths = vec4(halfLength,0,0);
	this->hitbox.color = vec4(1,0,1,0);
	this->timerAdd = timerAdd;
	this->button = button;
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
	if(button->isPressed && !isMoved)
	{
		this->hitbox.center.y -= hitbox.halfLengths.y * 2;
		isMoved = true;
		if (timerAdd > 0 && timer <= 0)
		{
			timer = timerAdd;
		}
	}
	else if (!button->isPressed && isMoved)
	{
		this->hitbox.center.y += hitbox.halfLengths.y * 2;
		isMoved = false;
		button->hitbox.center.y += button->hitbox.halfLengths.y * 2;
		button->isMoved = false;
	}
	if(timerAdd > 0)
	{
		if (timer > 0)
		{
			timer -= dt/10000;
		}
		else if (button->isPressed && timer < 0)
		{
			button->isPressed = false;
		}
	}

}
