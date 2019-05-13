#include "game.h"

void Game::init() noexcept {
   /* groundBox.hitbox.center      = vec4(0, -30, 0, 0);
    groundBox.hitbox.halfLengths = vec4(100, 10, 10, 0);
	groundBox.hitbox.color       = vec4(1.0, 1.0, 1.0, 0.0);
	level.boxes.push_back(groundBox.hitbox);

	level.colManager.registerEntry(groundBox, ColliderType::platform, groundBox.hitbox, true);

	testPlat.hitbox.center      = vec4(30.0f, 0.0f, 0.0f, 0.0f);
	testPlat.hitbox.halfLengths = vec4(10.0f, 20.0f, 10.0f, 0.0f);
	testPlat.hitbox.color       = vec4(0.0, 1.0, 0.0, 0.0);
	level.boxes.push_back(testPlat.hitbox);
	level.colManager.registerEntry(testPlat, ColliderType::platform, testPlat.hitbox, true);

	testplat2.hitbox.center      = vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	testplat2.hitbox.halfLengths = vec4(10.0f, 2.0f, 10.0f, 0.0f);
	testplat2.hitbox.color       = vec4(0.0, 0.5, 0.5, 0.0);
	level.boxes.push_back(testplat2.hitbox);
	level.colManager.registerEntry(testplat2, ColliderType::platform, testplat2.hitbox, true);*/
	editor.initialize("test2.png");
	for (int i = 0; i < editor.platforms.size(); i++)
	{
		editor.platforms.at(i).hitbox.color = glm::vec4(0, 1, 1, 0);
		level.boxes.push_back(editor.platforms.at(i).hitbox);
		level.colManager.registerEntry(editor.platforms.at(i), ColliderType::platform, editor.platforms.at(i).hitbox, true);
	}
    level.goal = std::make_unique<LevelGoal>( level.colManager, editor.goalPos, 12.0f );
// player & blobs:
    auto &player = level.player;
	player.pos = editor.startPos;
	for ( int i = 0;  i < player.blobCharges;  ++i ) {
        Blob b { player.pos };
		player.blobs.push_back( Blob(player.pos) );
		level.spheres.push_back( player.blobs[i].blobSphere );
	}
    for ( auto &b : player.blobs ) {
        level.colManager.registerEntry(b, ColliderType::blob, b.hitbox, false);
    }
	updatePlayerCollision();
	level.colManager.registerEntry(player, ColliderType::player, player.Hitbox, false);

// enemies:
    auto &enemy = level.enemy; // TODO: for ( auto &enemy : level.enemies )
	//level.colManager.registerEntry(enemy, ColliderType::enemy_bottom, enemy.HitboxBottom, false);
	//level.colManager.registerEntry(enemy, ColliderType::enemy_top,    enemy.HitboxTop,    false);
	//level.colManager.registerEntry(enemy, ColliderType::enemy_left,   enemy.HitboxLeft,   false);
	//level.colManager.registerEntry(enemy, ColliderType::enemy_right,  enemy.HitboxRight,  false);

	EnemyBox.color = vec4(1,0,0,0);

// LevelGoal

// PowerUps
	auto &powerup = level.TestPowerUp;
	level.TestPowerUp.powerBox.center = vec4(-30.0f, 15.0f, 0.0f, 0.0f);
	level.TestPowerUp.powerBox.halfLengths = vec4(2.0f, 2.0f, 2.0f, 0.0f);
	level.TestPowerUp.powerBox.color = vec4(0.0, 0.5, 0.75, 0);
	level.boxes.push_back(level.TestPowerUp.powerBox);
	level.colManager.registerEntry(powerup, ColliderType::powerup_bouncy, level.TestPowerUp.powerBox, true);

// MENU
	MenuBG.center = vec4(0.0f, 0.0f, 5.0f, 0.0f);
	MenuBG.halfLengths = vec4(100.0f, 100.0f, 0.0f, 0.0f);
	MenuBG.color = vec4(0.0f, 0.0f, 1.0f, 0.0f);

	MenuYes.center = vec4(30.0f, 10.0f, 0.0f, 0.0f);
	MenuYes.halfLengths = vec4(20.0f, 20.0f, 0.0f, 0.0f);
	MenuYes.color = vec4(0.0f, 1.0f, 0.0f, 0.0f);

	MenuNo.center = vec4(-30.0f, 10.0f, 0.0f, 0.0f);
	MenuNo.halfLengths = vec4(20.0f, 20.0f, 0.0f, 0.0f);
	MenuNo.color = vec4(1.0f, 0.0f, 0.0f, 0.0f);

	state = GameState::MenuState;
}

