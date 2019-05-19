#pragma once

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
#include"MovingPlatform.h"
#include"PowerUp.h"
#include"Button.h"
#include"Gate.h"

class Editor {
public:
	Editor();
	~Editor();
    void initialize( const char* filename );

    // TODO: privatize?
	glm::vec3                    goalPos,
                                 startPos;

    // TODO: replace with a private level pointer
	std::vector<PowerUp>         powerups;
	std::vector<glm::vec3>       enemyPos; // TODO
	std::vector<Gate>            gates;
	std::vector<Button>          buttons;
    std::vector<Platform>        platforms;
    std::vector<MovingPlatform>  movingPlatforms;

private:
    [[nodiscard]] inline bool isWhite(          glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isGoal(           glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isEnemy(          glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isStartPoint(     glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isPlatform(       glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isBouncy(         glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isHeavy(          glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isSticky(         glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isNone(           glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isMovingPlatform( glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isDoor(           glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] inline bool isButton(         glm::vec3 const &pixelColour ) const noexcept;
    [[nodiscard]] glm::vec3 getPixelColour( int index ) const noexcept;
    [[nodiscard]] int isPixelUsed( int index ) const noexcept;
    void addBoxToUsed( int startPosX, int startPosY, int endPosX, int endPosY );

    unsigned char   *rgb;

    int              minimumBoxSize          = 30,
                     width                   =  0,
                     height                  =  0,
                     doorHeight              =  5,
                     buttonWidth             =  5,
                     maxMovingPlatformLength = 30;

    std::vector<int> LeftSide,
                     topSide,
                     bottomSide,
                     rightSide;
};
