#include "Enemy.h"
#include "Platform.h"

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

void Enemy::updateGraphics() noexcept {
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

void Enemy::updateAnimations( double dt_s, double t_s ) noexcept {
    ; // stub
}
