#include"ReadGeometry.h"
#include "stb_image.h"


ReadGeometry::ReadGeometry()
{
}

ReadGeometry::~ReadGeometry()
{
}

void ReadGeometry::initialize()
{
	int width = 0;
	int height = 0;
	int bpp = 0;
	rgb = stbi_load("test.png", &width, &height, &bpp, 3);
	int startPosRGB = 0;
	int startPosX = 0;
	int startPosY = 0;
	float endPosX = 0;
	float endPosY = 0;
	int minimumBoxSizeRGB = 30 * 3;
	int oneYStepDown = 1280 * 3;
	glm::vec4 halfLength(0, 0, 0, 0);
	glm::vec4 center(0, 0, 0, 0);
	bool foundEdge = false;
	float pixelToUnitRatio = 8.8275862069f;
	float middleX = width / 2;
	float middleY = height / 2;
	for (int i = 0; i < (width * height) * 3; i += 3)
	{
		if (!isWhite(getPixelColour(i)))
		{

			if (isPixelUsed(i))
			{
				i += getBoxWidth(i);
			}
			else
			{
				startPosRGB = i;

				//Position baserat på mittpunkten av bilden
				startPosX = ((i / 3) % 1280) - middleX;
				startPosY = middleY - glm::floor((i / 3) / 1280);

				for (int j = 1; !foundEdge; j++)
				{
					if ((startPosRGB + (oneYStepDown * minimumBoxSizeRGB/3) * j) + oneYStepDown > (width*height) * 3 || isWhite(getPixelColour(startPosRGB + (oneYStepDown * minimumBoxSizeRGB / 3) * j + oneYStepDown)))
					{
						endPosY = (startPosY + (minimumBoxSizeRGB / 3) * j);
						foundEdge = true;
					}
				}
				foundEdge = false;
				for (int j = 1; !foundEdge; j++)
				{
					if ((startPosRGB + minimumBoxSizeRGB * j + 3) % 1280 == 0 || isWhite(getPixelColour(startPosRGB + minimumBoxSizeRGB * j + 3)))
					{
						endPosX = (startPosX + (minimumBoxSizeRGB / 3) * j);
						foundEdge = true;
						i += startPosRGB + minimumBoxSizeRGB * j;
						for (int k = 0; k < endPosY - startPosY; k++)
						{
							isUsed.push_back(startPosRGB + oneYStepDown * k);
							boxWidth.push_back(startPosRGB + minimumBoxSizeRGB * j);
						}
					}
				}

				foundEdge = false;
				halfLength.x = ((endPosX - startPosX) / 2) / pixelToUnitRatio;
				halfLength.y = ((endPosY - startPosY) / 2) / pixelToUnitRatio;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY + halfLength.y;

				platforms.push_back(Platform(center, halfLength));
			}

		}



		//Markera alla pixlar på plattformens vänster som använda så att dessa pixlar kan skippas i senare iterationer.


		//if (isWhite(getPixelColour(i + 3)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i + 6)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		////1280?
		//if (isWhite(getPixelColour(i * 1280 * 3)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i * 1280 * 3 + 3)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i * 1280 * 3 + 6)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i * 1280 * 3 * 2)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i * 1280 * 6 + 3)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (isWhite(getPixelColour(i * 1280 * 6 + 6)))
		//	corners.push_back(1);
		//else
		//	corners.push_back(0);
		//if (i > 0 && i % 1280 == 0)
		//{
		//	i += 1280;
		//}
	}


}

glm::vec3 ReadGeometry::getPixelColour(int index)
{
	return glm::vec3(rgb[index], rgb[index + 1], rgb[index + 2]);
}

bool ReadGeometry::isPixelUsed(int index)
{
	for (int i = 0; i < this->isUsed.size(); i++)
	{
		if (this->isUsed.at(i) == index)
			return true;
	}
	return false;
}

int ReadGeometry::getBoxWidth(int index)
{
	return boxWidth.at(findIndexByPixel(index));
}

int ReadGeometry::findIndexByPixel(int index)
{
	for (int i = 0; i < this->isUsed.size(); i++)
	{
		if (this->isUsed.at(i) == index)
			return i;
	}
	return -1;
}

bool ReadGeometry::isWhite(glm::vec3 pixelColour)
{

	return pixelColour.x == 255 && pixelColour.y == 255 && pixelColour.z == 255;
}

