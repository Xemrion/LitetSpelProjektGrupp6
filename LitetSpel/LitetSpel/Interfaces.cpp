#include "Interfaces.h"

bool IUnique::operator==( IUnique const &other ) const noexcept {
    return id == other.id;
}

bool IUnique::operator!=( IUnique const &other ) const noexcept {
    return id != other.id;
}

void IActor::addVelocity(glm::vec3 const &velocity) noexcept {
    this->velocity += velocity;
    float speed = length(this->velocity);
    _cap_speed();
}

void IActor::setVelocity(glm::vec3 const &velocity) noexcept {
    this->velocity = velocity;
    float speed = length(this->velocity);
    _cap_speed();
}

void IActor::putForce(glm::vec3 const &force) noexcept {
    velocity += force / mass;
    _cap_speed();
}

void IActor::_cap_speed() noexcept {
    float speed = glm::length(velocity);
    if ( speed > MAX_SPEED )
        velocity = glm::normalize(velocity) * MAX_SPEED;
}

void IActor::move(double dt_s) noexcept {
    position += velocity * (float)dt_s;
    updateHitboxes();
}

std::vector<Hitbox> const& IActor::getHitboxes() const noexcept {
    return hitboxes;
}
