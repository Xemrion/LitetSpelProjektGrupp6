#include "game.h"
#define GRAVITY_CONSTANT -200.0f
#define JUMP_CONSTANT 0.012f
#define COOLDOWN_CONSTANT 0.3f

void Game::init() {
    groundBox.hitbox.center = glm::vec4(0, -30, 0, 0);
    groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
	groundBox.hitbox.color = glm::vec4(1.0, 1.0, 1.0, 0.0);
	currentLevel.boxes.push_back(groundBox.hitbox);

	currentLevel.colManager.register_entry(groundBox, CollisionId::platform, groundBox.hitbox, true);

	testPlat.hitbox.center = glm::vec4(30.0f, 0.0f, 0.0f, 0.0f);
	testPlat.hitbox.halfLengths = glm::vec4(10.0f, 20.0f, 10.0f, 0.0f);
	testPlat.hitbox.color = glm::vec4(0.0, 1.0, 0.0, 0.0);
	currentLevel.boxes.push_back(testPlat.hitbox);
	currentLevel.colManager.register_entry(testPlat, CollisionId::platform, testPlat.hitbox, true);

	testplat2.hitbox.center = glm::vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	testplat2.hitbox.halfLengths = glm::vec4(10.0f, 2.0f, 10.0f, 0.0f);
	testplat2.hitbox.color = glm::vec4(0.0, 0.5, 0.5, 0.0);
	currentLevel.boxes.push_back(testplat2.hitbox);
	currentLevel.colManager.register_entry(testplat2, CollisionId::platform, testplat2.hitbox, true);

	for (int i = 0; i < currentLevel.player.blobCharges; i++)
	{
		currentLevel.player.blobs.push_back(Blob(currentLevel.player.pos));
		currentLevel.spheres.push_back(currentLevel.player.blobs[i].blobSphere);
	}
	updatePlayerCollision();

	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_bottom, currentLevel.player.HitboxBottom, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_top, currentLevel.player.HitboxTop, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_left, currentLevel.player.HitboxLeft, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_right, currentLevel.player.HitboxRight, false);

	//updateEnemyCollision();

	currentLevel.colManager.register_entry(currentLevel.enemy, CollisionId::enemy_bottom, currentLevel.enemy.HitboxBottom, false);
	currentLevel.colManager.register_entry(currentLevel.enemy, CollisionId::enemy_top, currentLevel.enemy.HitboxTop, false);
	currentLevel.colManager.register_entry(currentLevel.enemy, CollisionId::enemy_left, currentLevel.enemy.HitboxLeft, false);
	currentLevel.colManager.register_entry(currentLevel.enemy, CollisionId::enemy_right, currentLevel.enemy.HitboxRight, false);
	EnemyBox.color = grey;
}




Player::Player(glm::vec3 position) :
	IObject(),
	posPrev(position),
	pos(position),
	moveSpeed(10.0f),
	jumpSpeed(.0f),
	jumpCooldown(.0f),
	gravity(GRAVITY_CONSTANT),
	hasExtraJump(true),
	isStanding(false),
	isStuck(false),
	status(PlayerStatus::None) // TODO: enum!
{
	// skapa fyra hitboxar
}

Player::~Player() {}


void Player::move(float dt, glm::vec3 dir) noexcept {
	// TODO: movea alla player hitboxar
	posPrev = pos;
	pos = posPrev + (dir * moveSpeed * dt);
}

glm::vec3 const& Player::getPosition() const noexcept {
	return pos;
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
			this->gravity = GRAVITY_CONSTANT;
		}
	}
	for (int i = 0; i < blobs.size(); i++)
	{
		if (blobs[i].isBeingRecalled)
		{
			blobs[i].setDir(glm::normalize(pos - blobs[i].pos));
			if (glm::length((pos - blobs[i].pos)) < this->radius)
			{
				blobs[i].isBeingRecalled = false;
			}
		}
		else if (!blobs[i].isActive)
		{
			blobs[i].pos = pos + glm::vec3(0.0, 2.0, 0.0);
			blobs[i].blobSphere.centerRadius = glm::vec4(pos, 2);
		}
		blobs[i].move(dt);
	}
}

