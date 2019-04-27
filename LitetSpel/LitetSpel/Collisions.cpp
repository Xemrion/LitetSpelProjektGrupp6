#include "Collisions.h"
#include <cassert>


bool IObject::operator!=( IObject const &other ) const noexcept {
    return _id != other._id;
}



CollisionManager::CollisionManager() {
    _staticBoxes.reserve(  499 ); // arbitrary constants to minimize vector copying
    _mobileBoxes.reserve(   49 ); // arbitrary constants to minimize vector copying
    _staticSpheres.reserve( 99 ); // arbitrary constants to minimize vector copying
    _mobileSpheres.reserve( 49 ); // arbitrary constants to minimize vector copying
}

// possibility: separate base classes for mobiles and statics and use function specialization instead of the final bool
void CollisionManager::register_entry( IObject &parent, CollisionId id, Box const &hitbox, bool is_static ) noexcept {
    _HitboxEntry e = { &parent, &hitbox, id };
    if ( is_static )
         _staticBoxes.push_back( e );
    else _mobileBoxes.push_back( e ); 
}

// possibility: separate base classes for mobiles and statics and use function specialization instead of the final bool
void CollisionManager::register_entry( IObject &parent, CollisionId id, Sphere const &hitsphere, bool is_static ) noexcept {
    _HitsphereEntry e = { &parent, &hitsphere, id };
    if ( is_static )
         _staticSpheres.push_back( e );
    else _mobileSpheres.push_back( e ); 
}

// TODO: wrap with RAII
bool CollisionManager::unregister_entry( IObject const &parent ) noexcept {
    auto unary_predicate = [&parent]( auto const &e ) { return !(*(e.object) != parent); };
    return std::remove_if(   _mobileBoxes.begin(),   _mobileBoxes.end(), unary_predicate ) !=   _mobileBoxes.end()
        or std::remove_if( _mobileSpheres.begin(), _mobileSpheres.end(), unary_predicate ) != _mobileSpheres.end()
        or std::remove_if(   _staticBoxes.begin(),   _staticBoxes.end(), unary_predicate ) !=   _staticBoxes.end()
        or std::remove_if( _staticSpheres.begin(), _staticSpheres.end(), unary_predicate ) != _staticSpheres.end();
}

[[nodiscard]]
bool CollisionManager::intersects( Box const *a, Box const *b ) const noexcept {
    auto const dt_x    = abs(a->center.x   - b->center.x)      * 2; // midpoint delta x
    auto const dt_y    = abs(a->center.y   - b->center.y)      * 2; // midpoint delta y
    auto const cWidth  = (a->halfLengths.x + b->halfLengths.x) * 2; // combined width
    auto const cHeight = (a->halfLengths.y + b->halfLengths.y) * 2; // combined height
    return dt_x < cWidth  and  dt_y < cHeight;
}

[[nodiscard]]
bool CollisionManager::intersects( Sphere const *a, Box const *b ) const noexcept {
    // sphere center
    auto const &s = a->centerRadius;
    // box center
    auto bc = glm::vec2( b->center.x, b->center.y );
    // distance between box and sphere centers
    auto dtX = abs(s.x - bc.x);
    auto dtY = abs(s.y - bc.y);
    // check if they're too far apart to intersect
    if ( dtX > (b->halfLengths.x + s.w)  or  dtY > (b->halfLengths.y + s.w) )
        return false;
    // check if they're too close not to intersect
    if ( dtX <= b->halfLengths.x  or  dtY <= b->halfLengths.y )
        return true;
    // handle the corner cases:
    auto sqrVertDist = sqr(dtX - b->halfLengths.x) + sqr(dtY - b->halfLengths.y);
    return sqr(s.w) > sqrVertDist;
}

[[nodiscard]]
inline bool CollisionManager::intersects( Box const *a, Sphere const *b ) const noexcept {
    return intersects(b,a);
}

[[nodiscard]]
bool CollisionManager::intersects( Sphere const *s1, Sphere const *s2 ) const noexcept {
    auto const &a = s1->centerRadius;
    auto const &b = s2->centerRadius;
    return sqr(a.x-b.x) + sqr(a.y-b.y)  <  sqr(a.w + b.w);
}
 
float lerp( float src, float dest, float fac ) noexcept {
    return (1.0f - fac) * src + fac * dest;
}

