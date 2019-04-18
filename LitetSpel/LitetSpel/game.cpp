#include "game.h"
#define GRAVITY_CONSTANT -200.0f
#define JUMP_CONSTANT 200.0f
#define COOLDOWN_CONSTANT 0.1f

void Game::init() {
    groundBox.hitbox.center = glm::vec4(0, -30, 0, 0);
    groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
	currentLevel.boxes.push_back(groundBox.hitbox);
	
    currentLevel.colManager.register_entry( groundBox, CollisionId::platform, groundBox.hitbox, true );

	testPlat.hitbox.center = glm::vec4(30.0f,0.0f,0.0f,0.0f);
	testPlat.hitbox.halfLengths = glm::vec4(10.0f, 20.0f, 10.0f, 0.0f);
	currentLevel.boxes.push_back(testPlat.hitbox);
	currentLevel.colManager.register_entry(testPlat, CollisionId::platform, testPlat.hitbox, true);

	updatePlayerCollision();

    currentLevel.colManager.register_entry(currentLevel.player,  CollisionId::player_bottom, currentLevel.player.HitboxBottom, false);
	currentLevel.colManager.register_entry(currentLevel.player,  CollisionId::player_top,    currentLevel.player.HitboxTop,    false);
	currentLevel.colManager.register_entry(currentLevel.player,  CollisionId::player_left,   currentLevel.player.HitboxLeft,   false);
	currentLevel.colManager.register_entry(currentLevel.player,  CollisionId::player_right,   currentLevel.player.HitboxRight,  false);

	//currentLevel.boxes.push_back(currentLevel.player.HitboxBottom);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxLeft);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxRight);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxTop);
}




Player::Player( glm::vec3 position ):
    IObject(),
    posPrev      (position),
    posCurr      (position),
    moveSpeed    (10.0f),
    jumpSpeed    (.0f),
    jumpCooldown (.0f),
    gravity      (GRAVITY_CONSTANT),
    hasExtraJump (true),
    isStanding   (false),
    status       (0) // TODO: enum!
{
    // skapa fyra hitboxar
}

Player::~Player() {}


void Player::move( float dt, glm::vec3 dir ) noexcept {
    // TODO: movea alla player hitboxar
    posPrev = posCurr;
    posCurr = posPrev + (dir * moveSpeed * dt);
}

glm::vec3 const& Player::getPosition() const noexcept {
    return posCurr;
}


void Player::update() {
    if (isStanding) {
        gravity = 0;
        jumpSpeed = 0;
        hasExtraJump = true;
    }
	else
	{
		gravity = GRAVITY_CONSTANT;
	}
}

void Player::collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) {
	if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_bottom) 
	{
		isStanding = true;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_top)
	{
		jumpSpeed = 0;
		//posCurr.y -= 1;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_left)
	{
		posCurr.x += moveSpeed*dt;

	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_right)
	{
		posCurr.x -= moveSpeed * dt;
	}
}

void Game::update(double dt) {
	if (keys[0]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.status == 2) {
			//currentLevel.player.move(dt, glm::vec3(-0.2, 0, 0));
			currentLevel.player.posCurr.x -= currentLevel.player.moveSpeed*dt/3;
		}
		else {
			//currentLevel.player.move(dt, glm::vec3(-1, 0, 0));
			currentLevel.player.posCurr.x -= currentLevel.player.moveSpeed*dt;
		}

	}
	if (keys[1]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.status == 2) {
			//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
			currentLevel.player.posCurr.x += currentLevel.player.moveSpeed*dt/3;
		}
		else {
			//currentLevel.player.move(dt, glm::vec3(1, 0, 0));
			currentLevel.player.posCurr.x += currentLevel.player.moveSpeed*dt;
		}
	}
	if (keys[2]) {
		if (currentLevel.player.isStanding == true && currentLevel.player.jumpCooldown <= 0 && currentLevel.player.status != 2) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT * float(dt);
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
		else if (currentLevel.player.status == 1 && currentLevel.player.hasExtraJump == true && currentLevel.player.isStanding == false && currentLevel.player.jumpCooldown <= 0) {
			currentLevel.player.hasExtraJump = false;
			currentLevel.player.jumpSpeed = JUMP_CONSTANT * float(dt);
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}

		else if (currentLevel.player.status == 2 && currentLevel.player.isStanding == true) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT/2 * float(dt);
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
	}
	if (keys[3]) {
		//currentLevel.player.move(dt, glm::vec3(0, 1, 0));
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
    // gravity
	currentLevel.player.jumpSpeed = currentLevel.player.jumpSpeed + (currentLevel.player.gravity * float(dt)) / 100;
	currentLevel.player.posCurr.y += currentLevel.player.jumpSpeed;

	currentLevel.player.jumpCooldown -= dt;

	updatePlayerCollision();

	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);

	
	currentLevel.player.isStanding = false;
    currentLevel.colManager.update();
    currentLevel.player.update();
	
}

void Game::updatePlayerCollision()
{
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		5.0);
	currentLevel.spheres.push_back(playerSphere);
	//####################################################################Bottom
	currentLevel.player.HitboxBottom.center = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y - 0.5*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxBottom.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.6,
		playerSphere.centerRadius.w*0.6,
		playerSphere.centerRadius.w*0.8,
		0);
	//####################################################################Top
	currentLevel.player.HitboxTop.center = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y + 0.5*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxTop.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.6,
		playerSphere.centerRadius.w*0.5,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################Left
	currentLevel.player.HitboxLeft.center = glm::vec4(
		currentLevel.player.posCurr.x - 0.9*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxLeft.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.3,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);

	//####################################################################Right
	currentLevel.player.HitboxRight.center = glm::vec4(
		currentLevel.player.posCurr.x + 0.9*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxRight.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.3,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################
}
