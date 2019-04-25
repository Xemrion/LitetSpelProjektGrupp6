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

// lasrar
// spikfällor
// pressar
// kraftfält
// knuffblock
// spakar
// knappar


// s = playerSphere.centerRadius

     // mid.X               mid.Y                    halfX       halfY
//*A*/   x= s.x              y=s.y + 0.8*s.w          1.6*s.w     0.2*s.w
//*B*/   x= s.x - 0.8*s.w    y=s.y                    0.2*s.w     2.0*s.w
//*C*/   x= s.x + 0.8*s.w    y=s.y                    0.2*s.w     2.0*s.w
//*D*/   x= s.x              y=s.y - 0.2*s.w          1.6*s.w     1.8*s.w


// class ISlider
// {
// public:
//     ISlider() {}
//     virtual ~ISlider() {}
// 
//     void update(double dt) noexcept {
//         
//         if ( _target.position  != _path[_i] )
//             ; // TODO: dt * move _speed along vector between _target.pos och *_current_target
//         else { // target reached
//             if ( _isMovingForward ) {
//                  ++_i;
//                  if ( _i == _path.size()-1 )
//                      _isMovingForward = false;
//             }
//             else {
//                 --_i;
//                 if ( _i == 0 )
//                     _isMovingForward = true;
//             }
//         }
//     }
// 
// private:
//     bool                    _isMovingForward;
//     size_t                  _i;
//     float                   _speed;
//     IObject                &_target;
//     std::vector<glm::vec2>  _path;
// };
// 
// class IActivable {
// public:
//     IActivable( bool isActive=false ):
//         _isActive(false)
//     {
//         if (isActive) toggle();
//     }
// 
//     virtual ~IActivable() {}
// 
//     virtual bool onActivation()   noexcept {}
//     virtual bool onDeactivation() noexcept {}
// 
//     void toggle() noexcept {
//         _isActive = !_isActive;
//         if ( _isActive )
//             onActivation();
//         else onDeactivation();
//     }
// 
//     bool isActive() const { return _isActive; }
// 
// private:
//     bool _isActive;
// };
// 
// class PeriodicActivator : public IActivable {
// public:
//     PeriodicActivator(IActivable &target, double uptime, double downtime, bool startsActive):
//         IActivable (startsActive),
//         _target    (target),
//         _downtime  (downtime),
//         _uptime    (uptime),
//         _timeLeft  (startsActive? uptime : downtime)
//     {}
// 
//     virtual ~PeriodicActivator() {}
// 
//     virtual bool onActivation()   noexcept { _target.toggle(); } // forward signal
//     virtual bool onDeactivation() noexcept { _target.toggle(); } // forward signal
// 
//     virtual void update( double dt ) {
//         _timeLeft -= dt;
//         if ( _timeLeft < 0 ) {
//             toggle();
//             if ( isActive() )
//                  _timeLeft = _uptime;
//             else _timeLeft = _downtime;
//         }
//     }
// 
// private:
//     IActivable &_target;
//     double _downtime, _uptime, _timeLeft;
// };
// 
// class Lever : public IObject, public IActivable {
//     Lever(IActivable &target, bool isActive=false):
//         IObject(),
//         IActivable(isActive),
//         _target(target)
//     {}
// 
//     virtual ~Lever() {}
// 
//     virtual bool onActivation()   noexcept { _target.toggle(); } // forward signal
//     virtual bool onDeactivation() noexcept { _target.toggle(); } // forward signal
// 
//     // TODO: collide() som kollar PlayerInteraction och kör toggle()
//     // TODO: rotationslogik
// 
// private:
//     IActivable &_target;
//     Sphere _hitbox; // vid kollision, toggle() och rotera _stick
//     Box    _base;
//     Box    _stick;
// };
// 
// class Button : public IObject, public IActivable {
//     Button(IActivable &target, bool isActive=false):
//         IObject(),
//         IActivable(isActive),
//         _target(target)
//     {}
// 
//     virtual ~Button() {}
// 
//     virtual bool onActivation()   noexcept { _target.toggle(); } // forward signal
//     virtual bool onDeactivation() noexcept { _target.toggle(); } // forward signal
// 
//     // TODO: collide() som kollar tung player eller tungt block mot 
// 
// private:
//     IActivable &_target;
//     Sphere      _hitboxButton; // vid kollision, toggle() och höj/sänk _button
//     Box         _base;
//     Box         _button;
// };


// TODO: contemplate placement;
enum CollisionId { player_top,
                   player_bottom,
                   player_left,
                   player_right,
                   platform,
                   wall,
                   powerup_bouncy,
                   powerup_sticky,
                   powerup_heavy,
                   level_goal
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

    virtual void update(double dt) noexcept {}

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
    [[nodiscard]]        bool intersect(    Box const *a,    Box const *b ) noexcept;
    [[nodiscard]]        bool intersect( Sphere const *a,    Box const *b ) noexcept;
    [[nodiscard]] inline bool intersect(    Box const *a, Sphere const *b ) noexcept;
    [[nodiscard]]        bool intersect( Sphere const *a, Sphere const *b ) noexcept;

    // checks if any of the mobiles have collided,
    // and if so, notifies them with the relevant information
    // needed to determine the proper collision resolution.
    void update();

private:
    static auto constexpr sqr = []( auto n ) { return n*n; }; // TODO: refactor?

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
