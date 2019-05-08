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

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < width * height; i += minimumBoxSize)
	{
		if (!isWhite(getPixelColour(i)) && isPixelUsed(i) == -1)
		{
			if (isBlue(getPixelColour(i)))
			{
				startPosX = (i % width) - middleX;
				startPosY = middleY - glm::floor(i / width);
				endPosY = startPosY - goalWidth;
				endPosX = startPosX + goalWidth;
				glm::vec2 center = glm::vec2((startPosX + goalWidth) / pixelToUnitRatio, (startPosY - goalWidth) / pixelToUnitRatio);
				this->goalPos = glm::vec3(center, 0);
				addBoxToUsed(startPosX, startPosY, endPosX, endPosY, goalWidth);
			}
			else if (isRed(getPixelColour(i)))
			{
				startPosX = (i % width) - middleX;
				startPosY = middleY - glm::floor(i / width);
				endPosY = startPosY - enemyWidth;
				endPosX = startPosX + enemyWidth;

				glm::vec2 center = glm::vec2((startPosX + enemyWidth) / pixelToUnitRatio, (startPosY - enemyWidth) / pixelToUnitRatio);
				this->enemyPos.push_back(glm::vec3(center, 0));

				addBoxToUsed(startPosX, startPosY, endPosX, endPosY, enemyWidth);

			}
			else
			{
				for (int j = 0; j < minimumBoxSize + 1; j++)
				{

					if ((i - j) % width == 0)
					{
						i -= j;
						break;

					}
					if (isWhite(getPixelColour(i - j)) || isPixelUsed(i - j) != -1)
					{
						i -= j - 1;
						break;
					}
				}
				startPos = i;

				//Position baserat på mittpunkten av bilden
				startPosX = (i % width) - middleX;
				startPosY = middleY - glm::floor(i / width);

				for (int j = 1; !foundEdge; j++)
				{

					if (startPosY - minimumBoxSize * j <= (-height / 2)
						|| isWhite(getPixelColour(startPos + (width * minimumBoxSize * j))))
					{
						endPosY = startPosY - (minimumBoxSize) * (j)+1;
						foundEdge = true;
					}
				}
				foundEdge = false;
				for (int j = 1; !foundEdge; j++)
				{
					if ((startPosX + minimumBoxSize * j) >= width / 2
						|| isWhite(getPixelColour(startPos + minimumBoxSize * j))
						|| isPixelUsed(startPos + minimumBoxSize * j) != -1)
					{
						endPosX = startPosX + minimumBoxSize * (j)-1;
						foundEdge = true;
						//-1 because the for-loop will increase i with 1 for the next iteration
						i += minimumBoxSize * j - minimumBoxSize;

						//Check width of boxes below(if all match it forms a square)
						//height
						for (int k = 1; k < glm::abs(endPosY - startPosY) / minimumBoxSize && continueLoop; k++)
						{

							for (int l = minimumBoxSize * (j); l > 0 && continueLoop; l -= minimumBoxSize)
							{
								//If its the first iteration and not at the edge of png
								if (l == minimumBoxSize * j && startPosX + l + minimumBoxSize < width / 2)
								{

									if (!isWhite(getPixelColour((startPos + width * minimumBoxSize * k) + l + minimumBoxSize)))
									{
										endPosY = startPosY - minimumBoxSize * k + 1;
										continueLoop = false;
									}

								}
								else if (isWhite(getPixelColour((startPos + width * minimumBoxSize * k) + l)))
								{
									endPosY = startPosY - minimumBoxSize * k + 1;
									continueLoop = false;
								}

							}
						}
						continueLoop = true;
						addBoxToUsed(startPosX, startPosY, endPosX, endPosY, minimumBoxSize * j);

					}
				}

				foundEdge = false;
				//ful-lösning, fixar hål mellan boxes
				halfLength.x = (((glm::abs(endPosX - startPosX)) / 2) / pixelToUnitRatio) + 0.1f;
				halfLength.y = (((glm::abs(endPosY - startPosY)) / 2) / pixelToUnitRatio) + 0.1f;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;

				platforms.push_back(Platform(center, halfLength));


			}

		}

	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	int aa = 0;

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
		if (posY >= bottomSide.at(i)
			&& posY <= topSide.at(i)
			&& posX >= LeftSide.at(i)
			&& posX <= rightSide.at(i))
			return i;
	}
	return -1;
}

int Editor::getBoxWidth(int index)
{
	return boxWidth.at(index);
}
void Editor::addBoxToUsed(int startPosX, int startPosY, int endPosX, int endPosY, int width)
{
	LeftSide.push_back(startPosX);
	rightSide.push_back(endPosX);
	topSide.push_back(startPosY);
	bottomSide.push_back(endPosY);
	this->boxWidth.push_back(width);
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

