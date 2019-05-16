#include "Collisions.h"
#include <cassert>
#include <algorithm>

CollisionManager::CollisionManager() {
    hitboxes.reserve( 200 );
}

void CollisionManager::add( ICollider const &e ) noexcept {
    for ( auto const &box : e.getHitboxes() ) {
        hitboxes.push_back(box);
    }
}

bool CollisionManager::remove( ICollider const &parent ) noexcept {
    auto unary_predicate = [&parent]( Hitbox const &e ) { return *(e.object) == parent; };
    return std::remove_if( hitboxes.begin(), hitboxes.end(), unary_predicate ) != hitboxes.end();
}

bool CollisionManager::intersect( Box const &a, Box const &b ) noexcept {
    float const dx      = abs(a.center.x - b.center.x);        // midpoint delta x
    float const dy      = abs(a.center.y - b.center.y);        // midpoint delta y
    float const cWidth  = (a.halfLengths.x + b.halfLengths.x); // combined width
    float const cHeight = (a.halfLengths.y + b.halfLengths.y); // combined height
    return dx < cWidth and dy < cHeight;
}

void CollisionManager::update() noexcept {
    // test each mobile hitbox for intersections:
    for (auto &e : hitboxes )              // for every hitbox
        if ( !e.isStatic )                 // that's dynamic
            for ( auto &other : hitboxes ) // test it against all other hitboxes
                if ( *(e.object) != *(other.object) and intersect(e.box, other.box) ) {
                    e.object->collide( e.colliderType, other.colliderType, *other.object );
                    //*(other.object)->collide( e.colliderType, other.colliderType, *other.object );
                }
}
