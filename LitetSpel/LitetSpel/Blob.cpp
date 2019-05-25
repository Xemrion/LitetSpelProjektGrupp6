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
    recallSpeed     (400.0f),
    speed           (100.1f),
    radius          (  2.5f),
	status          (BlobStatus::Blob_None),
    velocity        (   .0f)
{
	deactivateHitbox();

	int hashVal = (int)this;
	offsetFromParent = (glm::fract(glm::vec3(sin(hashVal) * 123987.f, sin(hashVal) * 97623.f, sin(hashVal) * 8911.f)) * 2.0f - 1.0f) * glm::vec3(2.0, 2.0, 1.0);
}

void Blob::absorb() noexcept
{
	if (isActive != false && isBeingRecalled != false) {
		gameSounds->PlayAbsorbSound01();
	}
    isBeingRecalled = false;
    isActive        = false;
	isStuck			= false;
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
        isActive = true;
		pos = *parentPosition;
        velocity = (status == BlobStatus::Blob_Heavy)?  direction * speed/3.0f  :  direction * speed;
	}
}

void Blob::recall() noexcept
{
	if (isActive) {
		deactivateHitbox();
		isBeingRecalled = true;
	}
}

bool Blob::getIsActive()        const noexcept { return isActive; }

bool Blob::getIsBeingRecalled() const noexcept { return isBeingRecalled; }

bool Blob::getIsStuck() const noexcept { return isStuck; }

