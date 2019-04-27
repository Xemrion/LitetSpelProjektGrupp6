#ifndef BLOB_H
#define BLOB_H
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"
#include "globals.h"
#include "Collisions.h"

class Blob {
private:
	float recallSpeed = 200.0f;
	float speed = 100.1f;
	float radius = 2;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	Box hitbox;
public:
	bool isActive = false;
	bool isBeingRecalled = false;
	Sphere blobSphere;
	Blob();
	~Blob();
	Blob(glm::vec3 pos);
	glm::vec3 pos;
	void update(float dt);
	void move(float dt);
	void setVelocity(glm::vec3 velocity, bool useSpeed = false);
	void addVelocity(glm::vec3 velocity, bool useSpeed = false);
	void collide(ColliderType ownType, ColliderType otherType, Box otherBox);
};
#endif // !BLOBS_H