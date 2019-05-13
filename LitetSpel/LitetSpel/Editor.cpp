#include"Editor.h"
#include "stb_image.h"
#include <chrono>
Editor::Editor()
{
}

Editor::~Editor()
{

}

void Editor::initialize(const char* filename)
{

	int bpp = 0;
	rgb = stbi_load(filename, &width, &height, &bpp, 3);
	int startPos = 0;
	float startPosX = 0;
	float startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	glm::vec4 halfLength(0, 0, 10, 0);
	glm::vec4 center(0, 0, 0, 0);
	bool foundEdge = false;
	bool continueLoop = true;
	float pixelToUnitRatio = 8.8275862069f;
	int middleX = width / 2;
	int middleY = height / 2;
	int pixelUsedIndex = 0;

	for (int i = 0; i < width * height; i++)
	{
		if (!isWhite(getPixelColour(i)) && isPixelUsed(i) == -1)
		{
			//Position baserat på mittpunkten av bilden
			startPosX = (i % width) - middleX;
			startPosY = middleY - glm::floor(i / width);

			halfLength.x = minimumBoxSize / 2 / pixelToUnitRatio;
			halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio;

			if (isBlue(getPixelColour(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->goalPos = glm::vec3(center);
			}
			else if (isRed(getPixelColour(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->enemyPos.push_back(glm::vec3(center));
			}
			else if (isGreen(getPixelColour(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->startPos = glm::vec3(center);
			}
			else
			{
				startPosX = (i % width) - middleX;
				startPosY = middleY - glm::floor(i / width);
				startPos = i;
				for (int j = 1; !foundEdge; j++)
				{

					if (startPosY - j <= (-height / 2)
						|| isWhite(getPixelColour(startPos + (width * j))))
					{
						endPosY = startPosY - j;
						foundEdge = true;
					}
				}
				foundEdge = false;
				for (int j = 1; !foundEdge; j++)
				{
					if ((startPosX + j) >= width / 2
						|| isWhite(getPixelColour(startPos + j))
						|| isPixelUsed(startPos + j) != -1)
					{
						endPosX = startPosX + j;
						foundEdge = true;
						//-1 because the for-loop will increase i with 1 for the next iteration
						i += j - 1;

						//Check width of boxes below(if all match it forms a square)
						//height
						for (int k = 1; k < glm::abs(endPosY - startPosY) && continueLoop; k++)
						{

							for (int l = (j); l > 0 && continueLoop; l--)
							{
								if (isWhite(getPixelColour((startPos + width * k) + l)))
								{
									endPosY = startPosY - k - 1;
									continueLoop = false;
								}
							}
						}
						continueLoop = true;
						addBoxToUsed(startPosX, startPosY, endPosX, endPosY);

					}
				}

				foundEdge = false;
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				endPosX *= minimumBoxSize;
				endPosY *= minimumBoxSize;
				halfLength.x = (((glm::abs(endPosX - startPosX)) / 2) / pixelToUnitRatio);
				halfLength.y = (((glm::abs(endPosY - startPosY)) / 2) / pixelToUnitRatio);
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;

				platforms.push_back(Platform(center, halfLength));


			}

		}

	}

}

glm::vec3 Editor::getPixelColour(int index)
{
	//Convert to RGB
	index *= 3;
	if (index > width * height * 3)
	{
		return glm::vec3(0, 0, 0);
	}
	return glm::vec3(rgb[index], rgb[index + 1], rgb[index + 2]);
}

int Editor::isPixelUsed(int index)
{
	int posY = (height / 2) - glm::floor(index / width);
	int posX = index % width - (width / 2);

	for (int i = 0; i < topSide.size(); i++)
	{
		if (posY > bottomSide.at(i)
			&& posY <= topSide.at(i)
			&& posX >= LeftSide.at(i)
			&& posX < rightSide.at(i))
			return i;
	}
	return -1;
}
void Editor::addBoxToUsed(int startPosX, int startPosY, int endPosX, int endPosY)
{
	LeftSide.push_back(startPosX);
	rightSide.push_back(endPosX);
	topSide.push_back(startPosY);
	bottomSide.push_back(endPosY);
}
bool Editor::isWhite(glm::vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 255 && pixelColour.z == 255;
}

bool Editor::isBlue(glm::vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 0 && pixelColour.z == 255;
}

bool Editor::isRed(glm::vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 0 && pixelColour.z == 0;
}

bool Editor::isGreen(glm::vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 255 && pixelColour.z == 0;
}