void Player::collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) 
{
	if (CollisionId::player_bottom) {
		if (otherHitbox == CollisionId::platform) {
			this->isStanding = true;
		}
	}
	else if (CollisionId::player_top) {
		if (this->status == PlayerStatus::Sticky) {
			this->gravity = 0;
			this->isStuck = true;
			this->pos.y -= 1;
		}
		this->jumpSpeed = 0;
	}
	else if (CollisionId::player_left) {
		if (otherHitbox == CollisionId::platform) {
			if (this->status == PlayerStatus::Sticky) {
				this->gravity = 0;
				this->jumpSpeed = 0;
				this->isStuck = true;
			}
		}
		this->pos.x += this->moveSpeed*dt;
	}
	else if (CollisionId::player_right) {
		if (otherHitbox == CollisionId::platform) {
			if (this->status == PlayerStatus::Sticky) {
				this->gravity = 0;
				this->jumpSpeed = 0;
				this->isStuck = true;
			}
		}
		this->pos.x -= this->moveSpeed * dt;
	}
}

void Game::update(double dt) {

	if (leftButtonDown
		&& currentLevel.player.nrOfActiveBlobs < currentLevel.player.blobCharges
		&&  currentLevel.player.shootCooldown <= 0)
	{
		currentLevel.player.shoot(mousePos);
	}
	currentLevel.player.moveSpeed = 0.0f;
	if (keys[0]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.isStuck == false)
		{
			if (currentLevel.player.status == PlayerStatus::Heavy) {
				//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
				currentLevel.player.pos.x -= currentLevel.player.moveSpeed*dt / 3;
			}
			else {
				//currentLevel.player.move(dt, glm::vec3(1, 0, 0));
				currentLevel.player.pos.x -= currentLevel.player.moveSpeed*dt;
			}
		}

	}
	if (keys[1]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.isStuck == false)
		{
			if (currentLevel.player.status == PlayerStatus::Heavy) {
				//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
				currentLevel.player.pos.x += currentLevel.player.moveSpeed*dt / 3;
			}
			else {
				//currentLevel.player.move(dt, glm::vec3(1, 0, 0));
				currentLevel.player.pos.x += currentLevel.player.moveSpeed*dt;
			}
		}

	}
	if (keys[2]) {
		if (currentLevel.player.isStanding == true && currentLevel.player.jumpCooldown <= 0 && currentLevel.player.status != PlayerStatus::Heavy) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.isStanding = false;
			currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
			gameSounds->PlayJumpSound02();
		}
		else if (currentLevel.player.status == PlayerStatus::Bouncy && currentLevel.player.hasExtraJump == true && currentLevel.player.isStanding == false && currentLevel.player.jumpCooldown <= 0) {
			currentLevel.player.hasExtraJump = false;
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
			gameSounds->PlayJumpSound02();
		}

		else if (currentLevel.player.status == PlayerStatus::Heavy && currentLevel.player.isStanding == true) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT / 2;
			currentLevel.player.isStanding = false;
			currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
			gameSounds->PlayJumpSound03();
		}
	}
	if (keys[3]) {
		if (currentLevel.player.status == PlayerStatus::Sticky)
		{
			currentLevel.player.isStuck = false;
			if (currentLevel.player.isStanding == false)
			{
				currentLevel.player.gravity = GRAVITY_CONSTANT * 2;
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
	// gravity
	updatePhysics(dt);
	time += dt;

	currentLevel.spheres = vector<Sphere>();
	currentLevel.boxes = vector<Box>();
	currentLevel.player.jumpCooldown -= dt;
	currentLevel.player.shootCooldown -= dt;

	currentLevel.boxes.push_back(groundBox.hitbox);
	currentLevel.boxes.push_back(testPlat.hitbox);
	currentLevel.boxes.push_back(testplat2.hitbox);
	updatePlayerCollision();
	updateEnemyCollision();
	//currentLevel.boxes.push_back(currentLevel.player.HitboxBottom);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxLeft);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxRight);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxTop);

	//currentLevel.boxes.push_back(currentLevel.enemy.HitboxBottom);
	//currentLevel.boxes.push_back(currentLevel.enemy.HitboxTop);
	//currentLevel.boxes.push_back(currentLevel.enemy.HitboxLeft);
	//currentLevel.boxes.push_back(currentLevel.enemy.HitboxRight);


	currentLevel.spheres = vector<Sphere>();
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		currentLevel.player.radius);
	currentLevel.spheres.push_back(playerSphere);
	addSphereAnimation(playerSphere, glm::vec2(currentLevel.player.moveSpeed, currentLevel.player.jumpSpeed));
	
	currentLevel.player.isStanding = false;
	for (int i = 0; i < currentLevel.player.blobs.size(); i++)
	{
		currentLevel.spheres.push_back(currentLevel.player.blobs[i].blobSphere);
	}


	currentLevel.enemy.enemyStanding = false;
	currentLevel.colManager.update();
	currentLevel.player.update();
	currentLevel.enemy.update();
	//currentLevel.player.moveSpeed = 0;
	currentLevel.player.jumpCooldown -= dt;
}

void Game::updatePhysics(double dt) {
	float timestep = 0.0001f;
	float invTimestep = 1.0f / timestep;
	for (float i = (float)floor(time * invTimestep) * timestep; i < time + dt; i += timestep) {
		currentLevel.player.jumpSpeed = currentLevel.player.jumpSpeed + (currentLevel.player.gravity * timestep) * 0.0001;
		currentLevel.player.pos.y += currentLevel.player.jumpSpeed;

		currentLevel.enemy.EjumpSpeed = currentLevel.enemy.EjumpSpeed + (currentLevel.enemy.Egravity * timestep) * 0.0001;
		currentLevel.enemy.pos.y += currentLevel.enemy.EjumpSpeed;
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
			blobs[nrOfActiveBlobs + i].setDir(dir);
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
	}
	shootCooldown = 0.5f;
	nrOfActiveBlobs = 0;
}
void Game::updatePlayerCollision()
{
	//####################################################################Bottom
	currentLevel.player.HitboxBottom.center = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y - 0.4*playerSphere.centerRadius.w,
		currentLevel.player.pos.z,
		0);
	currentLevel.player.HitboxBottom.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.5,
		playerSphere.centerRadius.w*0.4,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################Top
	currentLevel.player.HitboxTop.center = glm::vec4(
		currentLevel.player.pos.x,
		currentLevel.player.pos.y + 0.4*playerSphere.centerRadius.w,
		currentLevel.player.pos.z,
		0);
	currentLevel.player.HitboxTop.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.5,
		playerSphere.centerRadius.w*0.4,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################Left
	currentLevel.player.HitboxLeft.center = glm::vec4(
		currentLevel.player.pos.x - 0.5*playerSphere.centerRadius.w,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		0);
	currentLevel.player.HitboxLeft.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.2,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);

	//####################################################################Right
	currentLevel.player.HitboxRight.center = glm::vec4(
		currentLevel.player.pos.x + 0.5*playerSphere.centerRadius.w,
		currentLevel.player.pos.y,
		currentLevel.player.pos.z,
		0);
	currentLevel.player.HitboxRight.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.2,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################
}

