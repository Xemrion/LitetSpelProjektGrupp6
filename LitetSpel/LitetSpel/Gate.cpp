#include "Gate.h"

Gate::Gate(glm::vec4 center, glm::vec4 halfLengths, glm::vec4 color, float timerAdd, vec4 buttonCenter, vec4 buttonHalflengths)
{
	this->hitbox.center = center;
	this->hitbox.halfLengths = halfLengths;
	this->hitbox.color = color;
	this->timerAdd = timerAdd;
	this->button = button;
	isMoved = false;
	button.hitbox.center = buttonCenter;
	button.hitbox.halfLengths = buttonHalflengths;
	button.hitbox.color = color;
}

Gate::~Gate()
{
}

void Gate::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{

}

void Gate::move(float dt)
{
	if(button.isPressed && !isMoved)
	{
		this->hitbox.center.y -= hitbox.halfLengths.y * 2;
		isMoved = true;
		if (timerAdd > 0 && timer <= 0)
		{
			timer = timerAdd;
		}
	}
	else if (!button.isPressed && isMoved) 
	{
		this->hitbox.center.y += hitbox.halfLengths.y * 2;
		isMoved = false;
		button.hitbox.center.y += button.hitbox.halfLengths.y * 2;
		button.isMoved = false;
	}
	if(timerAdd > 0)
	{
		if (timer > 0)
		{
			timer -= dt/10000;
		}
		else if (button.isPressed && timer < 0) 
		{
			button.isPressed = false;
		}
	}

}
