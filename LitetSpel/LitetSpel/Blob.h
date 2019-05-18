#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"
#include "globals.h"
#include "Collisions.h"
#include "Interfaces.h"

// TODO: improve encapsulation by reducing public exposure

class Blob : public IActor {
public:
    Blob() = delete;
	explicit Blob( glm::vec3 const & );

    virtual void updateGraphics() noexcept override;
    virtual void updateHitboxes() noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override;
    virtual void updatePhysics( double dt_s ) noexcept override;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override;

    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;


	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;

    void deactivateHitbox() noexcept;
    void reactivateHitbox() noexcept;

    void absorb() noexcept;
    void shoot( glm::vec3 const &direction ) noexcept;
    void recall() noexcept;

    [[nodiscard]] bool getIsActive() const noexcept;
    [[nodiscard]] bool getIsBeingRecalled() const noexcept;
	[[nodiscard]] bool getIsStuck() const noexcept;
    [[nodiscard]] Sphere const *getSphere() const noexcept;

    // TODO: refactor into IActor?
    virtual Box const& getVolume() const noexcept {
        static Box volume { {0,0,0,0}, {3,3,3,0}, {0,1,0,0} };
        return volume;
    }

    virtual void die() noexcept override;

private:
    glm::vec3 const *parentPosition;
    bool             isActive,
	                 isStuck,
                     isBeingRecalled;
    float            recallSpeed,
                     radius;
public:
    // TODO: refactor away:
    Sphere           blobSphere;
	PowerType        status;
};
