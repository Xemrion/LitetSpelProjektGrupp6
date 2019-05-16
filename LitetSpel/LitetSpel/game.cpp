#include <cassert>

#include "game.h"


void Game::loadLevel() { // temp
    level = std::make_unique<Level>();

    auto player = std::make_unique<Player>( glm::vec3{  .0f,  .0f, .0f } );
    player->attachInput(keyboard, mouse);

    auto enemy1 = std::make_unique<Enemy>(  glm::vec3{ -30.0f, 15.0f, .0f } );
    auto goal   = std::make_unique<LevelGoal>( glm::vec3{70.0f,-15.0f,.0f}, 12.0f );
    auto ground = std::make_unique<Platform>( Box{glm::vec4{  0, -30, 0, 0}, glm::vec4{100, 10, 10, 0}, green }, .950f ); // groundbox; color: (1.0, 1.0, 1.0, 0.0);
    auto plat1  = std::make_unique<Platform>( Box{glm::vec4{ 30,  0,  0, 0}, glm::vec4{ 10, 20, 10, 0}, red   }, .975f ); // testPlat1; color: (0.0, 1.0, 0.0, 0.0);
    auto plat2  = std::make_unique<Platform>( Box{glm::vec4{-30,  10, 0, 0}, glm::vec4{ 10,  2, 10, 0}, blue  }, .999f ); // testPlat2: color: (0.0, 0.5, 0.5, 0.0);

    level->add( std::move(player) );
    level->add( std::move(enemy1) );
    level->add( std::move(goal)   );
    level->add( std::move(ground) );
    level->add( std::move(plat1)  );
    level->add( std::move(plat2)  );
}

Game::Game( KeyboardInput *keyboard, MouseInput *mouse ):
    keyboard ( keyboard ),
    mouse    ( mouse    )
{}

void Game::init() noexcept {
    loadLevel();
// TODO!!!!
///    level.goal = std::make_unique<LevelGoal>( level.colManager, glm::vec3{70.0f,-15.0f,.0f}, 12.0f );
///    groundBox.hitbox.center      = glm::vec4(0, -30, 0, 0);
///    groundBox.hitbox.halfLengths = glm::vec4(100, 10, 10, 0);
///	groundBox.hitbox.color          = glm::vec4(1.0, 1.0, 1.0, 0.0);
///	level.boxes.push_back(groundBox.hitbox);
///
///    level.colManager.add({&groundBox, ColliderType::platform, groundBox.hitbox, true }); // TODO: ICollider::getHitboxes() + Level::add()
///
///	testPlat.hitbox.center      = glm::vec4(30.0f, 0.0f, 0.0f, 0.0f);
///	testPlat.hitbox.halfLengths = glm::vec4(10.0f, 20.0f, 10.0f, 0.0f);
///	testPlat.hitbox.color       = glm::vec4(0.0, 1.0, 0.0, 0.0);
///	level.boxes.push_back(testPlat.hitbox);
///    level.colManager.add({&testPlat, ColliderType::platform, testPlat.hitbox, true }); // TODO: ICollider::getHitboxes() + Level::add()
///
///	testplat2.hitbox.center      = glm::vec4(-30.0f, 10.0f, 0.0f, 0.0f);
///	testplat2.hitbox.halfLengths = glm::vec4(10.0f, 2.0f, 10.0f, 0.0f);
///	testplat2.hitbox.color       = glm::vec4(0.0, 0.5, 0.5, 0.0);
///	level.boxes.push_back(testplat2.hitbox);
///    level.colManager.add({&testplat2, ColliderType::platform, testplat2.hitbox, true}); // TODO: ICollider::getHitboxes() + Level::add()

// player & blobs:
    /*
    auto &player = level.player;
	for ( int i = 0;  i < player.blobCharges;  ++i ) {
        Blob b { player.pos };
		player.blobs.push_back( Blob(player.pos) );
		level.spheres.push_back( player.blobs[i].blobSphere );
	}
    for ( auto &b : player.blobs ) {
        level.colManager.add({&b, ColliderType::blob, b.hitbox, false});  // TODO: ICollider::getHitboxes() + 
        ()
    }
	updatePlayerCollision();
    */

// enemies:
    /*
    level.enemies.emplace_back( glm::vec3( -25.0f, 20.0f, 0.0f) );
    enemyBox.color = glm::vec4(1,0,0,0);
    for ( auto &enemy : level.enemies ) {
	    level.colManager.add({&enemy, ColliderType::enemy_bottom, enemy.HitboxBottom, false});  // TODO: ICollider::getHitboxes() + Level::add()
	    level.colManager.add({&enemy, ColliderType::enemy_top,    enemy.HitboxTop,    false});  // TODO: ICollider::getHitboxes() + Level::add()
	    level.colManager.add({&enemy, ColliderType::enemy_left,   enemy.HitboxLeft,   false});  // TODO: ICollider::getHitboxes() + Level::add()
	    level.colManager.add({&enemy, ColliderType::enemy_right,  enemy.HitboxRight,  false});  // TODO: ICollider::getHitboxes() + Level::add()
    }
    */
}

