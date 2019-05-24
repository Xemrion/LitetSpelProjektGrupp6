#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"
#include "globals.h"
#include "Collisions.h"

// TODO: improve encapsulation by reducing public exposure

enum BlobStatus {
	Blob_None,
	Blob_Bouncy,
	Blob_Sticky,
	Blob_Heavy
};

class Blob : public CollisionObject {
public:
    Blob() = delete;
	explicit Blob( glm::vec3 const & );

	void update(double dt) noexcept;

	void move(double dt) noexcept;
	void setVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void addVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void followPlayer();

	void collide(ColliderType ownType, const HitboxEntry& other) noexcept override;

    void deactivateHitbox() noexcept;
    void reactivateHitbox() noexcept;

    void absorb() noexcept;
    void shoot( glm::vec3 const &direction ) noexcept;
    void recall() noexcept;

    [[nodiscard]] bool getIsActive() const noexcept;
    [[nodiscard]] bool getIsBeingRecalled() const noexcept;
	[[nodiscard]] bool getIsStuck() const noexcept;

private:
    glm::vec3 const *parentPosition;
	glm::vec3 followParentSpeed = glm::vec3(15.0, 15.0, 1.0);
	glm::vec3 offsetFromParent;
    bool  isActive;
	bool isStuck;
    bool  isBeingRecalled;
	bool isLanding;
    float recallSpeed;
    float speed;
    float radius;
    glm::vec3 velocity;
public:
    glm::vec3 pos;
    Box       hitbox;
    Sphere    blobSphere;
	int status;
};