// called once per frame from Player::update
void Blob::update(double dt) noexcept
{
    #undef min
	collidingMovingPlatform = nullptr;

    if ( isBeingRecalled ) 
	{
        float speed          = std::min( recallSpeed * float(dt), glm::distance(pos, *parentPosition) );
        if (glm::length(*parentPosition - pos) > 0.0) {
			glm::vec3 direction  = glm::normalize( *parentPosition - pos );
			pos += recallSpeed * direction * float(dt);
		}
		isStuck              = false;
	}
	if (!isActive) {
		
    }
	if (isStuck == true && status != BlobStatus::Blob_Sticky) 
	{
		isStuck = false;
	}
	if (!isActive) 
	{
		deactivateHitbox();
	}
	if (velocity.y > 40 || velocity.y < -40) {
		isLanding = false;
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

void Blob::collide(ColliderType ownType, const HitboxEntry& other) noexcept
{
    if ( isBeingRecalled and(
         other.colliderType == ColliderType::player ))
    {
        absorb();
		return;
    }
	if (!isActive) return;

	if ((other.colliderType == ColliderType::platform || other.colliderType == ColliderType::movingPlatform) && !isBeingRecalled) {
		if(hitbox.color.w == 0)
		{
			reactivateHitbox();
		}

		MovingPlatform* platformPtr = dynamic_cast<MovingPlatform*>(other.object);

		float eps = 0.1; // fixes some issues with platform edges
		glm::vec3 pushUp    = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown  = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft  = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 minDistY  = glm::length(pushUp)   < glm::length(pushDown)  ? pushUp   : pushDown;
		glm::vec3 minDistX  = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		// posDiff is the direction and amount to push the blob away from the platform
		glm::vec3 posDiff   = glm::length(minDistY) < glm::length(minDistX) - eps ? minDistY : minDistX;
		pos				   += posDiff;

		// if blob collides with a floor
		if (posDiff.y > 0.0)
		{
			if (status == BlobStatus::Blob_Bouncy) {
				if (isBeingRecalled == false && isLanding == false) {
					gameSounds->PlayBlobSound02();
					isLanding = true;
				}
				this->velocity.y = -this->velocity.y;
				this->velocity.x = 0;
			}
			else {
				this->velocity = glm::vec3(0.0, -15, 0.0);
			}
			collidingMovingPlatform = platformPtr;
		}
		else if (status == BlobStatus::Blob_Sticky) 
		{
			if (isBeingRecalled == false && isLanding == false) {
				gameSounds->PlayBlobSound02();
				isLanding = true;
			}
			this->velocity = glm::vec3(0.0);
			isStuck = true;
			collidingMovingPlatform = platformPtr;

			//push the blob slightly into the platform to ensure that it collides next physics update
			if (posDiff.y < 0.0) {
				pos.y += 0.015f;
			}
			else if (posDiff.y > 0.0) {
				pos.y -= 0.015f;
			}
			else if (posDiff.x < 0.0) {
				pos.x += 0.015f;
			}
			else if (posDiff.x > 0.0) {
				pos.x -= 0.015f;
			}
		}
		else 
		{
			if (isBeingRecalled == false && isLanding == false) {
				gameSounds->PlayBlobSound02();
				isLanding = true;
			}
			this->velocity = glm::vec3(0.0);
		}
	}
	else if (isActive and other.colliderType == ColliderType::enemy)
	{
		if (pos.x < (other.hitbox->center.x + other.hitbox->halfLengths.x) or pos.x >(other.hitbox->center.x - other.hitbox->halfLengths.x))
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
			glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}
		else if (pos.x > (other.hitbox->center.x + other.hitbox->halfLengths.x) or pos.x < (other.hitbox->center.x - other.hitbox->halfLengths.x))
		{
			if (status == BlobStatus::Blob_Sticky)
			{
				this->velocity = glm::vec3(0.0);
				isStuck = true;
			}
			glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}	
	}
}

void Blob::followPlayer() {
	// gradient noise for offsets to target position
	const auto random2 = [&](glm::vec2 xy) {
		float hash = float(((int(this) * 987231) >> 4) & 0xFF) * float(((int(this) * 9102301) >> 3) & 0xFF);
		xy = glm::vec2(
			glm::dot(xy, glm::vec2(13.26, 17.97)),
			glm::dot(xy, glm::vec2(19.29, 16.47))
		);

		return 1.0f - 2.0f * glm::fract(glm::sin(xy) * hash);
	};

	const auto perlinNoise = [&](glm::vec2 pos) {
		glm::vec2 i = glm::floor(pos);
		glm::vec2 f = glm::fract(pos);
		glm::vec2 curve = 6.f *f*f*f*f*f - 15.f *f*f*f*f + 10.f *f*f*f;

		glm::vec2 a = glm::vec2(0.0, 0.0);
		glm::vec2 b = glm::vec2(1.0, 0.0);
		glm::vec2 c = glm::vec2(0.0, 1.0);
		glm::vec2 d = glm::vec2(1.0, 1.0);

		float wa = glm::dot(random2(i + a), f - a);
		float wb = glm::dot(random2(i + b), f - b);
		float wc = glm::dot(random2(i + c), f - c);
		float wd = glm::dot(random2(i + d), f - d);

		return glm::mix(
			glm::mix(wa, wb, curve.x),
			glm::mix(wc, wd, curve.x),
			curve.y
		);
	};

	offsetFromParent = glm::vec3(
		perlinNoise(glm::vec2(parentPosition->x, parentPosition->y) * 0.01f),
		perlinNoise(glm::vec2(parentPosition->x + 111.1, parentPosition->y + 111.1) * 0.01f),
		perlinNoise(glm::vec2(parentPosition->x - 123.4, parentPosition->y - 432.1) * 0.01f)) *
		glm::vec3(11.0, 11.0, 5.0);
	glm::vec3 targetPos = *parentPosition + offsetFromParent;
	setVelocity(glm::vec3(0.0));

	// smooth move to target position
	glm::vec3 moveDir = targetPos - pos;
	if (glm::length(moveDir) > 0) {
		glm::vec3 delta = glm::normalize(moveDir) * glm::smoothstep(0.f, 25.0f, glm::length(moveDir)) * followParentSpeed;

		if (glm::length(delta) > glm::length(moveDir)) {
			pos = targetPos;
		}
		else {
			pos += delta;
		}
	}

}