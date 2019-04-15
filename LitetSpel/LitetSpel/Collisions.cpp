#include "Collisions.h"
#include <cassert>


bool IObject::operator!=( IObject const &other ) const noexcept {
    return this->_id != other._id;
}



CollisionManager::CollisionManager() {
    _static.reserve( 200 ); // arbitrary constants to minimize vector copying
    _mobile.reserve(  25 ); // arbitrary constants to minimize vector copying
}

// possibility: separate base classes for mobiles and statics and use function specialization instead of the final bool
void CollisionManager::register_entry( IObject &parent, CollisionId id, Box const &hitbox, bool is_static ) noexcept {
    _HitboxEntry e = { parent, hitbox, id };
    if ( is_static )
         _staticBoxes.push_back( e );
    else _mobileBoxes.push_back( e ); 
}

// possibility: separate base classes for mobiles and statics and use function specialization instead of the final bool
void CollisionManager::register_entry( IObject &parent, CollisionId id, Sphere const &hitsphere, bool is_static ) noexcept {
    _HitsphereEntry e = { parent, hitsphere, id };
    if ( is_static )
         _staticSpheres.push_back( e );
    else _mobileSpheres.push_back( e ); 
}

[[nodiscard]] bool CollisionManager::intersect( Box const &a, Box const &b ) noexcept {
    auto constexpr x { 0U };
    auto constexpr y { 1U };
    auto const dt_x    = abs(a.center[x]   - b.center[x])      * 2; // midpoint delta x
    auto const dt_y    = abs(a.center[y]   - b.center[y])      * 2; // midpoint delta y
    auto const cWidth  = (a.halfLengths[x] + b.halfLengths[x]) * 2; // combined width
    auto const cHeight = (a.halfLengths[y] + b.halfLengths[y]) * 2; // combined height
    return dt_x < cWidth  and  dt_y < cHeight;
}

[[nodiscard]] bool CollisionManager::intersect( Sphere const &a, Box const &b ) noexcept {
    auto constexpr x { 0U };
    auto constexpr y { 1U };
    auto constexpr r { 3U };
    auto const    &s = a.centerRadius;

    auto sDist = glm::vec2( abs(s[x]-b[y]), abs(s[y]-b[y]) );

    if ( sDist[x] > (b.halfLengths[x]+s[r])
         or sDist[y] > (b.halfLengths[y]+s[r]) ) return false;

    if ( sDist[x] <= b.halfLengths[x]
         or sDist[y] <= b.halfLengths[y] ) return true;

    auto sqrVertDist = sqr(sDist[x]-b.halfLengths[x])
        + sqr(sDist[y]-b.halfLengths[y]);

    return sqr(s[r]) > sqrVertDist;
}

[[nodiscard]] inline bool CollisionManager::intersect( Box const &a, Sphere const &b ) noexcept {
    return intersect(b,a);
}

[[nodiscard]] bool CollisionManager::intersect( Sphere const &s1, Sphere const &s2 ) noexcept {
    auto const    &a = s1.centerRadius;
    auto const    &b = s2.centerRadius;
    auto constexpr x { 0U };
    auto constexpr y { 1U };
    auto constexpr r { 3U };
    auto constexpr sqr = []( auto n ) { return n*n; };
    return sqr(a[x]-b[x]) + sqr(a[y]-b[y])  <  sqr(a[r]+b[r])
}
 
// possibilities: cull distant objects based off of a preliminary spatial eligibility test
//                check for potential bounds breaches by lerp-integrating the object's delta state
void CollisionManager::update() { /// yay for combinatoric explosions /s
    // test each mobile hitbox for intersections:
    for ( auto &m : _mobileBoxes ) {
        // test against all other mobile hitboxes (that belong to another object)
        for ( auto &other : _mobileBoxes )
            if ( m.object != other.object  and  intersect( m.hitbox, other.hitbox ) )
                m.object.collide( m.id, other.id, other.object );
        // test against all mobile hitspheres (that belong to another object)
        for ( auto &other : _mobileSpheres )
            if ( m.object != other.object  and  intersect( m.hitbox, other.hitsphere ) )
                m.object.collide( m.id, other.id, other.object );
        // test against all static environment hitboxes
        for ( auto &environment : _staticBoxes )
            if ( intersect( m.hitbox, environment.hitbox ) )
                m.object.collide( m.id, environment.id, environment.object );
        // test against all static environment hitspheres
        for ( auto &environment : _staticSpheres )
            if ( intersect( m.hitbox, environment.hitsphere ) )
                m.object.collide( m.id, environment.id, environment.object );
    }
    // now repeat the process for all the mobiles hitspheres:
    for ( auto &m : _mobileSpheres ) {
        // test against all mobile hitspheres (that belong to another object)
        for ( auto &other : _mobileSpheres )
            if ( m.object != other.object  and  intersect( m.hitsphere, other.hitsphere ) )
                m.object.collide( m.id, other.id, other.object );
        // test against all mobile hitboxes (that belong to another object)
        for ( auto &other : _mobileBoxes )
            if ( m.object != other.object  and  intersect( m.hitsphere, other.hitbox ) )
                m.object.collide( m.id, other.id, other.object );
        // test against all static environment hitspheres
        for ( auto &environment : _staticSpheres )
            if ( intersect( m.hitsphere, environment.hitsphere ) )
                m.object.collide( m.id, environment.id, environment.object );
        // test against all static environment hitboxes
        for ( auto &environment : _staticBoxes )
            if ( intersect( m.hitsphere, environment.hitbox ) )
                m.object.collide( m.id, environment.id, environment.object );
    }
}
