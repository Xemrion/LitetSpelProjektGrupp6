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
	bool gateMoveDown = false;
	bool gateMoveUp = true;                    
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons.at(i)->isMoved && !isMoved)
		{
			gateMoveDown = true;
		}
		else if (buttons.at(i)->isMoved && isMoved)
		{
			gateMoveUp = false;
		}

	}
	if (gateMoveDown)
	{
		this->hitbox.center.y -= hitbox.halfLengths.y * 2 + 0.01;
		isMoved = true;

	}
	else if(gateMoveUp && isMoved)
	{
		this->hitbox.center.y += hitbox.halfLengths.y * 2 + 0.01;
		isMoved = false;
	}
}