Player::Player(vec3 position) :
	CollisionObject(),
	pos(position),
    radius(5.0f),
	velocity(vec3(0.0)),
	moveSpeed(150.0f),
	mass(10.0),
    blobCharges(5),
    shootCooldown(0),
	jumpForce(1200.0f),
	jumpCooldown(.0f),
	hasExtraJump(true),
	isStanding(false),
	isStuck(false),
	knockBack(false),
	levelCompleted(false),
	status(PlayerStatus::None)
{} // TODO: create hitboxes in ctor body

Player::~Player() {}

// Set useSpeed to true to multiply velocity by objects speed value
void Player::setVelocity(vec3 const &velocity, bool useSpeed) noexcept {
	if (useSpeed) {
		this->velocity = velocity * moveSpeed;
	}
	else this->velocity = velocity;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Player::addVelocity(vec3 const &velocity, bool useSpeed) noexcept {
	if (useSpeed) {
		this->velocity += velocity * (moveSpeed/2);
	}
	else this->velocity += velocity;
}

// velocity += force / mass;
void Player::putForce(vec3 const &force) noexcept {
	this->velocity += force / mass;
}

// Call from updatePhysics
void Player::move(double dt) noexcept {
	pos += velocity * float(dt);
}

// Updates logic, call once per frame
void Player::update(double dt) noexcept {
	jumpCooldown  -= float(dt);
	shootCooldown -= float(dt);

	if (isStanding)
		hasExtraJump = true;
	
    mass = (status == PlayerStatus::Heavy)? 20.0f : 10.0f;

	if (status != PlayerStatus::Sticky) {
		isStuck = false;
	}
    
	for ( auto &blob : blobs ) 
        blob.update(dt);
}

void Player::collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept
{
	
		if (otherHitbox == ColliderType::platform) {

			if (pos.y > (other.center.y+other.halfLengths.y)) 
			{
				isStanding = true;
				hasExtraJump = true;
				//pos.y        = other.center.y + other.halfLengths.y + (pos.y - Hitbox.center.y + Hitbox.halfLengths.y);
				velocity.y = 0;
			}
			else
			{
				vec3 pushUp = vec3(0.0, other.center.y + other.halfLengths.y + (-Hitbox.center.y + Hitbox.halfLengths.y), 0.0);
				vec3 pushDown = vec3(0.0, other.center.y - other.halfLengths.y + (-Hitbox.center.y - Hitbox.halfLengths.y), 0.0);
				vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-Hitbox.center.x + Hitbox.halfLengths.x), 0.0, 0.0);
				vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-Hitbox.center.x - Hitbox.halfLengths.x), 0.0, 0.0);
				vec3 minDistY = length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
				vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
				pos += length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
			}

			if (status == PlayerStatus::Sticky && !isStanding)
			{
				isStuck = true;
			}
			if (knockBack) 
			{
				knockBack = false;
				velocity.x = 0;
			}
		}
		else if (otherHitbox == ColliderType::blob && status == PlayerStatus::Sticky && other.color.w != 0 && !isStuck)
		{
			isStanding = true;
			hasExtraJump = true;
			//pos.y = other.center.y + other.halfLengths.y + (pos.y - Hitbox.center.y + Hitbox.halfLengths.y);
			velocity.y = 0;
			if (knockBack)
			{
				knockBack = false;
				velocity.x = 0;
			}
			vec3 pushUp = vec3(0.0, other.center.y + other.halfLengths.y + (-Hitbox.center.y + Hitbox.halfLengths.y), 0.0);
			vec3 pushDown = vec3(0.0, other.center.y - other.halfLengths.y + (-Hitbox.center.y - Hitbox.halfLengths.y), 0.0);
			vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-Hitbox.center.x + Hitbox.halfLengths.x), 0.0, 0.0);
			vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-Hitbox.center.x - Hitbox.halfLengths.x), 0.0, 0.0);
			vec3 minDistY = length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}

		else if (otherHitbox == ColliderType::blob && status == PlayerStatus::Bouncy && other.color.w != 0)
		{
			isStanding = true;
			hasExtraJump = true;
			//pos.y = other.center.y + other.halfLengths.y + (pos.y - Hitbox.center.y + Hitbox.halfLengths.y);
			velocity.y = 0;
			if (knockBack)
			{
				knockBack = false;
				velocity.x = 0;
			}
			vec3 pushUp = vec3(0.0, other.center.y + other.halfLengths.y + (-Hitbox.center.y + Hitbox.halfLengths.y), 0.0);
			vec3 pushDown = vec3(0.0, other.center.y - other.halfLengths.y + (-Hitbox.center.y - Hitbox.halfLengths.y), 0.0);
			vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-Hitbox.center.x + Hitbox.halfLengths.x), 0.0, 0.0);
			vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-Hitbox.center.x - Hitbox.halfLengths.x), 0.0, 0.0);
			vec3 minDistY = length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}
		else if (otherHitbox == ColliderType::enemy_bottom) 
		{
			if (knockBack)
			{
				knockBack = false;
				velocity.x = 0;
			}
		}
		//else {
			//isStanding   = false;
		//}


		else if (otherHitbox == enemy_right) 
		{
			putForce(vec3(2,3,0));
			knockBack = true;
			//pos.x = other.center.x + other.halfLengths.x + (pos.x - HitboxLeft.center.x + HitboxLeft.halfLengths.x);
		}
		else if(otherHitbox == ColliderType::enemy_left)
		{
			putForce(vec3(-2, 3, 0));
			knockBack = true;
			//pos.x = other.center.x - other.halfLengths.x + (pos.x - HitboxRight.center.x - HitboxRight.halfLengths.x);
		}
	
	if (otherHitbox == ColliderType::powerup_bouncy) 
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Bouncy;
		}
		status = PlayerStatus::Bouncy;
	}
	if (otherHitbox == ColliderType::powerup_heavy)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Heavy;
		}
		status = PlayerStatus::Heavy;
	}
	if (otherHitbox == ColliderType::powerup_sticky)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Sticky;
		}
		status = PlayerStatus::Sticky;
	}
	if (otherHitbox == ColliderType::level_goal) 
	{
		levelCompleted = true;
	}
}