Level& Game::getLevel() noexcept {
    return *level;
}

// TODO!!!
Player::Player( glm::vec3 position={.0f, .0f, .0f} ):
    IActor( false,   // isStanding
            PLAYER_JUMP_FORCE,
            JUMP_CD,
            PLAYER_MASS,
            PLAYER_SPEED,
            position
    ),
    hasExtraJump  ( true         ),
    isStuck       ( false        ),
    status        ( Status::none ),
    blobCharges   ( 5            ),
    shootCooldown ( SHOOT_CD     ),
    powerCooldown ( POWER_CD     ),
    radius        ( 5.0f         )
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,      // hitbox parent
        ColliderType::player,  // hitbox identifier
        Box {},                // box (default constructed; will be generated with updateHitboxes)
        false                  // hitbox is not static
    });
    updateHitboxes(); // generate hitbox
}

void Player::updatePhysics( double dt_s ) noexcept {
    if ( !isStuck )
        addVelocity(glm::vec3(.0f, -GRAVITY_CONSTANT * dt_s, .0f));
    else {
        setVelocity(glm::vec3(.0f));
    }
    move(dt_s);
    for ( auto &blob : blobs ) {
        if ( blob.getIsActive() and !blob.getIsStuck() ) {
            blob.addVelocity(glm::vec3(.0f, -GRAVITY_CONSTANT * dt_s, .0f)); // TODO: refactor into blob
        }
        blob.move(dt_s);
        // blob.updateCollisions(); ?
    }
};

[[nodiscard]]
Sphere const * Player::getSphere() const noexcept {
    return &sphere;
}

void Player::updateInput() noexcept {
    processMouse();
    processKeyboard();
    handleInput();
}

void Player::processMouse() noexcept {
    if (mouse->LeftIsPressed())
        input.isPressed[Input::Key::shoot] = true;
    if ( input.isPressed[Input::Key::shoot] ) {
        input.mousePosition = glm::vec3(mouse->GetXPos(), mouse->GetYPos(), 0);
        shoot( input.mousePosition );
    }
    else input.isPressed[Input::Key::shoot] = false;
}

// TODO:  Map<char,ActionEnum> keybindings
void Player::processKeyboard() noexcept {
    // Movement
    if ( keyboard->KeyIsPressed('D') )
        input.isPressed[Input::Key::right] = true;
    if ( keyboard->KeyIsPressed('A') )
        input.isPressed[Input::Key::left]  = true;
    if ( keyboard->KeyIsPressed('W') )
        input.isPressed[Input::Key::up]    = true;
    if ( keyboard->KeyIsPressed('S') )
        input.isPressed[Input::Key::down]  = true;

    if ( keyboard->KeyIsPressed('B') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == Player::Status::none ) {
                for ( int i = 0; i < blobCharges; ++i ) {
                    blobs[i].status = Blob::Status::bouncy;
                }
                status = Player::Status::bouncy;
                //graphics->setMetaballColorAbsorb(glm::vec3(1.0f, .5f, .25f)); // TODO!
            }
            else {
                for ( int i = 0; i < blobCharges; ++i )
                    blobs[i].status = Blob::Status::none;
                status = Player::Status::none;
                //graphics->setMetaballColorAbsorb(glm::vec3(.85f, .25f, .75f)); // TODO
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('H') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == Status::none) {
                for ( int i = 0; i < blobCharges; ++i ) {
                    blobs[i].status = Blob::Status::heavy;
                }
                status = Player::Status::heavy;
                //graphics.setMetaballColorAbsorb(glm::vec3(0.75, 0.75, 0.75)); // TODO
            }
            else {
                for ( int i = 0; i < blobCharges; ++i )
                    blobs[i].status = Blob::Status::none;
                status = Player::Status::none;
                //graphics.setMetaballColorAbsorb(glm::vec3(0.85, 0.25, 0.75)); // TODO
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('Y') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == Player::Status::none) {
                for ( int i = 0; i < blobCharges; ++i )
                    blobs[i].status = Blob::Status::sticky;
                status = Player::Status::sticky;
                // graphics.setMetaballColorAbsorb(glm::vec3(0.2, 0.2, 0.5));
            }
            else {
                for ( int i = 0; i < blobCharges; ++i )
                    blobs[i].status = Blob::Status::none;
                status = Player::Status::none;
                //graphics.setMetaballColorAbsorb(glm::vec3(0.85, 0.25, 0.75));
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('R') )
        recallBlobs();
    if ( keyboard->KeyIsPressed('P') )
        ;//createShaders(); // TODO
}

