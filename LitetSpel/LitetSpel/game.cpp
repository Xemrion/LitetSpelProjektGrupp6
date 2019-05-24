#include "game.h"

void Game::init() noexcept {
	editor.initialize("test.png");
	// Platforms
	for (int i = 0; i < editor.platforms.size(); i++)
	{
		level.staticBoxes.push_back(editor.platforms.at(i).hitbox);
		level.colManager.registerEntry(editor.platforms.at(i), ColliderType::platform, editor.platforms.at(i).hitbox, true);
	}
	// Moving Platforms
	for (int i = 0; i < editor.movingPlatforms.size(); i++)
	{
		level.movingPlatforms.push_back(editor.movingPlatforms.at(i));
	}
	// Moving Platform hitboxes
	for (int i = 0; i < level.movingPlatforms.size(); i++)
	{
		level.colManager.registerEntry((level.movingPlatforms.at(i)), ColliderType::movingPlatform, (level.movingPlatforms.at(i)).hitbox, true);
	}
	// Power Ups
	for (int i = 0; i < editor.powerups.size(); i++)
	{
		level.powerUps.push_back(editor.powerups.at(i));
	}
	// Power up hitboxes
	for (int i = 0; i < level.powerUps.size(); i++)
	{
		switch (editor.powerups.at(i).getType())
		{
		case PowerType::Bouncy:
			level.colManager.registerEntry((level.powerUps.at(i)), ColliderType::powerup_bouncy, (level.powerUps.at(i)).hitbox, true);
			break;
		case PowerType::Sticky:
			level.colManager.registerEntry((level.powerUps.at(i)), ColliderType::powerup_sticky, (level.powerUps.at(i)).hitbox, true);
			break;
		case PowerType::Heavy:
			level.colManager.registerEntry((level.powerUps.at(i)), ColliderType::powerup_heavy, (level.powerUps.at(i)).hitbox, true);
			break;
		default:
			level.colManager.registerEntry((level.powerUps.at(i)), ColliderType::powerup_none, (level.powerUps.at(i)).hitbox, true);
			break;
		}
	}
	// Buttons and Gates
	for (int i = 0; i < editor.buttons.size(); i++)
	{
		while (editor.buttons.at(i).index != i)
		{
			std::swap(editor.buttons.at(i), editor.buttons.at(editor.buttons.at(i).index));
		}
		while (editor.gates.at(i).index != i)
		{
			std::swap(editor.gates.at(i), editor.gates.at(editor.gates.at(i).index));
		}
		level.gates.push_back(editor.gates.at(i));
		level.buttons.push_back(editor.buttons.at(i));
	}
	for (int i = 0; i < editor.lasers.size(); i++)
	{
		level.lasers.push_back(editor.lasers.at(i));
	}
	// Button hitboxes
	for (int i = 0; i < level.buttons.size(); i++)
	{
		level.gates.at(i).button = &level.buttons.at(i);

		level.colManager.registerEntry(level.buttons.at(i), ColliderType::platform, level.buttons.at(i).hitbox, false);
	}
	//Gate hitboxes
	for (int i = 0; i < level.gates.size(); i++)
	{
		level.colManager.registerEntry(level.gates.at(i), ColliderType::platform, level.gates.at(i).hitbox, true);
	}
	//Laser hitboxes
	for (int i = 0; i < level.lasers.size(); i++)
	{
		level.colManager.registerEntry(level.lasers.at(i), ColliderType::damagePlatform, level.lasers.at(i).hitbox, true);
	}
	//Enemies
	for (int i = 0; i < editor.enemies.size(); i++)
	{
		level.enemies.push_back(Enemy(editor.enemies.at(i)));
	}
	//Enemy hitboxes
	for (int i = 0; i < editor.enemies.size(); i++)
	{
		level.enemies.at(i).hitbox.halfLengths = vec4(3, 3, 3, 0);
		level.enemies.at(i).hitbox.color = vec4(1, 0, 0, 0);
		level.colManager.registerEntry(level.enemies.at(i), ColliderType::enemy, level.enemies.at(i).hitbox, false);
	}
	// player & blobs:
	auto &player = level.player;
	player.gameSounds = gameSounds;
	player.pos = editor.startPos;
	for (int i = 0; i < player.blobCharges; ++i) {
		player.blobs.push_back(Blob(player.pos));
		level.spheres.push_back(player.blobs[i].blobSphere);
	}
	for (auto &b : player.blobs) {
		b.gameSounds = gameSounds;
		level.colManager.registerEntry(b, ColliderType::blob, b.hitbox, false);
	}
	updatePlayerCollision();
	level.colManager.registerEntry(player, ColliderType::player, player.hitbox, false);

	//level.lasers.push_back(Laser(vec3(20, 40, 0), vec3(20, -40, 0), vec3(0,0,1), 50));
	//level.colManager.registerEntry(level.lasers.at(0), ColliderType::damagePlatform, level.lasers.at(0).hitbox, true);
}

