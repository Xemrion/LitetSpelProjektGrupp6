#pragma once

#ifndef BLOB_H
#define BLOB_H

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"
#include "globals.h"
#include "Collisions.h"

class Blob : public CollisionObject {
public:
    Blob() = delete;
	explicit Blob( glm::vec3 const & );

	void update(double dt);

	void move(double dt);
	void setVelocity(glm::vec3 velocity, bool useSpeed = false);
	void addVelocity(glm::vec3 velocity, bool useSpeed = false);

	void collide(ColliderType ownType, ColliderType otherType, Box otherBox);

    void absorb() noexcept;
    void shoot( glm::vec3 const &direction ) noexcept;
    void recall() noexcept;

    bool getIsActive() const noexcept;
    bool getIsBeingRecalled() const noexcept;

private:
    glm::vec3 const *parentPosition;
    bool isActive;       
    bool isBeingRecalled;
    float recallSpeed;
    float speed;
    float radius;
    glm::vec3 velocity;
public:
    glm::vec3 pos;
    Box       hitbox;
    Sphere    blobSphere;
};
#endif // !BLOBS_H