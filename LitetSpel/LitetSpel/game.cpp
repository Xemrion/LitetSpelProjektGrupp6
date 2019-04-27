#include "game.h"

void Game::init() {
    groundBox.hitbox.center = glm::vec4(0, -30, 0, 0);
    groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
	groundBox.hitbox.color = glm::vec4(1.0, 1.0, 1.0, 0.0);
	level.boxes.push_back(groundBox.hitbox);

	level.colManager.registerEntry(groundBox, ColliderType::platform, groundBox.hitbox, true);

	testPlat.hitbox.center = glm::vec4(30.0f, 0.0f, 0.0f, 0.0f);
	testPlat.hitbox.halfLengths = glm::vec4(10.0f, 20.0f, 10.0f, 0.0f);
	testPlat.hitbox.color = glm::vec4(0.0, 1.0, 0.0, 0.0);
	level.boxes.push_back(testPlat.hitbox);
	level.colManager.registerEntry(testPlat, ColliderType::platform, testPlat.hitbox, true);

	testplat2.hitbox.center = glm::vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	testplat2.hitbox.halfLengths = glm::vec4(10.0f, 2.0f, 10.0f, 0.0f);
	testplat2.hitbox.color = glm::vec4(0.0, 0.5, 0.5, 0.0);
	level.boxes.push_back(testplat2.hitbox);
	level.colManager.registerEntry(testplat2, ColliderType::platform, testplat2.hitbox, true);

	for (int i = 0; i < level.player.blobCharges; i++)
	{
		level.player.blobs.push_back(Blob(level.player.pos));
		level.spheres.push_back(level.player.blobs[i].blobSphere);
	}
	updatePlayerCollision();

	level.colManager.registerEntry(level.player, ColliderType::player_bottom, level.player.HitboxBottom, false);
	level.colManager.registerEntry(level.player, ColliderType::player_top, level.player.HitboxTop, false);
	level.colManager.registerEntry(level.player, ColliderType::player_left, level.player.HitboxLeft, false);
	level.colManager.registerEntry(level.player, ColliderType::player_right, level.player.HitboxRight, false);

	level.colManager.registerEntry(level.enemy, ColliderType::enemy_bottom, level.enemy.HitboxBottom, false);
	level.colManager.registerEntry(level.enemy, ColliderType::enemy_top, level.enemy.HitboxTop, false);
	level.colManager.registerEntry(level.enemy, ColliderType::enemy_left, level.enemy.HitboxLeft, false);
	level.colManager.registerEntry(level.enemy, ColliderType::enemy_right, level.enemy.HitboxRight, false);

	for (Blob& b : level.player.blobs) {
		level.colManager.registerEntry(b, ColliderType::blob, b.hitbox, false);
	}
	EnemyBox.color = glm::vec4(1,0,0,0);
}




Player::Player(glm::vec3 position) :
	CollisionObject(),
	pos(position),
	velocity(glm::vec3(0.0)),
	moveSpeed(150.0f),
	mass(10.0),
	jumpForce(1200.0f),
	jumpCooldown(.0f),
	hasExtraJump(true),
	isStanding(false),
	isStuck(false),
	status(PlayerStatus::None)
{
	// skapa fyra hitboxar
}

Player::~Player() {}

// Set useSpeed to true to multiply velocity by objects speed value
void Player::setVelocity(glm::vec3 velocity, bool useSpeed) {
	if (useSpeed) {
		this->velocity = velocity * moveSpeed;
	}
	else {
		this->velocity = velocity;
	}
}

// Set useSpeed to true to multiply velocity by objects speed value
void Player::addVelocity(glm::vec3 velocity, bool useSpeed) {
	if (useSpeed) {
		this->velocity += velocity * moveSpeed;
	}
	else {
		this->velocity += velocity;
	}
}

// velocity += force / mass;
void Player::putForce(glm::vec3 force) {
	this->velocity += force / mass;
}

// Call from updatePhysics
void Player::move(float dt) {
	this->pos += velocity * dt;
}

// Updates logic, call once per frame
void Player::update(double dt) {
	jumpCooldown -= dt;
	shootCooldown -= dt;

	if (isStanding) {
		hasExtraJump = true;
	}
	
	if (status == PlayerStatus::Heavy) {
		mass = 20;
	}
	else {
		mass = 10;
	}

	for (int i = 0; i < blobs.size(); i++)
	{
		if (blobs[i].isBeingRecalled)
		{
			if (glm::length((pos - blobs[i].pos)) < this->radius)
			{
				blobs[i].isBeingRecalled = false;
				blobs[i].isActive = false;
			}
		}
		else if (!blobs[i].isActive)
		{
			blobs[i].pos = pos + glm::vec3(0.0, 2.0, 0.0);
			blobs[i].setVelocity(glm::vec3(0.0));
			blobs[i].blobSphere.centerRadius = glm::vec4(pos, 2);
		}
		blobs[i].update(dt);
	}
}

