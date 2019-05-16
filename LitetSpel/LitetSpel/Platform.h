#pragma once

#include <vector>

#include "Geometry.h"
#include "Collisions.h"
#include "Interfaces.h"

class Platform : public IObject  {
public:
	Platform();
	Platform(glm::vec4 center, glm::vec4 halfLengths);
    virtual ~Platform() noexcept;
	Box const& getBox() const { return hitboxes[0].box; }; // TODO: source
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual std::vector<Hitbox> const& getHitboxes() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override {}
    virtual void updateHitboxes()           noexcept override;
    virtual void updateLogic(    double dt) noexcept override;
    virtual void updatePhysics(  double dt) noexcept override;
    virtual void updateAnimation(double dt) noexcept override;

private:
    std::vector<Hitbox> hitboxes;
};
