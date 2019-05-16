#pragma once

#include <vector>
#include "Globals.h"
#include "Geometry.h"
#include "Collisions.h"
#include "Interfaces.h"

// TODO: make fiction into a vec2 instead

class Platform : public IObject  {
public:
	Platform();
	Platform( Box box, float friction=DEFAULT_FRICTION );
    virtual ~Platform() noexcept;
	Box const& getBox() const { return hitboxes[0].box; }; // TODO: source
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual std::vector<Hitbox> const& getHitboxes() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override {}
    virtual void updateGraphics() noexcept override;
    virtual void updateHitboxes() noexcept override;
    virtual void updateLogic( double dt_s ) noexcept override;
    virtual void updatePhysics( double dt_s ) noexcept override;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override;
    [[nodiscard]] float getFriction() const noexcept;
private:
    std::vector<Hitbox>  hitboxes;
    float                friction;
};
