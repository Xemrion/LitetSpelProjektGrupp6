#include <algorithm>
#include "Blob.h"
#include "Platform.h"

// The unused fields can be removed by inheriting from an
// intermediate class (IMobile, situated between IObject and IActor)
// instead of IMobile. TODO.
Blob::Blob( glm::vec3 const &parentPosition ):
    IActor( false,            // unused
            .0f,              // unused
            .0f,              // unused
            .0f,              // unused
            BLOB_SHOOT_SPEED, // moveSpeed
            parentPosition    // position
    ),
    parentPosition  ( &parentPosition                  ),
    blobSphere      ( glm::vec4(parentPosition,radius) ),
    isActive        ( false                            ),
    isBeingRecalled ( true                             ),
	isStuck         ( false                            ),
    recallSpeed     ( BLOB_RECALL_SPEED                ),
    radius          ( BLOB_ACTIVE_RADIUS               ),
	status          ( PowerType::none                  )
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
    updateHitboxes();
}

void Blob::absorb() noexcept {
    isBeingRecalled = false;
    isActive        = false;
}

// FIND ISSUE HERE WITH ISACTIVE AND IS STUCK !!!
void Blob::shoot( glm::vec3 const &direction ) noexcept {
    assert( glm::length(direction) >  .95f and 
            glm::length(direction) < 1.05f and "Needs to be a unit vector!" );
    if ( !isActive and !isBeingRecalled ) {
        isActive = true;
        addVelocity( status == PowerType::heavy?  (direction * moveSpeed/3.0f) : (direction * moveSpeed) );
	}
}

void Blob::recall() noexcept {
    isBeingRecalled = true;
}

bool Blob::getIsActive() const noexcept { return isActive; }

bool Blob::getIsBeingRecalled() const noexcept { return isBeingRecalled; }

bool Blob::getIsStuck() const noexcept { return isStuck; }

[[nodiscard]] Sphere const * Blob::getSphere() const noexcept {
    return &blobSphere;
}

// called once per frame from Player::update
void Blob::updateLogic( double dt_s ) noexcept {
    #undef min
    if ( isBeingRecalled ) {
        isStuck = false;
        glm::vec3 direction = glm::normalize( *parentPosition - position );
        addVelocity( (float)dt_s * recallSpeed * direction );
	}

    if ( !isActive ) {
        position = *parentPosition + glm::vec3(.0f, 2.0f, .0f); // ?
        setVelocity( glm::vec3(0.0) );
        radius = BLOB_INACTIVE_RADIUS;
    }
    else {
        radius = BLOB_ACTIVE_RADIUS;
        if ( isStuck and status != PowerType::sticky )
		    isStuck = false;
    }
}

void Blob::updateHitboxes() noexcept {
    hitboxes[0].box.center = glm::vec4( position, radius );
}

void Blob::updateGraphics() noexcept {
    blobSphere.centerRadius = glm::vec4( position, radius );
};

void Blob::updatePhysics( double dt_s ) noexcept {
    if ( isActive and !isStuck ) {
       addVelocity( glm::vec3(.0f, -GRAVITY_CONSTANT * dt_s, .0f) );
    }
    move( dt_s );
}
void Blob::updateAnimations( double dt_s, double t_s ) noexcept {}; // stub
void Blob::die() noexcept {}; // stub

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> Blob::getRepresentation() const noexcept {
    Spheres representation;
    representation.push_back( &blobSphere  );
    return representation;
}

void Blob::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {
    if ( isBeingRecalled ) {
        if ( otherHitbox == ColliderType::player )
            absorb();
        else return; // early exit to avoid colliding with the level
    }

//	if (other.colliderType == ColliderType::platform && !isBeingRecalled) {
//		if (status == PowerType::Blob_Bouncy) 
//		{
//			this->velocity.y = -this->velocity.y;
//			this->velocity.x = 0;
//		}
//		else if (status == PowerType::Blob_Sticky) 
//		{
//			this->velocity = glm::vec3(0.0);
//			isStuck = true;
//		}
//		else 
//		{
//			this->velocity = glm::vec3(0.0);
//		}
//		if(hitbox.color.w == 0)
//		{
//			reactivateHitbox();
//		}
//		glm::vec3 pushUp    = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
//		glm::vec3 pushDown  = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
//		glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
//		glm::vec3 pushLeft  = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
//		glm::vec3 minDistY  = glm::length(pushUp)   < glm::length(pushDown)  ? pushUp   : pushDown;
//		glm::vec3 minDistX  = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
//		pos                += glm::length(minDistY) < glm::length(minDistX)  ? minDistY : minDistX;
//	}
    /*
	else if (isActive and otherHitbox == ColliderType::enemy)
	{
        auto &enemy = dynamic_cast<Enemy&>( other );
        // TODO: dynamic_cast<Enemy>
		if (    position.x < ( other.hitbox->center.x + other.hitbox->halfLengths.x)
             or position.x > (other.hitbox->center.x - other.hitbox->halfLengths.x) ) {
			if ( status == PowerType::bouncy ) { // TODO: break out!
				velocity.y = -velocity.y;
				velocity.x = 0;
			}
			else if ( status == PowerType::sticky )
			{
				velocity = glm::vec3(.0f);
				isStuck  = true;
			}
			else {
				velocity = glm::vec3(.0f);
			}
			// glm::vec3 pushUp = glm::vec3(0.0, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			// glm::vec3 pushDown = glm::vec3(0.0, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			// glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			// glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			// glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			// glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			// pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}
		else if (    position.x > (other.hitbox->center.x + other.hitbox->halfLengths.x)
                  or position.x < (other.hitbox->center.x - other.hitbox->halfLengths.x) )
		{
			if ( status == PowerType::sticky ) {
				velocity = glm::vec3(.0f);
				isStuck  = true;
			}
			// glm::vec3 pushUp = glm::vec3(.0f, other.hitbox->center.y + other.hitbox->halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
			// glm::vec3 pushDown = glm::vec3(.0f, other.hitbox->center.y - other.hitbox->halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
			// glm::vec3 pushRight = glm::vec3(other.hitbox->center.x + other.hitbox->halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
			// glm::vec3 pushLeft = glm::vec3(other.hitbox->center.x - other.hitbox->halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
			// glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
			// glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
			// pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
		}	
	}
    */
    if ( !isActive )
        return; // early exit to avoid colliding with the player?

    else if ( otherHitbox == ColliderType::platform ) {
        auto &platBox     = dynamic_cast<Platform&>(other).getBox();
        auto const &myBox = hitboxes[0].box;
        if (status == PowerType::sticky) {
            velocity = glm::vec3(0.0);
            isStuck  = true;
        }
        else if (status == PowerType::bouncy) {
            velocity.y = -velocity.y;
            velocity.x = 0;
        }
        else {
            velocity = glm::vec3(0.0);
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
