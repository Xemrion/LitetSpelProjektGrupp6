#include "game.h"

#define GRAVITY_CONSTANT -200.0f
#define JUMP_CONSTANT 1.5f
#define COOLDOWN_CONSTANT 0.3f

/*



// one gravity factor:    gravFac
// one gravity modifier:  gravMod
// one gravity direction: gravDir
//
// two boolean states:   isStuck, isHeavy
// that affect the gravity modifier thusly:
//     isStuck              =>  gravMod = 0.0f;
//    !isStuck && !isHeavy  =>  gravMod = 1.0f;
//    !isStuck &&  isHeavy  =>  gravMod = 2.0f;

// potential start values:
auto gravFac = 9.81f; // Earth gravity
auto gravMod = 1.00f; // normal gravity
auto gravDir = glm::vec3{ .0f, -1.0f, .0f }; // down

// function wrapping gravity modifier changes
void IObject::updateGravMod() noexcept {
    if ( isStuck ) gravMod = .0f;
        else gravMod = isHeavy? 2.0f : 1.0f;
}

// at the start of each frame:
    updateGravMod(); // ensure the gravity modifier is up-to-date
    move( float(dt) * gravMod * gravFac, gravDir ); // apply gravity
// if on a platform, the collision system will keep the entity
// from falling through the floor, and collide() will reset
// the 'canJump' state whenever a player.bottomHitbox touches a platform


class IMobile : public IObject


// private:

glm::vec3 gravVel; // <=>  gravFac * gravDir
glm::vec3 velocity;
int       jumpsLeft;
int       jumpsMax;

void IMobile::collide CollisionId             ownHitbox,
                         CollisionId             otherHitbox,
                         IObject                &other,
                         CollisionManager const &collisionManager ) override
{
    // ...

    if ( floorCollision or wallCollision && canWallJump)
        jumpsLeft = jumpsMax; // resetting jumps counter

    if ( floorCollision or wallCollision )
        velocity = glm::vec3{ .0f }; // collided; clearing velocity

    if ( floorCollision )
        onGround = true; // reenabling walking

    if (
}

void IMobile::jump() noexcept {
    if (--jumpsLeft > 0) {
        velocity += jumpPower * -gravDir;
        onGround  = false; // disabling walking
    }
}

void IMobile::moveLeft() noexcept {
    if ( onGround )
        velocity += glm::vec3{ -moveSpeed * float(dt), .0f, .0f };
}

void IMobile::moveRight() noexcept {
    if ( onGround )
        velocity += glm::vec3{  moveSpeed * float(dt), .0f, .0f };
}

// public:
void IObject::update() {
    // on every frame:
    velocity += float(dt) * gravMod * gravVel; // apply gravity impact
    move( velocity );

    // ...
}



*/



LevelGoal::LevelGoal( CollisionManager &colManager, Box bounds, TriggerCallback cb ):
    _bounds(bounds),
    _triggerCallback(cb)
{
    colManager.register_entry(*this, CollisionId::level_goal, _bounds, true );
}

void LevelGoal::collide( CollisionId             ownHitbox,
                         CollisionId             otherHitbox,
                         IObject                &other,
                         CollisionManager const &collisionManager ) {
    if (    otherHitbox == player_top
         or otherHitbox == player_bottom
         or otherHitbox == player_left
         or otherHitbox == player_right)
    {
        _triggerCallback();
        // TODO:
        //    tone and blur screen?
        //    display text?
        //    wait for input?
        //    load next level?
    }
}




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

	for ( int i = 0; i < currentLevel.player.blobCharges; i++ ) {
		currentLevel.player.blobs.push_back(Blob(currentLevel.player.pos));
		currentLevel.spheres.push_back(currentLevel.player.blobs[i].blobSphere);
	}
	updatePlayerCollision();

    auto &player = currentLevel.player;
	currentLevel.colManager.register_entry(player, CollisionId::player_bottom, player.HitboxBottom, false);
	currentLevel.colManager.register_entry(player, CollisionId::player_top,    player.HitboxTop,    false);
	currentLevel.colManager.register_entry(player, CollisionId::player_left,   player.HitboxLeft,   false);
	currentLevel.colManager.register_entry(player, CollisionId::player_right,  player.HitboxRight,  false);

	// updateEnemyCollision();

    // TODO: for ( auto const &e : currLevel.enemies ) {
    auto &enemy = currentLevel.enemy;
	currentLevel.colManager.register_entry(enemy, CollisionId::enemy_bottom, enemy.HitboxBottom, false);
	currentLevel.colManager.register_entry(enemy, CollisionId::enemy_top,    enemy.HitboxTop,    false);
	currentLevel.colManager.register_entry(enemy, CollisionId::enemy_left,   enemy.HitboxLeft,   false);
	currentLevel.colManager.register_entry(enemy, CollisionId::enemy_right,  enemy.HitboxRight,  false);

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
	status(PlayerStatus::None)
{
	// TODO: create four hitboxes
}

