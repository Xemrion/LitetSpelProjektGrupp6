#include "LevelLoader.h"
#include "stb_image.h"
#include <chrono>
#include "Platform.h"
#include "MovingPlatform.h"
#include "PowerUp.h"
#include "Button.h"
#include "LevelGoal.h"
#include "Gate.h"
#include "Enemy.h"
#include "Player.h"

LevelLoader::LevelLoader( Graphics &graphics ):
    graphics(&graphics)
{}

std::unique_ptr<Level> LevelLoader::load(const char* filename) {
    auto      level            = std::make_unique<Level>();
    int       bpp              = 0,
	          startPos         = 0,
              endPosX          = 0,
              endPosY          = 0,
              middleX          = width  / 2,
              middleY          = height / 2,
              pixelUsedIndex   = 0;
    float     startPosX        =  .0f,
	          startPosY        =  .0f,
              pixelToUnitRatio = 8.8275862069f;
	glm::vec4 halfLength       = { .0f, .0f, 10.0f, .0f },
	          center           = { .0f, .0f,   .0f, .0f };
	bool      foundEdge        = false,
	          drawThis         = true,
	          continueLoop     = true;

    rgb = stbi_load( filename, &width, &height, &bpp, 3 );

    // TODO: break out redundant massive copy-pasted chunks
	for ( int i = 0; i < width * height; i++ ) {
		if ( !isWhite(getPixelColour(i)) and isPixelUsed(i) == -1 ) {
			// Position baserat på mittpunkten av bilden
			startPosX = float((i % width) - middleX);
			startPosY = float(middleY) - float(floor(i / width));

			halfLength.x = minimumBoxSize / 2 / pixelToUnitRatio;
			halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio;

            startPosX *= minimumBoxSize;
            startPosY *= minimumBoxSize;
            startPosX /= pixelToUnitRatio;
            startPosY /= pixelToUnitRatio;
            center.x   = startPosX + halfLength.x;
            center.y   = startPosY - halfLength.y;

			if ( isGoal(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX + 1), int(startPosY - 1));
                level->add( std::make_unique<LevelGoal>( Box{ center, {5,10,5,0}, magenta} ) );
			}
			else if ( isEnemy(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<Enemy>(glm::vec3(center)) );
			}
			else if ( isStartPoint(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY),int( startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<Player>( *graphics, level->getCollisionManager(), glm::vec3(center)) );
			}
			else if ( isBouncy(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<PowerUp>(glm::vec3(center), PowerType::bouncy) );
			}
			else if ( isHeavy(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<PowerUp>(glm::vec3(center), PowerType::heavy) );
			}
			else if ( isSticky(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<PowerUp>(glm::vec3(center), PowerType::sticky) );
			}
			else if ( isNone(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - 1);
                level->add( std::make_unique<PowerUp>(glm::vec3(center), PowerType::none) );
			}
			else if ( isDoor(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + 1, int(startPosY) - doorHeight);
				halfLength.y = minimumBoxSize / 2 / pixelToUnitRatio * doorHeight;
                int id       = int(getPixelColour(i).y) % 10;
                activableMap[id].push_back( level->add( std::make_unique<Gate>( Box{center,halfLength,{}} ) ) );
                // will be linked at the end
			}
			else if ( isButton(getPixelColour(i)) ) {
				addBoxToUsed(int(startPosX), int(startPosY), int(startPosX) + buttonWidth, int(startPosY) - 1);
				halfLength.x   = minimumBoxSize / 2 / pixelToUnitRatio * buttonWidth;
                int   id       = int(getPixelColour(i).y) % 10;
                float duration = 500.f;  // <-- TEMP    getPixelColour(i).z;
                buttonMap[id].push_back( level->add( std::make_unique<Button>( Box{center,halfLength,{}}, duration ) ) );
                // will be linked at the end
			}
			else {
				startPosX = float((i % width) - middleX);
				startPosY = float(middleY) - float(floor(i / width));
				startPos  = i;
				for ( int j = 1; !foundEdge; j++ ) {
					if ( startPosY - j <= (-height / 2)
                         or !isPlatform(getPixelColour(startPos + (width * j))) )
                    {
						endPosY = int(startPosY) - j;
						foundEdge = true;
					}
				}
				foundEdge = false;
				for ( int j = 1; !foundEdge and drawThis; j++ ) {
					if ((startPosX + j) >= width / 2
						or !isPlatform(getPixelColour(startPos + j))
						or isPixelUsed(startPos + j) != -1 )
					{
						endPosX = int(startPosX) + j;
						// if its a moving platform and 1x1 size, its a startpoint/endpoint, don't draw
						if (abs(endPosX - startPosX) == 1 and abs(endPosY - startPosY) == 1 and isMovingPlatform(getPixelColour(i)))
							drawThis = false;

						foundEdge = true;
						i += j - 1; // -1 because the for-loop will increase i with 1 for the next iteration

						// Check width of boxes below (if all match it forms a square)
						// height
						for ( int k = 1; k < abs(endPosY - startPosY) && continueLoop; k++ ) {
							for ( int l = (j); l > 0 && continueLoop; l-- ) {
								if ( l == j ) {
									if ( isPlatform(getPixelColour((startPos + width * k) + l)) ) {
										endPosY = int(startPosY) - k;
										continueLoop = false;
									}
								}
								else if ( !isPlatform(getPixelColour((startPos + width * k) + l)) ) {
									endPosY = int(startPosY) - k;
									continueLoop = false;
								}
							}
						}
						continueLoop = true;
						addBoxToUsed( int(startPosX), int(startPosY), endPosX, endPosY );
					}
				}
				foundEdge = false;
				if ( drawThis ) {
					foundEdge = false;
					if ( !isMovingPlatform(getPixelColour(i)) ) {
						startPosX       *= minimumBoxSize;
						startPosY       *= minimumBoxSize;
						endPosX         *= minimumBoxSize;
						endPosY         *= minimumBoxSize;
						halfLength.x     = ((abs(endPosX - startPosX)) / 2) / pixelToUnitRatio;
						halfLength.y     = ((abs(endPosY - startPosY)) / 2) / pixelToUnitRatio;
						startPosX        = startPosX / pixelToUnitRatio;
						startPosY        = startPosY / pixelToUnitRatio;
						center.x         = startPosX + halfLength.x;
						center.y         = startPosY - halfLength.y;
                        glm::vec4 color  = glm::vec4(getPixelColour(startPos), 0) / 255.f;
                        level->add( std::make_unique<Platform>( Box{center,halfLength,color} ) );
					}
					else {
						// left side
						int upperHalf   = startPos + int( floor((startPosY - endPosY) / 2     ) * width);
						int lowerHalf   = startPos + int(  ceil((startPosY - endPosY) / 2 - 1 ) * width);
						int MPendPosX   = INFINITE;
						int MPendPosY   = INFINITE;
						int MPstartPosX = INFINITE;
						int MPstartPosY = INFINITE;
						for ( int j = 1; j < maxMovingPlatformLength and MPstartPosX == INFINITE; j++ ) {
							if (getPixelColour(lowerHalf - j) == getPixelColour(i)) {
								MPstartPosX = ((lowerHalf - j) % width) - middleX;
								MPstartPosY = middleY - int(floor((lowerHalf - j) / width));
							}
							else if ( getPixelColour(upperHalf - j) == getPixelColour(i) ) {
								MPstartPosX = ((upperHalf - j) % width) - middleX;;
								MPstartPosY = middleY - int(floor((upperHalf - j) / width));
							}
						}
						// right side
						upperHalf += endPosX - int(startPosX);
						lowerHalf += endPosX - int(startPosX);
						for ( int j = 1; j < maxMovingPlatformLength
                              and MPstartPosX != INFINITE and MPendPosX == INFINITE; j++ ) {
							if ( getPixelColour(lowerHalf + j) == getPixelColour(i) ) {
								MPendPosX = ((lowerHalf + j) % width) - middleX;;
								MPendPosY = middleY - int(floor((lowerHalf + j) / width));
                            }
							else if ( getPixelColour(upperHalf + j) == getPixelColour(i) ) {
								MPendPosX = ((upperHalf + j) % width) - middleX;
								MPendPosY = middleY - int(floor((upperHalf + j) / width));
							}
						}
						// top side
						upperHalf = startPos + int(floor((endPosX - startPosX) / 2));
						lowerHalf = startPos + int(ceil((endPosX - startPosX) / 2 - 1));
						for ( int j = 1; j < maxMovingPlatformLength && MPstartPosX == INFINITE; j++ ) {
							if ( getPixelColour(lowerHalf - j * width) == getPixelColour(i) ) {
								MPstartPosX = (lowerHalf % width) - middleX;;
								MPstartPosY = middleY - int(floor((lowerHalf + j * width) / width));
							}
							else if ( getPixelColour(upperHalf - j * width) == getPixelColour(i) ) {
								MPstartPosX = (upperHalf % width) - middleX;;
								MPstartPosY = middleY - int(floor((upperHalf + j * width) / width));
							}
						}
						// bottom side
						upperHalf += (int(startPosY) - (endPosY + 1)) * width;
						lowerHalf += (int(startPosY) - (endPosY + 1)) * width;
						for ( int j = 1; j < maxMovingPlatformLength
                              and MPstartPosX != INFINITE and MPendPosX == INFINITE; j++ ) {
							// TODO: add check for edge of png
							if ( getPixelColour(lowerHalf + j * width) == getPixelColour(i) ) {
								MPendPosX = (lowerHalf % width) - middleX;;
								MPendPosY = middleY - int(floor((lowerHalf - j * width) / width));
							}
							else if ( getPixelColour(upperHalf + j * width) == getPixelColour(i) ) {
								MPendPosX = (upperHalf % width) - middleX;;
								MPendPosY = middleY - int(floor((upperHalf - j * width) / width));
							}
						}

						startPosX   *= minimumBoxSize;
						startPosY   *= minimumBoxSize;
						endPosX     *= minimumBoxSize;
						endPosY     *= minimumBoxSize;
						MPstartPosX *= minimumBoxSize;
						MPstartPosY *= minimumBoxSize;
						MPendPosY   *= minimumBoxSize;
						MPendPosX   *= minimumBoxSize;

						halfLength.x = (((abs(endPosX - startPosX)) / 2) / pixelToUnitRatio);
						halfLength.y = (((abs(endPosY - startPosY)) / 2) / pixelToUnitRatio);

						startPosX   /= pixelToUnitRatio;
						startPosY   /= pixelToUnitRatio;
						MPstartPosX  = int( MPstartPosX / pixelToUnitRatio);
						MPstartPosY  = int( MPstartPosY / pixelToUnitRatio);
						MPendPosY    = int( MPendPosY   / pixelToUnitRatio);
						MPendPosX    = int( MPendPosX   / pixelToUnitRatio);

						center.x   = startPosX + halfLength.x;
						center.y   = startPosY - halfLength.y;
						auto color = glm::vec4(getPixelColour(startPos), 0);
						color     /= 255.f;
                        level->add( std::make_unique<MovingPlatform>( glm::vec3{ MPstartPosX, MPstartPosY,  .0f },
                                                                      glm::vec3{ MPendPosX,   MPendPosY,    .0f },
                                                                            Box{ center,      halfLength, color } ) );
					}
				}
				drawThis = true;
			}
		}
	}
    // link buttons to activables
    for ( auto &[id,buttons] : buttonMap )
        for ( auto &button : buttons )
            for ( auto &activable : activableMap[id] )
                ((Button*)button)->link( (IActivable*)activable );

    return level;
}


