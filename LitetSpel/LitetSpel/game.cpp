#include "game.h"

void Game::init() noexcept {
	editor.initialize("test.png");
	for (int i = 0; i < editor.platforms.size(); i++)
	{
		editor.platforms.at(i).hitbox.color = glm::vec4(0, 1, 1, 0);
		level.staticBoxes.push_back(editor.platforms.at(i).hitbox);
		level.colManager.registerEntry(editor.platforms.at(i), ColliderType::platform, editor.platforms.at(i).hitbox, true);
	}
    level.goal = std::make_unique<LevelGoal>( level.colManager, editor.goalPos, 12.0f );
	level.staticBoxes.push_back(level.goal->representation);
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
	level.colManager.registerEntry(player, ColliderType::player, player.hitbox, false);

// enemies:
    auto &enemy = level.enemy; // TODO: for ( auto &enemy : level.enemies )
	level.colManager.registerEntry(enemy, ColliderType::enemy, enemy.hitbox, false);
	EnemyBox.color = vec4(1,0,0,0);

// LevelGoal

// PowerUps
	auto &powerup = level.TestPowerUp;
	level.TestPowerUp.powerBox.center = vec4(-30.0f, 15.0f, 0.0f, 0.0f);
	level.TestPowerUp.powerBox.halfLengths = vec4(2.0f, 2.0f, 2.0f, 0.0f);
	level.TestPowerUp.powerBox.color = vec4(0.0, 0.5, 0.75, 0);
	level.staticBoxes.push_back(level.TestPowerUp.powerBox);
	level.colManager.registerEntry(powerup, ColliderType::powerup_bouncy, level.TestPowerUp.powerBox, true);
}

