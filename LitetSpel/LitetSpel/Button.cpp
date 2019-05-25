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
		this->hitbox.center.y -= (hitbox.halfLengths.y * 2) + 0.01;
		isMoved = true;
		if (timerAdd > 0 && timer <= 0)
		{
			timer = timerAdd;
		}
		
	}
	else if (!isPressed && isMoved)
	{
		this->hitbox.center.y += (hitbox.halfLengths.y * 2) + 0.01;
		isMoved = false;
	}
	if (timerAdd > 0)
	{
		if (timer > 0)
		{
			timer -= dt / 50000;
		}
		else if (isPressed && timer < 0)
		{
			isPressed = false;
		}
	}

}