[[nodiscard]]
glm::vec3 LevelLoader::getPixelColour( int index ) const noexcept {
	// Convert to RGB
	index *= 3;
	if ( index > width * height * 3 )
		return glm::vec3( .0f );
    else return { rgb[index], rgb[index + 1], rgb[index + 2] };
}

[[nodiscard]]
int LevelLoader::isPixelUsed( int index ) const noexcept {
	int posY = (height / 2) - int(floor(index / width));
	int posX = index % width - (width / 2);

	for ( int i = 0; i < topSide.size(); i++ ) {
		if (    posY >  bottomSide.at(i)
			and posY <=    topSide.at(i)
			and posX >=   LeftSide.at(i)
			and posX <   rightSide.at(i) )
        {
			return i;
        }
	}
	return -1; // TODO: throw instead?
}

void LevelLoader::addBoxToUsed( int startPosX, int startPosY, int endPosX, int endPosY ) {
	LeftSide.push_back(startPosX);
	rightSide.push_back(endPosX);
	topSide.push_back(startPosY);
	bottomSide.push_back(endPosY);
}

[[nodiscard]]
inline bool LevelLoader::isWhite( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 255 and pixelColour.y == 255 and pixelColour.z == 255;
}

[[nodiscard]]
inline bool LevelLoader::isGoal( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 0 and pixelColour.y == 0 and pixelColour.z == 255;
}

