#include"Platform.h"

Platform::Platform(glm::vec4 center, glm::vec4 halfLengths)
{
    // register hitbox:
    hitboxes.push_back({
        this,                     // hitbox parent
        ColliderType::blob,       // hitbox identifier
        Box {                     // hitbox box
            center,                  // box position
            halfLengths,             // box volume
            {}                       // box color TODO
        },
        true                     // hitbox is static
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

void Platform::updateHitboxes() noexcept {
    ; // stub
}

void Platform::updateLogic(double dt) noexcept {
    ; // stub
}

void Platform::updatePhysics(double dt) noexcept {
    ; // stub
}

void Platform::updateAnimation(double dt) noexcept {
    ; // stub
}
