#pragma once
#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Geometry.h"

using namespace std;

class Player {
private:
public:
	glm::vec3 pos = glm::vec3(0, 5, 0);
	float speed = 100.0;
	void move(float dt, glm::vec3 dir) { pos += dir * speed * dt; };
};

class Level {
private:
public:
	Player player;
	vector<Box> boxes;
	vector<Sphere> spheres;
};

class Game {
private:
public:
	//left/right/up/down
	bool keys[4];
	Level currentLevel;
	
	void init();
	void update(double dt);
};