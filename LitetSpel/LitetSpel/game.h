#pragma once
#include <d3d11.h>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Geometry.h"
#include "Collisions.h"
#include "Sounds.h"
#include  "Platform.h"

using namespace std;


extern double dt;

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


struct LevelData { // POD
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
	LevelData currentLevel;
	
	void init();
	void update(double dt);

	//float gravity = 50.0f;
	Sounds* gameSounds;
    Sphere playerSphere;
    Platform groundBox;
};

struct SFX {
	char* fileName;
	IDirectSoundBuffer8* soundBuffer;
};