#include "Gate.h"

Gate::Gate() {}

// TODO: bryt ut all jävla buttonlogik till button

Gate::Gate( Box box, double timerAdd_s, int index ):
    IMobile    ( 100.0f, box.center ),
    index      ( index              ),
    button     ( nullptr            ),
    timer_s    ( .0f                ), // TODO: verify
    timerAdd_s ( timerAdd_s         ),
    isMoved    ( false              )
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,   // hitbox parent
        ColliderType::gate, // hitbox identifier
        std::move(box),     // hitbox bounds
        false               // hitbox is not static
    });
    updateHitboxes();
	button = button; // wtf?
}

Gate::~Gate() {}

void Gate::collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept {} // stub

/*void Gate::move( double dt_s ) noexcept {
	if( button->isPressed() and !isMoved ) {
		hitbox.center.y -= hitbox.halfLengths.y * 2;
		isMoved = true;
		if ( timerAdd_s > 0 and timer_s <= 0 ) {
			timer_s = timerAdd_s;
		}
	}
	else if ( !button->isPressed() and isMoved ) {
		hitbox.center.y += hitbox.halfLengths.y * 2;
		isMoved = false;
		button->hitbox.center.y += button->hitbox.halfLengths.y * 2;
		button->isMoved = false;
	}
	if( timerAdd_s > .0f ) {
		if ( timer_s > .0f ) {
			timer_s -= dt_s;
		}
		else if ( button->isPressed and timer_s < .0f ) {
			button->isPressed = false;
		}
	}
}*/

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
Gate::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}
