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
	glm::vec3 pos = glm::vec3(0, 0, 0);
	float speed = 100.0f;
	float jumpSpeed = 0.0f;
	void move(float dt, glm::vec3 dir) { pos += dir * speed * dt; };
	//Power ups indicator
	// 0 = none 1 = bouncy 2 = heavy
	int status = 2;
	bool extraJump = true;
	bool standing = true;
	float jumpCoolDown = 0;
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

	float gravity = 50.0f;
};