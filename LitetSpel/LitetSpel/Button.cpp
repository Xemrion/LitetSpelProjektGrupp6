#include "Button.h"

Button::Button()
{
	isPressed = false;
	isMoved = false;
}

Button::Button(vec4 buttonCenter, vec2 halfLength, vec4 color, float timerAdd, int index)
{
	this->hitbox.center = buttonCenter;
	this->hitbox.halfLengths = vec4(halfLength, 10, 0);
	this->hitbox.color = color;
	this->index = index;
	isPressed = false;
	isMoved = false;
	this->timerAdd = timerAdd;
	this->timer = 0;
}

Button::~Button()
{
}

void Button::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{
	if (other.colliderType == ColliderType::player && (other.hitbox->center.y >= hitbox.center.y) or other.colliderType == ColliderType::blob && (other.hitbox->center.y >= hitbox.center.y))
	{
		isPressed = true;
		timer = timerAdd;
	}
}

void Button::move(float dt)
{
	if (isPressed && !isMoved)
	{
		this->hitbox.halfLengths.y /= 2;
		this->hitbox.center.y -= hitbox.halfLengths.y;
		isMoved = true;
		if (timerAdd > 0 && timer <= 0)
		{
			timer = timerAdd;
		}
		
	}
	if (timerAdd > 0)
	{
		if (timer > 0)
		{
			counter++;
			timer -= dt;
		}
		else if (isPressed && timer < 0)
		{
			isPressed = false;
			this->hitbox.center.y += hitbox.halfLengths.y;
			this->hitbox.halfLengths.y *= 2;
			isMoved = false;
		}
	}

}