void Game::updateEnemyCollision()
{
	EnemyBox.center = glm::vec4(
		currentLevel.enemy.pos.x,
		currentLevel.enemy.pos.y,
		currentLevel.enemy.pos.z,
		5.0);
	EnemyBox.halfLengths = glm::vec4(
		3.0,
		3.0,
		3.0,
		0.0
	);
	currentLevel.boxes.push_back(EnemyBox);
	//####################################################################Bottom
	currentLevel.enemy.HitboxBottom.center = glm::vec4(
		currentLevel.enemy.pos.x,
		currentLevel.enemy.pos.y - 0.9*EnemyBox.halfLengths.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxBottom.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################Top
	currentLevel.enemy.HitboxTop.center = glm::vec4(
		currentLevel.enemy.pos.x,
		currentLevel.enemy.pos.y + 0.9*EnemyBox.halfLengths.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxTop.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################Left
	currentLevel.enemy.HitboxLeft.center = glm::vec4(
		currentLevel.enemy.pos.x - 0.9*EnemyBox.halfLengths.x,
		currentLevel.enemy.pos.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxLeft.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.8,
		EnemyBox.halfLengths.z,
		0);

	//####################################################################Right
	currentLevel.enemy.HitboxRight.center = glm::vec4(
		currentLevel.enemy.pos.x + 0.9*EnemyBox.halfLengths.x,
		currentLevel.enemy.pos.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxRight.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.8,
		EnemyBox.halfLengths.z,
		0);
	//####################################################################
}

Enemy::Enemy(glm::vec3 position):
IObject(),
posPrev(position),
pos(position),
EmoveSpeed(0.0f),
EjumpSpeed(0.0f),
EjumpCooldown(.0f),
Egravity(GRAVITY_CONSTANT),
enemyStanding(false),
isJumping(false),
canJump(false)
{

}

Enemy::~Enemy(){}

void Enemy::collide(CollisionId ownHitbox, CollisionId otherHitbox, IObject & other)
{
	if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_bottom)
	{
		this->enemyStanding = true;
		this->isJumping = false;
		this->canJump = true;
		if(EmoveSpeed == 0.0f)
		{
			EmoveSpeed = 20.0f;
		}
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_top)
	{
		this->EjumpSpeed = 0;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_left)
	{
		//this->posCurr.x += EmoveSpeed * dt;
		EmoveSpeed = EmoveSpeed * -1;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_right)
	{
		//this->posCurr.x -= EmoveSpeed * dt;
		EmoveSpeed = EmoveSpeed * -1;
	}
}

void Enemy::update()
{
	if (enemyStanding) {
		this->Egravity = 0;
		this->EjumpSpeed = 0;
	}
	else
	{
		this->Egravity = GRAVITY_CONSTANT;
	}
	move();
}

void Enemy::move()
{
	pos.x += EmoveSpeed * dt;
	if (enemyStanding == false && isJumping == false && canJump == true) 
	{
		EmoveSpeed = EmoveSpeed * -1;
		EjumpSpeed = JUMP_CONSTANT / 2;
		enemyStanding = false;
		isJumping = true;
		canJump = false;
	}
}

//Adds two orbiting spheres around a sphere for animation
void Game::addSphereAnimation(Sphere sphere, glm::vec2 moveSpeed, glm::vec3 amplitude) {
	glm::vec3 rotationSpeed = glm::vec3(0.81, 0.53, 0.1);
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	glm::vec3 offset = glm::vec3(0.2, 0.0, 0.0);
	// Multiplier to animate faster when moving a certain direction. Not smooth.
	glm::vec2 movementMultiplier = glm::vec2(
		glm::clamp(abs(moveSpeed.x), 1.0f, 5.0f),
		glm::clamp(abs(moveSpeed.y), 1.0f, 2.0f)
	);

	Sphere sphere1(glm::vec4(
		sphere.centerRadius.x + sin(time * (rotationSpeed.x * movementMultiplier.x) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(time * (rotationSpeed.y * movementMultiplier.y) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(time * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	currentLevel.spheres.push_back(sphere1);

	rotationSpeed = -rotationSpeed;
	offset = glm::vec3(1.45, 0.9, 1.1);
	Sphere sphere2(glm::vec4(
		sphere.centerRadius.x + sin(time * (rotationSpeed.x * movementMultiplier.x) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(time * (rotationSpeed.y * movementMultiplier.y) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(time * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	currentLevel.spheres.push_back(sphere2);
}

