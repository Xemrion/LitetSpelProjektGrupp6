#include "Button.h"

Button::Button( Box box, double activeTime_s, std::vector<IActivable*> targets ):
    IObject      ( box.center   ),
    targets      ( targets      ),
    activeTime_s ( 5.f ), // activeTime_s ),
    timer_s      ( .0f          ),
    downFactor   ( .0f          )
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,      // hitbox parent
        ColliderType::button,  // hitbox identifier
        std::move(box),        // hitbox bounds
        true                   // hitbox is static
    });
}

Button::~Button() {}

void Button::link( IActivable *target ) noexcept {
    targets.push_back( target );
}

void Button::collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept {}

void Button::updateLogic( double dt_s ) noexcept {
    bool wasActive = timer_s > .0f;
    timer_s = max( .0f, timer_s - dt_s );
    if ( wasActive ) {
        if ( timer_s == .0f ) // if deactivated by timer
            for ( auto &t : targets )
                t->toggle();
    }
    downFactor = 1.f - float( (activeTime_s - timer_s) / activeTime_s );
}

void Button::trigger() noexcept {
    if ( timer_s <= .0f ) // if inactive
        for ( auto &t : targets )
            t->toggle();

    timer_s = activeTime_s;
}

// also updates visuals since hitbox is used for representation
void Button::updateHitboxes() noexcept {
    auto &box = hitboxes[0].box;
    box.center.y = position.y - downFactor * box.halfLengths.y * 1.8f;
}

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
Button::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}
