#ifndef POWERUP_H
#define POWERUP_H
#include"Geometry.h"
#include"Library.h"

class PowerUp
{
public:
	PowerUp();
	PowerUp(glm::vec4 center, glm::vec4 halflengths, PowerType type);
	PowerUp(glm::vec4 center, glm::vec4 halflengths, int type);
	~PowerUp();

	Box getBox() const { return this->powerBox; }
	PowerType getType() const { return this->typeOfPowerUp; }
	int getTypeAsInt() const { return static_cast<int>(this->typeOfPowerUp); }

	void setType(PowerType type);
	void setTypeByInt(int type);
private:
	Box powerBox;
	PowerType typeOfPowerUp;
	//hitbox

};



#endif // !POWERUP_H