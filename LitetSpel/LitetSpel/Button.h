#pragma once
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Globals.h"
#include "Collisions.h"

// TODO: allow for multiple targets
class Button : public IObject {
public:
    Button( Box box, double activeTime_s, std::vector<IActivable*> targets={} );
	virtual ~Button();

    void link( IActivable *target ) noexcept;

	virtual void collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override; // stub
    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override;
    virtual void updateGraphics() noexcept override {} // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    void trigger() noexcept;

private:
    std::vector<IActivable*> targets;

	double activeTime_s,
           timer_s;

    float downFactor;
};
