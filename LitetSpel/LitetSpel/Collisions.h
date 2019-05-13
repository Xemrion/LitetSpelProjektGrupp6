#pragma once

#include <vector>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "geometry.h"

enum ColliderType { player,
				    enemy,
                    level_goal,
                    platform,
					movingPlatform,
                    wall,
                    powerup_bouncy,
                    powerup_sticky,
					powerup_heavy,
				    blob };

class CollisionObject { // Interface / abstract base class
public:
    CollisionObject(): id( generateID() ) {}
    //IObject( IObject const  & )         = delete;
    //IObject( IObject       && )         = delete;
    //auto& operator=( IObject const  & ) = delete;
    //auto& operator=( IObject       && ) = delete;
    virtual ~CollisionObject() noexcept {}
    virtual void collide(ColliderType ownHitbox, ColliderType otherHitbox, Box const &other) = 0;
    size_t getID() const noexcept { return id; }
    bool operator!=(CollisionObject const &other) const noexcept;

private:
    size_t id;
    glm::vec3 position;

    static size_t generateID() noexcept {
        static size_t idCounter { 0 };
        return idCounter++;
    }
};

class CollisionManager {
public:
    CollisionManager();
    // registers a new hitbox
    void registerEntry(CollisionObject &parent, ColliderType id, Box const &hitbox, bool is_static ) noexcept;
    bool unregisterEntry(CollisionObject const &parent) noexcept;
    bool intersect(Box const &a, Box const &b) noexcept;
    // checks if any of the mobiles have collided,
    // and if so calls their collide function
    void update() noexcept;
private:
    struct HitboxEntry {
        CollisionObject *object; // parent object   (multiple hitboxes can share the same parent)
        const Box       *hitbox;
        ColliderType     colliderType;
    };

    std::vector<HitboxEntry> mobileBoxes;
    std::vector<HitboxEntry> staticBoxes;
};