void Game::menuLoad()
{
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
			glm::vec3 pushUp = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

			// if colliding in Y-axis
			if (glm::length(minDistY) < glm::length(minDistX)) {
				posDiff = minDistY;
				velocity.y = 0;

				// if colliding with floor
				if (minDistY.y > 0.0) {
					isStanding = true;
					hasExtraJump = true;
				}
				//if colliding with ceiling
				else {
					if (status == PlayerStatus::Sticky)
					{
						isStuck = true;
					}
				}
			}
			// if colliding in X-axis
			else {
				posDiff = minDistX;

				if (status == PlayerStatus::Sticky && !isStanding)
				{
					isStuck = true;
					velocity.x = 0;
				}
			}

			pos += posDiff;
			
			if (knockBack)
			{
				knockBack = false;
				velocity = vec3(0, 0, 0);
				pos.y += 1;
			}
		}
	
	else if (otherHitbox == ColliderType::blob && status == PlayerStatus::Sticky && other.color.w != 0 && !isStuck && !isStanding)
		{
			if (pos.y > (other.center.y + other.halfLengths.y))
			{
				isStanding = true;
				hasExtraJump = true;
				velocity = vec3(0, 0, 0);
			}
			if (knockBack)
			{
				knockBack = false;
				velocity = vec3(0, 0, 0);
			}
			if (pos.y < (other.center.y - other.halfLengths.y))
			{
				vec3 pushUp = vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
				vec3 pushDown = vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
				vec3 minDistY = length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
				pos.y -= length(minDistY);
			}
			// Collision with blob to the left
			if (pos.x > (other.center.x + other.halfLengths.x))
			{
				vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
				vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
				vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
				pos.x += length(minDistX);
			}
			// Collision with blob to the right
			if (pos.x < (other.center.x - other.halfLengths.x))
			{
				vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
				vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
				vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
				pos.x -= length(minDistX);
			}
		}

	else if (otherHitbox == ColliderType::blob && status == PlayerStatus::Bouncy && other.color.w != 0)
		{
			if (pos.y > (other.center.y + other.halfLengths.y))
			{
				isStanding = true;
				hasExtraJump = true;
				velocity.y = 0;
			}
			if (knockBack)
			{
				knockBack = false;
				velocity.x = 0;
			}
			vec3 pushUp = vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			vec3 pushDown = vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			vec3 pushRight = vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			vec3 pushLeft = vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			vec3 minDistY = length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			vec3 minDistX = length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}
	
	else if (otherHitbox == ColliderType::enemy) 
		{
		glm::vec3 pushUp = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		if (glm::length(minDistY) < glm::length(minDistX)) {
		}
		// if colliding in X-axis
		else {
			posDiff = minDistX;

			if(status != PlayerStatus::Heavy)
			{
				putForce(vec3((hitbox.center.x - other.center.x), 3, 0));
				knockBack = true;
			}
		}

		pos += posDiff;
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
		if (keys[Keys::left]) {
				player.addVelocity(vec3(-1, 0, 0), true);
		}
		if (keys[Keys::right]) {
				player.addVelocity(vec3(1, 0, 0), true);
		}

		if (keys[Keys::down]) {
			if (player.status == PlayerStatus::Sticky && player.isStuck == true) {
				player.isStuck = false;
				player.velocity = vec3(0,0,0);
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
	player.hitbox.center = vec4(
		player.pos.x,
		player.pos.y,
		player.pos.z,
		0);
	player.hitbox.halfLengths = vec4(
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
	level.movingBoxes.push_back(EnemyBox);

	// Bottom:
	enemy.hitbox.center = vec4(
		enemy.pos.x,
		enemy.pos.y,
		enemy.pos.z,
		0);
	enemy.hitbox.halfLengths = vec4(
		EnemyBox.halfLengths.x,
		EnemyBox.halfLengths.y,
		EnemyBox.halfLengths.z,
		0);
}

// Call after all other per frame updates
void Game::updateGraphics() {
	level.spheres = vector<Sphere>();
	level.movingBoxes   = vector<Box>();

	if (state == GameState::LevelState) 
	{
		EnemyBox.color = vec4((float)level.enemy.isStanding, 1.0 - (float)level.enemy.isStanding, 0.0, 0.0);

		if (level.enemy.alive) {
			level.movingBoxes.push_back(EnemyBox);
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

		//showHitboxes();
	}
	else 
	{
		level.movingBoxes.push_back(MenuBG);
		level.movingBoxes.push_back(MenuYes);
		level.movingBoxes.push_back(MenuNo);
	}
}

void Game::showHitboxes()
{
	level.movingBoxes.push_back(level.player.hitbox);

	level.movingBoxes.push_back(level.enemy.hitbox);


	for (int i = 0; i < level.player.blobCharges; i++)
	{
		level.movingBoxes.push_back(level.player.blobs[i].hitbox);
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
		if (pos.y > (other.center.y + other.halfLengths.y))
		{
			pos.y = other.center.y + other.halfLengths.y + (pos.y - hitbox.center.y + hitbox.halfLengths.y);
			velocity.y = 0;
			isStanding = true;
		}
		else if (pos.y < (other.center.y - other.halfLengths.y))
		{
			pos.y = other.center.y - other.halfLengths.y + (pos.y - hitbox.center.y - hitbox.halfLengths.y);
			velocity.y = 0;
		}
		else if (pos.x > (other.center.x + other.halfLengths.x))
		{
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}
		else if (pos.x < (other.center.x - other.halfLengths.x))
		{
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}
	}
	else if (otherHitbox == ColliderType::player && (other.center.y+other.halfLengths.y) > (pos.y+hitbox.halfLengths.y)) 
	{
		alive = false;
	}
	else if (otherHitbox == ColliderType::blob) 
	{
		if (other.color.w == 0.25) 
		{
			if (pos.y > (other.center.y + other.halfLengths.y))
			{
				pos.y = other.center.y + other.halfLengths.y + (pos.y - hitbox.center.y + hitbox.halfLengths.y);
				velocity.y = 0;
				isStanding = true;
			}
			else if (pos.x > (other.center.x + other.halfLengths.x))
			{
				controlDir.x = -controlDir.x;
				velocity.x = -velocity.x;
			}
			else if (pos.x < (other.center.x - other.halfLengths.x))
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

