#include"PowerUp.h"

PowerUp::PowerUp()
{
	this->powerBox.center = glm::vec4();
	this->powerBox.halfLengths = glm::vec4();
	this->typeOfPowerUp = PowerType::Heavy;
}
PowerUp::PowerUp(glm::vec4 center, glm::vec4 halflengths, PowerType type) {
	this->powerBox.center = center;
	this->powerBox.halfLengths = halflengths;
	this->typeOfPowerUp = type;
}
PowerUp::PowerUp(glm::vec4 center, glm::vec4 halflengths, int type) {
	this->powerBox.center = center;
	this->powerBox.halfLengths = halflengths;
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

void PowerUp::setType(PowerType type) {
	if (this->typeOfPowerUp != type) {
		this->typeOfPowerUp = type;
	}
	else {
		//Already has that type
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