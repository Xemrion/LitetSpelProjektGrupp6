#pragma once

#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

struct Sphere {
    glm::vec4 centerRadius { .0f };
    glm::vec4 color        { .0f }; //w is reflectivity 0.0 is completely non-reflective and 1.0 is completely reflective
};

struct Box {
    glm::vec4 center      { .0f };
    glm::vec4 halfLengths { .0f };
	glm::vec4 color       { .0f };
};

struct Line {
	glm::vec3 start { .0f };
	glm::vec3 end   { .0f };
	glm::vec3 color { .0f };
};