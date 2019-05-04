#include"ReadGeometry.h"
#include "stb_image.h"
#include <chrono>
ReadGeometry::ReadGeometry()
{
}

ReadGeometry::~ReadGeometry()
{

}

void ReadGeometry::initialize()
{

	int bpp = 0;
	rgb = stbi_load("test.png", &width, &height, &bpp, 3);
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
	std::vector<float> highestDur;
	int index = 0;
	int counter = 0;
	int counters = 0;
	int pixelUsedIndex = 0;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < width * height; i += minimumBoxSize)
	{
		counter++;
		if (counter == 89496)
			int aa = 0;
		if (!isWhite(getPixelColour(i)))
		{
			for (int j = 0; j < minimumBoxSize; j++)
			{

				if ((i - j) % width == 0)
				{
					i -= j;
					break;

				}
				if (isWhite(getPixelColour(i - j)) || isPixelUsed(i-j) != -1)
				{
					i -= j - 1;
					break;
				}
			}
			pixelUsedIndex = isPixelUsed(i);
			if (pixelUsedIndex != -1)
			{
				i += getBoxWidth(pixelUsedIndex) - minimumBoxSize;
			}
			else
			{
				counters++;

				startPos = i;

				//Position baserat p� mittpunkten av bilden
				startPosX = (i % width) - middleX;
				startPosY = middleY - glm::floor(i / width);

				for (int j = 1; !foundEdge; j++)
				{

					if (startPosY - minimumBoxSize * j <= (-height / 2)
						|| isWhite(getPixelColour(startPos + (width * minimumBoxSize * j))))
					{
						endPosY = startPosY - (minimumBoxSize) * (j);
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
						endPosX = startPosX + minimumBoxSize * (j);
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
										endPosY = startPosY - minimumBoxSize * k;
										continueLoop = false;
									}

								}
								else if (isWhite(getPixelColour((startPos + width * minimumBoxSize * k) + l)))
								{
									endPosY = startPosY - minimumBoxSize * k;
									continueLoop = false;
								}

							}
						}
						continueLoop = true;
						usedPixelMinX.push_back(startPosX);
						usedPixelMaxY.push_back(startPosY);
						usedPixelMaxX.push_back(endPosX);
						usedPixelMinY.push_back(endPosY);
						boxWidth.push_back(minimumBoxSize * j);
					}
				}

				foundEdge = false;
				halfLength.x = ((glm::abs(endPosX - startPosX)) / 2) / pixelToUnitRatio;
				halfLength.y = ((glm::abs(endPosY - startPosY)) / 2) / pixelToUnitRatio;
				startPosX /= pixelToUnitRatio;
				startPosY /= pixelToUnitRatio;
				center.x = startPosX + halfLength.x;
				center.y = startPosY - halfLength.y;

				platforms.push_back(Platform(center, halfLength));
			}

		}

	}
	//TODO: en range ist�llet? om Z �r �ver startposX, under endposX osv..
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	int aa = 0;

}

glm::vec3 ReadGeometry::getPixelColour(int index)
{
	//Convert to RGB
	index *= 3;
	if (index > 1280 * 720 * 3)
	{
		return glm::vec3(0, 0, 0);
	}
	return glm::vec3(rgb[index], rgb[index + 1], rgb[index + 2]);
}

int ReadGeometry::isPixelUsed(int index)
{
	int posY = (height / 2) - glm::floor(index / width) ;
	int posX = index % width - (width / 2);

	for (int i = 0; i < usedPixelMaxX.size(); i++)
	{
		if (posY > usedPixelMinY.at(i)
			&& posY < usedPixelMaxY.at(i)
			&& posX >= usedPixelMinX.at(i)
			&& posX <= usedPixelMaxX.at(i))
			return i;
	}
	return -1;
}

int ReadGeometry::getBoxWidth(int index)
{
	return boxWidth.at(index);
}
bool ReadGeometry::isWhite(glm::vec3 pixelColour)
{
	return pixelColour.x == 255 && pixelColour.y == 255 && pixelColour.z == 255;
}