void Game::reset()
{
	level.spheres = vector<Sphere>();
	level.movingBoxes = vector<Box>();
	level.buttons = vector<Button>();
	level.enemies = vector<Enemy>();
	level.gates = vector<Gate>();
	level.movingPlatforms = vector<MovingPlatform>();
	level.powerUps = vector<PowerUp>();
	level.staticBoxes = vector<Box>();
	//TODO:
	//Deregister hitboxes
	init();
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
	moveSpeed(75.0f),
	mass(10.0),
	blobCharges(5),
	shootCooldown(1.0f),
	jumpForce(1200.0f),
	jumpCooldown(.0f),
	takeDamageCooldown(3.0f),
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
		this->velocity += velocity * moveSpeed;
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
	jumpCooldown -= float(dt);
	shootCooldown -= float(dt);
	takeDamageCooldown -= float(dt);

	if (isStanding)
		hasExtraJump = true;
	if (velocity.y < -40 || velocity.y > 40) {
		landing = false;
	}
	mass = (status == PlayerStatus::Heavy) ? 15.0f : 10.0f;
	isStuck = false;
	if (status != PlayerStatus::Sticky) {
		isStuck = false;
	}
	if (status == PlayerStatus::Heavy)
	{
		hitbox.color.w = 0.1;
	}
	else
		hitbox.color.w = 0;

	for (auto &blob : blobs)
		blob.update(dt);
}