void Player::shoot(vec3 mousePos) noexcept
{
    if (shootCooldown > 0) return;

    auto mouseScreenPos = vec3((mousePos.x - 1280 / 2) * 9, (-(mousePos.y - 980 / 2)) * 16, 0);
    vec3 dir = normalize( mouseScreenPos - pos);
    for ( auto &blob : blobs ) {
        if ( !blob.getIsActive() and !blob.getIsBeingRecalled() ) {
            blob.shoot( dir );
            shootCooldown = .5f; // TODO: refactor into a constexpr constant in Globals.h 
            break;
        }
    }
}


void Player::recallBlobs() noexcept
{
    for ( auto &blob : blobs ) 
        blob.recall();
    shootCooldown   = .5f; // TODO: refactor into a constexpr constant in Globals.h 
}

void Game::update(double dt)  {
	if (state == GameState::LevelState) 
	{
		time += dt;
		vec3 temp = vec3(float(keys[Keys::left]) - float(keys[Keys::right]), 0.0, 0.0);
		//level.player.velocity.x = max(level.player.velocity.x - level.player.moveSpeed, 0.0);
		handleInput();
		level.player.isStanding = false;
		level.player.update(dt);
		if (level.enemy.alive)
		{
			level.enemy.update(dt);
		}
		else if (!level.enemy.alive && level.enemy.isDeregistered)
		{
			level.colManager.unregisterEntry(level.enemy);
		}
		updatePhysics();
		level.player.addVelocity(temp, true);
	}
	updateGraphics();
}

