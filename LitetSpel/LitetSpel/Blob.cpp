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
	this->status = BlobStatus::Passive;
}
void Blob::move(float dt)
{
	if (this->status == BlobStatus::Active)
	{
		dir -= glm::vec3(1, 60, 0) * fallSpeed * dt;
		pos += dir * speed * dt;
		this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
	}
	else if (this->status == BlobStatus::BeingRecalled)
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
