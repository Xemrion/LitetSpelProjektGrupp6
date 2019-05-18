#include "Collisions.h"
#include <cassert>
#include <algorithm>

CollisionManager::CollisionManager() {
    colliders.reserve( 1000 );
}

void CollisionManager::add( ICollider &e ) noexcept {
    colliders.push_back(&e);
}

bool CollisionManager::remove( ICollider &parent ) noexcept {
    auto unary_predicate = [&parent]( ICollider const *e ) { return *e == parent; };
    return std::remove_if( colliders.begin(), colliders.end(), unary_predicate ) != colliders.end();
}

bool CollisionManager::intersect( Box const &a, Box const &b ) noexcept {
    float const dx      =  abs(a.center.x - b.center.x);     // midpoint delta x
    float const dy      =  abs(a.center.y - b.center.y);     // midpoint delta y
    float const cWidth  = a.halfLengths.x + b.halfLengths.x; // combined width
    float const cHeight = a.halfLengths.y + b.halfLengths.y; // combined height
    return dx < cWidth and dy < cHeight;
}

// TODO: have IColliders request hitbox references from the CollisionManager
//       instead of having the collision manager ask them for them?
// Outcome: better data locality, simpler loops.
void CollisionManager::update() noexcept {
    for ( auto *thisObj : colliders )
        for ( auto &thisHb : thisObj->getHitboxes() ) if ( !thisHb.isStatic ) 
            for ( auto *thatObj : colliders ) if ( *thisObj != *thatObj )
                for ( auto &thatHb : thatObj->getHitboxes() ) if ( intersect(thisHb.box, thatHb.box) ) {
                     thisObj->collide( thisHb.colliderType, thatHb.colliderType, *thatObj );
                     thatObj->collide( thatHb.colliderType, thisHb.colliderType, *thisObj );
                  }
}
