#pragma once

const float GRAVITY_CONSTANT = 100.f;
const float COOLDOWN_CONSTANT = 0.3f;

enum class PowerType {
	None,
	Bouncy,
	Heavy,
	Sticky
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


/*
|----- Current Sound FileNames ----|
test01.wav
menu01.wav O
game01.wav O
jump01.wav O
jump02.wav O
jump03.wav O
land01.wav O
land02.wav O
land03.wav O
blob01.wav O
blob02.wav O
blob03.wav O
enmy01.wav O
enmy02.wav X
enmy03.wav X
sorb01.wav O
sorb02.wav O
sorb03.wav O
pdeath.wav X
pdmg01.wav O
pdmg02.wav O
pstart.wav X
p-goal.wav O
mhlbtn.wav O
mclkbt.wav O
m-back.wav O
|----------------------------------|
O = implemented
X = not
*/
//Player sound. Wet/slimy
//Enemy sound, robotic/metalic
//Level sound, metalic/sci-fi or industrial?