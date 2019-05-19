#pragma once

#include <vector>
#include <variant>

#include "Globals.h"
#include "Geometry.h"

#include "KeyboardInput.h"
#include "MouseInput.h"

// TODO: consider ECS? The current design is a clusterfuck even after all this refactoring.

// interface for graphics system
class IRepresentable {
public:
    using Boxes   = std::vector<Box const*>;
    using Spheres = std::vector<Sphere const*>;
    virtual ~IRepresentable() noexcept {};
    virtual void updateGraphics() noexcept = 0;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept = 0;
    virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept = 0;
};

// TODO: refactor IObject out of Collisions.h/cpp
// IObject interface makes the derived adaptee comparable to other IObjects
class IUnique {
public:
    IUnique(): id(generateID()) {}
    virtual ~IUnique() noexcept {}
    virtual size_t getID() const noexcept final { return id; }
    bool operator==(IUnique const &other) const noexcept;
    bool operator!=(IUnique const &other) const noexcept;

private:
    static size_t generateID() noexcept {
        static size_t idCounter { 0 };
        return idCounter++;
    }
    size_t id;
};

class ICollider; // forward decl
struct Hitbox {
    ICollider     *object; // parent object (multiple hitboxes can share the same parent)
    ColliderType   colliderType;
    Box            box;
    bool           isStatic;
};

// ICollider interface makes the derived adaptee compatible with CollisionManager
class ICollider : public IUnique {
public:
    using Hitboxes = std::vector<Hitbox>;
    virtual ~ICollider() noexcept {}
    virtual void collide( ColliderType  ownHitbox,
                          ColliderType  otherHitbox,
                          IUnique      &other ) = 0;
    virtual void updateHitboxes() noexcept = 0;
    virtual Hitboxes const& getHitboxes() const noexcept = 0;
};


class IObject : public IRepresentable,
                public ICollider {
public:
    IObject( glm::vec3 position );
    virtual ~IObject() noexcept {}
    // inherited:
    //    comparable    (operator!=, operator==, getID)
    //    collideable   (collide, getHitboxes)
    //    representable (getRepresentation)

    virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept = 0;
    virtual void updateLogic( double dt_s ) noexcept = 0;
    virtual void updatePhysics( double dt_s ) noexcept = 0;
    virtual void updateHitboxes() noexcept = 0;
    virtual Hitboxes const& getHitboxes() const noexcept;
    glm::vec3 const& getPosition() const noexcept;
protected:
    std::vector<Hitbox> hitboxes;
    glm::vec3           position;
};

struct Input {
    // TODO: do a better proper keymapping solution
    enum Key { left,
               right,
               up,
               down,
               shoot,
               SIZE }; // NOTE: 'SIZE' must be last!
    bool isPressed[Key::SIZE] {};
    bool shootPressed         { false };
    glm::vec2 mousePosition   { .0f, .0f };
};

class IInputListener {
public:
    void         attachInput( KeyboardInput *, MouseInput * ) noexcept;
    virtual void updateInput() noexcept = 0;
protected:
    KeyboardInput *keyboard = nullptr;
    MouseInput    *mouse    = nullptr;
};

class IMobile : public IObject {
public:
    IMobile( float     mass     = 1.0f,
             glm::vec3 position = glm::vec3(.0f),
             glm::vec3 velocity = glm::vec3( .0f ) ):
        IObject   ( position ),
        mass      ( mass     ),
        velocity  ( velocity )
    {}

    virtual ~IMobile() noexcept {};
    void         addVelocity( glm::vec3 const &velocity ) noexcept;
    void         setVelocity( glm::vec3 const &velocity ) noexcept;
    void         multiplyVelocity( glm::vec3 const &fac ) noexcept;
    void         putForce(glm::vec3 const &force) noexcept;
    virtual void move(double dt) noexcept;

protected:
    float      mass;
    glm::vec3  velocity;

private:
    // makes sure that the IMobile doesn't exceed a maximum speed
    void _cap_speed() noexcept;
};

class IActor : public IMobile {
public:
    IActor( bool      isStanding   = false,
            float     jumpForce    = 1.0f,
            float     jumpCooldown = JUMP_CD,
            float     mass         = 1.0f,
            float     moveSpeed    = 1.0f,
            glm::vec3 position     = glm::vec3(.0f),
            glm::vec3 velocity     = glm::vec3(.0f) ):
        IMobile(mass, position, velocity),
        isStanding(isStanding),
        jumpForce(jumpForce),
        jumpCooldown(jumpCooldown),
        moveSpeed(moveSpeed)
    {}

    virtual ~IActor() noexcept {}
    virtual void die() noexcept = 0;

protected:
    bool                isStanding;
    float               jumpForce,
                        jumpCooldown,
                        moveSpeed;
};