Player::~Player() {}


void Player::move(float dt, glm::vec3 dir) noexcept {
	// TODO: move all player hitboxes
	posPrev = pos;
	pos     = posPrev + (dir * moveSpeed * dt);
}

glm::vec3 const& Player::getPosition() const noexcept {
	return pos;
}


void Player::update() {
	if ( isStanding ) {
		gravity      = 0; // refactoring out gravity (gravity always active)
		jumpSpeed    = 0;
		hasExtraJump = true;
	}
	else if ( !isStuck ) {
        gravity = GRAVITY_CONSTANT; // refactoring out gravity (gravity always active)
    }
	for ( int i = 0; i < blobs.size(); i++ ) {
		if ( blobs[i].isBeingRecalled ) {
			blobs[i].setDir(glm::normalize(pos - blobs[i].pos));
			if (glm::length((pos - blobs[i].pos)) < this->radius) {
				blobs[i].isBeingRecalled = false;
			}
		}
		else if ( !blobs[i].isActive ) {
			blobs[i].pos                     = pos + glm::vec3(0.0, 2.0, 0.0);
			blobs[i].blobSphere.centerRadius = glm::vec4(pos, 2);
		}
		blobs[i].move( float(dt) );
	}
}

void Player::collide( CollisionId             ownHitbox,
                      CollisionId             otherHitbox,
                      IObject                &other,
                      CollisionManager const &collisionManager ) 
{
    bool wasBlocked = false;

    //if ( otherHitbox == CollisionId::platform || otherHitbox == CollisionId::wall )
    //    wasBlocked = true;
    
	if (    otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_bottom
         or otherHitbox == CollisionId::enemy_top && ownHitbox == CollisionId::player_bottom)
    {
		isStanding = true;
		//move(dt, glm::vec3(-0.5f, 0.0f, 0.0f));
        wasBlocked = true;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::player_top) {
		if ( status == PlayerStatus::Sticky) {
			gravity  = 0;
			isStuck  = true;
			//pos.y   -= 1;
		}
		jumpSpeed  = 0;
        wasBlocked = true;
	}
	else if (    otherHitbox == CollisionId::platform    && ownHitbox == CollisionId::player_left
              or otherHitbox == CollisionId::enemy_right && ownHitbox == CollisionId::player_left ) {
        if ( status == PlayerStatus::Sticky && isStanding == false && otherHitbox != CollisionId::enemy_right) {
		    gravity   = 0;
		    jumpSpeed = 0;
		    isStuck   = true;
        }
        //pos.x      += moveSpeed * float(dt);
        wasBlocked  = true;
	}
	else if (    otherHitbox == CollisionId::platform   && ownHitbox == CollisionId::player_right
              or otherHitbox == CollisionId::enemy_left && ownHitbox == CollisionId::player_right ) {
        if ( status == PlayerStatus::Sticky  && isStanding == false && otherHitbox != CollisionId::enemy_left) {
            gravity   = 0;
            jumpSpeed = 0;
            isStuck   = true;
		}
		//pos.x      -= moveSpeed * float(dt);
        wasBlocked  = true;
	}

    if ( wasBlocked ) {
        // using one box for whole player instead of top/left/right/bottom
        glm::vec4 pDim { radius, radius, radius, .0f };
        glm::vec4 pCol {    .0f,    .0f,    .0f, .0f }; 
        Box       pSrc { glm::vec4{posPrev, .0f}, pDim, pCol }; // based off of last frame's pos
        Box       pDst { glm::vec4{pos,     .0f}, pDim, pCol }; // based off of the current pos (where the collision event occurred)
        collisionManager.move( *this, pSrc, pDst, blockRules );
        // TODO: use the mutated values of pSrc to update the player position
        pos = { pSrc.center.x, pSrc.center.y, pSrc.center.z };
    }
}

