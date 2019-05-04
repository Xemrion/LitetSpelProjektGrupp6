#ifndef READGEOMETRY_H
#define READGEOMETRY_H
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
class ReadGeometry
{

private:
	std::vector<int> usedPixelMinX;
	std::vector<int> usedPixelMaxX;
	std::vector<int> usedPixelMinY;
	std::vector<int> usedPixelMaxY;
	std::vector<int> boxWidth;
	std::vector<int> boxHeight;
	unsigned char* rgb;
	int minimumBoxSize = 10;
	int width = 0;
	int height = 0;
public:
	ReadGeometry();
	~ReadGeometry();
	std::vector<Platform> platforms;
	void initialize();
	bool isWhite(glm::vec3 pixelColour);
	glm::vec3 getPixelColour(int index);
	int isPixelUsed(int index);
	int getBoxWidth(int index);

};
#endif
 