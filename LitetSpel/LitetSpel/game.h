#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "Geometry.h"

using namespace std;
using namespace DirectX::SimpleMath;

class Player {
private:
public:
	Vector3 pos = Vector3(0, 0, 0);
	float speed = 100.0;
	void move(double dt, Vector3 dir) { pos += dir * speed * dt; };
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