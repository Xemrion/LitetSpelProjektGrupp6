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
//PowerUp::PowerUp(glm::vec4 center, int type) {
//	hitbox.center = center;
//	hitbox.halfLengths = glm::vec4(1,1,0,0);
//	if (static_cast<PowerType>(type) == PowerType::Bouncy ||
//		static_cast<PowerType>(type) == PowerType::Heavy ||
//		static_cast<PowerType>(type) == PowerType::Sticky) {
//		this->typeOfPowerUp = static_cast<PowerType>(type);
//	}
//	else {
//		this->typeOfPowerUp = PowerType::Heavy;
//	}
//}

void PowerUp::collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept {} // stub

//void PowerUp::setType(PowerType type) {
//	if (type != PowerType::None) {
//		if (this->typeOfPowerUp != type) {
//			this->typeOfPowerUp = type;
//		}
//		else {
//			//Already has that type
//		}
//	}
//	else {
//		//Cannot set power type to none
//	}
//}
//void PowerUp::setTypeByInt(int type) {
//	if (static_cast<PowerType>(type) == PowerType::Bouncy ||
//		static_cast<PowerType>(type) == PowerType::Heavy ||
//		static_cast<PowerType>(type) == PowerType::Sticky) {
//		if (this->typeOfPowerUp != static_cast<PowerType>(type)) {
//			this->typeOfPowerUp = static_cast<PowerType>(type);
//		}
//		else {
//			//Already has that type
//		}
//	}
//	else {
//		if (this->typeOfPowerUp != PowerType::Heavy) {
//			this->typeOfPowerUp = PowerType::Heavy;
//		}
//		else {
//			//Already Heavy
//		}
//	}
//}

[[nodiscard]]
std::variant<IRepresentable::Boxes, IRepresentable::Spheres>
PowerUp::getRepresentation() const noexcept {
    Boxes representation;
    representation.push_back( &hitboxes[0].box );
    return representation;
}

