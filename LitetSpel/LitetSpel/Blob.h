#ifndef BLOB_H
#define BLOB_H
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"
#include "game.h"

class Blob {
private:
	float speed = 50.0f;
	float recallSpeed = 200.0f;
	float radius = 2;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
public:
	bool isActive = false;
	bool isBeingRecalled = false;
	Sphere blobSphere;
	Blob();
	~Blob();
	Blob(glm::vec3 pos);
	glm::vec3 pos;
	void move(float dt);
	void setVelocity(glm::vec3 velocity);
};
#endif // !BLOBS_H