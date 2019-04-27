#include "Blob.h"

Blob::Blob()
{
	this->pos = glm::vec3(0);
	this->blobSphere.centerRadius = glm::vec4(
		pos,
		radius);
	this->hitbox.center = glm::vec4(pos, 0);
	this->hitbox.halfLengths = glm::vec4(radius, radius, radius, 0);
	this->hitbox.color = glm::vec4(0.5, 0.5, 1.0, 0.0);
}

Blob::Blob(glm::vec3 pos)
{
	this->blobSphere.centerRadius = glm::vec4(
		pos,
		radius);
	this->pos = pos;
	this->hitbox.center = glm::vec4(pos, 0);
	this->hitbox.halfLengths = glm::vec4(radius, radius, radius, 0);
	this->hitbox.color = glm::vec4(0.5, 0.5, 1.0, 0.0);
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
	this->hitbox.center = glm::vec4(pos, 0.0);
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
		glm::vec3 pushUp = glm::vec3(0.0, other.center.y + other.halfLengths.y + (-hitbox.center.y + hitbox.halfLengths.y), 0.0);
		glm::vec3 pushDown = glm::vec3(0.0, other.center.y - other.halfLengths.y + (-hitbox.center.y - hitbox.halfLengths.y), 0.0);
		glm::vec3 pushRight =  glm::vec3(other.center.x + other.halfLengths.x + (-hitbox.center.x + hitbox.halfLengths.x), 0.0, 0.0);
		glm::vec3 pushLeft = glm::vec3(other.center.x - other.halfLengths.x + (-hitbox.center.x - hitbox.halfLengths.x), 0.0, 0.0);
#undef min
		glm::vec3 minDistY = glm::length(pushUp) < glm::length(pushDown) ? pushUp : pushDown;
		glm::vec3 minDistX = glm::length(pushLeft) < glm::length(pushRight) ? pushLeft : pushRight;
		this->pos += glm::length(minDistY) < glm::length(minDistX) ? minDistY : minDistX;
	}
}
