#pragma once
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Globals.h"
#include "Collisions.h"

class Button : public IObject {
public:
	Button( Box box, int index );
	virtual ~Button();
	virtual void collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override {} // stub
    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override {} // stub
    virtual void updateGraphics() noexcept override {} // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
private:
    // IActivable target; TODO!
	int  index;
	bool isPressed,
         isMoved;
};
