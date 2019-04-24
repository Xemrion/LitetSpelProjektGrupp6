#include "game.h"
#define GRAVITY_CONSTANT -200.0f
#define JUMP_CONSTANT 1.5f
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


	testplat2.hitbox.center = glm::vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	testplat2.hitbox.halfLengths = glm::vec4(10.0f, 2.0f, 10.0f, 0.0f);
	currentLevel.boxes.push_back(testplat2.hitbox);
	currentLevel.colManager.register_entry(testplat2, CollisionId::platform, testplat2.hitbox, true);

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
    moveSpeed    (0.0f),
    jumpSpeed    (.0f),
    jumpCooldown (.0f),
    gravity      (GRAVITY_CONSTANT),
    hasExtraJump (true),
    isStanding   (false),
	isStuck      (false),
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
		if (isStuck == false) 
		{
			gravity = GRAVITY_CONSTANT;
		}

	}
}

void Player::collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) {
	if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_bottom) 
	{
		isStanding = true;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_top)
	{
		if (status == 3)
		{
			gravity = 0;
			isStuck = true;
			posCurr.y -= 1;
		}
		jumpSpeed = 0;
		
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_left)
	{
		if (status == 3 && isStanding == false)
		{
			gravity = 0;
			jumpSpeed = 0;
			isStuck = true;
		}
		posCurr.x += moveSpeed*dt;

	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_right)
	{
		if (status == 3 && isStanding == false)
		{
			gravity = 0;
			jumpSpeed = 0;
			isStuck = true;
		}
		posCurr.x -= moveSpeed * dt;
	}
}

void Game::update(double dt) {
	time += dt;
	currentLevel.player.moveSpeed = 0.0f;
	if (keys[0]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.isStuck == false)
		{
			if (currentLevel.player.status == 2) {
				//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
				currentLevel.player.posCurr.x = currentLevel.player.moveSpeed*dt / 3;
			}
			else {
				//currentLevel.player.move(dt, glm::vec3(1, 0, 0));
				currentLevel.player.posCurr.x -= currentLevel.player.moveSpeed*dt;
			}
		}

	}
	if (keys[1]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.isStuck == false) 
		{
			if (currentLevel.player.status == 2) {
				//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
				currentLevel.player.posCurr.x += currentLevel.player.moveSpeed*dt / 3;
			}
			else {
				//currentLevel.player.move(dt, glm::vec3(1, 0, 0));
				currentLevel.player.posCurr.x += currentLevel.player.moveSpeed*dt;
			}
		}

	}
	if (keys[2]) {
		if (currentLevel.player.isStanding == true && currentLevel.player.jumpCooldown <= 0 && currentLevel.player.status != 2) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
		else if (currentLevel.player.status == 1 && currentLevel.player.hasExtraJump == true && currentLevel.player.isStanding == false && currentLevel.player.jumpCooldown <= 0) {
			currentLevel.player.hasExtraJump = false;
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}

		else if (currentLevel.player.status == 2 && currentLevel.player.isStanding == true) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT/2;
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
	}
	if (keys[3]) {
		if (currentLevel.player.status == 3) 
		{
			currentLevel.player.isStuck = false;
			if (currentLevel.player.isStanding == false) 
			{
				currentLevel.player.gravity = GRAVITY_CONSTANT*2;
			}
		}
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

	/*** Animation of orbiting spheres ***/
	glm::vec3 rotationSpeed = glm::vec3(0.81, 0.53, 0.1);
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	glm::vec3 offset = glm::vec3(0.2, 0.0, 0.0);
	// Multiplier to animate faster when moving a certain direction. Not smooth.
	glm::vec2 movementMultiplier = glm::vec2(
		glm::clamp(abs(currentLevel.player.moveSpeed), 1.0f, 5.0f), 
		glm::clamp(currentLevel.player.jumpSpeed, 1.0f, 2.0f)
	);

	// How far the spheres move away from the original sphere. Careful with the z axis, it can cause graphical glitches if it is too big
	glm::vec3 amplitude = glm::vec3(2.4, 1.7, 0.8);

	Sphere playerSphere2;
	playerSphere2.centerRadius = glm::vec4(
		playerSphere.centerRadius.x + sin(time * (rotationSpeed.x * movementMultiplier.x) + offset.x) * amplitude.x,
		playerSphere.centerRadius.y + sin(time * (rotationSpeed.y * movementMultiplier.y) + offset.y) * amplitude.y,
		playerSphere.centerRadius.z + sin(time * rotationSpeed.z + offset.z) * amplitude.z,
		2.5
	);
	currentLevel.spheres.push_back(playerSphere2);

	offset = glm::vec3(0.45, 0.9, 1.1);
	Sphere playerSphere3;
	playerSphere3.centerRadius = glm::vec4(
		playerSphere.centerRadius.x + sin(time * (rotationSpeed.x * movementMultiplier.x) + offset.x) * amplitude.x,
		playerSphere.centerRadius.y + sin(time * (rotationSpeed.y * movementMultiplier.y) + offset.y) * amplitude.y,
		playerSphere.centerRadius.z + sin(time * rotationSpeed.z + offset.z) * amplitude.z,
		2.5
	);
	currentLevel.spheres.push_back(playerSphere3);
	/*** ***/
	
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
