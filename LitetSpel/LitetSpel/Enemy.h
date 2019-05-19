#pragma once

#include "Globals.h"
#include "Interfaces.h"

class Enemy : public IActor {
public:
    Enemy(glm::vec3 position={.0f,.0f,.0f});
    virtual ~Enemy() noexcept;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    virtual void updateGraphics() noexcept override;
    virtual void updateHitboxes() noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override;
    virtual void updatePhysics( double dt_s ) noexcept override;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override;
    virtual void die() noexcept override;
    [[nodiscard]] virtual inline Box const* getVolume() const noexcept {
        return &volume;
    }

private:
    Box       volume;
    glm::vec3 controlDir;
};