// Call first of all per frame updates
void Game::handleInput() {
    auto &player = level.player;

	if (leftButtonDown) {
		player.shoot(mousePos);
	}

		if (keys[Keys::left]) {
			if (!player.isStuck) {
				player.addVelocity(vec3(-1, 0, 0), true);
			}
		}
		if (keys[Keys::right]) {
			if (!player.isStuck) {
				player.addVelocity(vec3(1, 0, 0), true);
			}
		}
		if (keys[Keys::up]) {
			if (player.isStanding) {
				player.isStanding = false;
				player.jumpCooldown = COOLDOWN_CONSTANT;
				player.putForce(vec3(0.0, level.player.jumpForce, 0.0));
			}
			else if (player.status == PlayerStatus::Bouncy && player.hasExtraJump && player.jumpCooldown <= 0) {
				player.hasExtraJump = false;
				player.jumpCooldown = COOLDOWN_CONSTANT;
				player.velocity.y = 0;
				player.putForce(vec3(0.0, player.jumpForce, 0.0));
			}
		}
		if (keys[Keys::down]) {
			if (player.status == PlayerStatus::Sticky) {
				player.isStuck = false;
				if(!player.isStanding)
					player.pos.y -= 0.01;
				if (player.isStanding == false) {
					//player.addVelocity(vec3(0.0, -GRAVITY_CONSTANT, 0.0));
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

 // player:
	auto &player = level.player;

	while (physicsSimTime + timestep < time) {
		if ( !player.isStuck ) {
			player.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
		}
		else {
            player.setVelocity(vec3(0.0));
        }
        player.move(timestep);

// enemies:
        auto &enemy = level.enemy; // TODO: for ( auto &enemy : level.enemies )
		if (enemy.alive) {
			player.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
			enemy.move(timestep);
		}

// blobs:
		for ( auto &blob : player.blobs ) {
			if (blob.getIsActive() && blob.getIsStuck() == false) {
				blob.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
			}
			blob.move(timestep);
		}
		updatePlayerCollision();
		updateEnemyCollision();
		level.colManager.update();

		physicsSimTime += timestep;
	}
}

void Game::updatePlayerCollision()
{
    auto &player = level.player;

	// Bottom:
	player.Hitbox.center = vec4(
		player.pos.x,
		player.pos.y,
		player.pos.z,
		0);
	player.Hitbox.halfLengths = vec4(
		player.radius,
		player.radius,
		player.radius,
		0);
}

void Game::updateEnemyCollision()
{
    auto &enemy = level.enemy; // TODO: for ( auto &enemy : level.enemies )

	EnemyBox.center = vec4(
		enemy.pos.x,
		enemy.pos.y,
		enemy.pos.z,
		5.0);
	EnemyBox.halfLengths = vec4(
		3.0,
		3.0,
		3.0,
		0.0
	);
	level.boxes.push_back(EnemyBox);

	// Bottom:
	enemy.HitboxBottom.center = vec4(
		enemy.pos.x,
		enemy.pos.y - 0.9*EnemyBox.halfLengths.y,
		enemy.pos.z,
		0);
	enemy.HitboxBottom.halfLengths = vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);

	// Top:
	enemy.HitboxTop.center = vec4(
		enemy.pos.x,
		enemy.pos.y + 0.9*EnemyBox.halfLengths.y,
		enemy.pos.z,
		0);
	enemy.HitboxTop.halfLengths = vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y*0.2,
		EnemyBox.halfLengths.z,
		0);

    // Left:
	enemy.HitboxLeft.center = vec4(
		enemy.pos.x - 0.9*EnemyBox.halfLengths.x,
		enemy.pos.y,
		enemy.pos.z,
		0);
	enemy.HitboxLeft.halfLengths = vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.7,
		EnemyBox.halfLengths.z,
		0);

    // Right:
	enemy.HitboxRight.center = vec4(
		enemy.pos.x + 0.9*EnemyBox.halfLengths.x,
		enemy.pos.y,
		enemy.pos.z,
		0);
	enemy.HitboxRight.halfLengths = vec4(
		EnemyBox.halfLengths.x*0.2,
		EnemyBox.halfLengths.y*0.7,
		EnemyBox.halfLengths.z,
		0);


}

