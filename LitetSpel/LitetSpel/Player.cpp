#include "Player.h"
#include "Platform.h"
#include "Gate.h"
#include "Button.h"

Player::Player( Graphics         &graphics,
                CollisionManager &colMan,
                glm::vec3         position={.0f, .0f, .0f} ): // TODO: const ref or value+move
    IActor( false, // isStanding
            PLAYER_JUMP_FORCE,
            JUMP_CD,
            PLAYER_MASS,
            PLAYER_SPEED,
            position
    ),
    colMan        ( &colMan         ), // hacky evil to deal with blob registration
    graphics      ( &graphics       ),
    hasExtraJump  ( true            ),
    isStuck       ( false           ),
    hasWon        ( false           ),
    status        ( PowerType::none ),
    blobCharges   ( 5               ),
    shootCooldown ( SHOOT_CD        ),
    powerCooldown ( POWER_CD        ),
    radius        ( 5.0f            )
{
    blobs.reserve( blobCharges );
    assert( this->graphics and "Graphics mustn't be null!" );
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
    for ( auto &blob : blobs )
        blob.updatePhysics( dt_s );
};

[[nodiscard]]
Sphere const * Player::getSphere() const noexcept {
    return &blobSphere;
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
    if ( keyboard->KeyIsPressed('D') ) {
        input.isPressed[Input::Key::right] = true;
    }
    if ( keyboard->KeyIsPressed('A') ) {
        input.isPressed[Input::Key::left]  = true;
    }
    if ( keyboard->KeyIsPressed('W') ) {
        input.isPressed[Input::Key::up]    = true;
    }
    if ( keyboard->KeyIsPressed('S') ) {
        input.isPressed[Input::Key::down]  = true;
    }

    // TODO: trim code duplication in blobs branches
    if ( keyboard->KeyIsPressed('B') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == PowerType::none ) {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::bouncy;
                }
                status = PowerType::bouncy;
            }
            else {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::none;
                }
                status = PowerType::none;
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('H') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == PowerType::none) {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::heavy;
                }
                status = PowerType::heavy;
            }
            else {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::none;
                }
                status = PowerType::none;
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('Y') ) {
        if ( powerCooldown <= .0f ) {
            if ( status == PowerType::none) {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::sticky;
                }
                status = PowerType::sticky;
            }
            else {
                for ( auto &blob : blobs ) {
                    blob.status = PowerType::none;
                }
                status = PowerType::none;
            }
            powerCooldown = POWER_CD;
        }
    }
    if ( keyboard->KeyIsPressed('R') )
        recallBlobs();
}

void Player::updateGraphics() noexcept {
    if ( !hasWon ) {
        auto color = PowerTypeColor[ static_cast<size_t>(status)];
        animSphere1.color = color;
        animSphere2.color = color;
        blobSphere.color  = color;
    }
    
    blobSphere.centerRadius = {position, radius};
    for ( auto &blob : blobs )
        blob.updateGraphics();
 
}

// TODO: refactor blob code
void Player::updateHitboxes() noexcept {
    // account for charge count decreasing
    while ( hitboxes.size() > blobCharges ) {
        blobs.pop_back();
        // TODO: unregister
    }
    // account for charge count increasing
    while ( blobs.size() != blobCharges ) {
        blobs.push_back( Blob(position) );
        colMan->add( *(blobs.end()-1) );
    }
    assert(  blobs.size() == blobCharges );
    auto &hitbox = hitboxes[0].box;
    hitbox.center = glm::vec4( position, .0f );
    hitbox.halfLengths = { glm::vec3(radius/2), .0f };
}

Player::~Player() {}

// Updates logic, call once per frame
void Player::updateLogic(double dt_s) noexcept {
    jumpCooldown  -= (float)dt_s;
    shootCooldown -= (float)dt_s;
    powerCooldown -= (float)dt_s;

    if ( isStanding )
        hasExtraJump = true;
    mass = (status == PowerType::heavy)?  20.0f : 10.0f;

    if ( status != PowerType::sticky )
        isStuck = false;

    for ( auto &blob : blobs ) 
        blob.updateLogic( dt_s );
}

