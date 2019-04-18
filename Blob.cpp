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
	if (dir != glm::vec3(0, 0, 0))
	{
		dir -= glm::vec3(1, 60, 0) * fallSpeed * dt;
		pos += dir * speed * dt;
		this->blobSphere.centerRadius = glm::vec4(
			pos,
			radius);
	}

}