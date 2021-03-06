#pragma once

#include <vector>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "geometry.h"
#include "Sounds.h"

struct HitboxEntry;
enum ColliderType { player,
				    enemy,
                    level_goal,
                    platform,
					movingPlatform,
					damagePlatform,
                    wall,
                    powerup_bouncy,
                    powerup_sticky,
					powerup_heavy,
					powerup_none,
				    blob };


class CollisionObject { // Interface / abstract base class
public:
    CollisionObject(): id( generateID() ) {}
    //IObject( IObject const  & )         = delete;
    //IObject( IObject       && )         = delete;
    //auto& operator=( IObject const  & ) = delete;
    //auto& operator=( IObject       && ) = delete;
    virtual ~CollisionObject() noexcept {}
    virtual void collide(ColliderType ownType, const HitboxEntry& otherCollider) = 0;
    size_t getID() const noexcept { return id; }
    bool operator==(CollisionObject const &other) const noexcept;
    bool operator!=(CollisionObject const &other) const noexcept;
	Sounds* gameSounds;

	const Box& getHitbox() { return hitbox; };
private:
    size_t id;
	Box hitbox;
    static size_t generateID() noexcept {
        static size_t idCounter { 0 };
        return idCounter++;
    }
};

struct HitboxEntry {
    CollisionObject *object; // parent object   (multiple hitboxes can share the same parent)
    const Box       *hitbox;
    ColliderType     colliderType;
};

class CollisionManager {
public:
    CollisionManager();
    // registers a new hitbox
    void registerEntry(CollisionObject &parent, ColliderType id, Box const &hitbox, bool isStatic ) noexcept;
    void unregisterEntry(CollisionObject const &target) noexcept;
    static bool intersect(Box const &a, Box const &b) noexcept;
    // checks if any of the mobiles have collided,
    // and if so calls their collide function
    void update() noexcept;
	// cleans out box vectors
	void clean();
private:

    std::vector<HitboxEntry> mobileBoxes;
    std::vector<HitboxEntry> staticBoxes;
};