void Player::collide(ColliderType ownHitbox, ColliderType otherHitbox, Box other) 
{
	if (ownHitbox == ColliderType::player_bottom) {
		if (otherHitbox == ColliderType::platform) {
			this->isStanding = true;
			this->pos.y = other.center.y + other.halfLengths.y + (pos.y - HitboxBottom.center.y + HitboxBottom.halfLengths.y);
			this->velocity.y = 0;
		}
		else {
			this->isStanding = false;
		}
	}
	else if (ownHitbox == ColliderType::player_top) {
		if (otherHitbox == ColliderType::platform) {
			this->pos.y = other.center.y - other.halfLengths.y + (pos.y - HitboxTop.center.y - HitboxTop.halfLengths.y);
			this->velocity.y = 0;
			if (this->status == PlayerStatus::Sticky) {
				this->isStuck = true;
				this->pos.y -= 1;
			}
		}
	}
	else if (ownHitbox == ColliderType::player_left) {
		if (otherHitbox == ColliderType::platform) {
			this->pos.x = other.center.x + other.halfLengths.x + (pos.x - HitboxLeft.center.x + HitboxLeft.halfLengths.x);
			
			if (this->status == PlayerStatus::Sticky) {
				this->isStuck = true;
			}
		}
	}
	else if (ownHitbox == ColliderType::player_right) {
		if (otherHitbox == ColliderType::platform) {
			this->pos.x = other.center.x - other.halfLengths.x + (pos.x - HitboxRight.center.x - HitboxRight.halfLengths.x);

			if (this->status == PlayerStatus::Sticky) {
				this->isStuck = true;
			}
		}
	}
}

void Player::shoot(glm::vec3 mousePos)
{
	mousePos = glm::vec3((mousePos.x - 1280 / 2) * 9, (-(mousePos.y - 980 / 2)) * 16, 0);
	glm::vec3 dir = glm::normalize(mousePos - pos);
	for (int i = 0; i < blobs.size(); i++)
	{
		if (!blobs[nrOfActiveBlobs + i].isBeingRecalled)
		{
			blobs[nrOfActiveBlobs + i].setVelocity(dir, true);
			blobs[nrOfActiveBlobs + i].isActive = true;
			shootCooldown = 0.5f;
			nrOfActiveBlobs++;
			break;
		}
	}

}

void Player::recallBlobs()
{
	for (int i = 0; i < nrOfActiveBlobs; i++)
	{
		blobs[i].isActive = false;
		blobs[i].isBeingRecalled = true;
		blobs[i].setVelocity(glm::normalize(pos - blobs[i].pos), true);
	}
	shootCooldown = 0.5f;
	nrOfActiveBlobs = 0;
}

void Game::update(double dt) {
	time += dt;
	level.player.velocity.x = 0;

	handleInput();
	level.player.update(dt);
	level.enemy.update(dt);
	updatePhysics();
	updateGraphics();
}

