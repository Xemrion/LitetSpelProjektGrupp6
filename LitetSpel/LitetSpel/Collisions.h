#pragma once

#include <vector>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "geometry.h"
#include <algorithm>

constexpr bool debugCollisions = true;
#include <cstdio>

// use case example:
//
// virtual void Player::collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) override {
//     switch (ownHitbox) {
//         case CollisionId::player_top {
//             if ( otherHitbox == CollisionId::wall )
//                 // collision! return to previous position?
//         }; break;
// 
//         case CollisionId::player_side {
//             if ( otherHitbox == CollisionId::wall )
//                 // collision! return to previous position?
//         }; break;
// 
//         case CollisionId::player_bottom {
//             if ( (!isFalling  and  otherHitbox == CollisionId::platform )
//                                or  otherHitbox == CollisionId::wall )
//             {
//                 // collision! return to previous position?
//             }
//         }; break;
//     }
//
//     // for powerups it doesn't matter which of the players hitbox collided with it,
//     // so the check can be outside of the switch case.
//     if ( otherHitbox == CollisionId::powerup_bouncy ) {
//         _bouncyProjectiles = true;
//         ((BouncyPowerup&)other).destroy(); // or via some EntityManager
//     }
// }



// TODO: contemplate placement;
enum CollisionId { player_top,
                   player_bottom,
                   player_side,
                   platform,
                   wall,
                   powerup_bouncy,
                   powerup_sticky
                   /* TODO: populate */ };

// TODO: refactor into more suitable header/source?
class IObject { // Interface / abstract base class
public:
    IObject(): _id( _generateId() ) {}

    //IObject( IObject const  & )         = delete;
    //IObject( IObject       && )         = delete;
    //auto& operator=( IObject const  & ) = delete;
    //auto& operator=( IObject       && ) = delete;

    virtual ~IObject() noexcept {}

    virtual void collide( CollisionId ownHitbox, CollisionId otherHitbox, IObject &other ) = 0;

    [[nodiscard]] auto inline getId() const noexcept { return _id; }

    bool operator!=( IObject const &other ) const noexcept;

private:
    size_t const _id;
    [[nodiscard]] static size_t _generateId() noexcept {
        static size_t next_id { 0 };
        return next_id++;
    }
};

class CollisionManager {
public:
    CollisionManager();

    // registers a new hitbox
    void register_entry( IObject &parent, CollisionId id,    Box const &hitbox,    bool is_static ) noexcept; // call from object constructor
    void register_entry( IObject &parent, CollisionId id, Sphere const &hitsphere, bool is_static ) noexcept; // call from object constructor

    void unregister_entry( IObject const &parent ) noexcept;

    // evaluates whether two hitboxes are intersecting or not
    [[nodiscard]]        bool intersect(    Box const &a,    Box const &b ) noexcept;
    [[nodiscard]]        bool intersect( Sphere const &a,    Box const &b ) noexcept;
    [[nodiscard]] inline bool intersect(    Box const &a, Sphere const &b ) noexcept;
    [[nodiscard]]        bool intersect( Sphere const &a, Sphere const &b ) noexcept;

    // checks if any of the mobiles have collided,
    // and if so, notifies them with the relevant information
    // needed to determine the proper collision resolution.
    void update();

private:
    struct _HitboxEntry { // POD
        IObject            *object; // parent object   (multiple hitboxes can share the same parent)
        Box const          *hitbox; // the hitbox itself
        CollisionId         id;     // enum identifier (provided to the parent of a hitbox on collision)
    };

    struct _HitsphereEntry { // POD
        IObject           *object;    // parent object   (multiple hitboxes/hitspheres can share the same parent)
        Sphere const      *hitsphere; // the hitsphere itself
        CollisionId        id;        // enum identifier (provided to the parent of a hitbox on collision)
    };

    std::vector<_HitboxEntry>    _mobileBoxes;   // stores the dynamic hitboxes
    std::vector<_HitboxEntry>    _staticBoxes;   // stores the  static hitboxes
    std::vector<_HitsphereEntry> _mobileSpheres; // stores the dynamic hitspheres
    std::vector<_HitsphereEntry> _staticSpheres; // stores the  static hitspheres
};
