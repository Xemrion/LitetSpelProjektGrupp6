#include "Button.h"

Button::Button( Box box, int index ):
    IObject   ( box.center ),
    index     ( index      ),
    isPressed ( false      ),
    isMoved   ( false      ) // wtf?

{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,      // hitbox parent
        ColliderType::button,  // hitbox identifier
        box,                   // hitbox bounds
        true                   // hitbox is static
    });
}

Button::~Button() {}

void Button::collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept {
	auto &hitbox = hitboxes[0].box;
    if ( otherHitbox == ColliderType::player ) {
        auto &player = dynamic_cast<IActor&>( other );
        if ( player.getPosition().y >= hitbox.center.y ) {
		    isPressed = true;
		    if ( !isMoved and isPressed ) {
			    hitbox.center.y -= hitbox.halfLengths.y * 2;
			    isMoved = true;
		    }
        }
	}
}

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
Button::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}
