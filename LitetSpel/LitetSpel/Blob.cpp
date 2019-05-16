#include <algorithm>
#include "Blob.h"
#include "Platform.h"

// The unused fields can be removed by inheriting from an
// intermediate class (IMobile, situated between IObject and IActor)
// instead of IMobile. TODO.
Blob::Blob( glm::vec3 const &parentPosition ):
    IActor( false,         // unused
            .0f,           // unused
            .0f,           // unused
            .0f,           // unused
            100.0f,        // moveSpeed
            parentPosition // position
    ),
    parentPosition  ( &parentPosition                  ),
    blobSphere      ( glm::vec4(parentPosition,radius) ),
    isActive        ( false                            ),
    isBeingRecalled ( true                             ),
	isStuck         ( false                            ),
    recallSpeed     ( 200.0f                           ),
    radius          (   2.0f                           ),
	status          ( Status::none                     )
{
    // register hitbox:
    hitboxes.push_back({
         (ICollider*)this,         // hitbox parent
         ColliderType::blob,       // hitbox identifier
         Box {                     // hitbox box
            glm::vec4{position,0},              // box position
            glm::vec4{radius,radius,radius,0},  // box volume
            glm::vec4{.5, .5, 1.0, .0}          // box color
         },
         false                     // hitbox is not static
    });
	deactivateHitbox();
}

void Blob::absorb() noexcept {
    isBeingRecalled = false;
    isActive        = false;
}

void Blob::deactivateHitbox() noexcept {
	hitboxes[0].box.color.w = 0; // temp hack; TODO: find cleaner solution
}

void Blob::reactivateHitbox() noexcept {
	hitboxes[0].box.color.w = 1; // temp hack; TODO: find cleaner solution
}

void Blob::shoot( glm::vec3 const &direction ) noexcept {
    if ( !isActive and !isBeingRecalled ) {
        reactivateHitbox();
        isActive = true;
        velocity = (status == Status::heavy)?  direction * speed/3.0f  :  direction * speed;
	}
}

void Blob::recall() noexcept {
    deactivateHitbox();
    isBeingRecalled = true;
}

bool Blob::getIsActive()        const noexcept { return isActive; }

bool Blob::getIsBeingRecalled() const noexcept { return isBeingRecalled; }

bool Blob::getIsStuck() const noexcept { return isStuck; }

// called once per frame from Player::update
void Blob::update(double dt) noexcept {
    #undef min
    if ( isBeingRecalled ) {
        float speed          = std::min( recallSpeed * (float)dt,
                                         glm::distance(position, *parentPosition) );
        glm::vec3 direction  = glm::normalize( *parentPosition - position );
        position            += speed * direction;
		isStuck              = false;
	}
    if ( !isActive ) {
        position = *parentPosition + glm::vec3(.0f, 2.0f, .0f);
        setVelocity(glm::vec3(0.0));
        blobSphere.centerRadius = glm::vec4(*parentPosition, 2.0f);
        deactivateHitbox();
    }
	if  ( isStuck and status != Status::sticky) {
		isStuck = false;
	}
	blobSphere.centerRadius = glm::vec4( position, radius );
}

void Blob::updateHitboxes() noexcept {
    hitboxes[0].box.center = glm::vec4( position, .0f );
}

void Blob::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {
    if ( isBeingRecalled and otherHitbox == ColliderType::player )
        absorb();
    if ( !isActive )
        return; // early exit
    else if ( otherHitbox == ColliderType::platform ) {
        auto &platBox     = dynamic_cast<Platform&>(other).getBox();
        auto const &myBox = hitboxes[0].box;
        if (status == Status::sticky) {
            velocity            = glm::vec3(0.0);
            isStuck = true;
        }
        else if (status == Status::bouncy) {
            velocity.y          = -velocity.y;
            velocity.x          = 0;
        }
        else {
            velocity            = glm::vec3(0.0);
        }
        glm::vec3 pushUp    = glm::vec3(0.0, platBox.center.y + platBox.halfLengths.y + (-myBox.center.y + myBox.halfLengths.y), 0.0);
        glm::vec3 pushDown  = glm::vec3(0.0, platBox.center.y - platBox.halfLengths.y + (-myBox.center.y - myBox.halfLengths.y), 0.0);
        glm::vec3 pushRight = glm::vec3(platBox.center.x + platBox.halfLengths.x + (-myBox.center.x + myBox.halfLengths.x), 0.0, 0.0);
        glm::vec3 pushLeft  = glm::vec3(platBox.center.x - platBox.halfLengths.x + (-myBox.center.x - myBox.halfLengths.x), 0.0, 0.0);
        glm::vec3 minDistY  = glm::length(pushUp)   < glm::length(pushDown)  ? pushUp   : pushDown;
        glm::vec3 minDistX  = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
        position           += glm::length(minDistY) < glm::length(minDistX)  ? minDistY : minDistX;
    }
}