void Player::collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept
{
	if (other.colliderType == ColliderType::platform) {
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		// posDiff is the direction and amount to push the player away from the platform
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		float eps = 0.1; // fixes some issues with platform edges
		if (glm::length(minDistY) <= glm::length(minDistX) - eps) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y >= 0.0) {
				velocity.y = -15;
				isStanding = true;
				hasExtraJump = true;
				isStuck = false;
				if (landing != true) {
					if (status == PlayerStatus::Bouncy) {
						gameSounds->PlayLandingSound03();
					}
					else if (status == PlayerStatus::Heavy) {
						gameSounds->PlayLandingSound02();
					}
					else {
						gameSounds->PlayLandingSound01();
					}
					landing = true;
				}
			}
			//if colliding with ceiling
			else {
				velocity.y = min(0, velocity.y);
			}
		}
		// if colliding in X-axis
		else {
			posDiff = minDistX;
		}

		pos += posDiff;

		// if the player is sticky, not standing and holding a control button
		if (status == PlayerStatus::Sticky && !isStanding && length(controlDir))
		{
			isStuck = true;

			// push the player a little into the platform to ensure they collide with it as long as they are sticky
			if (controlDir.y > 0.0 && posDiff.y < 0.0) {
				pos.y += 0.005f;
			}
			else if (controlDir.x > 0.0) {
				pos.x += 0.005f;
			}
			else if (controlDir.x < 0.0) {
				pos.x -= 0.005f;
			}
		}
		else if (knockBack)
		{
			knockBack = false;
			velocity = vec3(0, 0, 0);
			pos.y += 1;
		}
	}
	else if (other.colliderType == ColliderType::movingPlatform) {
		MovingPlatform* platformPtr = (MovingPlatform*)other.object;
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		float eps = 0.1; // fixes some issues with platform edges
		if (glm::length(minDistY) < glm::length(minDistX) - eps) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y >= 0.0) {
				velocity.y = -15;
				isStanding = true;
				collidingMovingPlatform = platformPtr;
				hasExtraJump = true;
				isStuck = false;
				if (landing != true) {
					if (status == PlayerStatus::Bouncy) {
						gameSounds->PlayLandingSound03();
					}
					else if (status == PlayerStatus::Heavy) {
						gameSounds->PlayLandingSound02();
					}
					else {
						gameSounds->PlayLandingSound01();
					}
					landing = true;
				}
			}
			//if colliding with ceiling
			else {
				velocity.y = min(0, velocity.y);
			}
		}
		// if colliding in X-axis
		else {
			posDiff = minDistX;
		}

		pos += posDiff;

		//if the player is sticky, not standing and holding a control button
		if (status == PlayerStatus::Sticky && !isStanding && length(controlDir))
		{
			isStuck = true;
			collidingMovingPlatform = platformPtr;

			// push the player a little into the platform to ensure they collide with it as long as they are sticky
			// if holding up and colliding with ceiling
			if (controlDir.y > 0.0 && posDiff.y < 0.0) {
				pos.y += 0.005f;
			}
			else if (controlDir.x > 0.0) {
				pos.x += 0.005f;
			}
			else if (controlDir.x < 0.0) {
				pos.x -= 0.005f;
			}
		}
		else if (knockBack)
		{
			knockBack = false;
			velocity = vec3(0, 0, 0);
			pos.y += 1;
		}
	}
	else if (other.colliderType == ColliderType::damagePlatform) {
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		// posDiff is the direction and amount to push the player away from the platform
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		float eps = 0.1; // fixes some issues with platform edges
		if (glm::length(minDistY) <= glm::length(minDistX) - eps) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y >= 0.0) {
				putForce(vec3(0, jumpForce / 2, 0));
				knockBack = false;
				if (takeDamageCooldown <= 0) {
					if (lifeCharges >= 2) {
						gameSounds->PlayDamagedSound01();
					}
					else if (lifeCharges = 1) {
						gameSounds->PlayDamagedSound02();
					}
					lifeCharges -= 1;
					takeDamageCooldown = 3.0f;
				}

			}
			//if colliding with ceiling
			else {
				putForce(vec3(0, -(jumpForce / 2), 0));
				knockBack = false;
				if (takeDamageCooldown <= 0) {
					if (lifeCharges >= 2) {
						gameSounds->PlayDamagedSound01();
					}
					else if (lifeCharges = 1) {
						gameSounds->PlayDamagedSound02();
					}
					lifeCharges -= 1;
					takeDamageCooldown = 3.0f;
				}
			}
		}
		// if colliding in X-axis
		else {
			if (minDistX.x >= 0) {
				putForce(vec3(jumpForce / 3, jumpForce / 2, 0));
				knockBack = true;
				if (takeDamageCooldown <= 0) {
					if (lifeCharges >= 2) {
						gameSounds->PlayDamagedSound01();
					}
					else if (lifeCharges = 1) {
						gameSounds->PlayDamagedSound02();
					}
					lifeCharges -= 1;
					takeDamageCooldown = 3.0f;
				}
			}
			else {
				putForce(vec3(-jumpForce / 3, jumpForce / 2, 0));
				knockBack = true;
				if (takeDamageCooldown <= 0) {
					if (lifeCharges >= 2) {
						gameSounds->PlayDamagedSound01();
					}
					else if (lifeCharges = 1) {
						gameSounds->PlayDamagedSound02();
					}
					lifeCharges -= 1;
					takeDamageCooldown = 3.0f;
				}
			}
			posDiff = minDistX;
		}

		pos += posDiff;
	}

	else if (other.colliderType == ColliderType::blob && status == PlayerStatus::Sticky && other.hitbox->color.w != 0 && !isStuck && !isStanding)
	{
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

	// if colliding in Y-axis
	if (glm::length(minDistY) < glm::length(minDistX)) {
		posDiff = minDistY;
	
		// if colliding with floor
		if (minDistY.y > 0.0) {
			velocity.y = -15;
			isStanding = true;
			hasExtraJump = true;
			isStuck = false;
		}
		//if colliding with ceiling
		else {
			velocity.y = min(0, velocity.y);
		}
	}
	// if colliding in X-axis
	else {
		posDiff = minDistX;
	}

		pos += posDiff;
	}

	else if (other.colliderType == ColliderType::blob && status == PlayerStatus::Bouncy && other.hitbox->color.w != 0)
	{
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		if (glm::length(minDistY) < glm::length(minDistX)) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y > 0.0) {
				velocity.y = -15;
				isStanding = true;
				hasExtraJump = true;
				isStuck = false;
			}
			//if colliding with ceiling
			else {
				velocity.y = min(0, velocity.y);
			}
		}
		// if colliding in X-axis
		else {
			posDiff = minDistX;
		}

		pos += posDiff;
	}

	else if (other.colliderType == ColliderType::enemy)
	{
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		if (glm::length(minDistY) < glm::length(minDistX))
		{
			if (minDistY.y >= 0.0) {
				velocity.y = 0;
				isStanding = true;
				hasExtraJump = true;
				isStuck = false;
			}
		}
		// if colliding in X-axis
		else {
			if (status != PlayerStatus::Heavy && !knockBack)
			{

				if (minDistX.x >= 0.0)
				{
					putForce(vec3(jumpForce / 3, jumpForce / 2, 0));
					knockBack = true;
					if (takeDamageCooldown <= 0) {
						if (lifeCharges >= 2) {
							gameSounds->PlayDamagedSound01();
						}
						else if (lifeCharges = 1) {
							gameSounds->PlayDamagedSound02();
						}
						lifeCharges -= 1;
						takeDamageCooldown = 3.0f;
					}
				}
				else
				{
					putForce(vec3(-jumpForce / 3, jumpForce / 2, 0));
					knockBack = true;
					if (takeDamageCooldown <= 0) {
						if (lifeCharges >= 2) {
							gameSounds->PlayDamagedSound01();
						}
						else if (lifeCharges = 1) {
							gameSounds->PlayDamagedSound02();
						}
						lifeCharges -= 1;
						takeDamageCooldown = 3.0f;
					}
				}

			}


			posDiff = minDistX;


		}

		pos += posDiff;
	}

	if (other.colliderType == ColliderType::powerup_bouncy)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Bouncy;
		}
		if (status != PlayerStatus::Bouncy) {
			gameSounds->PlayAbsorbSound03();
		}
		status = PlayerStatus::Bouncy;
	}
	if (other.colliderType == ColliderType::powerup_heavy)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Heavy;
		}
		if (status != PlayerStatus::Heavy) {
			gameSounds->PlayAbsorbSound03();
		}
		status = PlayerStatus::Heavy;
	}
	if (other.colliderType == ColliderType::powerup_sticky)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_Sticky;
		}
		if (status != PlayerStatus::Sticky) {
			gameSounds->PlayAbsorbSound03();
		}
		status = PlayerStatus::Sticky;
	}
	if (other.colliderType == ColliderType::powerup_none)
	{
		for (int i = 0; i < blobCharges; i++)
		{
			blobs[i].status = BlobStatus::Blob_None;
		}
		if (status != PlayerStatus::None) {
			gameSounds->PlayAbsorbSound03();
		}
		status = PlayerStatus::None;
	}
	if (other.colliderType == ColliderType::level_goal)
	{
		levelCompleted = true;
		gameSounds->PlayEndOfLevelSound();
	}
}