void Player::updateRepresentation() noexcept {
    sphere = { position, radius };
}

void Player::updateHitboxes() noexcept {
    auto &hitbox = hitboxes[0].box;
    //level.boxes.push_back(volume); // TODO!
    //hitbox.center      = { position, .0f };
    hitbox.center.x = position.x;
    hitbox.center.y = position.y;
    hitbox.center.z = position.z;
    hitbox.halfLengths = { glm::vec3(radius/2), .0f };
}

Player::~Player() {}

// Updates logic, call once per frame
void Player::updateLogic(double dt_s) noexcept {
	jumpCooldown  -= (float)dt_s;
	shootCooldown -= (float)dt_s;

	if ( isStanding )
		hasExtraJump = true;
    mass = (status == Status::heavy)?  20.0f : 10.0f;

	if ( status != Status::sticky )
		isStuck = false;

	for ( auto &blob : blobs ) 
        blob.update(dt_s);
}

void Player::updateAnimation( double dt_s ) noexcept {
    ; // TODO
}

void Player::die() noexcept {
    ; // TODO
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> Player::getRepresentation() const noexcept {
    Spheres representation;
    representation.push_back( getSphere() );
    for ( auto &blob : blobs )
        representation.push_back( blob.getSphere() );
    return representation;
}

void Player::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {
    auto &hitbox = hitboxes[0].box;

    if ( otherHitbox == ColliderType::platform ) {
        auto &platform = dynamic_cast<Platform&>(other);
        auto &platBox  = platform.getBox();
        auto dtPos = glm::vec3(platBox.center) - position;
        if ( dtPos.x > platBox.halfLengths.x ) { // player is to the left
            hasExtraJump  = true;
            // TODO: y friction?
            velocity.x    = 0;
            position.x    = platform.getBox().center.x
                            - platform.getBox().halfLengths.x
                            - radius/2;
        }
        else if ( dtPos.x < -platBox.halfLengths.x ) { // player is to the right
            hasExtraJump  = true;
            // TODO: y friction?
            velocity.x    = 0;
            position.x    = platform.getBox().center.x
                            + platform.getBox().halfLengths.x
                            + radius/2;
        }
        if ( dtPos.y > platBox.halfLengths.y ) { // player is below
            velocity.y    = 0;
            position.y    = platform.getBox().center.y
                            - platform.getBox().halfLengths.y
                            - radius/2;
        }
        else if ( dtPos.y < -platBox.halfLengths.y ) { // player is above
            isStanding    = true;
            hasExtraJump  = true;
            velocity.y    = 0;
            velocity.x   *= platform.getFriction();
            position.y    = platform.getBox().center.y
                            + platform.getBox().halfLengths.y
                            + radius/2;
        }
            
        if ( false ) { // direction down temp
			isStanding    = true;
			hasExtraJump  = true;
            velocity.y    = 0;
            velocity.x   *= platform.getFriction();
			position.y    = platform.getBox().center.y
                            + platform.getBox().halfLengths.y
                            + radius/2;
        }/*
        else if ( ownHitbox == ColliderType::player_top ) {
            if ( status == Status::sticky ) {
                isStuck = true;
            }
            velocity.y = 0;
            position.y = platBox.center.y
                         - platBox.halfLengths.y
                         + position.y
                         - top.center.y
                         - top.halfLengths.y;
        }
        else if ( ownHitbox == ColliderType::player_left ) {
            if ( status == Status::sticky ) {
                isStuck = true;
            }
            position.x = platBox.center.x
                         + platBox.halfLengths.x
                         + position.x
                         - top.center.x
                         + top.halfLengths.x;
        }
        else if ( ownHitbox == ColliderType::player_right ) {
            if ( status == Status::sticky ) {
                isStuck = true;
            }
            position.x = platBox.center.x
                         - platBox.halfLengths.x
                         + position.x
                         - right.center.x
                         - right.halfLengths.x;
        }*/
    }
    /*
    // TODO: get delta vector between player/enemy, invert, normalize, multiply by knockback factor
    else if ( otherHitbox == ColliderType::blob ) { ///and other.color.w == 1
        auto       &blob       = dynamic_cast<Blob&>(other);
        auto const &blobVolume = blob.getVolume();
        
        if ( ownHitbox == ColliderType::player_bottom ) { 
            if ( status == Status::sticky and !isStuck ) {/// and other.color.w == 1
                isStanding   = true;
                hasExtraJump = true;
                velocity.y   = 0;
                position.y   = blobVolume.center.y
                               + blobVolume.halfLengths.y
                               + position.y
                               - bottom.center.y
                               + bottom.halfLengths.y;
            }
            else if ( status == Status::bouncy ) { /// and other.color.w == 1
                isStanding   = true;
                hasExtraJump = true;
                velocity.y   = 0;
                position.y   = blobVolume.center.y
                               + blobVolume.halfLengths.y
                               + position.y
                               - bottom.center.y
                               + bottom.halfLengths.y;
            }
        }
    }
    // TODO: get delta vector between player/enemy, invert, normalize, multiply by knockback factor
    else if ( otherHitbox == ColliderType::enemy_left ) {
        auto &enemyBox  = dynamic_cast<Enemy&>(other).getHitboxes()[hitboxLeft].box;
        position.x = enemyBox.center.x
                     - enemyBox.halfLengths.x
                     + position.x
                     - right.center.x
                     - right.halfLengths.x;
    }
    else if ( otherHitbox == ColliderType::enemy_right ) {
        auto &enemyBox  = dynamic_cast<Enemy&>(other).getHitboxes()[hitboxRight].box;
        position.x = enemyBox.center.x
                     + enemyBox.halfLengths.x
                     + position.x
                     - left.center.x
                     + left.halfLengths.x;
    }
    */
}

void Player::shoot(glm::vec2 const &mousePos) noexcept {
    if ( shootCooldown > .0f )
        return;
    auto mouseScreenPos = glm::vec3((mousePos.x - 1280 / 2) * 9, (-(mousePos.y - 980 / 2)) * 16, 0); // TODO
    glm::vec3 dir = glm::normalize( mouseScreenPos - position);
    for ( auto &blob : blobs ) {
        if ( !blob.getIsActive() and !blob.getIsBeingRecalled() ) {
            blob.shoot(dir);
            shootCooldown = SHOOT_CD;
            return;
        }
    }
}

void Player::recallBlobs() noexcept {
    for ( auto &blob : blobs ) {
        blob.recall();
    }
    shootCooldown = SHOOT_CD;
}
void Player::handleInput() noexcept {
    if ( input.isPressed[Input::Key::shoot] ) {
        shoot(input.mousePosition);
    }
    if ( input.isPressed[Input::Key::left] ) {
        if ( !isStuck )
            addVelocity(glm::vec3(isStanding? -moveSpeed : (-FLOAT_REDUCTION*moveSpeed), .0f, .0f));
    }
    if ( input.isPressed[Input::Key::right] ) {
        if ( !isStuck )
            addVelocity(glm::vec3(isStanding? moveSpeed : (FLOAT_REDUCTION*moveSpeed), .0f, .0f));
    }
    if ( input.isPressed[Input::Key::up] ) {
        if ( isStanding ) {
            isStanding   = false;
            jumpCooldown = JUMP_CD;
            putForce(glm::vec3(.0f, jumpForce, .0f));
        }
        else if ( status == Status::bouncy
                  and hasExtraJump
                  and jumpCooldown <= .0f )
        {
            hasExtraJump = false;
            jumpCooldown = JUMP_CD;
            velocity.y   = .0f;
            putForce(glm::vec3(.0f, jumpForce, .0f));
        }
    }
    if ( input.isPressed[Input::Key::down] ) {
        if ( status == Status::sticky ) {
            isStuck = false;
            //if ( !isStanding )
            //    ;/// addVelocity(glm::vec3(0.0, -GRAVITY_CONSTANT, 0.0)); // wot?
        }
    }
    // clear input
    for ( auto &b : input.isPressed )
        b = false;
 }


void Game::update( double dt_s )  {
    level->onFrameStart(); // TEMP: clearing boxes & spheres
    Game::time += dt_s; // NOTE: Should be redundant (but would require some revamping)
    // level.player.velocity.x = 0;     // TODO: wtf?
    // level.player.isStanding = false; // TODO: put at end of Player::update()?
    updateInput();
    updateLogic(dt_s);
    updatePhysics();
    updateGraphics();
}

void Game::updateLogic( double dt_s ) {
    for ( auto &e : level->getScene() )
        e->updateLogic(dt_s);
}

// Call first of all per frame updates
void Game::updateInput() {
    level->getPlayer().updateInput();
}

// Catches up the physics simulation time to the actual game time
// Call last of all logic updates (but before graphics)
void Game::updatePhysics() {
	double constexpr timestep = .0015; // TODO: refactor into Globals?
    while ( physicsSimTime < Game::time ) {
        for ( auto &e : level->getScene() ) {
            e->updatePhysics(timestep);
        }
/// TODO: refactor into colManager!
///        for ( auto &e : level.scene ) {
///            e.object->updateCollisions();
///        }
		level->getCollisionManager().update();
        physicsSimTime += timestep;
	}
}

// TODO: SphereHandle gfxManager::requestSphere();
// TODO: BoxHandle    gfxManager::requestBox();

// TODO:
//template <typename Location, class T>
//class Handle {
//public:
//    Handle( T &data, Container &container ): IObject(), data(data), container(container) {}
//    Handle( Handle       && ) = default;
//    Handle( Handle const  & ) = delete;
//    auto operator=( Handle       && ) = delete;
//    auto operator=( Handle const  & ) = delete;
//    virtual ~Handle() noexcept { container.remove(data); }
//    &operator T() { return data; }
//    const &operator T() const { return data; }
//private:
//    T         &data;
//    Container &container;
//};
//template <typename Location, class T>
//Handle<Location,T> make_handle( )

// Call after all other per frame updates
void Game::updateGraphics() { // TODO: refactor into level?
    // TODO: make it update elements instead of repopulate vectors?
    for ( auto const &e : level->getScene() ) {
        e->updateRepresentation();
        auto const &gfx = e->getRepresentation();
        if ( std::holds_alternative<IRepresentable::Boxes>(gfx) )
            for ( auto const *p : std::get<IRepresentable::Boxes>(gfx) )
                level->getBoxes().push_back( *p );
        else if ( std::holds_alternative<IRepresentable::Spheres>(gfx) )
            for ( auto const *p : std::get<IRepresentable::Spheres>(gfx) )
                level->getSpheres().push_back( *p );
    }
    if constexpr ( SHOW_HITBOXES )
        showHitboxes();

//	level.boxes.push_back( groundBox.hitbox );
//	level.boxes.push_back(  testPlat.hitbox );
//	level.boxes.push_back( testplat2.hitbox );

    // TODO: for ( auto &e : level.scene )
    //      e.updateGraphics();

//    for ( auto &enemy : level.enemies ) {
//        Box eBox;
//        eBox.center      = { enemy.position, .0f };
//        eBox.halfLengths = enemyBox.halfLengths;
//        eBox.color       = glm::vec4((float)enemy.isStanding, 1.0 - (float)enemy.isStanding, 0.0, 0.0);
//		level.boxes.push_back(eBox);
//    }
//
//
//	level.spheres = vector<Sphere>();
//	playerSphere.centerRadius = glm::vec4( level.player.pos, level.player.radius );
//	level.spheres.push_back(playerSphere);
//
//	glm::vec2 animationSpeed = glm::smoothstep(-150.0f, 150.0f, glm::vec2(level.player.velocity.x, level.player.velocity.y));
//	animateSphere(playerSphere, animationSpeed, glm::vec3(3.0, 3.0, 0.5));
//
//	for (int i = 0; i < level.player.blobs.size(); i++) {
//		level.spheres.push_back(level.player.blobs[i].blobSphere);
//	}
//
//    level.boxes.push_back(level.goal->representation);

	//showHitboxes();
}

void Game::showHitboxes() {
    for ( auto const &e : level->getScene() )
        for ( auto const &hitbox : e->getHitboxes() )
            level->getBoxes().push_back(hitbox.box);
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres>
Enemy::getRepresentation() const noexcept {
    return Boxes{ getVolume() };
}

// TODO!!!!
Enemy::Enemy(glm::vec3 position):
    IActor( false,   // isStanding
            950.0f,  // jumpForce
            JUMP_CD, // jumpCooldown
            10.0f,   // mass
            20.0f,   // moveSpe:ed
            position
    ),      // center      h-lenghts  colour
    volume( {{position,0}, {3,3,3,0}, {0,1,0,0} } )
{
    // register hitboxex:
    hitboxes.push_back({
        (ICollider*)this,     // hitbox parent
        ColliderType::enemy,  // hitbox identifier
        Box {},               // box (default constructed; will be generated with updateHitboxes)
        false                 // hitbox is not static
    });
    updateHitboxes(); // generate hitbox
}

Enemy::~Enemy() {}

void Enemy::die() noexcept {
    ;// TODO: level->remove(*this); ?
}

void Enemy::updatePhysics( double dt_s ) noexcept {
    addVelocity(glm::vec3(.0f, -GRAVITY_CONSTANT * dt_s, .0f));
    move(dt_s);
};

void Enemy::updateRepresentation() noexcept {
    volume.center = { position, .0f };
}

void Enemy::updateHitboxes() noexcept {
    auto &hitbox = hitboxes[0].box;
    //level.boxes.push_back(volume); // TODO!
    hitbox.center      = { position, .0f };
    hitbox.halfLengths = { getVolume()->halfLengths };
}

void Enemy::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {
	if ( otherHitbox == ColliderType::platform ) {
        auto &platform = dynamic_cast<Platform&>(other);

		if ( true ) {  // direction down TEMP
            isStanding    = true;
            velocity.y    = 0;
            velocity.x   *= platform.getFriction();
            position.y    = platform.getBox().center.y
                            + platform.getBox().halfLengths.y
                            + getVolume()->halfLengths.y;
		}
        /*
		else if ( ownHitbox == ColliderType::enemy_top ) {
            velocity.y = 0;
			position.y = platBox.center.y
                         - platBox.halfLengths.y
                         + position.y
                         - top.center.y
                         - top.halfLengths.y;
		}
		else if ( ownHitbox == ColliderType::enemy_left ) {
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}
		else if ( ownHitbox == ColliderType::enemy_right ) {
			controlDir.x = -controlDir.x;
			velocity.x   = -velocity.x;
		}*/
	}
    /*
	else if ( otherHitbox == ColliderType::player_bottom and ownHitbox == ColliderType::enemy_top )
        die();
    */
}

// Updates logic, call once per frame before updatePhysics
void Enemy::updateLogic(double dt_s) noexcept {
//	velocity.y   -= float(GRAVITY_CONSTANT) * (float)dt_s;
	jumpCooldown -= (float)dt_s;
	if ( !isStanding ) {
		if ( jumpCooldown <= .0f ) {
			controlDir.x = -controlDir.x;
			jumpCooldown = JUMP_CD;
			putForce( glm::vec3(.0f, jumpForce, .0f) );
		}
	}
	addVelocity( controlDir * moveSpeed );
	isStanding = false;
}

void Enemy::updateAnimation( double dt_s ) noexcept {
    ; // stub
}

//Adds two orbiting spheres around a sphere for animation
void Game::animateSphere(Sphere const &sphere, glm::vec2 const &moveSpeed, glm::vec3 const &amplitude) {
	glm::vec3 rotationSpeed = glm::vec3( .81f, .53f, .1f );
	// Offset the start rotation of the spheres to avoid them all starting at the same place
	glm::vec3 offset = glm::vec3(.2f, .0f, .0f);
	// Multiplier to animate faster when moving a certain direction. Not smooth.
	glm::vec2 movementMultiplier = glm::vec2(
		glm::clamp( abs(moveSpeed.x) + .0f, .0f, 2.0f ),
		glm::clamp( abs(moveSpeed.y) + .0f, .0f, 2.0f )
	);

	Sphere sphere1(glm::vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * abs(moveSpeed.x)) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * abs(moveSpeed.y)) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	level->getSpheres().push_back(sphere1);

	rotationSpeed = -rotationSpeed;
	offset = glm::vec3(1.45f, .9f, 1.1f);
	Sphere sphere2(glm::vec4(
		sphere.centerRadius.x + sin(float(time) * (rotationSpeed.x * abs(moveSpeed.x)) + offset.x) * amplitude.x,
		sphere.centerRadius.y + sin(float(time) * (rotationSpeed.y * abs(moveSpeed.y)) + offset.y) * amplitude.y,
		sphere.centerRadius.z + sin(float(time) * rotationSpeed.z + offset.z) * amplitude.z,
		sphere.centerRadius.w / 2
	));
	level->getSpheres().push_back(sphere2);
}


// when to use: when loading a level or dynamically spawning entities in a level.
//
// usage:
//            LevelData level = /*...*/;
//            // ...
//
//            // either:
//            auto enemy = std::make_unique<Enemy>(/*some_constructor_params*/);
//            level.add( {10.0f, .0f, .0f}, std::move(enemy) }; // transfer ownership to level
//
//            // or:
//            level.add( {10.0f, .0f, .0f}, std::make_unique<Enemy>(/*some_constructor_params*/) );
//            
void Level::add(std::unique_ptr<IObject> object) noexcept {
    Player *player = dynamic_cast<Player*>(object.get());
    if ( player ) {
        if ( this->player )
            assert( false and "The level already contains a player!" );
        else this->player = player;
    }
    collisionManager.add( *object );
    scene.push_back({ std::move(object) });
}

Level::Level() {}

void Level::onFrameStart() {
    // TODO: let IRepresentables have mutable handles to a GraphicsManager instead!
    boxes.clear();
    spheres.clear();
}

// when to use: when a level in an entity is destroyed by game logic (e.g. an enemy dies)
//
// usage:
//     level.remove( *this );
bool Level::remove( IObject &target ) noexcept {
    // TODO: handle Player edge case
    auto unary_predicate = [&target]( auto const &e ) { 
        return *e == target;
    };
    return std::remove_if( scene.begin(), scene.end(), unary_predicate ) != scene.end() 
           or collisionManager.remove(target);
}

Player& Level::getPlayer() noexcept {
    return *player;
}

CollisionManager & Level::getCollisionManager() noexcept {
    return collisionManager;
}

Level::Boxes & Level::getBoxes() noexcept {
    return boxes;
}

Level::Spheres & Level::getSpheres() noexcept {
    return spheres;
}

Level::SceneEntries & Level::getScene() noexcept {
    return scene;
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> LevelGoal::getRepresentation() const noexcept {
    return Boxes{ &representation };
}

void LevelGoal::updateRepresentation()       noexcept {}; // stub
void LevelGoal::updateHitboxes()             noexcept {}; // stub
void LevelGoal::updateLogic(double dt_s)     noexcept {}; // stub
void LevelGoal::updatePhysics(double dt_s)   noexcept {}; // stub
void LevelGoal::updateAnimation(double dt_s) noexcept {}; // stub

LevelGoal::LevelGoal( glm::vec3 const &position, float radius, TriggerCallback cb ):
    representation({ glm::vec4(position,0), {2.0f,5.0f,2.0f,.0f}, {1.0f,.0f,1.0f,.0f} }),
    triggerCallback(cb)
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,         // hitbox parent
        ColliderType::blob,       // hitbox identifier
        Box {                     // hitbox box
            {position,.0f},            // box position
            {radius,radius,radius,.0f},// box volume
            {}                         // box color
        },
        false                     // hitbox is not static
    });
///    _colMan->add({&*this, ColliderType::level_goal, _bounds, true});  // TODO: ICollider::getHitboxes() + Level::add()
}

ICollider::Hitboxes const & LevelGoal::getHitboxes() const noexcept {
    return hitboxes;
}

LevelGoal::~LevelGoal() {}

void LevelGoal::collide( ColliderType,
                         ColliderType  otherHitbox,
                         IUnique& ) noexcept {
    if ( otherHitbox == ColliderType::player )
        triggerCallback();
    // TODO (in callback):
    ///    tone and blur screen?
    ///    display text?
    ///    wait for input?
    ///    load next level?
}
