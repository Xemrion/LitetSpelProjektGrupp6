#include "LevelGoal.h"

[[nodiscard]]
std::variant<IRepresentable::Boxes,IRepresentable::Spheres> LevelGoal::getRepresentation() const noexcept {
    return Boxes{ &representation };
}

void LevelGoal::updateGraphics()                            noexcept {}; // stub
void LevelGoal::updateHitboxes()                            noexcept {}; // stub
void LevelGoal::updateLogic(      double dt_s )             noexcept {}; // stub
void LevelGoal::updatePhysics(    double dt_s )             noexcept {}; // stub
void LevelGoal::updateAnimations( double dt_s, double t_s ) noexcept {}; // stub

LevelGoal::LevelGoal( glm::vec3 const &position, float radius, TriggerCallback cb ):
    IObject(position),
    representation({ glm::vec4(position,0), {2.0f,5.0f,2.0f,.0f}, {1.0f,.0f,1.0f,.0f} }),
    onWinCallback(cb)
{
    // register hitbox:
    hitboxes.push_back({
        (ICollider*)this,         // hitbox parent
        ColliderType::level_goal, // hitbox identifier
        Box {                     // hitbox box
            {position,.0f},             // box position
            {radius,radius,radius,.0f}, // box volume
            {}                          // box color
        },
        true                     // hitbox is static
    });
    ///    _colMan->add({&*this, ColliderType::level_goal, _bounds, true});  // TODO: ICollider::getHitboxes() + Level::add()
}

ICollider::Hitboxes const & LevelGoal::getHitboxes() const noexcept {
    return hitboxes;
}

void LevelGoal::setWinCallback( TriggerCallback onWin ) noexcept {
    onWinCallback = onWin;
}

LevelGoal::~LevelGoal() {}

void LevelGoal::collide( ColliderType, ColliderType  otherHitbox, IUnique& ) noexcept {
    if ( otherHitbox == ColliderType::player )
        onWinCallback();
}
