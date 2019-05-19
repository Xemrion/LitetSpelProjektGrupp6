#pragma once

#include "Globals.h"
#include "Interfaces.h"
#include "Collisions.h"
#include "Player.h"

class Level {
    using SceneEntry   = std::unique_ptr<IObject>;  
    using SceneEntries = std::vector<SceneEntry>;
    using Boxes        = std::vector<Box>;
    using Spheres      = std::vector<Sphere>;

public:
    Level();
    void              add(std::unique_ptr<IObject> object) noexcept;
    bool              remove(IObject &target) noexcept;
    Player&           getPlayer()             noexcept;
    SceneEntries&     getScene()              noexcept;
    Boxes&            getDynamicBoxes()       noexcept;
    Boxes&            getStaticBoxes()        noexcept;
    Spheres&          getSpheres()            noexcept;
    CollisionManager& getCollisionManager()   noexcept;
    void              onFrameStart();

private:
    void notifyOfVictory() noexcept; // for LevelGoal callback
 
    Boxes             dynamicBoxes,
                      staticBoxes;
    Spheres           spheres;
    CollisionManager  collisionManager;
    SceneEntries      scene;
    Player           *player;
};
