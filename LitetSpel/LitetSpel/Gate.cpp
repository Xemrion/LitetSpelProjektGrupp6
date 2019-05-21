#include "Gate.h"

Gate::Gate( Box box ):
    IActivable( box.center  ),
    openFactor( .0f )
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,   // hitbox parent
        ColliderType::gate, // hitbox identifier
        std::move(box),     // hitbox bounds
        false               // hitbox is not static
    });
    updateHitboxes();
}

void Gate::updateLogic( double dt_s ) noexcept {
    // TODO: slerp instead of lerp
    if ( isActive() )
       openFactor = min( 1.0f, openFactor + float(dt_s) * DOOR_MOVEMENT_SPEED );
    else
       openFactor = max( 0.0f, openFactor - float(dt_s) * DOOR_MOVEMENT_SPEED );
}

void Gate::updateHitboxes() noexcept {
    auto    &box = hitboxes[0].box;
    box.center.y = position.y + (openFactor * box.halfLengths.y * 2.0f );
}

Gate::~Gate() {}

void Gate::collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept {} // stub

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
Gate::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}
