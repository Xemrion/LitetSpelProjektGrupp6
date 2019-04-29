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
	glm::vec2 resolution = glm::vec2(1280, 720);
	ID3D11Texture2D* texture;
	std::vector<char> corners;
	std::vector<std::vector<char> > pixelColour;
	int length = 1280 * 720;
	std::vector<bool> isUsed;
	unsigned char* rgb;


public:
	ReadGeometry();
	~ReadGeometry();
	std::vector<Platform> platforms;
	void initialize();
	bool isWhite(glm::vec3 pixelColour);
	glm::vec3 getPixelColour(int index);

};
#endif
 