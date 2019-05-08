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
class Editor
{

private:
	std::vector<int> LeftSide;
	std::vector<int> topSide;
	std::vector<int> bottomSide;
	std::vector<int> rightSide;
	bool isWhite(glm::vec3 pixelColour);
	bool isBlue(glm::vec3 pixelColour);
	bool isRed(glm::vec3 pixelColour);
	bool isGreen(glm::vec3 pixelColour);
	glm::vec3 getPixelColour(int index);
	int isPixelUsed(int index);
	void addBoxToUsed(int startPosX, int startPosY, int endPosX, int endPosY);
	unsigned char* rgb;
	int minimumBoxSize = 30;
	int width = 0;
	int height = 0;

public:
	Editor();
	~Editor();
	std::vector<Platform> platforms;
	glm::vec3 goalPos;
	glm::vec3 startPos;
	std::vector<glm::vec3> enemyPos;
	void initialize(const char* filename);

};
#endif
 