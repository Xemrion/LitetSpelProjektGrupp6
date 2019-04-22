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
	time += dt;

	Sphere playerSphere;
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		2.5);
	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);

	for (int i = 1; i < 250; ++i)
	{
		Sphere s;
		s.centerRadius = glm::vec4(
			playerSphere.centerRadius.x + sin(time * (i*0.1) + i) * 7.4,
			playerSphere.centerRadius.y - cos(time * 0.3 + i) * 6.7,
			playerSphere.centerRadius.z + sin(time * 0.3 + i) * 0.8,
			2.5);
		currentLevel.spheres.push_back(s);
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
}