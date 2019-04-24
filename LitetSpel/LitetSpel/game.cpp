#include "game.h"

void Game::init()
{
	Box groundBox;
	groundBox.center = glm::vec4(-10, -10, 0, 0);
	groundBox.halfLengths = glm::vec4(100, 10, 10, 0);
	currentLevel.boxes.push_back(groundBox);

	for (int i = 0; i < 99; i++)
	{
		Box box;
		box.center = glm::vec4((i-10) * 20, 10, 0, 0);
		box.halfLengths = glm::vec4(5, 1, 10, 0);
		currentLevel.boxes.push_back(box);
	}
	
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
		currentLevel.player.move((float)dt, glm::vec3(-1, 0, 0));
	}
	if (keys[1]) {
		currentLevel.player.move((float)dt, glm::vec3(1, 0, 0));
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

	for (int i = 1; i < 5; ++i)
	{
		Sphere s;
		s.centerRadius = glm::vec4((i - 2) * 15.0, 5, 0, 5);
		currentLevel.spheres.push_back(s);
	}
}