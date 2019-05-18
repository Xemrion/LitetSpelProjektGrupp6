#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <functional>
#include <algorithm>
#include <variant>
#include <memory>
#include <utility>

#include "Geometry.h"
#include "Collisions.h"
#include "Platform.h"
#include "Blob.h"
#include "Globals.h"
#include "Interfaces.h"

#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Graphics.h"

// TODO: refactor into the class hierarchy

class Player : public IActor, public IInputListener {
    using Blobs = std::vector<Blob>;
public:
    enum class Status { none,
                        bouncy,
                        sticky,
                        heavy };
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
    [[nodiscard]] Sphere const *getSphere() const noexcept;

private:
    void processMouse()    noexcept;
    void processKeyboard() noexcept;
    CollisionManager *colMan;
    Graphics         *graphics;
    Input             input;
    bool              hasExtraJump,
                      isStuck;
    Status            status;
    int               blobCharges;
    float             shootCooldown,
                      powerCooldown,
                      radius;
    Blobs             blobs;
    Sphere            blobSphere, animSphere1, animSphere2;
};


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
    Box volume;
    glm::vec3 controlDir;
  //  bool      isStanding; ///, alive, isDeregistered;
};


class LevelGoal : public IObject {
public:
    using TriggerCallback = std::function<void(void)>;

    LevelGoal(glm::vec3 const &pos, float radius, TriggerCallback cb=[](){});
    virtual ~LevelGoal();
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    virtual Hitboxes const& getHitboxes() const noexcept override;
    virtual void updateGraphics() noexcept override;
    virtual void updateHitboxes() noexcept override;
    virtual void updateLogic( double dt_s) noexcept override;
    virtual void updatePhysics( double dt_s) noexcept override;
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override;

private:
    Box                 representation;
    std::vector<Hitbox> hitboxes;
    TriggerCallback     triggerCallback;
};


class Level {
    using SceneEntry   = std::unique_ptr<IObject>;  
    using SceneEntries = std::vector<SceneEntry>;
    using Boxes        = std::vector<Box>;
    using Spheres      = std::vector<Sphere>;

public:
    Level();
    void              add(std::unique_ptr<IObject> object) noexcept;
    bool              remove(IObject &target) noexcept;
    Player&           getPlayer()  noexcept;
    SceneEntries&     getScene()   noexcept;
    Boxes&            getBoxes()   noexcept;
    Spheres&          getSpheres() noexcept;
    CollisionManager& getCollisionManager() noexcept;
    void              onFrameStart();

private:
    Boxes             boxes;        
    Spheres           spheres;
    CollisionManager  collisionManager;
    SceneEntries      scene;
    Player           *player;
};

class Game {
public:
    Game( KeyboardInput &, MouseInput &, Graphics & );
	void   init() noexcept;
	void   update(double dt_s);
    void   loadLevel();
    Input& getInput() noexcept;
    Level& getLevel() noexcept; // temp

private:
    void updateInput();
    void updateLogic(double dt_s);
    void updatePhysics();
    void updateAnimations( double dt_s ) noexcept;
    void updateGraphics();
    void showHitboxes();

    double                  physicsSimTime  { .0 },
                            time            { .0 };
    std::unique_ptr<Level>  level           {};
    KeyboardInput          *keyboard;
    MouseInput             *mouse;
    Graphics               *graphics;
};
