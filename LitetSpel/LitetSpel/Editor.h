#ifndef EDITOR_H
#define EDITOR_H
#include <d3d11.h>
#include <d3dcompiler.h>
#include "stb_image.h"
#include <vector>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"Geometry.h"
#include"Platform.h"
#include"MovingPlatform.h"
#include"PowerUp.h"
#include"Button.h"
#include"Gate.h"
using namespace glm;
class Editor
{

private:
	std::vector<int> LeftSide;
	std::vector<int> topSide;
	std::vector<int> bottomSide;
	std::vector<int> rightSide;
	bool isWhite(vec3 pixelColour);
	bool isGoal(vec3 pixelColour);
	bool isEnemy(vec3 pixelColour);
	bool isStartPoint(vec3 pixelColour);
	bool isPlatform(vec3 pixelColour);
	bool isBouncy(vec3 pixelColour);
	bool isHeavy(vec3 pixelColour);
	bool isSticky(vec3 pixelColour);
	bool isNone(vec3 pixelColour);
	bool isMovingPlatform(vec3 pixelColour);
	bool isDoor(vec3 pixelColour);
	bool isButton(vec3 pixelColour);
	vec3 getPixelColour(int index);
	int isPixelUsed(int index);
	void addBoxToUsed(int startPosX, int startPosY, int endPosX, int endPosY);
	unsigned char* rgb;
	int minimumBoxSize = 30;
	int width = 0;
	int height = 0;
	int doorHeight = 5;
	int buttonWidth = 5;
	int maxMovingPlatformLength = 30;

public:
	Editor();
	~Editor();
	std::vector<Platform> platforms;
	std::vector<MovingPlatform> movingPlatforms;
	vec3 goalPos;
	vec3 startPos;
	std::vector<PowerUp> powerups;
	std::vector<vec3> enemyPos;
	std::vector<Gate> gates;
	std::vector<Button> buttons;
	void initialize(const char* filename);

};
#endif