void Game::update(double dt) {
    auto &player = currentLevel.player;
    player.posPrev = player.pos;
	if ( leftButtonDown
		&& player.nrOfActiveBlobs < player.blobCharges
		&&  player.shootCooldown <= 0)
	{
		player.shoot(mousePos);
	}
	time += dt;
	player.moveSpeed = 0.0f;
	if ( keys[0] ) { // TODO: legible alias
		currentLevel.player.moveSpeed = 100.0f;
		if ( !player.isStuck ) {
			if (player.status == PlayerStatus::Heavy) {
				//player.move(dt, glm::vec3(0.2, 0, 0));
				player.pos.x -= player.moveSpeed * float(dt) / 3;
			}
			else {
				//player.move(dt, glm::vec3(1, 0, 0));
				player.pos.x -= player.moveSpeed * float(dt);
			}
		}
	}
	if ( keys[1] ) { // TODO: legible alias
		player.moveSpeed = 100.0f;
		if ( !player.isStuck ) {
			if ( player.status == PlayerStatus::Heavy ) {
				//player.move(dt, glm::vec3(0.2, 0, 0));
				player.pos.x += player.moveSpeed * float(dt) / 3;
			}
			else {
				//player.move(dt, glm::vec3(1, 0, 0));
				player.pos.x += player.moveSpeed * float(dt);
			}
		}

	}
	if ( keys[2] ) { // TODO: legible alias
		if ( player.isStanding && player.jumpCooldown <= 0
             and player.status != PlayerStatus::Heavy ) {
			player.jumpSpeed    = JUMP_CONSTANT;
			player.isStanding   = false;
			player.gravity      = GRAVITY_CONSTANT;
			player.jumpCooldown = COOLDOWN_CONSTANT;
		}
		else if ( player.status == PlayerStatus::Bouncy && player.hasExtraJump 
                  and player.jumpCooldown <= 0 && !player.isStanding ) {
			player.hasExtraJump = false;
			player.jumpSpeed    = JUMP_CONSTANT;
			player.jumpCooldown = COOLDOWN_CONSTANT;
		}

		else if ( player.status == PlayerStatus::Heavy && player.isStanding ) {
			player.jumpSpeed    = JUMP_CONSTANT / 2;
			player.isStanding   = false;
			player.gravity      = GRAVITY_CONSTANT;
			player.jumpCooldown = COOLDOWN_CONSTANT;
		}
	}
	if ( keys[3] ) { // TODO: legible alias
		if ( player.status == PlayerStatus::Sticky ) {
			player.isStuck = false;
			if ( !player.isStanding ) {
				 player.gravity = GRAVITY_CONSTANT * 2;
			}
		}
	}

	for ( int i = 0; i < 4; ++i )
		keys[i] = false;

	// gravity
	player.jumpSpeed  = player.jumpSpeed + (player.gravity * float(dt)) / 100;
	player.pos.y     += player.jumpSpeed;

    auto &enemy       = currentLevel.enemy; // TODO: for ( auto &enemy : currentLevel.enemies )
	enemy.EjumpSpeed  = enemy.EjumpSpeed + (enemy.Egravity * float(dt)) / 100;
	enemy.pos.y      += enemy.EjumpSpeed;

	currentLevel.spheres = vector<Sphere>();
	currentLevel.boxes = vector<Box>();
	player.jumpCooldown  -= float(dt);
	player.shootCooldown -= float(dt);

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
		player.pos.x,
		player.pos.y,
		player.pos.z,
		player.radius);
	currentLevel.spheres.push_back(playerSphere);
	addSphereAnimation(playerSphere, glm::vec2(player.moveSpeed, player.jumpSpeed));
	
	player.isStanding = false;
	currentLevel.colManager.update();
	player.update();

	for ( int i = 0; i < player.blobs.size(); i++ )
		currentLevel.spheres.push_back(player.blobs[i].blobSphere);

	enemy.enemyStanding = false;
	currentLevel.colManager.update();
	player.update();
	enemy.update();
	//player.moveSpeed = 0;
	player.jumpCooldown -= float(dt);
}

void Player::shoot(glm::vec3 mousePos) {
	mousePos = glm::vec3((mousePos.x - 1280 / 2) * 9, (-(mousePos.y - 980 / 2)) * 16, 0);
	glm::vec3 dir = glm::normalize(mousePos - pos);
	for (int i = 0; i < blobs.size(); i++) {
		if (!blobs[nrOfActiveBlobs + i].isBeingRecalled) {
			blobs[nrOfActiveBlobs + i].setDir(dir);
			blobs[nrOfActiveBlobs + i].isActive = true;
			shootCooldown = .5f;
			nrOfActiveBlobs++;
			break;
		}
	}
}

void Player::recallBlobs() {
	for (int i = 0; i < nrOfActiveBlobs; i++) {
		blobs[i].isActive = false;
		blobs[i].isBeingRecalled = true;
	}
	shootCooldown = 0.5f;
	nrOfActiveBlobs = 0;
}

