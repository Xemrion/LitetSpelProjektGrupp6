#pragma once

#include"Geometry.h"
#include "globals.h"
#include "Collisions.h"

class PowerUp : public IObject {
public:
	PowerUp(glm::vec3 position, PowerType type );
//	PowerUp(glm::vec4 center, int type); // wtf?
    virtual ~PowerUp() {};

	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
//	PowerType getType() const { return type; }  // wtf?
//	int getTypeAsInt() const { return static_cast<int>(type); }  // wtf?
//	void setType(PowerType type);
//	void setTypeByInt(int type); // wtf?
    virtual void updateLogic( double dt_s ) noexcept override {} // stub
    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override {} // stub
    virtual void updateGraphics() noexcept override {} // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
private:
	PowerType type;
};