void Player::shoot(vec3 mousePos) noexcept
{
	if (shootCooldown > 0) return;

	vec3 dir = normalize(mousePos - pos);
	for (auto &blob : blobs) {
		if (!blob.getIsActive() and !blob.getIsBeingRecalled()) {
			blob.shoot(dir);
			gameSounds->PlayBlobSound01();
			shootCooldown = .5f; // TODO: refactor into a constexpr constant in Globals.h 
			break;
		}
	}
}

void Player::recallBlobs() noexcept
{
	if (isStanding)
	{
		for (auto &blob : blobs)
			blob.recall();
		shootCooldown = .5f; // TODO: refactor into a constexpr constant in Globals.h 
	}

}

void Game::update(double dt) {
	if (state == GameState::LevelState)
	{
		time += dt;
		vec3 temp = vec3(float(keys[Keys::left]) - float(keys[Keys::right]), 0.0, 0.0);
		if (!level.player.knockBack)
		{
			level.player.velocity.x = max(level.player.velocity.x - level.player.moveSpeed, 0.0);
		}
		cameraPos = level.player.pos + cameraOffset;
		panCamera = true;

		handleInput();
		level.player.isStanding = false;
		level.player.collidingMovingPlatform = nullptr;
		level.player.update(dt);
		for (int i = 0; i < level.enemies.size(); i++)
		{
			if (level.enemies.at(i).alive)
			{
				level.enemies.at(i).update(dt);
			}
			else if (!level.enemies.at(i).alive && !level.enemies.at(i).isDeregistered)
			{
				level.enemies.at(i).isDeregistered = true;
				level.enemies.at(i).pos = vec3(0, -1000, 0);
			}
		}

		updatePhysics();
		level.player.addVelocity(temp, true);

		for (Blob& b : level.player.blobs) {
			if (!b.getIsActive()) {
				b.followPlayer();
			}
		}
	}
	updateGraphics();
}

