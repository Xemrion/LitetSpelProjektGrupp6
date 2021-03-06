#pragma once
#include "../../INCLUDE/glm/glm/glm.hpp"

const float GRAVITY_CONSTANT = 200.f;
const float COOLDOWN_CONSTANT = 0.3f;
const float PHYSICS_TIME_STEP = 0.0001f;
const float INVINCIBILITY_TIME = 1.5f;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

enum class PowerType {
	None,
	Bouncy,
	Sticky,
	Heavy
}; //Adding more requires change in PowerUp class

//Colours
static const glm::vec4 red		= glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
static const glm::vec4 green	= glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
static const glm::vec4 blue		= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
static const glm::vec4 yellow	= glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
static const glm::vec4 magenta	= glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
static const glm::vec4 purple	= glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
static const glm::vec4 cyan		= glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
static const glm::vec4 black	= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
static const glm::vec4 white	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
static const glm::vec4 grey		= glm::vec4(0.77f, 0.77f, 0.77f, 1.0f);
static const glm::vec4 olive	= glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

static const glm::vec4 enemyColor = glm::vec4(0.9, 1.0, 1.0, 0.0);

//Use player.status as index
static const glm::vec4 playerStatusColors[4] = {
	glm::vec4(0.5, 0.05, 0.5, 0.0),
	glm::vec4(.5, 0.5, 0.05, 0.0),
	glm::vec4(0.05, 0.05, 0.5, 0.0),
	glm::vec4(1.05, 1.05, 1.05, 0.5)
};
