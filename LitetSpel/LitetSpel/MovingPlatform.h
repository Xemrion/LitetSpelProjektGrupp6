#pragma once

#include"Geometry.h"
#include "Collisions.h"
#include "Interfaces.h"
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

class MovingPlatform : public IMobile {
public:
	MovingPlatform( glm::vec3 startPos, glm::vec3 endPos, Box box ); // TODO: box
    virtual ~MovingPlatform() noexcept {};
	void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override {}; // stub
    [[nodiscard]] glm::vec3 moveFunction(double dt_s) const;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override;
    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override;
    virtual void updateGraphics() noexcept override {} // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub

private:
    Box box;
	float period; // time in seconds that the full path takes
    glm::vec3 startPos,
              endPos;
};
