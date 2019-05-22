#include "Button.h"

Button::Button()
{
	isPressed = false;
	isMoved = false;
}

Button::Button(vec4 buttonCenter, vec2 halfLength, int index)
{
	this->hitbox.center = buttonCenter;
	this->hitbox.halfLengths = vec4(halfLength,10,0);
	this->hitbox.color = vec4(0.75f, 0.75f, 0.75f, 0);
	this->index = index;
	isPressed = false;
	isMoved = false;
}

Button::~Button()
{
}

void Button::collide(ColliderType ownHitbox, const HitboxEntry & other) noexcept
{
	if (other.colliderType == ColliderType::player && (other.hitbox->center.y >= hitbox.center.y) or other.colliderType == ColliderType::blob && (other.hitbox->center.y >= hitbox.center.y))
	{
		isPressed = true;
		if (!isMoved && isPressed)
		{
			hitbox.center.y -= (hitbox.halfLengths.y * 2)+0.1;
			isMoved = true;
		}
	}
}