// Call after all other per frame updates
void Game::updateGraphics() {
	level.spheres = vector<Sphere>();
	level.boxes   = vector<Box>();

	if (state == GameState::LevelState) 
	{
		//level.boxes.push_back(groundBox.hitbox);
		//level.boxes.push_back(testPlat.hitbox);
		//level.boxes.push_back(testplat2.hitbox);
		//level.boxes.push_back(level.TestPowerUp.powerBox);
		for (int i = 0; i < editor.platforms.size(); i++)
		{
			editor.platforms.at(i).hitbox.color = glm::vec4(0, 1, 1, 0);
			level.boxes.push_back(editor.platforms.at(i).hitbox);
		}
		EnemyBox.color = vec4((float)level.enemy.isStanding, 1.0 - (float)level.enemy.isStanding, 0.0, 0.0);

		if (level.enemy.alive) {
			level.boxes.push_back(EnemyBox);
		}

		level.spheres = vector<Sphere>();
		playerSphere.centerRadius = vec4(
			level.player.pos.x,
			level.player.pos.y,
			level.player.pos.z,
			level.player.radius);
		playerSphere.color = playerStatusColors[level.player.status];
		level.spheres.push_back(playerSphere);

		if (level.player.levelCompleted == false)
		{
			animateSphere(playerSphere, vec3(3.0, 3.0, 0.5));
		}
		else
			animateVictory(playerSphere);

		for (int i = 0; i < level.player.blobs.size(); i++)
		{
			level.player.blobs[i].blobSphere.color = playerSphere.color;
			level.spheres.push_back(level.player.blobs[i].blobSphere);
		}

		level.boxes.push_back(level.goal->representation);

		//showHitboxes();
	}
	else 
	{
		level.boxes.push_back(MenuBG);
		level.boxes.push_back(MenuYes);
		level.boxes.push_back(MenuNo);
	}
}

void Game::showHitboxes()
{
	level.boxes.push_back(level.player.Hitbox);

	level.boxes.push_back(level.enemy.HitboxBottom);
	level.boxes.push_back(level.enemy.HitboxTop);
	level.boxes.push_back(level.enemy.HitboxLeft);
	level.boxes.push_back(level.enemy.HitboxRight);

	for (int i = 0; i < level.player.blobCharges; i++)
	{
		level.boxes.push_back(level.player.blobs[i].hitbox);
	}
}

Enemy::Enemy(vec3 position):
    CollisionObject(),
    pos(position),
    velocity(vec3(0.0, 0.0, 0.0)),
    controlDir(vec3(1.0, 0.0, 0.0)),
    moveSpeed(20.0f),
    jumpForce(450.0f),
    jumpCooldown(0.3f),
    mass(10.0),
    isStanding(false),
    alive(true),
	isStuck(false),
	isDeregistered(false)
{}

Enemy::~Enemy(){}

void Enemy::collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) noexcept
{
	if (otherHitbox == ColliderType::platform) {
		if (ownHitbox == ColliderType::enemy_bottom)
		{
			pos.y = other.center.y + other.halfLengths.y + (pos.y - HitboxBottom.center.y + HitboxBottom.halfLengths.y);
			velocity.y = 0;
			isStanding = true;
		}
		else if (ownHitbox == ColliderType::enemy_top)
		{
			pos.y = other.center.y - other.halfLengths.y + (pos.y - HitboxTop.center.y - HitboxTop.halfLengths.y);
			velocity.y = 0;
		}
		else if (ownHitbox == ColliderType::enemy_left)
		{
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}
		else if (ownHitbox == ColliderType::enemy_right)
		{
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}
	}
	else if (otherHitbox == ColliderType::player && ownHitbox == enemy_top && other.center.y > pos.y) 
	{
		alive = false;
	}
	else if (otherHitbox == ColliderType::blob) 
	{
		if (other.color.w == 0.25) 
		{
			if (ownHitbox == ColliderType::enemy_bottom)
			{
				pos.y = other.center.y + other.halfLengths.y + (pos.y - HitboxBottom.center.y + HitboxBottom.halfLengths.y);
				velocity.y = 0;
				isStanding = true;
			}
			else if (ownHitbox == ColliderType::enemy_left)
			{
				controlDir.x = -controlDir.x;
				velocity.x = -velocity.x;
			}
			else if (ownHitbox == ColliderType::enemy_right)
			{
				controlDir.x = -controlDir.x;
				velocity.x = -velocity.x;
			}
		}
		if (other.color.w == 0.5) 
		{
			isStuck = true;
		}
	}
}

