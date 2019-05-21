#pragma once

#include <functional>
#include "Globals.h"
#include "Interfaces.h"
#include "Blob.h"
#include "Collisions.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Graphics.h"

class Player : public IActor, public IInputListener {
    using Blobs = std::vector<Blob>;
public:
    Player( Graphics &, CollisionManager &, glm::vec3 position );
    virtual ~Player() noexcept;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    void          shoot(glm::vec2 const &mousePos) noexcept;
    void          recallBlobs() noexcept;
    virtual void  updateGraphics() noexcept override;
    virtual void  updateHitboxes() noexcept override;
    virtual void  updateInput() noexcept override;
    virtual void  updateLogic( double dt_s ) noexcept override;
    virtual void  updatePhysics( double dt_s ) noexcept override;
    virtual void  updateAnimations( double dt_s, double t_s ) noexcept override;
    virtual void  collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    void          handleInput() noexcept;
    void          die() noexcept override;
    void          animateColor( double t_s );
    void          animateVictory( double dt_s, double t_s ) noexcept; // Merge with updateAnimations
    [[nodiscard]] Sphere const *getSphere() const noexcept;
    void win( std::function<void(void)> f );

private:
    void processMouse()    noexcept;
    void processKeyboard() noexcept;
    std::function<void(void)> changeLevelFunction = nullptr;
    CollisionManager *colMan;
    Graphics         *graphics;
    Input             input;
    bool              hasExtraJump,
                      isStuck,
                      hasWon;
    PowerType         status;
    int               blobCharges;
    float             shootCooldown,
                      powerCooldown,
                      radius;
    Blobs             blobs;
    Sphere            blobSphere,
                      animSphere1,
                      animSphere2;
};
