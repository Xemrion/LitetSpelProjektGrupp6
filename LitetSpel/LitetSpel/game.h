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
#include "Blob.h"

using namespace std;


extern double dt;

enum PlayerStatus 
{
	None,
	Bouncy,
	Sticky,
	Heavy
};

class Player : public IObject {
public:
    Player( glm::vec3 position = {.0f, .0f, .0f} );
    virtual ~Player();
    virtual void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) override;
    void move(float dt, glm::vec3 dir) noexcept;
	vector<Blob> blobs;
	int blobCharges = 5;
	int nrOfActiveBlobs = 0;
	float shootCooldown = 0;
	void shoot(glm::vec3 mousePos);
	void recallBlobs();
    void update();
    [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
    // TODO: accessors & mutations; refactor Player logic into >>Player<<; refactor member privacy
    glm::vec3  posPrev, pos;
    float      moveSpeed, jumpSpeed, jumpCooldown, gravity;
    bool       hasExtraJump, isStanding, isStuck;
    int        status; // TODO: enum!  powerup indicator: 0 = none |1 = bouncy |2 = heavy |3 = Sticky
	double radius = 5.0;
	Box HitboxBottom, HitboxTop, HitboxLeft, HitboxRight;
};

class Enemy : public IObject 
{
public:
	glm::vec3  posPrev, pos;
	float      EmoveSpeed, EjumpSpeed, EjumpCooldown, Egravity;
	bool enemyStanding, isJumping, canJump;
	Box HitboxBottom, HitboxTop, HitboxLeft, HitboxRight;

	Enemy(glm::vec3 position = {-20.0f, 40.0f, 0.0f});
	virtual ~Enemy();
	virtual void collide(CollisionId ownHitbox, CollisionId otherHitbox, IObject &other) override;
	void update();
	void move();
};


struct LevelData { // POD
    Player         player;
	Enemy          enemy;
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

struct SFX {
	char* fileName;
	IDirectSoundBuffer8* soundBuffer;
};

class Game {
private:
public:
	double time = 0.0;
	//left/right/up/down
	bool keys[4];
	LevelData currentLevel;
	
	glm::vec3 mousePos;
	bool leftButtonDown = false;

	void init();
	void update(double dt);
	void updatePlayerCollision();
	void updateEnemyCollision();

	void addSphereAnimation(Sphere sphere, glm::vec2 moveSpeed);

	//float gravity = 50.0f;

	Box EnemyBox;
	Sounds* gameSounds;
    Sphere playerSphere;
    Platform groundBox;
	Platform testPlat;
	Platform testplat2;
};


