#include"PowerUp.h"

PowerUp::PowerUp()
{
	this->powerBox.center = glm::vec4();
	this->powerBox.halfLengths = glm::vec4();
	this->typeOfPowerUp = PowerType::heavy;
}
PowerUp::PowerUp(glm::vec4 center, glm::vec4 halflengths, PowerType type) {
	this->powerBox.center = center;
	this->powerBox.halfLengths = halflengths;
	this->typeOfPowerUp = type;
}
PowerUp::PowerUp(glm::vec4 center, glm::vec4 halflengths, int type) {
	this->powerBox.center = center;
	this->powerBox.halfLengths = halflengths;
	if (static_cast<PowerType>(type) == PowerType::bouncy ||
		static_cast<PowerType>(type) == PowerType::heavy ||
		static_cast<PowerType>(type) == PowerType::sticky) {
		this->typeOfPowerUp = static_cast<PowerType>(type);
	}
	else {
		this->typeOfPowerUp = PowerType::heavy;
	}
}
PowerUp::~PowerUp()
{
}

void PowerUp::setType(PowerType type) {
	if (type != PowerType::none) {
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
	if (static_cast<PowerType>(type) == PowerType::bouncy ||
		static_cast<PowerType>(type) == PowerType::heavy ||
		static_cast<PowerType>(type) == PowerType::sticky) {
		if (this->typeOfPowerUp != static_cast<PowerType>(type)) {
			this->typeOfPowerUp = static_cast<PowerType>(type);
		}
		else {
			//Already has that type
		}
	}
	else {
		if (this->typeOfPowerUp != PowerType::heavy) {
			this->typeOfPowerUp = PowerType::heavy;
		}
		else {
			//Already Heavy
		}
	}
}