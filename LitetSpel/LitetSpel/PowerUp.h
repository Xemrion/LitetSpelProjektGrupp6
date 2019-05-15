#ifndef POWERUP_H
#define POWERUP_H
#include"Geometry.h"
#include "globals.h"
#include "Collisions.h"

class PowerUp : public CollisionObject
{
public:
	PowerUp();
	PowerUp(glm::vec4 center, PowerType type);
	PowerUp(glm::vec4 center, int type);
	~PowerUp();
	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept override;
	Box getBox() const { return this->hitbox; }
	PowerType getType() const { return this->typeOfPowerUp; }
	int getTypeAsInt() const { return static_cast<int>(this->typeOfPowerUp); }

	void setType(PowerType type);
	void setTypeByInt(int type);
	Box hitbox;
private:
	PowerType typeOfPowerUp;
	//hitbox

};



#endif // !POWERUP_H