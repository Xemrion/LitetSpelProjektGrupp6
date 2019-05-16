#include "Button.h"

Button::Button()
{
	isPressed = false;
	isMoved = false;
}

Button::~Button()
{
}

void Button::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{
	if(other.colliderType == ColliderType::player && (other.hitbox->center.y >= hitbox.center.y))
	{
		isPressed = true;
		if (!isMoved && isPressed)
		{
			hitbox.center.y -= hitbox.halfLengths.y * 2;
			isMoved = true;
		}
	}
}