glm::vec4 lerp( glm::vec4 const &src, glm::vec4 const &dest, float fac ) noexcept {
    return glm::vec4( lerp( src.x, dest.x, fac ),
                      lerp( src.y, dest.y, fac ),
                      lerp( src.z, dest.z, fac ),
                      lerp( src.w, dest.w, fac ) );
}

Box lerp( Box const &src, Box const &dest, float fac ) noexcept {
    return { lerp( src.center,      dest.center,      fac ),
        lerp( src.halfLengths, dest.halfLengths, fac ) };
}

Sphere lerp( Sphere const &src, Sphere const &dest, float fac ) noexcept {
    return { lerp( src.centerRadius, dest.centerRadius, fac ) };
}

// possibilities: cull distant objects based off of a preliminary spatial eligibility test
//                check for potential bounds breaches by lerp-integrating the object's delta state
void CollisionManager::update() { /// yay for combinatoric explosions /s
    // test each mobile hitbox for intersections:
    for ( auto &m : _mobileBoxes ) {
        auto &mObject = *(m.object);
        // test against all other mobile hitboxes (that belong to another object)
        for ( auto &other : _mobileBoxes ) {
            auto &otherObject = *(other.object);
            if ( mObject != otherObject  and  intersects( m.hitbox, other.hitbox ) ) {
                m.object->collide( m.id, other.id, otherObject, *this );
                otherObject.collide( other.id, m.id, *(m.object), *this );
            }
        }
        // test against all mobile hitspheres (that belong to another object)
        for ( auto &other : _mobileSpheres ) {
            auto &otherObject = *(other.object);
            if ( mObject != otherObject  and  intersects( m.hitbox, other.hitsphere ) ) {
                m.object->collide( m.id, other.id, otherObject, *this );
                otherObject.collide( other.id, m.id, *(m.object), *this );
            }
        }
        // test against all static environment hitboxes
        for ( auto &environment : _staticBoxes ) {
            auto &environmentObject = *(environment.object);
            if ( intersects( m.hitbox, environment.hitbox ) ) {
                m.object->collide( m.id, environment.id, environmentObject, *this );
                environmentObject.collide( environment.id, m.id, *(m.object), *this );
            }
        }
        // test against all static environment hitspheres
        for ( auto &environment : _staticSpheres ) {
            auto &environmentObject = *(environment.object);
            if ( intersects( m.hitbox, environment.hitsphere ) ) {
                m.object->collide( m.id, environment.id, environmentObject, *this );
                environmentObject.collide( environment.id, m.id, *(m.object), *this );
            }
        }
    }
    // now repeat the process for all the mobiles hitspheres:
    for ( auto &m : _mobileSpheres ) {
        auto &mObject = *(m.object);
        // test against all mobile hitspheres (that belong to another object)
        for ( auto &other : _mobileSpheres ) {
            auto &otherObject = *(other.object);
            if ( mObject != otherObject  and  intersects( m.hitsphere, other.hitsphere ) ) {
                mObject.collide( m.id, other.id, otherObject, *this );
                otherObject.collide( other.id, m.id, *(m.object), *this );
            }
        }
        // test against all mobile hitboxes (that belong to another object)
        for ( auto &other : _mobileBoxes ) {
            auto &otherObject = *(other.object);
            if ( *(m.object) != otherObject  and  intersects( m.hitsphere, other.hitbox ) ) {
                mObject.collide( m.id, other.id, otherObject, *this );
                otherObject.collide( other.id, m.id, *(m.object), *this );
            }
        }
        // test against all static environment hitspheres
        for ( auto &environment : _staticSpheres ) {
            auto &environmentObject = *(environment.object);
            if ( intersects(m.hitsphere, environment.hitsphere ) ) {
                mObject.collide( m.id, environment.id, *(environment.object), *this );
                environmentObject.collide( environment.id, m.id, *(m.object), *this );
            }
        }
        // test against all static environment hitboxes
        for ( auto &environment : _staticBoxes ) {
            auto &environmentObject = *(environment.object);
            if ( intersects( m.hitsphere, environment.hitbox ) ) {
                mObject.collide( m.id, environment.id, *(environment.object), *this );
                environmentObject.collide( environment.id, m.id, *(m.object), *this );
            }
        }
    }
}
