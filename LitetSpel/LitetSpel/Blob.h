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
    enum class Status { none,
                        bouncy,
                        sticky,
                        heavy };

    Blob() = delete;
	explicit Blob( glm::vec3 const & );

	virtual void update(double dt) noexcept;

	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;

    void deactivateHitbox() noexcept;
    void reactivateHitbox() noexcept;
    virtual void updateHitboxes() noexcept;

    void absorb() noexcept;
    void shoot( glm::vec3 const &direction ) noexcept;
    void recall() noexcept;

    [[nodiscard]] bool getIsActive() const noexcept;
    [[nodiscard]] bool getIsBeingRecalled() const noexcept;
	[[nodiscard]] bool getIsStuck() const noexcept;

    // TODO: refactor into IActor?
    virtual Box const& getVolume() const noexcept {
        static Box volume { {0,0,0,0}, {3,3,3,0}, {0,1,0,0} };
        return volume;
    }

private:
    glm::vec3 const *parentPosition;
    glm::vec3        velocity;
    bool             isActive,
	                 isStuck,
                     isBeingRecalled;
    float            recallSpeed,
                     speed,
                     radius;
public:
    glm::vec3           position;
    std::vector<Hitbox> hitboxes;
    Sphere              blobSphere;
	Status              status;
};
