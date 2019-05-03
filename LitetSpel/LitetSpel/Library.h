#ifndef LIBRARY_H
#define LIBRARY_H

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
menu01.wav
game01.wav
jump01.wav
jump02.wav
jump03.wav
land01.wav
land02.wav
land03.wav
blob01.wav
blob02.wav
blob03.wav
enmy01.wav
enmy02.wav
enmy03.wav
sorb01.wav
sorb02.wav
sorb03.wav
pdeath.wav
pdmg01.wav
pdmg02.wav
pstart.wav
p-goal.wav
mhlbtn.wav
mclkbt.wav
m-back.wav
|----------------------------------|
*/

#endif // !LIBRARY_H