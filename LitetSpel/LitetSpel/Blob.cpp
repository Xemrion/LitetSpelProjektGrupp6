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
void Blob::move(float dt)
{
	if (this->isActive)
	{
		velocity -= -GRAVITY_CONSTANT * dt;
		pos += velocity * speed * dt;
	}
	else if (this->isBeingRecalled)
	{
		pos += velocity * recallSpeed * dt;
	}
	
	this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
}

void Blob::setVelocity(glm::vec3 dir)
{
	this->velocity = dir;
}
