#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include "Geometry.h"
#include "Collisions.h"
#include "Platform.h"
#include "Blob.h"
#include "Globals.h"

// TODO: improve encapsulation by reducing public exposure

using namespace std; // läs: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rs-using-directive



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
	Enemy(glm::vec3 position);
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

// TODO: commented lines
class LevelGoal : public CollisionObject {
public:
    using TriggerCallback = std::function<void(void)>;
    LevelGoal( CollisionManager &_colMan, glm::vec3 const &position, float radius, TriggerCallback cb=[](){} );
    virtual ~LevelGoal();
    virtual void collide( ColliderType  ownHitbox,
                          ColliderType  otherHitbox,
                          Box const    &other ) noexcept override;

    Box               representation;

private:
    Box               _bounds;
    TriggerCallback   _triggerCallback;
    CollisionManager *_colMan;
};



struct LevelData { // POD -- TODO: switch from POD struct to class 
    // TODO: refactor game objects into a hierarchy
    //       i.e. a vector<SceneEntry> instead of vector<Enemy> + vector<Platform> + ... etc
    Player                player;
	vector<Enemy>         enemies;
    vector<Platform>      platforms;
    unique_ptr<LevelGoal> goal;
    vector<Box>           boxes;
    vector<Sphere>        spheres;
    CollisionManager      colManager;
    // TODO:
    // struct SceneEntry { unique_ptr<GObject> object; glm::vec3 position; };     
    // vector<SceneEntry> scene;
    // void LevelData::add( glm::vec3 const &location, unique_ptr<GObject> object ) noexcept;
    // bool LevelData::remove( GObject const &target ) noexcept;
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

    Box      enemyBox {{0,0,0,0},{3,3,3,0},{0,1,0,0}};
    Sphere   playerSphere;
    Platform groundBox;
	Platform testPlat;
	Platform testplat2;
};


