#include "game.h"

void Game::init()
{
	Box groundBox;
	groundBox.center = glm::vec4(-10, -10, 10, 0);
	groundBox.halfLengths = glm::vec4(100, 10, 10, 0);
	currentLevel.boxes.push_back(groundBox);
	
	Sphere playerSphere;
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		5.0);
	currentLevel.spheres.push_back(playerSphere);
}

void Game::update(double dt)
{
	if (keys[0]) {
		currentLevel.player.move(dt, glm::vec3(-1, 0, 0));
	}
	if (keys[1]) {
		currentLevel.player.move(dt, glm::vec3(1, 0, 0));
	}



	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}

	Sphere playerSphere;
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		5.0);
	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);
}