void Player::updateAnimations( double dt_s, double t_s ) noexcept {
    if ( hasWon ) {
        animateVictory( dt_s, t_s );
        return;
    }

    for ( auto &blob : blobs )
        blob.updateAnimations( dt_s, t_s );
    // move speed
    auto movement = glm::smoothstep(-150.0f, 150.0f, glm::vec2(velocity) );
    glm::vec3 rotationSpeed = { .81f, .53f, .1f };
    // Offset the start rotation of the spheres to avoid them all starting at the same place
    glm::vec3 offset = { .2f, .0f, .0f };
    // Multiplier to animate faster when moving a certain direction. Not smooth.
    glm::vec2 movementMultiplier = { glm::clamp( abs(movement.x) + .0f, .0f, 2.0f ),
                                     glm::clamp( abs(movement.y) + .0f, .0f, 2.0f ) };

    auto timeFactor1 = (float)sin(t_s);
    animSphere1.centerRadius = {
        blobSphere.centerRadius.x + (timeFactor1 * (rotationSpeed.x * abs(movement.x)) + offset.x) * BLOB_ANIM_AMPLITUDE::X,
        blobSphere.centerRadius.y + (timeFactor1 * (rotationSpeed.y * abs(movement.y)) + offset.y) * BLOB_ANIM_AMPLITUDE::Y,
        blobSphere.centerRadius.z + (timeFactor1 *  rotationSpeed.z                    + offset.z) * BLOB_ANIM_AMPLITUDE::Z,
        blobSphere.centerRadius.w / 2
    };

    auto timeFactor2 = (float)cos(t_s);
    rotationSpeed = -rotationSpeed;
    offset = glm::vec3(1.45f, .9f, 1.1f);
    animSphere2.centerRadius = {
        blobSphere.centerRadius.x + (timeFactor2 * (rotationSpeed.y * abs(movement.x)) + offset.x) * BLOB_ANIM_AMPLITUDE::X,
        blobSphere.centerRadius.y + (timeFactor2 * (rotationSpeed.x * abs(movement.y)) + offset.y) * BLOB_ANIM_AMPLITUDE::Y,
        blobSphere.centerRadius.z + (timeFactor2 *  rotationSpeed.z                    + offset.z) * BLOB_ANIM_AMPLITUDE::Z,
        blobSphere.centerRadius.w / 2
    };
}

