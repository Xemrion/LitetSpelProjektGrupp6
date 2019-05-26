#include "MovingPlatform.h"

MovingPlatform::MovingPlatform( glm::vec3 startPos, glm::vec3 endPos, Box box ):
    IMobile  ( 1.0f, startPos               ),
    box      ( box                          ),
    period   ( glm::length(endPos-startPos) ),
    startPos ( startPos                     ),
    endPos   ( endPos                       )
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,       // hitbox parent
        ColliderType::platform, // hitbox identifier          (TODO)
        box,                    // the hitbox box
        false                   // hitbox is not static       (TODO)
    });
}

[[nodiscard]]
glm::vec3 MovingPlatform::moveFunction( double dt_s ) const {
	return startPos + abs(glm::mod((float)dt_s / period, 2.0f) - 1.0f) * (endPos - startPos);
}

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres>
MovingPlatform::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &box );
    return representation;
}

void MovingPlatform::updateLogic( double dt_s ) noexcept {
    position = moveFunction( dt_s );
}

void MovingPlatform::updateHitboxes() noexcept {
    box.center = { position, .0f };
}
