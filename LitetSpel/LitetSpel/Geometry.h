#pragma once

#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

class Sphere {
public:
	Sphere() : centerRadius(0.0) {};
	Sphere(glm::vec4 centerRadius) : centerRadius(centerRadius) {};
	Sphere(glm::vec3 pos, float radius) { centerRadius = glm::vec4(pos.x, pos.y, pos.z, radius); };

	glm::vec4 centerRadius;
	glm::vec4 color; //w is reflectivity 0.0 is completely non-reflective and 1.0 is completely reflective
};

class Box {
public:
	glm::vec4 center;
	glm::vec4 halfLengths;
	glm::vec4 color;
};