// Call first of all per frame updates
void Game::handleInput() {

	if (leftButtonDown
		&& level.player.nrOfActiveBlobs < level.player.blobCharges
		&& level.player.shootCooldown <= 0)
	{
		level.player.shoot(mousePos);
	}

	if (keys[Keys::left]) {
		if (level.player.isStuck == false)
		{
			level.player.addVelocity(glm::vec3(-1, 0, 0), true);
		}
	}
	if (keys[Keys::right]) {
		if (level.player.isStuck == false)
		{
			level.player.addVelocity(glm::vec3(1, 0, 0), true);
		}

	}
	if (keys[Keys::up]) {
		if (level.player.isStanding == true && level.player.jumpCooldown <= 0) {
			level.player.isStanding = false;
			level.player.jumpCooldown = COOLDOWN_CONSTANT;
			level.player.putForce(glm::vec3(0.0, level.player.jumpForce, 0.0));
		}
		else if (level.player.status == PlayerStatus::Bouncy && level.player.hasExtraJump == true) {
			level.player.hasExtraJump = false;
			level.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
	}
	if (keys[Keys::down]) {
		if (level.player.status == PlayerStatus::Sticky)
		{
			level.player.isStuck = false;
			if (level.player.isStanding == false)
			{
				level.player.addVelocity(glm::vec3(0.0, GRAVITY_CONSTANT, 0.0));
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
}

// Catches up the physics simulation time to the actual game time
// Call last of all logic updates (but before graphics)
void Game::updatePhysics() {
	float timestep = 0.0001f;
	
	while (physicsSimTime + timestep < time) {
		level.player.addVelocity(glm::vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
		level.player.move(timestep);

		if (level.enemy.alive) {
			level.player.addVelocity(glm::vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
			level.enemy.move(timestep);
		}

		for (int i = 0; i < level.player.blobs.size(); i++) {
			if (level.player.blobs[i].isActive) {
				level.player.blobs[i].addVelocity(glm::vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
			}
			level.player.blobs[i].move(timestep);
			//level.player.blobs[i].updateCollisions();
		}
		updatePlayerCollision();
		updateEnemyCollision();
		level.colManager.update();

		physicsSimTime += timestep;
	}
}

void Game::updatePlayerCollision()
{
	//####################################################################Bottom
	level.player.HitboxBottom.center = glm::vec4(
		level.player.pos.x,
		level.player.pos.y - 0.4*playerSphere.centerRadius.w,
		level.player.pos.z,
		0);
	level.player.HitboxBottom.halfLengths = glm::vec4(
		level.player.radius*0.5,
		level.player.radius*0.4,
		level.player.radius*0.1,
		0);
	//####################################################################Top
	level.player.HitboxTop.center = glm::vec4(
		level.player.pos.x,
		level.player.pos.y + 0.4*playerSphere.centerRadius.w,
		level.player.pos.z,
		0);
	level.player.HitboxTop.halfLengths = glm::vec4(
		level.player.radius*0.5,
		level.player.radius*0.4,
		level.player.radius*0.1,
		0);
	//####################################################################Left
	level.player.HitboxLeft.center = glm::vec4(
		level.player.pos.x - 0.5*playerSphere.centerRadius.w,
		level.player.pos.y,
		level.player.pos.z,
		0);
	level.player.HitboxLeft.halfLengths = glm::vec4(
		level.player.radius*0.2,
		level.player.radius*0.7,
		level.player.radius*0.1,
		0);

	//####################################################################Right
	level.player.HitboxRight.center = glm::vec4(
		level.player.pos.x + 0.5*playerSphere.centerRadius.w,
		level.player.pos.y,
		level.player.pos.z,
		0);
	level.player.HitboxRight.halfLengths = glm::vec4(
		level.player.radius* 0.2,
		level.player.radius* 0.7,
		level.player.radius* 0.1,
		0);
	//####################################################################
}

void Game::updateEnemyCollision()
{
	EnemyBox.center = glm::vec4(
		level.enemy.pos.x,
		level.enemy.pos.y,
		level.enemy.pos.z,
		5.0);
	EnemyBox.halfLengths = glm::vec4(
		3.0,
		3.0,
		3.0,
		0.0
	);
	level.boxes.push_back(EnemyBox);
	//####################################################################Bottom
	level.enemy.HitboxBottom.center = glm::vec4(
		level.enemy.pos.x,
		level.enemy.pos.y - 0.9*EnemyBox.halfLengths.y,
		level.enemy.pos.z,
		0);
	level.enemy.HitboxBottom.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################Top
	level.enemy.HitboxTop.center = glm::vec4(
		level.enemy.pos.x,
		level.enemy.pos.y + 0.9*EnemyBox.halfLengths.y,
		level.enemy.pos.z,
		0);
	level.enemy.HitboxTop.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################Left
	level.enemy.HitboxLeft.center = glm::vec4(
		level.enemy.pos.x - 0.9*EnemyBox.halfLengths.x,
		level.enemy.pos.y,
		level.enemy.pos.z,
		0);
	level.enemy.HitboxLeft.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.7,
		EnemyBox.halfLengths.z,
		0);

	//####################################################################Right
	level.enemy.HitboxRight.center = glm::vec4(
		level.enemy.pos.x + 0.9*EnemyBox.halfLengths.x,
		level.enemy.pos.y,
		level.enemy.pos.z,
		0);
	level.enemy.HitboxRight.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.7,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################
}

// Call after all other per frame updates
void Game::updateGraphics() {
	level.spheres = vector<Sphere>();
	level.boxes = vector<Box>();

	level.boxes.push_back(groundBox.hitbox);
	level.boxes.push_back(testPlat.hitbox);
	level.boxes.push_back(testplat2.hitbox);
	EnemyBox.color = glm::vec4((float)level.enemy.isStanding, 1.0 - (float)level.enemy.isStanding, 0.0, 0.0);
	
	if (level.enemy.alive) {
		level.boxes.push_back(EnemyBox);
	}

	level.spheres = vector<Sphere>();
	playerSphere.centerRadius = glm::vec4(
		level.player.pos.x,
		level.player.pos.y,
		level.player.pos.z,
		level.player.radius);
	level.spheres.push_back(playerSphere);

	glm::vec2 animationSpeed = glm::smoothstep(-150.0f, 150.0f, glm::vec2(level.player.velocity.x, level.player.velocity.y));

	animateSphere(playerSphere, animationSpeed, glm::vec3(3.0, 3.0, 0.5));

	for (int i = 0; i < level.player.blobs.size(); i++)
	{
		level.spheres.push_back(level.player.blobs[i].blobSphere);
	}

	showHitboxes();
}

void Game::showHitboxes()
{
	level.boxes.push_back(level.player.HitboxBottom);
	level.boxes.push_back(level.player.HitboxLeft);
	level.boxes.push_back(level.player.HitboxRight);
	level.boxes.push_back(level.player.HitboxTop);

	level.boxes.push_back(level.enemy.HitboxBottom);
	level.boxes.push_back(level.enemy.HitboxTop);
	level.boxes.push_back(level.enemy.HitboxLeft);
	level.boxes.push_back(level.enemy.HitboxRight);

	for (int i = 0; i < level.player.blobCharges; i++)
	{
		level.boxes.push_back(level.player.blobs[i].hitbox);
	}
}

Enemy::Enemy(glm::vec3 position):
CollisionObject(),
pos(position),
velocity(glm::vec3(0.0, 0.0, 0.0)),
controlDir(glm::vec3(1.0, 0.0, 0.0)),
moveSpeed(20.0f),
jumpForce(450.0f),
jumpCooldown(0.3f),
mass(10.0),
isStanding(false),
alive(true)
{

}

Enemy::~Enemy(){}

void Enemy::collide(ColliderType ownHitbox, ColliderType otherHitbox, Box other)
{
	if (otherHitbox == ColliderType::platform) {
		if (ownHitbox == ColliderType::enemy_bottom)
		{
			this->pos.y = other.center.y + other.halfLengths.y + (pos.y - HitboxBottom.center.y + HitboxBottom.halfLengths.y);
			this->velocity.y = 0;
			this->isStanding = true;
		}
		else if (ownHitbox == ColliderType::enemy_top)
		{
			this->pos.y = other.center.y - other.halfLengths.y + (pos.y - HitboxTop.center.y - HitboxTop.halfLengths.y);
			this->velocity.y = 0;
		}
		else if (ownHitbox == ColliderType::enemy_left)
		{
			this->controlDir.x = -this->controlDir.x;
			this->velocity.x = -velocity.x;
		}
		else if (ownHitbox == ColliderType::enemy_right)
		{
			this->controlDir.x = -this->controlDir.x;
			this->velocity.x = -velocity.x;
		}
	}
}

// Updates logic, call once per frame before updatePhysics
void Enemy::update(double dt)
{
	velocity.x = 0.0;
	velocity.y -= GRAVITY_CONSTANT * dt;
	jumpCooldown -= dt;

	if (isStanding == false)
	{
		if (jumpCooldown <= 0.0) {
			controlDir.x = -controlDir.x;
			jumpCooldown = 1.0;
			putForce(glm::vec3(0.0, jumpForce, 0.0));
		}
	}

	addVelocity(controlDir, true);
	this->isStanding = false;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Enemy::addVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		this->velocity += velocity * moveSpeed;
	}
	else {
		this->velocity += velocity;
	}
}

// Set useSpeed to true to multiply velocity by objects speed value
void Enemy::setVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		this->velocity = velocity * moveSpeed;
	}
	else {
		this->velocity = velocity;
	}
}

// velocity += force / mass;
void Enemy::putForce(glm::vec3 force)
{
	this->velocity += force / mass;
}

// Call from updatePhysics
void Enemy::move(float dt)
{
	this->pos += this->velocity * dt;
}

//Adds two orbiting spheres around a sphere for animation
void Game::animateSphere(Sphere sphere, glm::vec2 moveSpeed, glm::vec3 amplitude) {
	glm::vec3 rotationSpeed = glm::vec3(0.81, 0.53, 0.1);
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	glm::vec3 offset = glm::vec3(0.2, 0.0, 0.0);
	// Multiplier to animate faster when moving a certain direction. Not smooth.
	glm::vec2 movementMultiplier = glm::vec2(
		glm::clamp(abs(moveSpeed.x) + 0.0f, 0.0f, 2.0f),
		glm::clamp(abs(moveSpeed.y) + 0.0f, 0.0f, 2.0f)
	);

	Sphere sphere1(glm::vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * abs(moveSpeed.x)) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * abs(moveSpeed.y)) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	level.spheres.push_back(sphere1);

	rotationSpeed = -rotationSpeed;
	offset = glm::vec3(1.45, 0.9, 1.1);
	Sphere sphere2(glm::vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * abs(moveSpeed.x)) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * abs(moveSpeed.y)) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	level.spheres.push_back(sphere2);
}