void Game::updatePlayerCollision() {
    auto const &pPos    = currentLevel.player.pos;
    auto const &pRadius = playerSphere.centerRadius.w;

    // Bottom hitbox:
	currentLevel.player.HitboxBottom.center = glm::vec4(
		pPos.x,
		pPos.y - 0.4 * pRadius,
		pPos.z,
		0);
	currentLevel.player.HitboxBottom.halfLengths = glm::vec4(
		pRadius * 0.5,
		pRadius * 0.4,
		pRadius * 0.1,
		0);
	
    // Top hitbox:
	currentLevel.player.HitboxTop.center = glm::vec4(
		pPos.x,
		pPos.y + 0.4 * pRadius,
		pPos.z,
		0);
	currentLevel.player.HitboxTop.halfLengths = glm::vec4(
        pRadius * 0.5,
        pRadius * 0.4,
        pRadius * 0.1,
		0);
	
    // Left hitbox:
	currentLevel.player.HitboxLeft.center = glm::vec4(
		pPos.x - 0.5 * pRadius,
		pPos.y,
		pPos.z,
		0);
	currentLevel.player.HitboxLeft.halfLengths = glm::vec4(
		pRadius * 0.2,
		pRadius * 0.8,
		pRadius * 0.1,
		0);

    // Right hitbox:
	currentLevel.player.HitboxRight.center = glm::vec4(
		pPos.x + 0.5 * pRadius,
		pPos.y,
		pPos.z,
		0);
	currentLevel.player.HitboxRight.halfLengths = glm::vec4(
		pRadius * 0.2,
		pRadius * 0.8,
		pRadius * 0.1,
		0);

}

void Game::updateEnemyCollision() {
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

	// Bottom hitbox:
	currentLevel.enemy.HitboxBottom.center = glm::vec4(
		currentLevel.enemy.pos.x,
		currentLevel.enemy.pos.y - 0.9*EnemyBox.halfLengths.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxBottom.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y * 0.2,
		EnemyBox.halfLengths.z,
		0);
	
    // Top hitbox:
	currentLevel.enemy.HitboxTop.center = glm::vec4(
		currentLevel.enemy.pos.x,
		currentLevel.enemy.pos.y + 0.9*EnemyBox.halfLengths.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxTop.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y * 0.2,
		EnemyBox.halfLengths.z,
		0);

	// Left hitbox:
	currentLevel.enemy.HitboxLeft.center = glm::vec4(
		currentLevel.enemy.pos.x - 0.9*EnemyBox.halfLengths.x,
		currentLevel.enemy.pos.y,
		currentLevel.enemy.pos.z,
		0);
	currentLevel.enemy.HitboxLeft.halfLengths = glm::vec4(
		EnemyBox.halfLengths.x * 0.2,
		EnemyBox.halfLengths.y * 0.8,
		EnemyBox.halfLengths.z,
		0);

	// Right hitbox:
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
{}

Enemy::~Enemy() {}

void Enemy::collide( CollisionId             ownHitbox,
                     CollisionId             otherHitbox,
                     IObject                &other,
                     CollisionManager const &collisionManager )
{
	if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_bottom) {
		enemyStanding = true;
		isJumping     = false;
		canJump       = true;
		if (EmoveSpeed == 0.0f) {
			EmoveSpeed = 20.0f;
		}
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_top) {
		this->EjumpSpeed = 0;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_left) {
		//this->posCurr.x += EmoveSpeed * dt;
		EmoveSpeed = EmoveSpeed * -1;
	}
	else if (otherHitbox == CollisionId::platform && ownHitbox == CollisionId::enemy_right) {
		//this->posCurr.x -= EmoveSpeed * dt;
		EmoveSpeed = EmoveSpeed * -1;
	}
}

void Enemy::update() {
	if (enemyStanding) {
		Egravity = 0; // TODO: refactor out gravity (gravity always active)
		EjumpSpeed = 0;
	}
	else Egravity = GRAVITY_CONSTANT; //  TODO: refactor out gravity (gravity always active)
	move();
}

void Enemy::move() {
	pos.x += EmoveSpeed * float(dt);
	pos.y += EjumpSpeed * float(dt);
	if ( !enemyStanding && !isJumping && canJump ) {
		EmoveSpeed    = EmoveSpeed * -1;
		EjumpSpeed    = 0.5f;
		enemyStanding = false;
		isJumping     = true;
		canJump       = false;
	}
}

//Adds two orbiting spheres around a sphere for animation
void Game::addSphereAnimation( Sphere sphere, glm::vec2 moveSpeed, glm::vec3 amplitude ) {
	glm::vec3 rotationSpeed = glm::vec3( 0.81, 0.53, 0.1 );
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	glm::vec3 offset = glm::vec3( 0.2, 0.0, 0.0 );
	// Multiplier to animate faster when moving a certain direction. Not smooth.
	glm::vec2 movementMultiplier = glm::vec2(
		glm::clamp( abs(moveSpeed.x), 1.0f, 5.0f ),
		glm::clamp( abs(moveSpeed.y), 1.0f, 2.0f )
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
