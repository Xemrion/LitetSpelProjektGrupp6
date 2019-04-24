#include "game.h"
#define GRAVITY_CONSTANT -200.0f
#define JUMP_CONSTANT 1.5f
#define COOLDOWN_CONSTANT 0.2f

void Game::init() {
	groundBox.hitbox.center = glm::vec4(0, -30, 0, 0);
	groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
	currentLevel.boxes.push_back(groundBox.hitbox);

	currentLevel.colManager.register_entry(groundBox, CollisionId::platform, groundBox.hitbox, true);

	testPlat.hitbox.center = glm::vec4(30.0f, 0.0f, 0.0f, 0.0f);
	testPlat.hitbox.halfLengths = glm::vec4(10.0f, 20.0f, 10.0f, 0.0f);
	currentLevel.boxes.push_back(testPlat.hitbox);
	currentLevel.colManager.register_entry(testPlat, CollisionId::platform, testPlat.hitbox, true);


	testplat2.hitbox.center = glm::vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	testplat2.hitbox.halfLengths = glm::vec4(10.0f, 2.0f, 10.0f, 0.0f);
	currentLevel.boxes.push_back(testplat2.hitbox);
	currentLevel.colManager.register_entry(testplat2, CollisionId::platform, testplat2.hitbox, true);

	for (int i = 0; i < currentLevel.player.blobCharges; i++)
	{
		currentLevel.player.blobs.push_back(Blob(currentLevel.player.posCurr));
		currentLevel.spheres.push_back(currentLevel.player.blobs[i].blobSphere);
	}
	updatePlayerCollision();

	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_bottom, currentLevel.player.HitboxBottom, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_top, currentLevel.player.HitboxTop, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_left, currentLevel.player.HitboxLeft, false);
	currentLevel.colManager.register_entry(currentLevel.player, CollisionId::player_right, currentLevel.player.HitboxRight, false);

	//currentLevel.boxes.push_back(currentLevel.player.HitboxBottom);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxLeft);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxRight);
	//currentLevel.boxes.push_back(currentLevel.player.HitboxTop);
}




Player::Player(glm::vec3 position) :
	IObject(),
	posPrev(position),
	posCurr(position),
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

	for (int i = 0; i < blobs.size(); i++)
	{
		if (blobs[i].isBeingRecalled)
		{
			blobs[i].setDir(glm::normalize(posCurr - blobs[i].pos));
			if (glm::length((posCurr - blobs[i].pos)) < 0.2f)
			{
				blobs[i].isBeingRecalled = false;
			}
		}
		else if (!blobs[i].isActive)
		{
			blobs[i].pos = posCurr;
			blobs[i].blobSphere.centerRadius = glm::vec4(posCurr, 2);
		}
		blobs[i].move(dt);

	}
	if (isStuck == false)
	{
		gravity = GRAVITY_CONSTANT;
	}
}

void Player::collide(CollisionId ownHitbox, CollisionId otherHitbox, IObject &other) {
	if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_bottom)
	{
		isStanding = true;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_top)
	{
		if (status == PlayerStatus::Sticky)
		{
			gravity = 0;
			isStuck = true;
			posCurr.y -= 1;
		}
		jumpSpeed = 0;
		//posCurr.y -= 1;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_left)
	{
		if (status == PlayerStatus::Sticky && isStanding == false)
		{
			gravity = 0;
			jumpSpeed = 0;
			isStuck = true;
		}
		posCurr.x += moveSpeed * dt;

	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_right)
	{
		if (status == PlayerStatus::Sticky && isStanding == false)
		{
			gravity = 0;
			jumpSpeed = 0;
			isStuck = true;
		}
		posCurr.x -= moveSpeed * dt;
	}
}

void Game::update(double dt) {

	if (leftButtonDown
		&& currentLevel.player.nrOfActiveBlobs < currentLevel.player.blobCharges
		&&  currentLevel.player.shootCooldown <= 0)
	{
		currentLevel.player.shoot(mousePos);
	}
	if (keys[0]) {
		currentLevel.player.moveSpeed = 100.0f;
		if (currentLevel.player.isStuck == false)
		{
			if (currentLevel.player.status == PlayerStatus::Heavy) {
				//currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
				currentLevel.player.posCurr.x -= currentLevel.player.moveSpeed*dt / 3;
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
			if (currentLevel.player.status == PlayerStatus::Heavy) {
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
		if (currentLevel.player.isStanding == true && currentLevel.player.jumpCooldown <= 0 && currentLevel.player.status != PlayerStatus::Heavy) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.isStanding = false;
			currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}
		else if (currentLevel.player.status == PlayerStatus::Bouncy && currentLevel.player.hasExtraJump == true && currentLevel.player.isStanding == false && currentLevel.player.jumpCooldown <= 0) {
			currentLevel.player.hasExtraJump = false;
			currentLevel.player.jumpSpeed = JUMP_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
		}

		else if (currentLevel.player.status == PlayerStatus::Heavy && currentLevel.player.isStanding == true) {
			currentLevel.player.jumpSpeed = JUMP_CONSTANT / 2;
			currentLevel.player.isStanding = false;
			currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = COOLDOWN_CONSTANT;
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
	currentLevel.player.jumpSpeed = currentLevel.player.jumpSpeed + (currentLevel.player.gravity * float(dt)) / 100;
	currentLevel.player.posCurr.y += currentLevel.player.jumpSpeed;

	currentLevel.player.jumpCooldown -= dt;
	currentLevel.player.shootCooldown -= dt;

	updatePlayerCollision();

	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);


	currentLevel.player.isStanding = false;
	currentLevel.colManager.update();
	currentLevel.player.update();
	for (int i = 0; i < currentLevel.player.blobs.size(); i++)
	{
		currentLevel.spheres.push_back(currentLevel.player.blobs[i].blobSphere);
	}


}
void Player::shoot(glm::vec3 mousePos)
{
	mousePos = glm::vec3((mousePos.x - 1280 / 2) * 9, (-(mousePos.y - 980 / 2)) * 16, 0);
	glm::vec3 dir = glm::normalize(mousePos - posCurr);
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
	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		5.0);
	currentLevel.spheres.push_back(playerSphere);
	//####################################################################Bottom
	currentLevel.player.HitboxBottom.center = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y - 0.4*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxBottom.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.5,
		playerSphere.centerRadius.w*0.4,
		playerSphere.centerRadius.w*0.8,
		0);
	//####################################################################Top
	currentLevel.player.HitboxTop.center = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y + 0.4*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxTop.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.5,
		playerSphere.centerRadius.w*0.4,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################Left
	currentLevel.player.HitboxLeft.center = glm::vec4(
		currentLevel.player.posCurr.x - 0.5*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxLeft.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.2,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);

	//####################################################################Right
	currentLevel.player.HitboxRight.center = glm::vec4(
		currentLevel.player.posCurr.x + 0.5*playerSphere.centerRadius.w,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		0);
	currentLevel.player.HitboxRight.halfLengths = glm::vec4(
		playerSphere.centerRadius.w*0.2,
		playerSphere.centerRadius.w*0.8,
		playerSphere.centerRadius.w*0.1,
		0);
	//####################################################################
}
