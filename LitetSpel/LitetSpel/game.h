#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <memory>

#include "Geometry.h"
#include "Platform.h"
#include "Interfaces.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Graphics.h"
#include "Player.h"
#include "Level.h"
#include "LevelLoader.h"

class Game {
public:
    enum class State { level,
                       menu };

    Game( KeyboardInput &, MouseInput &, Graphics & );
	void   init() noexcept;
	void   update(double dt_s);
    void   loadLevel();
    void   menuLoad();

    Input& getInput() noexcept;
    Level& getLevel() noexcept; // temp
    State  getState() const noexcept;

private:
    void updateInput();
    void updateLogic(double dt_s);
    void updatePhysics();
    void updateAnimations( double dt_s ) noexcept;
    void updateGraphics();
    void showHitboxes() noexcept;

    double                  physicsSimTime  { .0 },
                            time            { .0 };
    std::unique_ptr<Level>  level;
    KeyboardInput          *keyboard;
    MouseInput             *mouse;
    Graphics               *graphics;
    LevelLoader             levelLoader{ *graphics }; // temp hack
    Box                     menuBg,
                            menuYes,
                            menuNo;
    State                   state;
};