[[nodiscard]]
inline bool LevelLoader::isEnemy( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 255 && pixelColour.y == 0 && pixelColour.z == 0;
}

[[nodiscard]]
inline bool LevelLoader::isStartPoint( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 0 && pixelColour.y == 255 && pixelColour.z == 0;
}
[[nodiscard]]
inline bool LevelLoader::isBouncy( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 255 && pixelColour.y == 0 && pixelColour.z == 255;
}

[[nodiscard]]
inline bool LevelLoader::isHeavy( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 192 && pixelColour.y == 192 && pixelColour.z == 192;
}

[[nodiscard]] 
inline bool LevelLoader::isSticky( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 0 && pixelColour.y == 64 && pixelColour.z == 0;
}

[[nodiscard]]
inline bool LevelLoader::isNone( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 128 && pixelColour.y == 128 && pixelColour.z == 0;
}

[[nodiscard]]
inline bool LevelLoader::isPlatform( glm::vec3 const &pixelColour ) const noexcept {
	return !(isEnemy(pixelColour) || isStartPoint(pixelColour) || isGoal(pixelColour) || isWhite(pixelColour));
}

[[nodiscard]]
inline bool LevelLoader::isMovingPlatform( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 0 && pixelColour.y == 255 && pixelColour.z >= 120 && pixelColour.z <= 130;
}

[[nodiscard]]
inline bool LevelLoader::isButton( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 150 && pixelColour.y >= 70 && pixelColour.y < 80 && pixelColour.z == 0;
}

bool LevelLoader::isDoor( glm::vec3 const &pixelColour ) const noexcept {
	return pixelColour.x == 150 && pixelColour.y >= 80 && pixelColour.y < 90;
}
