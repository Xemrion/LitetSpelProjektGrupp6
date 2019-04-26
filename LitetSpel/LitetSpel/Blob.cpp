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
		dir -= glm::vec3(1, 60, 0) * fallSpeed * dt;
		pos += dir * speed * dt;
		this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
		//AA
	}
	else if (this->isBeingRecalled)
	{
		pos += dir * recallSpeed * dt;
		this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
	}

}

void Blob::setDir(glm::vec3 dir)
{
	this->dir = dir;
}

void Blob::setMoveSpeed(int speed)
{
	this->speed = speed;
}

void Blob::setFallSpeed(int speed)
{
	this->fallSpeed = speed;
}


void Blob::collide(CollisionId ownHitbox, CollisionId otherHitbox, IObject & other)
{
	if (otherHitbox == CollisionId::platform)
	{
		this->fallSpeed = 0;
		this->speed = 0;
	}
}

void Blob::updateBlobCollisions()
{
	//####################################################################Bottom
	HitboxBottom.center = glm::vec4(
		pos.x,
		pos.y - 0.4*blobSphere.centerRadius.w,
		pos.z,
		0);
	HitboxBottom.halfLengths = glm::vec4(
		blobSphere.centerRadius.w*0.5,
		blobSphere.centerRadius.w*0.4,
		blobSphere.centerRadius.w*0.1,
		0);
	//####################################################################Top
	HitboxTop.center = glm::vec4(
		pos.x,
		pos.y + 0.4*blobSphere.centerRadius.w,
		pos.z,
		0);
	HitboxTop.halfLengths = glm::vec4(
		blobSphere.centerRadius.w*0.5,
		blobSphere.centerRadius.w*0.4,
		blobSphere.centerRadius.w*0.1,
		0);
	//####################################################################Left
	HitboxLeft.center = glm::vec4(
		pos.x - 0.5*blobSphere.centerRadius.w,
		pos.y,
		pos.z,
		0);
	HitboxLeft.halfLengths = glm::vec4(
		blobSphere.centerRadius.w*0.2,
		blobSphere.centerRadius.w*0.8,
		blobSphere.centerRadius.w*0.1,
		0);

	//####################################################################Right
	HitboxRight.center = glm::vec4(
		pos.x + 0.5*blobSphere.centerRadius.w,
		pos.y,
		pos.z,
		0);
	HitboxRight.halfLengths = glm::vec4(
		blobSphere.centerRadius.w*0.2,
		blobSphere.centerRadius.w*0.8,
		blobSphere.centerRadius.w*0.1,
		0);
	//####################################################################
}