#include"PowerUp.h"

PowerUp::PowerUp()
{
	this->hitbox.center = glm::vec4();
	this->hitbox.halfLengths = glm::vec4();
	this->typeOfPowerUp = PowerType::Heavy;
}
PowerUp::PowerUp(glm::vec4 center, PowerType type) {
	this->hitbox.center = center;
	this->hitbox.halfLengths = glm::vec4(1,1,0,0);
	this->typeOfPowerUp = type;
}
PowerUp::PowerUp(glm::vec4 center, int type) {
	this->hitbox.center = center;
	this->hitbox.halfLengths = glm::vec4(1,1,0,0);
	if (static_cast<PowerType>(type) == PowerType::Bouncy ||
		static_cast<PowerType>(type) == PowerType::Heavy ||
		static_cast<PowerType>(type) == PowerType::Sticky) {
		this->typeOfPowerUp = static_cast<PowerType>(type);
	}
	else {
		this->typeOfPowerUp = PowerType::Heavy;
	}
}
PowerUp::~PowerUp()
{
}

void PowerUp::collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept
{
}

void PowerUp::setType(PowerType type) {
	if (type != PowerType::None) {
		if (this->typeOfPowerUp != type) {
			this->typeOfPowerUp = type;
		}
		else {
			//Already has that type
		}
	}
	else {
		//Cannot set power type to none
	}
}
void PowerUp::setTypeByInt(int type) {
	if (static_cast<PowerType>(type) == PowerType::Bouncy ||
		static_cast<PowerType>(type) == PowerType::Heavy ||
		static_cast<PowerType>(type) == PowerType::Sticky) {
		if (this->typeOfPowerUp != static_cast<PowerType>(type)) {
			this->typeOfPowerUp = static_cast<PowerType>(type);
		}
		else {
			//Already has that type
		}
	}
	else {
		if (this->typeOfPowerUp != PowerType::Heavy) {
			this->typeOfPowerUp = PowerType::Heavy;
		}
		else {
			//Already Heavy
		}
	}
}