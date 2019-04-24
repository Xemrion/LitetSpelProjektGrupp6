#ifndef BLOB_H
#define BLOB_H
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"

class Blob {
private:
	float speed = 150.0f;
	float recallSpeed = 200.0f;
	float fallSpeed = 0.01f;
	float radius = 2;
	glm::vec3 dir = glm::vec3(0, 0, 0);
public:
	bool isActive = false;
	bool isBeingRecalled = false;
	Sphere blobSphere;
	Blob();
	~Blob();
	Blob(glm::vec3 pos);
	glm::vec3 pos;
	void move(float dt);
	void setDir(glm::vec3 dir);
};
#endif // !BLOBS_H