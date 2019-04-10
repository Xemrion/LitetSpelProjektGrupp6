#include "game.h"

void Game::init()
{
	Box groundBox;
	groundBox.center = Vector4(-10, -10, 10, 0);
	groundBox.halfLengths = Vector4(100, 10, 10, 0);
	Box groundBox2;
	groundBox2.center = Vector4(0, 0, 0, 0);
	groundBox2.halfLengths = Vector4(5, 100, 5, 0);

	currentLevel.boxes.push_back(groundBox);
	currentLevel.boxes.push_back(groundBox2);

	for (int i = 0; i < 50; ++i) {
		Box box;
		box.center = Vector4(i, 20, 2 * i, 0);
		box.halfLengths = Vector4(1, 1, 1, 0);
		currentLevel.boxes.push_back(box);
	}
	
	Sphere playerSphere;
	playerSphere.centerRadius = Vector4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		5.0);
	currentLevel.spheres.push_back(playerSphere);
}

void Game::update(double dt)
{
	if (keys[0]) {
		currentLevel.player.move(dt, Vector3(-1, 0, 0));
	}
	if (keys[1]) {
		currentLevel.player.move(dt, Vector3(1, 0, 0));
	}



	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}

	Sphere playerSphere;
	playerSphere.centerRadius = Vector4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		5.0);
	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);
}