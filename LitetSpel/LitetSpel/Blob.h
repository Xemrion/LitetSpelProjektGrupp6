#ifndef BLOB_H
#define BLOB_H
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "Geometry.h"

class Blob {
private:
	float speed = 150.0f;
	float fallSpeed = 0.01f;
	float radius = 2;
public:
	bool isActive = false;
	bool isBeingRecalled = false;
	Sphere blobSphere;
	Blob();
	~Blob();
	Blob(glm::vec3 pos);
	glm::vec3 pos;
	glm::vec3 dir = glm::vec3(0, 0, 0);
	void move(float dt);

};
#endif // !BLOBS_H