// Call first of all per frame updates
void Game::handleInput() {
	auto &player = level.player;
	player.controlDir = glm::vec3(keys[Keys::right] - keys[Keys::left], keys[Keys::up] - keys[Keys::down], 0.0);
	if (leftButtonDown) {
		player.shoot(mousePos);
	}
	if (keys[Keys::up]) {
		if (player.isStanding) {
			player.isStanding = false;
			player.jumpCooldown = COOLDOWN_CONSTANT;
			player.velocity.y = 0;
			player.putForce(vec3(0.0, level.player.jumpForce, 0.0));
			if (player.status == PlayerStatus::Bouncy) {
				gameSounds->PlayJumpSound03();
			}
			else if (player.status == PlayerStatus::Heavy) {
				gameSounds->PlayJumpSound02();
			}
			else {
				gameSounds->PlayJumpSound01();
			}
		}
		else if (player.status == PlayerStatus::Bouncy && player.hasExtraJump && player.jumpCooldown <= 0) {
			player.hasExtraJump = false;
			player.jumpCooldown = COOLDOWN_CONSTANT;
			player.velocity.y = 0;
			player.putForce(vec3(0.0, player.jumpForce, 0.0));
			gameSounds->PlayJumpSound03();
		}
	}
	if (keys[Keys::left]) {
		if (player.status == PlayerStatus::Sticky && player.isStuck == true) {
			//player.isStuck = false;
			//player.pos.x -= 0.001;
		}
		else {
			player.addVelocity(vec3(-1, 0, 0), true);
		}
	}
	if (keys[Keys::right]) {

		if (player.status == PlayerStatus::Sticky && player.isStuck == true) {
			player.isStuck = false;
			//player.pos.x += 0.001;
		}
		else {
			player.addVelocity(vec3(1, 0, 0), true);
		}
	}
	if (keys[Keys::down]) {
		if (player.status == PlayerStatus::Sticky && player.isStuck == true) {
			player.isStuck = false;
			//player.pos.y -= 0.001;
		}
		else {
			cameraPos = level.player.pos + cameraLookDownOffset;
		}
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
}

// Catches up the physics simulation time to the actual game time
// Call last of all logic updates (but before graphics)
void Game::updatePhysics() {
	float timestep = PHYSICS_TIME_STEP;


	auto &player = level.player;

	while (physicsSimTime + timestep < time) {
		// player:
		if (player.isStuck) {
			player.setVelocity(vec3(0.0));
		}
		else {
			player.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
		}
		if (player.collidingMovingPlatform != nullptr) {
			player.pos += player.collidingMovingPlatform->moveFunction(physicsSimTime) - player.collidingMovingPlatform->moveFunction(physicsSimTime - timestep);
		}

		player.move(timestep);
		player.isStuck = false;
		// enemies:
		for (int i = 0; i < level.enemies.size(); i++)
		{
			auto &enemy = level.enemies.at(i); // TODO: for ( auto &enemy : level.enemies )
			if (enemy.alive) {
				enemy.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
				enemy.move(timestep);
			}
		}

		// blobs:
		for (auto &blob : player.blobs) {
			if (blob.getIsActive() && blob.getIsStuck() == false && !blob.getIsBeingRecalled())
			{
				blob.addVelocity(vec3(0.0, -GRAVITY_CONSTANT * timestep, 0.0));
			}
			blob.move(timestep);
		}

		//Moving platforms:
		for (auto& movingPlatform : level.movingPlatforms)
		{
			movingPlatform.move(physicsSimTime);
		}
		//gates
		for (auto& Gates : level.gates)
		{
			Gates.move(physicsSimTime);
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

	// player:
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
	for (int i = 0; i < level.enemies.size(); i++)
	{
		auto &enemy = level.enemies.at(i); // TODO: for ( auto &enemy : level.enemies )
		// Bottom:
		enemy.hitbox.center = vec4(
			enemy.pos.x,
			enemy.pos.y,
			enemy.pos.z,
			0);
		enemy.hitbox.halfLengths = vec4(
			enemy.hitbox.halfLengths.x,
			enemy.hitbox.halfLengths.y,
			enemy.hitbox.halfLengths.z,
			0);
		level.movingBoxes.push_back(enemy.hitbox);
	}

}

// Call after all other per frame updates
void Game::updateGraphics() {
	level.spheres = vector<Sphere>();
	level.movingBoxes = vector<Box>();
	level.laserGraphics = vector<Line>();

	if (state == GameState::LevelState)
	{
		// Enemies
		for (int i = 0; i < level.enemies.size(); i++)
		{
			level.enemies.at(i).hitbox.color = vec4((float)level.enemies.at(i).isStanding, 1.0 - (float)level.enemies.at(i).isStanding, 0.0, 0.0);
			if (level.enemies.at(i).alive) {
				//level.movingBoxes.push_back(level.enemies.at(i).hitbox);

				Sphere s;
				s.centerRadius = level.enemies.at(i).hitbox.center;
				s.centerRadius.w = 5;
				s.color = enemyColor;
				level.spheres.push_back(s);
			}
		}
		// Moving platforms
		for (int i = 0; i < level.movingPlatforms.size(); i++)
		{
			level.movingBoxes.push_back(level.movingPlatforms.at(i).hitbox);
		}
		// Gates & buttons
		for (int i = 0; i < level.gates.size(); i++)
		{
			level.movingBoxes.push_back(level.gates.at(i).hitbox);
			level.movingBoxes.push_back(level.buttons.at(i).hitbox);
		}
		// Lasers
		for (int i = 0; i < level.lasers.size(); i++)
		{
			level.laserGraphics.push_back(level.lasers.at(i).visual);
			//level.movingBoxes.push_back(level.lasers.at(i).hitbox);
		}

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

		for (int i = 0; i < level.powerUps.size(); ++i) {
			Sphere powerUpSphere;
			powerUpSphere.centerRadius = level.powerUps.at(i).hitbox.center;
			powerUpSphere.centerRadius.w = 10.0;
			powerUpSphere.color = playerStatusColors[level.powerUps.at(i).getTypeAsInt()];
			level.spheres.push_back(powerUpSphere);
			animateSphere(powerUpSphere, glm::vec3(10, 10, 2));
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
	for (int i = 0; i < level.enemies.size(); i++)
	{
		level.movingBoxes.push_back(level.enemies.at(i).hitbox);
	}
	for (int i = 0; i < level.player.blobs.size(); i++)
	{
		level.movingBoxes.push_back(level.player.blobs[i].hitbox);
	}
}
Enemy::Enemy(vec3 position) :
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

Enemy::~Enemy() {}

void Enemy::collide(ColliderType ownHitbox, const HitboxEntry& other) noexcept
{
	if (other.colliderType == ColliderType::platform) {
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		if (glm::length(minDistY) < glm::length(minDistX)) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y >= 0.0) {
				velocity.y = 0;
				isStanding = true;
			}
			//if colliding with ceiling
			else {
				velocity.y = 0;
				velocity.y = min(0, velocity.y);
			}
		}
		// if colliding in X-axis
		else {
			posDiff = minDistX;
			controlDir.x = -controlDir.x;
			velocity.x = -velocity.x;
		}

		pos += posDiff;
	}
	else if (other.colliderType == ColliderType::player)
	{
		glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

		// if colliding in Y-axis
		if (glm::length(minDistY) < glm::length(minDistX)) {
			posDiff = minDistY;

			// if colliding with floor
			if (minDistY.y >= 0.0) {
				velocity.y = 0;
				isStanding = true;
			}
			//if colliding with ceiling
			else {
				alive = false;
			}
		}
		// if colliding in X-axis
		else {
			if (other.hitbox->color.w != 0)
			{
				posDiff = minDistX;
				controlDir.x = -controlDir.x;
				velocity.x = -velocity.x;
			}
		}
		pos += posDiff;

	}
	else if (other.colliderType == ColliderType::blob)
	{
		if (other.hitbox->color.w == 0.25)
		{
			glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			glm::vec3 posDiff = glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;

			// if colliding in Y-axis
			if (glm::length(minDistY) < glm::length(minDistX)) {
				posDiff = minDistY;

				// if colliding with floor
				if (minDistY.y >= 0.0) {
					velocity.y = 0;
					isStanding = true;
				}
				//if colliding with ceiling
				else {
					velocity.y = 0;
					velocity.y = min(0, velocity.y);
				}
			}
			// if colliding in X-axis
			else {
				posDiff = minDistX;
				controlDir.x = -controlDir.x;
				velocity.x = -velocity.x;
			}

			pos += posDiff;
		}
		if (other.hitbox->color.w == 0.5)
		{
			if (isStuck != true) {
				gameSounds->PlayEnemySound03();
			}
			isStuck = true;
		}
	}
}

// Updates logic, call once per frame before updatePhysics
void Enemy::update(double dt) noexcept
{
	velocity.x = 0.0;
	velocity.y -= float(GRAVITY_CONSTANT) * float(dt);
	jumpCooldown -= float(dt);

	if (isStanding == false) {
		if (jumpCooldown <= 0.0) {
			controlDir.x = -controlDir.x;
			jumpCooldown = 1.0;
			putForce(vec3(0.0, jumpForce * 2, 0.0));
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
	vec3 rotationSpeed = vec3(2.81, 2.53, 0.1);
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
LevelGoal::LevelGoal(CollisionManager &colMan, vec3 const &position, float radius, TriggerCallback cb) :
	_bounds({ vec4(position,0), {2.0f,5.0f,2.0f,0}, {0,0,0,0} }),
	representation({ vec4(position,0), {2.0f,5.0f,2.0f,.0f}, {1.0f,.0f,1.0f,0.0f} }),
	_triggerCallback(cb),
	_colMan(&colMan)
{
	_colMan->registerEntry(*this, ColliderType::level_goal, _bounds, true);
}

LevelGoal::~LevelGoal() {
	_colMan->unregisterEntry(*this);
}

void LevelGoal::collide(ColliderType  ownHitbox, const HitboxEntry& other) noexcept {
	if (other.colliderType == player)
	{
		_triggerCallback();
		// TODO (in callback):
		//    tone and blur screen?
		//    display text?
		//    wait for input?
		//    load next level?
	}
}

