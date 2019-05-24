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
#include"Laser.h"
class Editor
{

private:
	std::vector<int> LeftSide;
	std::vector<int> topSide;
	std::vector<int> bottomSide;
	std::vector<int> rightSide;
	bool isBackground(int alpha);
	bool isGoal(int alpha);
	bool isEnemy(int alpha);
	bool isStartPoint(int alpha);
	bool isPlatform(int alpha);
	bool isBouncy(int alpha);
	bool isHeavy(int alpha);
	bool isSticky(int alpha);
	bool isNone(int alpha);
	bool isMovingPlatform(int alpha);
	bool isDoor(int alpha);
	bool isButton(int alpha);
	bool isLaser(int alpha);
	vec3 getPixelColour(int index);
	int getAlpha(int index);
	int isPixelUsed(int index);
	void addBoxToUsed(int startPosX, int startPosY, int endPosX, int endPosY);
	unsigned char* rgba;
	int minimumBoxSize = 30;
	int width = 0;
	int height = 0;
	int doorHeight = 5;
	int buttonWidth = 5;
	int maxMovingPlatformLength = 100;
	int maxLaserLength = 100;

public:
	Editor();
	~Editor();
	std::vector<Platform> platforms;
	std::vector<MovingPlatform> movingPlatforms;
	vec3 goalPos;
	vec3 startPos;
	std::vector<PowerUp> powerups;
	std::vector<vec3> enemies;
	std::vector<Gate> gates;
	std::vector<Button> buttons;
	std::vector<Laser> lasers;
	int deathHeight;
	void initialize(const char* filename);

};
#endif
