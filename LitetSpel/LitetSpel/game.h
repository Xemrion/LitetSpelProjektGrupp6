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

class Player : public IActor, public IInputListener {
    using Blobs = std::vector<Blob>;

public:
    enum class Status { none,
                        bouncy,
                        sticky,
                        heavy };
    Player( glm::vec3 position );
    virtual ~Player() noexcept;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
	void          shoot(glm::vec2 const &mousePos) noexcept;
	void          recallBlobs() noexcept;
    virtual void  updateHitboxes()             noexcept override;
    virtual void  updateInput()                noexcept override;
    virtual void  updateLogic(    double dt_s) noexcept override;
    virtual void  updatePhysics(  double dt_s) noexcept override;
    virtual void  updateAnimation(double dt_s) noexcept override;
    virtual void  collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    void          handleInput() noexcept;
    void          die() noexcept override;
    [[nodiscard]] Sphere const *getSphere() const noexcept;

private:
    void processMouse()    noexcept;
    void processKeyboard() noexcept;

    Input   input;
    bool    hasExtraJump,
            isStuck;
    Status  status;
    int     blobCharges;
    float   shootCooldown,
            powerCooldown,
            radius;
    Blobs   blobs;
    Sphere  sphere;
};


class Enemy : public IActor {
public:
    Enemy(glm::vec3 position={.0f,.0f,.0f});
	virtual ~Enemy() noexcept;
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;
	virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other) noexcept override;
    virtual void updateHitboxes()           noexcept override;
    virtual void updateLogic(    double dt_s) noexcept override;
    virtual void updatePhysics(  double dt_s) noexcept override;
    virtual void updateAnimation(double dt_s) noexcept override;
    virtual void die() noexcept override;

    // TODO: refactor into IActor?
    [[nodiscard]] virtual inline Box const* getVolume() const noexcept {
        static Box volume { {position,0}, {3,3,3,0}, {0,1,0,0} };
        return &volume;
    }

private:
    glm::vec3 controlDir; // wot?
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
    virtual void updateHitboxes()           noexcept override;
    virtual void updateLogic(    double dt_s) noexcept override;
    virtual void updatePhysics(  double dt_s) noexcept override;
    virtual void updateAnimation(double dt_s) noexcept override;

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
    bool              remove(IObject const &target)   noexcept;
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
	void   init() noexcept;
	void   update(double dt_s);
    void   loadLevel();
    Input& getInput() noexcept;
    Level& getLevel() noexcept; // temp

private:
    void updateLogic(double dt_s);
    void updatePhysics();
    void updateInput();
    void updateGraphics();
    void showHitboxes();
    void animateSphere( Sphere    const &sphere,
                        glm::vec2 const &moveSpeed,
                        glm::vec3 const &amplitude = {2.4f, 1.7f, 0.8f} );

    double                 physicsSimTime  { .0 },
                           time            { .0 };
    std::unique_ptr<Level> level           {};
};
