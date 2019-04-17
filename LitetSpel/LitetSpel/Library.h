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



#endif // !LIBRARY_H