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
		if (!isWhite(getPixelColour(i)) && isPixelUsed(i) == -1)
		{
		//Position baserat på mittpunkten av bilden
		startPosX = (i % width) - middleX;
		startPosY = middleY - floor(i / width);

		halfLength.x = minimumBoxSize / 2 / pixelToUnitRatio;
		halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio;

		if (isGoal(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->goalPos = vec3(center);
		}
		else if (isEnemy(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->enemyPos.push_back(vec3(center));
		}
		else if (isStartPoint(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->startPos = vec3(center);
		}
		else if (isBouncy(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->powerups.push_back(PowerUp(center, PowerType::Bouncy));
		}
		else if (isHeavy(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->powerups.push_back(PowerUp(center, PowerType::Heavy));

		}
		else if (isSticky(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->powerups.push_back(PowerUp(center, PowerType::Sticky));

		}
		else if (isNone(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - halfLength.y;
			this->powerups.push_back(PowerUp(center, PowerType::None));

		}
		else if (isDoor(getPixelColour(i)))
		{
			addBoxToUsed(startPosX, startPosY, startPosX, startPosY - doorHeight);
			startPosX *= minimumBoxSize;
			startPosY *= minimumBoxSize;
			startPosX /= pixelToUnitRatio;
			startPosY /= pixelToUnitRatio;
			center.x = startPosX + halfLength.x;
			center.y = startPosY - (doorHeight / 2) / pixelToUnitRatio;
			
			this->gates.insert(gates.begin() + glm::floatBitsToInt(getPixelColour(i).z) % 10, Gate(center, 5));
		}
			else if (isButton(getPixelColour(i)))
			{
				addBoxToUsed(startPosX, startPosY, startPosX + buttonWidth, startPosY - doorHeight);
				startPosX *= minimumBoxSize;
				startPosY *= minimumBoxSize;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + (buttonWidth / 2) / pixelToUnitRatio;
				center.y = startPosY - halfLength.y;
				this->buttons.insert(buttons.begin() + glm::floatBitsToInt(getPixelColour(i).z) % 10, Button(center));
			}
			else
			{
				startPosX = (i % width) - middleX;
				startPosY = middleY - floor(i / width);
				startPos = i;
				for (int j = 1; !foundEdge; j++)
				{

					if (startPosY - j <= (-height / 2)
						|| !isPlatform(getPixelColour(startPos + (width * j))))
					{
						endPosY = startPosY - j;
						foundEdge = true;
					}
				}
				foundEdge = false;
				for (int j = 1; !foundEdge && drawThis; j++)
				{
					if ((startPosX + j) >= width / 2
						|| !isPlatform(getPixelColour(startPos + j))
						|| isPixelUsed(startPos + j) != -1)
					{
						endPosX = startPosX + j;
						//if its a moving platform and 1x1 size, its a startpoint/endpoint, don't draw
						if (abs(endPosX - startPosX) == 1 && abs(endPosY - startPosY) == 1 && isMovingPlatform(getPixelColour(i)))
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
									if (isPlatform(getPixelColour((startPos + width * k) + l)))
									{
										endPosY = startPosY - k;
										continueLoop = false;
									}
								}
								else if (!isPlatform(getPixelColour((startPos + width * k) + l)))
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
					if (!isMovingPlatform(getPixelColour(i)))
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
						int MPendPosX = INFINITE;
						int MPendPosY = INFINITE;
						int MPstartPosX = INFINITE;
						int MPstartPosY = INFINITE;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX == INFINITE; j++)
						{
							if (getPixelColour(lowerHalf - j) == getPixelColour(i))
							{
								MPstartPosX = ((lowerHalf - j) % width) - middleX;;
								MPstartPosY = middleY - floor((lowerHalf - j) / width);
							}
							else if (getPixelColour(upperHalf - j) == getPixelColour(i))
							{
								MPstartPosX = ((upperHalf - j) % width) - middleX;;
								MPstartPosY = middleY - floor((upperHalf - j) / width);
							}
						}
						//Right side
						upperHalf += endPosX - startPosX;
						lowerHalf += endPosX - startPosX;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX != INFINITE && MPendPosX == INFINITE; j++)
						{
							if (getPixelColour(lowerHalf + j) == getPixelColour(i))
							{
								MPendPosX = ((lowerHalf + j) % width) - middleX;;
								MPendPosY = middleY - floor((lowerHalf + j) / width);
							}
							else if (getPixelColour(upperHalf + j) == getPixelColour(i))
							{
								MPendPosX = ((upperHalf + j) % width) - middleX;
								MPendPosY = middleY - floor((upperHalf + j) / width);
							}
						}
						//top side
						upperHalf = startPos + floor((endPosX - startPosX) / 2);
						lowerHalf = startPos + ceil((endPosX - startPosX) / 2 - 1);
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX == INFINITE; j++)
						{
							if (getPixelColour(lowerHalf - j * width) == getPixelColour(i))
							{
								MPstartPosX = (lowerHalf % width) - middleX;;
								MPstartPosY = middleY - floor((lowerHalf + j * width) / width);
							}
							else if (getPixelColour(upperHalf - j * width) == getPixelColour(i))
							{
								MPstartPosX = (upperHalf % width) - middleX;;
								MPstartPosY = middleY - floor((upperHalf + j * width) / width);
							}
						}
						//bottom side
						upperHalf += (startPosY - (endPosY + 1)) * width;
						lowerHalf += (startPosY - (endPosY + 1)) * width;
						for (int j = 1; j < maxMovingPlatformLength && MPstartPosX != INFINITE && MPendPosX == INFINITE; j++)
						{
							//TODO: add check for edge of png
							if (getPixelColour(lowerHalf + j * width) == getPixelColour(i))
							{
								MPendPosX = (lowerHalf % width) - middleX;;
								MPendPosY = middleY - floor((lowerHalf - j * width) / width);
							}
							else if (getPixelColour(upperHalf + j * width) == getPixelColour(i))
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
	//Convert to RGB
	index *= 3;
	if (index > width * height * 3)
	{
		return vec3(0, 0, 0);
	}
	return vec3(rgb[index], rgb[index + 1], rgb[index + 2]);
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
bool Editor::isWhite(vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 255 && pixelColour.z == 255;
}

bool Editor::isGoal(vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 0 && pixelColour.z == 255;
}

bool Editor::isEnemy(vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 0 && pixelColour.z == 0;
}

bool Editor::isStartPoint(vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 255 && pixelColour.z == 0;
}
bool Editor::isBouncy(vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 0 && pixelColour.z == 255;
}

bool Editor::isHeavy(vec3 pixelColour)
{
	return pixelColour.x == 192 && pixelColour.y == 192 && pixelColour.z == 192;
}

bool Editor::isSticky(vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 64 && pixelColour.z == 0;
}

bool Editor::isNone(vec3 pixelColour)
{
	return pixelColour.x == 128 && pixelColour.y == 128 && pixelColour.z == 0;

}

bool Editor::isPlatform(vec3 pixelColour)
{
	return !(isEnemy(pixelColour) || isStartPoint(pixelColour) || isGoal(pixelColour) || isWhite(pixelColour));
}

bool Editor::isMovingPlatform(vec3 pixelColour)
{
	return pixelColour.x == 0 && pixelColour.y == 255 && pixelColour.z >= 120 && pixelColour.z <= 130;
}

bool Editor::isButton(vec3 pixelColour)
{
	return pixelColour.x == 150 && pixelColour.y >= 70 && pixelColour.y < 80 && pixelColour.z == 0;
}
bool Editor::isDoor(vec3 pixelColour)
{
	return pixelColour.x == 150 && pixelColour.y >= 80 && pixelColour.y < 90;
}

