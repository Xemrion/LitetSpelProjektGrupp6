#include "Blob.h"

Blob::Blob()
{
}

Blob::~Blob()
{
}

Blob::Blob(glm::vec3 pos)
{
	this->blobSphere.centerRadius = glm::vec4(
		pos,
		radius);
	this->pos = pos;
}

// called once per frame from Player::update
void Blob::update(float dt)
{
	if (this->isActive)
	{
		
	}
	else if (this->isBeingRecalled)
	{
		//pos += velocity * recallSpeed * dt;
	}
	
	this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
}

// Call from updatePhysics
void Blob::move(float dt)
{
	this->pos += this->velocity * dt;
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::setVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		float speedMultiplier = isBeingRecalled ? recallSpeed : speed;
		this->velocity = velocity * speedMultiplier;
	}
	else {
		this->velocity = velocity;
	}
}

// Set useSpeed to true to multiply velocity by objects speed value
void Blob::addVelocity(glm::vec3 velocity, bool useSpeed)
{
	if (useSpeed) {
		float speedMultiplier = isBeingRecalled ? recallSpeed : speed;
		this->velocity += velocity * speedMultiplier;
	}
	else {
		this->velocity += velocity;
	}
}

void Blob::collide(ColliderType ownType, ColliderType otherType, Box other)
{
	if (otherType == ColliderType::platform)
	{
		this->velocity = glm::vec3(0.0);
		glm::vec3 bot = glm::vec3(0.0, other.center.y + other.halfLengths.y + (hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 top = glm::vec3(0.0, other.center.y - other.halfLengths.y + (hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 left =  glm::vec3(other.center.x + other.halfLengths.x + (hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 right = glm::vec3(other.center.x - other.halfLengths.x + (hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
#undef min
		//this->pos -= glm::min(glm::min(glm::min(bot, top), left), right);
	}
}


/*

this->pos.y = other.center.y + other.halfLengths.y + (pos.y - HitboxBottom.center.y + HitboxBottom.halfLengths.y);

pos.y = other.center.y + other.halfLengths.y - pos.y + HitboxBottom.center.y - HitboxBottom.halfLengths.y;

pos.y += other.center.y + other.halfLengths.y + HitboxBottom.center.y - HitboxBottom.halfLengths.y;

*/