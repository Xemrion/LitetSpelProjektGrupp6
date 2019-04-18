#pragma once
#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"


class Sphere {
public:
	glm::vec4 centerRadius;
};

class Box {
public:
	glm::vec4  center;
	glm::vec4  halfLengths;
};
