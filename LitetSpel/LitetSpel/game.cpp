#include "game.h"
#define GRAVITY_CONSTANT 50.0f

void Game::init() {
    groundBox.hitbox.center = glm::vec4(-10, -30, 10, 0);
    groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
	currentLevel.boxes.push_back(groundBox.hitbox);
	
    currentLevel.colManager.register_entry( groundBox, CollisionId::platform, groundBox.hitbox, true );

	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		5.0);
	currentLevel.spheres.push_back(playerSphere);

    currentLevel.colManager.register_entry( currentLevel.player, CollisionId::player_bottom, playerSphere, false );
}




Player::Player( glm::vec3 position ):
    IObject(),
    posPrev      (position),
    posCurr      (position),
    moveSpeed    (100.0f),
    jumpSpeed    (.0f),
    jumpCooldown (.0f),
    gravity      (GRAVITY_CONSTANT),
    hasExtraJump (true),
    isStanding   (false),
    status       (2) // TODO: enum!
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
}


void Player::collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) {
    //if ( otherHitbox == CollisionId::platform )
    isStanding = true;
    //posCurr = posPrev;

        //posCurr.y = 0;
        //move(dt, glm::vec3(.5f, 0, 0));
    //}
   //move( dt, glm::vec3(-0.1f, 0.0f, 0.0f));

}





void Game::update(double dt) {
	if (keys[0]) {
		if (currentLevel.player.status == 2) {
			currentLevel.player.move(dt, glm::vec3(-0.2, 0, 0));
		}
		else {
			currentLevel.player.move(dt, glm::vec3(-1, 0, 0));
		}

	}
	if (keys[1]) {
		if (currentLevel.player.status == 2) {
			currentLevel.player.move(dt, glm::vec3(0.2, 0, 0));
		}
		else {
			currentLevel.player.move(dt, glm::vec3(1, 0, 0));
		}
	}
	if (keys[2]) {
		if (currentLevel.player.isStanding == true && currentLevel.player.jumpCooldown <= 0 && currentLevel.player.status != 2) {
			currentLevel.player.jumpSpeed = -100.0f * float(dt);
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = 0.2;
		}
		else if (currentLevel.player.posCurr.y < -0.1f && currentLevel.player.status == 1 && currentLevel.player.hasExtraJump == true && currentLevel.player.isStanding == false && currentLevel.player.jumpCooldown <= 0) {
			currentLevel.player.hasExtraJump = false;
			currentLevel.player.jumpSpeed = -100.0f * float(dt);
			currentLevel.player.jumpCooldown = 0.2;
		}

		else if (currentLevel.player.status == 2 && currentLevel.player.isStanding == true) {
			currentLevel.player.jumpSpeed = -50.0f * float(dt);
			currentLevel.player.isStanding = false;
            currentLevel.player.gravity = GRAVITY_CONSTANT;
			currentLevel.player.jumpCooldown = 0.2;
		}
	}
	if (keys[3]) {
		currentLevel.player.move(dt, glm::vec3(0, 1, 0));
	}

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}
    // gravity
	currentLevel.player.jumpSpeed = currentLevel.player.jumpSpeed + (currentLevel.player.gravity * float(dt)) / 100;
	currentLevel.player.posCurr.y += currentLevel.player.jumpSpeed;

	currentLevel.player.jumpCooldown -= dt;

	playerSphere.centerRadius = glm::vec4(
		currentLevel.player.posCurr.x,
		currentLevel.player.posCurr.y,
		currentLevel.player.posCurr.z,
		5.0);
	currentLevel.spheres = vector<Sphere>();
	currentLevel.spheres.push_back(playerSphere);

    currentLevel.colManager.update();
    currentLevel.player.update();
}

