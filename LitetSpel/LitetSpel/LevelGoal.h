#pragma once

#include <functional>

#include "Globals.h"
#include "Interfaces.h"

class LevelGoal : public IObject {
public:
    using TriggerCallback = std::function<void(void)>;

    LevelGoal( Box box );
    virtual ~LevelGoal();
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    virtual void updateGraphics() noexcept override {}; // stub
    virtual void updateHitboxes() noexcept override {}; // stub
    virtual void updateLogic( double dt_s) noexcept override {}; // stub
    virtual void updatePhysics( double dt_s) noexcept override {}; // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {}; // stub
    void setWinCallback( TriggerCallback onWin ) noexcept;

private:
    TriggerCallback     onWinCallback;
};
