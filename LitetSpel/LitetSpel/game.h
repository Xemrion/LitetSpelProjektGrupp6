#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Geometry.h"
#include "Collisions.h"
#include "Platform.h"
#include "Blob.h"
#include "Globals.h"

// TODO: improve encapsulation by reducing public exposure

using namespace std;

enum PlayerStatus {
	None,
	Bouncy,
	Sticky,
	Heavy
};

class Player : public CollisionObject {
public:
    Player( glm::vec3 position = {.0f, .0f, .0f} );
    virtual ~Player() noexcept;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept override;
    void move(double dt) noexcept;
	void setVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void addVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void putForce(glm::vec3 const &force) noexcept;
	void shoot(glm::vec3 mousePos) noexcept;
	void recallBlobs() noexcept;
    void update(double dt) noexcept;

    glm::vec3 pos, velocity;
    float moveSpeed, jumpForce, jumpCooldown, mass;
    bool hasExtraJump, isStanding, isStuck;
    int status;
	double radius;
	Box HitboxBottom, HitboxTop, HitboxLeft, HitboxRight;
    vector<Blob> blobs;
    int blobCharges;
    float shootCooldown;
};

class Enemy : public CollisionObject 
{
public:
	Enemy(glm::vec3 position = { -25.0f, 20.0f, 0.0f });
	virtual ~Enemy() noexcept;
	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept override;
	void update(double dt) noexcept;
	void addVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void setVelocity(glm::vec3 const &velocity, bool useSpeed = false) noexcept;
	void putForce(glm::vec3 const &force) noexcept;
	void move(float dt) noexcept;

    glm::vec3 pos, velocity, controlDir;
    float moveSpeed, jumpForce, jumpCooldown, mass;
    bool isStanding, alive, isDeregistered;
    Box HitboxBottom, HitboxTop, HitboxLeft, HitboxRight;
};

struct LevelData { // POD
    // TODO: switch from POD struct to class 
    // and merge in level start / goal code from falk branch
    Player           player;
	Enemy            enemy;
    vector<Box>      boxes;
    vector<Sphere>   spheres;
    CollisionManager colManager;
};

class Game {
private:
public:
    // TODO: refactor into ctor initializer list
	double physicsSimTime = 0.0;
	double time = 0.0;

	enum Keys { // TODO: refactor into Globals.h
        left,
		right,
		up,
		down,
		length
	};
	bool keys[Keys::length];
	LevelData level;
	
	glm::vec3 mousePos;
	bool leftButtonDown = false;

	void init() noexcept;
	void update(double dt);
	void updatePhysics();
	void updatePlayerCollision();
	void updateEnemyCollision();
	void showHitboxes();
	void handleInput();
	void updateGraphics();

	void animateSphere( Sphere    const &sphere,
                        glm::vec2 const &moveSpeed,
                        glm::vec3 const &amplitude = {2.4f, 1.7f, 0.8f} );

	Box      EnemyBox;
    Sphere   playerSphere;
    Platform groundBox;
	Platform testPlat;
	Platform testplat2;
};