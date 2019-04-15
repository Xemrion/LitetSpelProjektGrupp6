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
		if (currentLevel.player.status == 2) 
		{
			currentLevel.player.move(dt, glm::vec3(-0.2, 0, 0));
		}
		else 
		{
			currentLevel.player.move(dt, glm::vec3(-1, 0, 0));
		}

	}
	if (keys[1]) {
		if (currentLevel.player.status == 2)
		{
			currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
		}
		else
		{
			currentLevel.player.move(dt, glm::vec3(1, 0, 0));
		}
	}
	if (keys[2]) {

		if(currentLevel.player.standing == true && currentLevel.player.jumpCoolDown <= 0 && currentLevel.player.status != 2)
		{
			currentLevel.player.jumpSpeed = -100.0f * float(dt);
			currentLevel.player.standing = false;
			gravity = 50.0f;
			currentLevel.player.jumpCoolDown = 0.2;
			currentLevel.player.move(dt, glm::vec3(10, 0, 0));
		}
		else if (currentLevel.player.pos.y < -0.1f && currentLevel.player.status == 1 && currentLevel.player.extraJump == true && currentLevel.player.standing == false && currentLevel.player.jumpCoolDown <= 0)
		{
			currentLevel.player.extraJump = false;
			currentLevel.player.jumpSpeed = -100.0f * float(dt);
			currentLevel.player.jumpCoolDown = 0.2;
			currentLevel.player.move(dt, glm::vec3(10, 0, 0));
		}

		else if (currentLevel.player.status == 2 && currentLevel.player.standing == true)
		{
			currentLevel.player.jumpSpeed = -50.0f * float(dt);
			currentLevel.player.standing = false;
			gravity = 50.0f;
			currentLevel.player.jumpCoolDown = 0.2;
		}
	}
	if (keys[3]) {
		currentLevel.player.move(dt, glm::vec3(0, 1, 0));
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
	//##########################################################
	if (currentLevel.player.pos.y <= 0.0f)
	{
		currentLevel.player.jumpSpeed = currentLevel.player.jumpSpeed + (gravity * float(dt)) / 100;
		currentLevel.player.pos.y += currentLevel.player.jumpSpeed;
	}
	else if (currentLevel.player.pos.y > 0.0f)
	{
		currentLevel.player.pos.y = 0;
		currentLevel.player.standing = true;
		currentLevel.player.extraJump = true;
		gravity = 0.0f;
		currentLevel.player.jumpSpeed = 0;
	}
	//##########################################################

	currentLevel.player.jumpCoolDown -= dt;

	Sphere playerSphere;
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		5.0);
	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);
}