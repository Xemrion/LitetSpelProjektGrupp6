#include"PowerUp.h"

PowerUp::PowerUp( glm::vec3 position, PowerType type ):
    IObject ( position ),
    type    ( type     )
{
    ColliderType cType;
    switch (type) {
        case PowerType::bouncy: cType = ColliderType::powerup_bouncy; break;
        case PowerType::heavy:  cType = ColliderType::powerup_heavy;  break;
        case PowerType::sticky: cType = ColliderType::powerup_sticky; break;
        case PowerType::none:   cType = ColliderType::powerup_none;   break;
        default: assert( false and "Must have valid power type!" );
    }
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,         // hitbox parent
        cType,                    // hitbox identifier
        Box {                     // hitbox box
            glm::vec4{position, .0f},         // box position
            glm::vec4{1.0f, 1.0f, 1.0f, .0f}, // box volume
            glm::vec4{}                       // unused; box color
        },
        true                      // hitbox is static
    });
}

void PowerUp::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {} // stub

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
PowerUp::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}
