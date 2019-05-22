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
#include "PowerUp.h"
#include "Graphics.h"
#include "Editor.h"
#include "Gate.h"

using namespace glm;
enum PlayerStatus {
	None,
	Bouncy,
	Sticky,
	Heavy
};

class Player : public CollisionObject {
public:
    Player(vec3 position = {.0f, .0f, .0f} );
    virtual ~Player() noexcept;
    virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
    void move(double dt) noexcept;
	void setVelocity(vec3 const &velocity, bool useSpeed = false) noexcept;
	void addVelocity(vec3 const &velocity, bool useSpeed = false) noexcept;
	void putForce(vec3 const &force) noexcept;
	void shoot(vec3 mousePos) noexcept;
	void recallBlobs() noexcept;
    void update(double dt) noexcept;

    vec3 pos, velocity, controlDir;
    float moveSpeed, jumpForce, jumpCooldown, mass;
    bool hasExtraJump, isStanding, isStuck, knockBack, levelCompleted, landing;
    PlayerStatus status;
	double radius;
	Box hitbox;
    vector<Blob> blobs;
    int blobCharges;
	int lifeCharges;
    float shootCooldown;
	float takeDamageCooldown;
	const MovingPlatform* collidingMovingPlatform = nullptr;
};

class Enemy : public CollisionObject 
{
public:
	Enemy(vec3 position = { 0.0f, 20.0f, 0.0f });
	virtual ~Enemy() noexcept;
	virtual void collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept override;
	void update(double dt) noexcept;
	void addVelocity(vec3 const &velocity, bool useSpeed = false) noexcept;
	void setVelocity(vec3 const &velocity, bool useSpeed = false) noexcept;
	void putForce(vec3 const &force) noexcept;
	void move(float dt) noexcept;

    vec3 pos, velocity, controlDir, playerPos;
    float moveSpeed, jumpForce, jumpCooldown, mass;
	int enemyIndex;
    bool isStanding, alive, isDeregistered, isStuck;
    Box hitbox;
};

// TODO: commented lines
class LevelGoal : public CollisionObject {
public:
    using TriggerCallback = std::function<void(void)>;
    LevelGoal( CollisionManager &_colMan, vec3 const &position, float radius, TriggerCallback cb=[](){} );
    virtual ~LevelGoal();
    virtual void collide( ColliderType  ownHitbox,
                          const HitboxEntry& other) noexcept override;

    Box               representation;

private:
    Box               _bounds;
    TriggerCallback   _triggerCallback;
    CollisionManager *_colMan;
};


struct LevelData { // POD
    // TODO: switch from POD struct to class 
    // and merge in level start / goal code from falk branch
    Player           player;
	Enemy            enemy;
	vector<PowerUp>  powerUps;
	std::unique_ptr<LevelGoal> goal;
	vector<MovingPlatform> movingPlatforms;
    vector<Box>      staticBoxes;
	vector<Box>      movingBoxes;
    vector<Sphere>   spheres;
	vector<Gate>     gates;
	vector<Button>   buttons;
    CollisionManager colManager;
};

enum GameState 
{
	LevelState,
	MenuState
};

class Game {
private:
	glm::vec3 cameraOffset = glm::vec3(0.0, 20.0, -150.0);
	glm::vec3 cameraLookDownOffset = glm::vec3(0.0, -20.0, -150);
	glm::vec3 cameraPos = glm::vec3(0.0, 20.0, -100.0);
	bool panCamera = false;
public:
	double physicsSimTime = 0.0;
	double time = 0.0;
	GameState state;
	

	enum Keys {
		left,
		right,
		up,
		down,
		length
	};
	bool keys[Keys::length];
	LevelData level;
	Editor editor;
	glm::vec3 mousePos;
	bool leftButtonDown = false;

	void init() noexcept;
	void menuLoad();
	void update(double dt);
	void updatePhysics();
	void updatePlayerCollision();
	void updateEnemyCollision();
	void showHitboxes();
	void handleInput();
	void updateGraphics();

	void animateSphere( Sphere    const &sphere,
                        vec3 const &amplitude = {2.4f, 1.7f, 0.8f} );
	void animateColor(Graphics& graphics);
	void animateVictory(Sphere    const &sphere);

	glm::vec3 getCameraPos() { return cameraPos; };
	bool getCameraPan() { return panCamera; };

	Sounds* gameSounds;
	Box      EnemyBox;
    Sphere   playerSphere;
    Platform groundBox;
	Platform testPlat;
	Platform testplat2;

	Box      MenuBG;
	Box      MenuYes;
	Box      MenuNo;
};