void Player::die() noexcept {
    ; // TODO
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres>
Player::getRepresentation() const noexcept {
    Spheres representation;
    representation.push_back( &blobSphere  );
    representation.push_back( &animSphere1 );
    representation.push_back( &animSphere2 );
    for ( auto &blob : blobs )
        representation.push_back( blob.getSphere() );
    return representation;
}

void Player::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {
    auto &hitbox = hitboxes[0].box;

    Box const *collidedBox = nullptr;
    float      friction    = DEFAULT_FRICTION;

    if ( otherHitbox == ColliderType::platform ) {
        auto &platform = dynamic_cast<Platform&>( other );
        collidedBox = &platform.getBox();
        friction    =  platform.getFriction();
    }

    if ( otherHitbox == ColliderType::gate ) {
        auto &gate = dynamic_cast<Gate&>( other );
        collidedBox = &gate.getHitboxes()[0].box;
    }

    if ( otherHitbox == ColliderType::button ) {
        auto &button = dynamic_cast<Button&>( other );
        collidedBox = &button.getHitboxes()[0].box;
    }

    if ( collidedBox ) {
        auto dtPos = glm::vec3(collidedBox->center) - position;

        if ( dtPos.x > collidedBox->halfLengths.x ) { // player is to the left
            hasExtraJump  = true;
            // TODO: y friction?
            velocity.x    = 0;
            position.x    = collidedBox->center.x
                            - collidedBox->halfLengths.x
                            - radius/2;
        }
        else if ( dtPos.x < -collidedBox->halfLengths.x ) { // player is to the right
            hasExtraJump  = true;
            // TODO: y friction?
            velocity.x    = 0;
            position.x    = collidedBox->center.x
                            + collidedBox->halfLengths.x
                            + radius/2;
        }
        if ( dtPos.y > collidedBox->halfLengths.y ) { // player is below
            velocity.y    = 0;
            position.y    = collidedBox->center.y
                            - collidedBox->halfLengths.y
                            - radius/2;
        }
        else if ( dtPos.y < -collidedBox->halfLengths.y ) { // player is above
            isStanding    = true;
            hasExtraJump  = true;
            velocity.y    = 0;
            velocity.x   *= friction;
            position.y    = collidedBox->center.y
                            + collidedBox->halfLengths.y
                            + radius/2;
        }
    }
/*
         else if ( ownHitbox == ColliderType::player_top ) {
         if ( status == PowerType::sticky ) {
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
         if ( status == PowerType::sticky ) {
         isStuck = true;
         }
         position.x = platBox.center.x
         + platBox.halfLengths.x
         + position.x
         - top.center.x
         + top.halfLengths.x;
         }
         else if ( ownHitbox == ColliderType::player_right ) {
         if ( status == PowerType::sticky ) {
         isStuck = true;
         }
         position.x = platBox.center.x
         - platBox.halfLengths.x
         + position.x
         - right.center.x
         - right.halfLengths.x;
         }*/

    /*
    // TODO: get delta vector between player/enemy, invert, normalize, multiply by knockback factor
    else if ( otherHitbox == ColliderType::blob ) { ///and other.color.w == 1
    auto       &blob       = dynamic_cast<Blob&>(other);
    auto const &blobVolume = blob.getVolume();

    if ( ownHitbox == ColliderType::player_bottom ) { 
    if ( status == PowerType::sticky and !isStuck ) {/// and other.color.w == 1
    isStanding   = true;
    hasExtraJump = true;
    velocity.y   = 0;
    position.y   = blobVolume.center.y
    + blobVolume.halfLengths.y
    + position.y
    - bottom.center.y
    + bottom.halfLengths.y;
    }
    else if ( status == PowerType::bouncy ) { /// and other.color.w == 1
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
    glm::vec3 dir = glm::normalize( mouseScreenPos - position );
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
        if ( hasWon )
            changeLevelFunction();
        else shoot(input.mousePosition);
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
        else if ( status == PowerType::bouncy
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
        if ( status == PowerType::sticky ) {
            isStuck = false;
            //if ( !isStanding )
            //    ;/// addVelocity(glm::vec3(0.0, -GRAVITY_CONSTANT, 0.0)); // wot?
        }
    }
    // clear input
    for ( auto &b : input.isPressed )
        b = false;
}

void Player::animateColor( double t_s ) {
    auto getColor = [&t_s]( double offset ) {
        return glm::vec4(  sin(float(t_s+offset)),
                          -sin(float(t_s+offset)),
                           cos(float(t_s+offset)),
                           1.0f );
    };
    double offset = 0.0;
    blobSphere.color  = getColor( offset );
    for ( auto &blob : blobs )
        blob.setColor(  getColor( offset += 1.0 ) );
    animSphere1.color = getColor( offset += 3.0 );
    animSphere2.color = getColor( offset += 5.0 );
}

// Code duplication yet again... ffs.
void Player::animateVictory( double dt_s, double t_s ) noexcept {
    float distance      = 3.6f;
    auto  rotationSpeed = glm::vec3{ 4.8f, 5.3f, .9f };
    auto  offset        = glm::vec3( distance );

    { // blob 1
        auto timeSin = (float)sin(5.f * t_s);
        auto timeCos = (float)cos(5.f * t_s);
        animSphere1.centerRadius = {                                                                                 
            blobSphere.centerRadius.x + (distance * timeCos),
            blobSphere.centerRadius.y + (distance * timeSin),
            blobSphere.centerRadius.z + (distance * (timeSin+timeCos)/2),
            blobSphere.centerRadius.w / (3.f + .6f * (timeSin + .7f))
        };
    }

    { // blob 2
        auto timeSin = (float)sin(7.f * t_s + 3.7f);
        auto timeCos = (float)cos(7.f * t_s + 3.7f);
        animSphere2.centerRadius = {                                                                                 
            blobSphere.centerRadius.x - (distance * timeSin),
            blobSphere.centerRadius.y - (distance * timeCos),
            blobSphere.centerRadius.z - (distance * (timeSin+timeCos)/2),
            blobSphere.centerRadius.w / (2.f + .3f * (timeCos + .5f))
        };
    }

    animateColor( t_s );
}

void Player::win( std::function<void(void)> f ) {
    assert( f and "Invalid f; must not be null!" );
    hasWon = true;
    changeLevelFunction = f;
}