// Updates logic, call once per frame before updatePhysics
void Enemy::update(double dt) noexcept
{
	velocity.x    = 0.0;
	velocity.y   -= float(GRAVITY_CONSTANT) * float(dt);
	jumpCooldown -= float(dt);

	if (isStanding == false) {
		if (jumpCooldown <= 0.0) {
			controlDir.x = -controlDir.x;
			jumpCooldown = 1.0;
			putForce(vec3(0.0, jumpForce, 0.0));
		}
	}
	if (!isStuck) 
	{
		addVelocity(controlDir, true);
	}

	isStanding = false;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Enemy::addVelocity(vec3 const &velocity, bool useSpeed) noexcept
{
	if (useSpeed) {
		this->velocity += velocity * moveSpeed;
	}
	else {
		this->velocity += velocity;
	}
}

// Set useSpeed to true to multiply velocity by objects speed value
void Enemy::setVelocity(vec3 const &velocity, bool useSpeed) noexcept
{
	if (useSpeed) {
		this->velocity = velocity * moveSpeed;
	}
	else {
		this->velocity = velocity;
	}
}

// velocity += force / mass;
void Enemy::putForce(vec3 const &force) noexcept
{
	velocity += force / mass;
}

// Call from updatePhysics
void Enemy::move(float dt) noexcept
{
	pos += velocity * dt;
}

//Adds two orbiting spheres around a sphere for animation
void Game::animateSphere(Sphere const &sphere, vec3 const &amplitude) {
	vec3 rotationSpeed = vec3(0.81, 0.53, 0.1);
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	vec3 offset = vec3(0.2, 0.0, 0.0);
	// Multiplier to animate faster when moving a certain direction. Not smooth.

	Sphere sphere1(vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x)) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y)) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	sphere1.color = sphere.color;
	level.spheres.push_back(sphere1);

	rotationSpeed = -rotationSpeed;
	offset = vec3(1.45, 0.9, 1.1);
	Sphere sphere2(vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x + offset.x)) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y + offset.y)) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	sphere2.color = sphere.color;
	level.spheres.push_back(sphere2);
}

void Game::animateColor(Graphics& graphics)
{
	//graphics.setMetaballColorAbsorb(vec3(sin(float(time)), -sin(float(time)), cos(float(time))));
}

void Game::animateVictory(Sphere const & sphere)
{
	float distance = 2;
	float orbit = 8;
	vec3 rotationSpeed = vec3(0.81, 0.53, 0.1);
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	vec3 offset = vec3(distance, distance, 2.0);

	Sphere sphere1(vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * sin(float(time)) + offset.x))*orbit,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * sin(float(time)) + offset.y))*orbit,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z)*orbit,
		sphere.centerRadius.w / 2
	));
	sphere1.color = sphere.color;
	level.spheres.push_back(sphere1);

	rotationSpeed = -rotationSpeed;
	offset = vec3(-distance, -distance, -2.0);
	Sphere sphere2(vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * sin(float(time)) + offset.x))*orbit,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * sin(float(time)) + offset.y))*orbit,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z)*orbit,
		sphere.centerRadius.w / 2
	));
	sphere2.color = sphere.color;
	level.spheres.push_back(sphere2);
}




// TODO: commented lines
LevelGoal::LevelGoal( CollisionManager &colMan, vec3 const &position, float radius, TriggerCallback cb ):
    _bounds({vec4(position,0), {2.0f,5.0f,2.0f,0}, {0,0,0,0} }),
     representation({vec4(position,0), {2.0f,5.0f,2.0f,.0f}, {1.0f,.0f,1.0f,0.0f} }),
    _triggerCallback(cb),
    _colMan(&colMan)
{
    _colMan->registerEntry( *this, ColliderType::level_goal, _bounds, true );
}

LevelGoal::~LevelGoal() {
    _colMan->unregisterEntry( *this );
}

void LevelGoal::collide( ColliderType  ownHitbox,
                         ColliderType  otherHitbox,
                         Box const    &other ) noexcept {
    if (    otherHitbox == player)
    {
        _triggerCallback();
        // TODO (in callback):
        //    tone and blur screen?
        //    display text?
        //    wait for input?
        //    load next level?
    }
}

