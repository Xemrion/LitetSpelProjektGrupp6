#include "Blob.h"
#include <algorithm>

Blob::Blob( glm::vec3 const &parentPosition ):
    parentPosition  ( &parentPosition ),
    pos             (  parentPosition ),
    blobSphere      ({ glm::vec4(pos,radius) }),
    hitbox          ({ glm::vec4(pos,0), glm::vec4(radius,radius,radius,0), glm::vec4(.5, .5, 1.0, .0) }),
    isActive        (false),
    isBeingRecalled (true),
	isStuck         (false),
    recallSpeed     (200.0f),
    speed           (100.1f),
    radius          (  2.0f),
	status          (BlobStatus::Blob_None),
    velocity        (   .0f)
{
	deactivateHitbox();
}

void Blob::absorb() noexcept
{
    isBeingRecalled = false;
    isActive        = false;
}

void Blob::deactivateHitbox() noexcept {
	hitbox.color.w = 0;
}

void Blob::reactivateHitbox() noexcept {
	if (status == BlobStatus::Blob_Heavy)
	{
		hitbox.color.w = 0.25;
	}
	else if (status == BlobStatus::Blob_Sticky)
	{
		hitbox.color.w = 0.5;
	}
	else
	{
		hitbox.color.w = 1;
	}

}

void Blob::shoot( glm::vec3 const &direction ) noexcept
{
    if ( !isActive && !isBeingRecalled ) {
        reactivateHitbox();
        isActive = true;
        velocity = (status == BlobStatus::Blob_Heavy)?  direction * speed/3.0f  :  direction * speed;
	}
}

void Blob::recall() noexcept
{
    deactivateHitbox();
    isBeingRecalled = true;
}

bool Blob::getIsActive()        const noexcept { return isActive; }

bool Blob::getIsBeingRecalled() const noexcept { return isBeingRecalled; }

bool Blob::getIsStuck() const noexcept { return isStuck; }

// called once per frame from Player::update
void Blob::update(double dt) noexcept
{
    #undef min
    if ( isBeingRecalled ) {
        float speed          = std::min( recallSpeed * float(dt), glm::distance(pos, *parentPosition) );
        glm::vec3 direction  = glm::normalize( *parentPosition - pos );
		pos                 += speed * direction;
		isStuck              = false;
	}
    if (!isActive) {
        pos = *parentPosition + glm::vec3(0.0, 2.0, 0.0);
        setVelocity(glm::vec3(0.0));
        blobSphere.centerRadius = glm::vec4(*parentPosition, 2);
    }
	if (isStuck == true && status != BlobStatus::Blob_Sticky) 
	{
		isStuck = false;
	}
	if (!isActive) 
	{
		deactivateHitbox();
	}
	blobSphere.centerRadius = glm::vec4( pos, radius );
}

// Call from updatePhysics
void Blob::move(double dt) noexcept
{
	pos           += velocity * float(dt);
	hitbox.center  = glm::vec4( pos, 0.0 );
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::setVelocity(glm::vec3 const &velocity, bool useSpeed) noexcept
{
	if (useSpeed) {
		float speedMultiplier = isBeingRecalled ? recallSpeed : speed;
		this->velocity        = velocity * speedMultiplier;
	}
	else this->velocity = velocity;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::addVelocity(glm::vec3 const &velocity, bool useSpeed) noexcept
{
	if (useSpeed) {
		float speedMultiplier  = isBeingRecalled ? recallSpeed : speed;
		this->velocity        += velocity * speedMultiplier;
	}
	else this->velocity += velocity;
}

void Blob::collide(ColliderType ownType, ColliderType otherType, Box const &other) noexcept
{
    if ( isBeingRecalled and(
         otherType == ColliderType::player ))
    {
        absorb();
    }

	if ( hitbox.color.w != 0 and otherType == ColliderType::platform) {
		if (status == BlobStatus::Blob_Bouncy) 
		{
			this->velocity.y = -this->velocity.y;
			this->velocity.x = 0;
		}
		else if (status == BlobStatus::Blob_Sticky) 
		{
			this->velocity = glm::vec3(0.0);
			isStuck = true;
		}
		else 
		{
			this->velocity = glm::vec3(0.0);
		}
		glm::vec3 pushUp    = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown  = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft  = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY  = glm::length(pushUp)   < glm::length(pushDown)  ? pushUp   : pushDown;
		glm::vec3 minDistX  = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		pos                += glm::length(minDistY) < glm::length(minDistX)  ? minDistY : minDistX;
	}
	else if (isActive and otherType == ColliderType::enemy)
	{
		if (pos.x < (other.center.x + other.halfLengths.x) or pos.x >(other.center.x - other.halfLengths.x))
		{
			if (status == BlobStatus::Blob_Bouncy)
			{
				this->velocity.y = -this->velocity.y;
				this->velocity.x = 0;
			}
			else if (status == BlobStatus::Blob_Sticky)
			{
				this->velocity = glm::vec3(0.0);
				isStuck = true;
			}
			else
			{
				this->velocity = glm::vec3(0.0);
			}
			glm::vec3 pushUp = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}
		else if (pos.x > (other.center.x + other.halfLengths.x) or pos.x < (other.center.x - other.halfLengths.x))
		{
			if (status == BlobStatus::Blob_Sticky)
			{
				this->velocity = glm::vec3(0.0);
				isStuck = true;
			}
			glm::vec3 pushUp = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}	
	}
}
