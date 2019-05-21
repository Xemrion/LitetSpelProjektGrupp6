#include "LevelGoal.h"

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> LevelGoal::getRepresentation() const noexcept {
    return Boxes{ &hitboxes[0].box };
}

LevelGoal::LevelGoal( Box box ):
    IObject(position),
    onWinCallback(nullptr)
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,         // hitbox parent
        ColliderType::level_goal, // hitbox identifier
        std::move(box),           // hitbox box
        true                      // hitbox is static
    });
}

void LevelGoal::setWinCallback( TriggerCallback onWin ) noexcept {
    onWinCallback = onWin;
}

LevelGoal::~LevelGoal() {}

void LevelGoal::collide( ColliderType, ColliderType  otherHitbox, IUnique& ) noexcept {
    assert( onWinCallback != nullptr );
    if ( otherHitbox == ColliderType::player )
        onWinCallback();
}
