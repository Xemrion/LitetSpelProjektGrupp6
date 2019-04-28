#include "Blob.h"
#include <algorithm>

Blob::Blob( glm::vec3 const &parentPosition ):
    parentPosition  ( &parentPosition ),
    pos             (  parentPosition ),
    blobSphere      ({ glm::vec4(pos,radius) }),
    hitbox          ({ glm::vec4(pos,0), glm::vec4(radius,radius,radius,0), glm::vec4(.5, .5, 1.0, .0) }),
    isActive        (false),
    isBeingRecalled (true),
    recallSpeed     (200.0f),
    speed           (100.1f),
    radius          (  2.0f),
    velocity        (   .0f)
{}

void Blob::absorb() noexcept
{
    isBeingRecalled = false;
    isActive        = false;
}

void Blob::shoot( glm::vec3 const &direction ) noexcept
{
    if ( !isActive && !isBeingRecalled ) {
        isActive = true;
        velocity = direction * speed;
    }
}

void Blob::recall() noexcept
{
    isBeingRecalled = true;
}

bool Blob::getIsActive()        const noexcept { return isActive; }
bool Blob::getIsBeingRecalled() const noexcept { return isBeingRecalled; }

// called once per frame from Player::update
void Blob::update(double dt)
{
    #undef min
    if ( isBeingRecalled ) {
        float speed          = std::min( recallSpeed * float(dt), glm::distance(pos, *parentPosition) );
        glm::vec3 direction  = glm::normalize( *parentPosition - pos );
		pos                 += speed * direction;
	}
    if (!isActive) {
        pos = *parentPosition + glm::vec3(0.0, 2.0, 0.0);
        setVelocity(glm::vec3(0.0));
        blobSphere.centerRadius = glm::vec4(*parentPosition, 2);
    }
	blobSphere.centerRadius = glm::vec4( pos, radius );
}

// Call from updatePhysics
void Blob::move(double dt)
{
	pos           += velocity * float(dt);
	hitbox.center  = glm::vec4( pos, 0.0 );
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::setVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		float speedMultiplier = isBeingRecalled ? recallSpeed : speed;
		velocity              = velocity * speedMultiplier;
	}
	else this->velocity = velocity;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::addVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		float speedMultiplier  = isBeingRecalled ? recallSpeed : speed;
		this->velocity        += velocity * speedMultiplier;
	}
	else this->velocity += velocity;
}

void Blob::collide(ColliderType ownType, ColliderType otherType, Box other)
{
    if ( isBeingRecalled and (
         otherType == ColliderType::player_bottom ||
         otherType == ColliderType::player_top    ||
         otherType == ColliderType::player_left   ||
         otherType == ColliderType::player_right  ))
    {
        absorb();
    }

	if ( isActive and otherType == ColliderType::platform) {
		this->velocity      = glm::vec3(0.0);
		glm::vec3 pushUp    = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown  = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft  = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY  = glm::length(pushUp)   < glm::length(pushDown)  ? pushUp   : pushDown;
		glm::vec3 minDistX  = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		pos                += glm::length(minDistY) < glm::length(minDistX)  ? minDistY : minDistX;
	}
}
