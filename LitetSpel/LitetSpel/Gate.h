#pragma once
#include "Button.h"

class Gate : public IActivable { // TODO: IActivable
public:
	Gate( Box box );
	virtual ~Gate();

	virtual void collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept override;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override;

    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override; // stub
    virtual void updateGraphics() noexcept override {}; // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub

private:
    float openFactor;
};
