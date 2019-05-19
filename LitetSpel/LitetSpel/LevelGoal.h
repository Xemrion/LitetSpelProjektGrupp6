#pragma once

#include <functional>

#include "Globals.h"
#include "Interfaces.h"

class LevelGoal : public IObject {
public:
    using TriggerCallback = std::function<void(void)>;

    LevelGoal(glm::vec3 const &position, float radius, TriggerCallback cb=[](){});
    virtual ~LevelGoal();
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    virtual Hitboxes const& getHitboxes() const noexcept override;
    virtual void updateGraphics() noexcept override;
    virtual void updateHitboxes() noexcept override;
    virtual void updateLogic( double dt_s) noexcept override;
    virtual void updatePhysics( double dt_s) noexcept override;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override;
    void setWinCallback( TriggerCallback onWin ) noexcept;

private:
    Box                 representation;
    std::vector<Hitbox> hitboxes;
    TriggerCallback     onWinCallback;
};
