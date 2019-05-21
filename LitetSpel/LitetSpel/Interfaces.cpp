#include "Interfaces.h"

bool IUnique::operator==( IUnique const &other ) const noexcept {
    return id == other.id;
}

bool IUnique::operator!=( IUnique const &other ) const noexcept {
    return id != other.id;
}

IObject::IObject( glm::vec3 position ):
    position ( std::move(position) )
{}

std::vector<Hitbox> const& IObject::getHitboxes() const noexcept {
    return hitboxes;
}

glm::vec3 const& IObject::getPosition() const noexcept {
    return position;
}

void IMobile::addVelocity(glm::vec3 const &velocity) noexcept {
    this->velocity += velocity;
    _cap_speed();
}

void IMobile::multiplyVelocity( glm::vec3 const &fac ) noexcept {
    velocity *= fac;
    _cap_speed();
}

void IMobile::setVelocity( glm::vec3 const &velocity ) noexcept {
    this->velocity = velocity;
    float speed = length(this->velocity);
    _cap_speed();
}

void IMobile::putForce(glm::vec3 const &force) noexcept {
    velocity += force / mass;
    _cap_speed();
}

void IMobile::_cap_speed() noexcept {
    float speed = glm::length(velocity);
    if ( speed > MAX_SPEED )
        velocity = glm::normalize(velocity) * MAX_SPEED;
}

void IMobile::move(double dt_s) noexcept {
    position += velocity * (float)dt_s;
    updateHitboxes();
}

void IInputListener::attachInput( KeyboardInput *keyboard, MouseInput *mouse ) noexcept {
    this->keyboard = keyboard;
    this->mouse    = mouse;
}
