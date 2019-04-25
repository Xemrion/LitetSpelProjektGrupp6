#pragma once
#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Geometry.h"
#include "Collisions.h"
#include "Platform.h"
#include <functional>

using namespace std; // usch

extern double dt;



class LevelGoal : public IObject {
public:
    using TriggerCallback = std::function<void(void)>;
    LevelGoal( CollisionManager &colManager, Box bounds, TriggerCallback cb=[](){} );
    virtual ~LevelGoal() {}
    virtual void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) override;
private:
//  auto             _representation; // TODO
    Box              _bounds;
    TriggerCallback  _triggerCallback;
};



class Player : public IObject {
public:
    Player( glm::vec3 position = {.0f, .0f, .0f} );
    virtual ~Player();
    virtual void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) override;
    void move(float dt, glm::vec3 dir) noexcept;
    void update();
    [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
    // TODO: accessors & mutations; refactor Player logic into >>Player<<; refactor member privacy
    glm::vec3  posPrev, posCurr;
    float      moveSpeed, jumpSpeed, jumpCooldown, gravity;
    bool       hasExtraJump, isStanding;
    int        status; // TODO: enum!  powerup indicator: 0 = none 1 = bouncy 2 = heavy
};

//class Platform : public IObject {
//public:
//    Box hitbox;
//    void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other );
//};


class Level { // POD

public:
    Level( glm::vec3 startPos = { .0f, .0f, .0f } ):
        player ( startPos )
    {}

    Player         player;
    vector<Box>    boxes;
    vector<Sphere> spheres;
    CollisionManager colManager;

    //void fun() {
    //    colManager.register_entry( player, CollisionId::player_top,    pBoxTop   true );
    //    colManager.register_entry( player, CollisionId::player_side,   pBoxLeft, true );
    //    colManager.register_entry( player, CollisionId::player_side,   pBoxRight true );
    //    colManager.register_entry( player, CollisionId::player_bottom, pBoxBot,  true );
    //    colManager.register_entry( groundBox, CollisionId::platform, platBox,  false );
    //}
};

class Game {
private:
public:
	//left/right/up/down
	bool keys[4];
	Level currentLevel { { .0f, .0f, .0f } };
	
	void init();
	void update(double dt);

	//float gravity = 50.0f;

    Sphere playerSphere;
    Platform groundBox;
};