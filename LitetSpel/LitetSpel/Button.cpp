#include "Button.h"

Button::Button()
{
	isPressed = false;
	isMoved = false;
}

Button::Button(vec4 buttonCenter)
{
	this->hitbox.center = buttonCenter;
	this->hitbox.halfLengths = vec4(1, 1, 0, 0);
	this->hitbox.color = vec4(1, 1, 1, 1);
}

Button::~Button()
{
}

void Button::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{
	if (other.colliderType == ColliderType::player && (other.hitbox->center.y >= hitbox.center.y))
	{
		isPressed = true;
		if (!isMoved && isPressed)
		{
			hitbox.center.y -= hitbox.halfLengths.y * 2;
			isMoved = true;
		}
	}
}
