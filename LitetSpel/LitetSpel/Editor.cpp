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
	rgba = stbi_load(filename, &width, &height, &bpp, 4);
	int startPos = 0;
	float startPosX = 0;
	float startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	vec4 halfLength(0, 0, 10, 0);
	vec4 center(0, 0, 0, 0);
	bool foundEdge = false;
	bool drawThis = true;
	bool continueLoop = true;
	float pixelToUnitRatio = 8.8275862069f;
	int middleX = width / 2;
	int middleY = height / 2;
	int pixelUsedIndex = 0;

	for (int i = 0; i < width * height; i++)
	{
		if (!isBackground(getAlpha(i)) && isPixelUsed(i) == -1)
		{
			//Position baserat på mittpunkten av bilden
			startPosX = (i % width) - middleX;
			startPosY = middleY - floor(i / width);

			halfLength.x = minimumBoxSize / 2 / pixelToUnitRatio;
			halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio;

			if (isGoal(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->goalPos = vec3(center);
			}
			else if (isEnemy(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->enemyPos.push_back(vec3(center));
			}
			else if (isStartPoint(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->startPos = vec3(center);
			}
			else if (isBouncy(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->powerups.push_back(PowerUp(center, PowerType::Bouncy));
			}
			else if (isHeavy(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->powerups.push_back(PowerUp(center, PowerType::Heavy));

			}
			else if (isSticky(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->powerups.push_back(PowerUp(center, PowerType::Sticky));

			}
			else if (isNone(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				this->powerups.push_back(PowerUp(center, PowerType::None));

			}
			else if (isDoor(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + 1, startPosY - doorHeight);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio * doorHeight;

				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				vec4 color = vec4(getPixelColour(i), 0);
				color /= 255;
				this->gates.push_back(Gate(center, halfLength, color, int(getPixelColour(i).y) % 10));
			}
			else if (isButton(getAlpha(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + buttonWidth, startPosY - 1);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				halfLength.x = minimumBoxSize / 2 / pixelToUnitRatio * buttonWidth;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;
				vec4 color = vec4(getPixelColour(i), 0);
				color /= 255;
				this->buttons.push_back(Button(center, halfLength, color, 50, int(getPixelColour(i).y) % 10));
			}
			else
			{
				startPosX = (i % width) - middleX;
				startPosY = middleY - floor(i / width);
				startPos = i;
				for (int j = 1; !foundEdge; j++)
				{

					if (startPosY - j <= (-height / 2)
						|| !isPlatform(getAlpha(startPos + width * j)))
					{
						endPosY = startPosY - j;
						foundEdge = true;
					}
				}
				foundEdge = false;
				for (int j = 1; !foundEdge && drawThis; j++)
				{
					if ((startPosX + j) >= width / 2
						|| !isPlatform(getAlpha(startPos + j))
						|| isPixelUsed(startPos + j) != -1)
					{
						endPosX = startPosX + j;
						//if its a moving platform and 1x1 size, its a startpoint/endpoint, don't draw
						if (abs(endPosX - startPosX) == 1 && abs(endPosY - startPosY) == 1 && isMovingPlatform(getAlpha(i)))
							drawThis = false;

						foundEdge = true;
						//-1 because the for-loop will increase i with 1 for the next iteration
						i += j - 1;

						//Check width of boxes below(if all match it forms a square)
						//height
						for (int k = 1; k < abs(endPosY - startPosY) && continueLoop; k++)
						{

							for (int l = (j); l > 0 && continueLoop; l--)
							{
								if (l == j)
								{
									if (isPlatform(getAlpha(startPos + width * k + l)))
									{
										endPosY = startPosY - k;
										continueLoop = false;
									}
								}
								else if (!isPlatform(getAlpha(startPos + width * k + l)))
								{
									endPosY = startPosY - k;
									continueLoop = false;
								}
							}
						}
						continueLoop = true;
						addBoxToUsed(startPosX, startPosY, endPosX, endPosY);

					}

				}
				foundEdge = false;
				if (drawThis)
				{
					foundEdge = false;
					if (!isMovingPlatform(getAlpha(i)))
					{
						startPosX *= minimumBoxSize;
						startPosY *= minimumBoxSize;
						endPosX *= minimumBoxSize;
						endPosY *= minimumBoxSize;
						halfLength.x = ((abs(endPosX - startPosX)) / 2) / pixelToUnitRatio;
						halfLength.y = ((abs(endPosY - startPosY)) / 2) / pixelToUnitRatio;
						startPosX /= pixelToUnitRatio;
						startPosY /= pixelToUnitRatio;
						center.x = startPosX + halfLength.x;
						center.y = startPosY - halfLength.y;
						vec4 color = vec4(getPixelColour(startPos), 0);
						color /= 255;
						platforms.push_back(Platform(center, halfLength, color));
					}
					else
					{
						//left side
						int upperHalf = startPos + (floor((startPosY - endPosY) / 2) * width);
						int lowerHalf = startPos + (ceil((startPosY - endPosY) / 2 - 1) * width);
						int MPendPosX = 10000;
						int MPendPosY = 10000;
						int MPstartPosX = 10000;
						int MPstartPosY = 10000;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX == 10000; j++)
						{
							if (getPixelColour(lowerHalf - j) == getPixelColour(i) && isMovingPlatform(getAlpha(lowerHalf - j)))
							{
								MPstartPosX = ((lowerHalf - j) % width) - middleX;;
								MPstartPosY = middleY - floor((lowerHalf - j) / width);
							}
							else if (getPixelColour(upperHalf - j) == getPixelColour(i) && isMovingPlatform(getAlpha(upperHalf - j)))
							{
								MPstartPosX = ((upperHalf - j) % width) - middleX;;
								MPstartPosY = middleY - floor((upperHalf - j) / width);
							}
						}
						//Right side
						upperHalf += endPosX - startPosX;
						lowerHalf += endPosX - startPosX;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX != 10000 && MPendPosX == 10000; j++)
						{
							if (getPixelColour(lowerHalf + j) == getPixelColour(i) && isMovingPlatform(getAlpha(lowerHalf + j)))
							{
								MPendPosX = ((lowerHalf + j) % width) - middleX;;
								MPendPosY = middleY - floor((lowerHalf + j) / width);
							}
							else if (getPixelColour(upperHalf + j) == getPixelColour(i) && isMovingPlatform(getAlpha(upperHalf + j)))
							{
								MPendPosX = ((upperHalf + j) % width) - middleX;
								MPendPosY = middleY - floor((upperHalf + j) / width);
							}
						}
						//top side
						upperHalf = startPos + floor((endPosX - startPosX) / 2);
						lowerHalf = startPos + ceil((endPosX - startPosX) / 2 - 1);
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX == 10000; j++)
						{
							if (getPixelColour(lowerHalf - j * width) == getPixelColour(i) && isMovingPlatform(getAlpha(lowerHalf - j * width)))
							{
								MPstartPosX = (lowerHalf % width) - middleX;;
								MPstartPosY = middleY - floor((lowerHalf + j * width) / width);
							}
							else if (getPixelColour(upperHalf - j * width) == getPixelColour(i) && isMovingPlatform(getAlpha(upperHalf - j * width)))
							{
								MPstartPosX = (upperHalf % width) - middleX;;
								MPstartPosY = middleY - floor((upperHalf + j * width) / width);
							}
						}
						//bottom side
						upperHalf += (startPosY - (endPosY + 1)) * width;
						lowerHalf += (startPosY - (endPosY + 1)) * width;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX != 10000 && MPendPosX == 10000; j++)
						{
							//TODO: add check for edge of png
							if (getPixelColour(lowerHalf + j * width) == getPixelColour(i) && isMovingPlatform(getAlpha(lowerHalf+ j * width)))
							{
								MPendPosX = (lowerHalf % width) - middleX;;
								MPendPosY = middleY - floor((lowerHalf - j * width) / width);
							}
							else if (getPixelColour(upperHalf + j * width) == getPixelColour(i) && isMovingPlatform(getAlpha(upperHalf + j * width)))
							{
								MPendPosX = (upperHalf % width) - middleX;;
								MPendPosY = middleY - floor((upperHalf - j * width) / width);
							}
						}

						startPosX *= minimumBoxSize;
						startPosY *= minimumBoxSize;
						endPosX *= minimumBoxSize;
						endPosY *= minimumBoxSize;
						MPstartPosX *= minimumBoxSize;
						MPstartPosY *= minimumBoxSize;
						MPendPosY *= minimumBoxSize;
						MPendPosX *= minimumBoxSize;

						halfLength.x = (((abs(endPosX - startPosX)) / 2) / pixelToUnitRatio);
						halfLength.y = (((abs(endPosY - startPosY)) / 2) / pixelToUnitRatio);

						startPosX /= pixelToUnitRatio;
						startPosY /= pixelToUnitRatio;
						MPstartPosX /= pixelToUnitRatio;
						MPstartPosY /= pixelToUnitRatio;
						MPendPosY /= pixelToUnitRatio;
						MPendPosX /= pixelToUnitRatio;

						center.x = startPosX + halfLength.x;
						center.y = startPosY - halfLength.y;
						vec4 color = vec4(getPixelColour(startPos), 0);
						color /= 255;
						this->movingPlatforms.push_back(MovingPlatform(vec3(MPstartPosX, MPstartPosY, 0),
							vec3(MPendPosX, MPendPosY, 0),
							center,
							halfLength,
							color));

					}
				}
				drawThis = true;


			}

		}

	}

}


vec3 Editor::getPixelColour(int index)
{
	//Convert to RGBA
	index *= 4;
	if (index > width * height * 4)
	{
		return vec3(0, 0, 0);
	}
	return vec3(rgba[index], rgba[index + 1], rgba[index + 2]);
}
int Editor::getAlpha(int index)
{
	index *= 4;
	return rgba[index + 3];
}
int Editor::isPixelUsed(int index)
{
	int posY = (height / 2) - floor(index / width);
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
bool Editor::isBackground(int alpha)
{
	return alpha == 255;
}

bool Editor::isGoal(int alpha)
{
	return alpha == 250;
}

bool Editor::isEnemy(int alpha)
{
	return alpha == 249;
}

bool Editor::isStartPoint(int alpha)
{
	return alpha == 248;
}
bool Editor::isBouncy(int alpha)
{
	return alpha == 245;
}

bool Editor::isHeavy(int alpha)
{
	return alpha == 247;
}

bool Editor::isSticky(int alpha)
{
	return alpha == 246;
}

bool Editor::isNone(int alpha)
{
	return alpha == 244;

}

bool Editor::isPlatform(int alpha)
{
	return alpha == 254 || alpha == 253;
}

bool Editor::isMovingPlatform(int alpha)
{
	return alpha == 253;
}

bool Editor::isButton(int alpha)
{
	return alpha == 251;
}
bool Editor::isDoor(int alpha)
{
	return alpha == 252;
}

