#include"Platform.h"

Platform::Platform( Box box, float friction ):
    IObject  ( box.center ),
    friction ( friction   )
{
    // register hitbox:
    hitboxes.push_back({
        this,                   // hitbox parent
        ColliderType::platform, // hitbox identifier
        box,       // hitbox box (also used for representation)
        true                    // hitbox is static
     });
}

Platform::~Platform() {}

std::vector<Hitbox> const& Platform::getHitboxes() const noexcept {
    return hitboxes;
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> Platform::getRepresentation() const noexcept {
    return Boxes{ &getBox() };
}

[[nodiscard]]
float Platform::getFriction() const noexcept {
    return friction;
}

void Platform::updateGraphics()  noexcept                           {}; // stub
void Platform::updateHitboxes()  noexcept                           {}; // stub
void Platform::updateLogic(      double dt_s )             noexcept {}; // stub
void Platform::updatePhysics(    double dt_s )             noexcept {}; // stub
void Platform::updateAnimations( double dt_s, double t_s ) noexcept {}; // stub