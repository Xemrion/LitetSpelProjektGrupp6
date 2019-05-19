#pragma once

#include <vector>
#include "../../INCLUDE/glm/glm/glm.hpp"
#include "geometry.h"
#include "Interfaces.h"
#include "Globals.h"

class CollisionManager {
public:
    CollisionManager();
    void add( ICollider &) noexcept;
    bool remove( ICollider &target ) noexcept;
    [[nodiscard]] static bool intersect( Box const &a, Box const &b ) noexcept;
    // checks if any of the mobiles have collided, and if so calls the appropriate collide function
    void update() noexcept;
private:
    std::vector<ICollider*> colliders;
};


/*
// class ISlider
// {
// public:
//     ISlider() {}
//     virtual ~ISlider() {}
// 
//     void update(double dt_s) noexcept {
//         
//         if ( _target.position  != _path[_i] )
//             ; // TODO: dt_s * move _speed along vector between _target.pos och *_current_target
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
//     virtual void update( double dt_s ) {
//         _timeLeft -= dt_s;
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
//     Box         _hitboxButton; // vid kollision, toggle() och höj/sänk _button
//     Box         _base;
//     Box         _button;
// };



LevelGoal::LevelGoal( CollisionManager &colManager, Box bounds, TriggerCallback cb ):
_bounds(bounds),
_triggerCallback(cb)
{
colManager.register_entry(*this, CollisionId::level_goal, _bounds, true );
}

void LevelGoal::collide( CollisionId  ownHitbox,
CollisionId  otherHitbox,
Bo          &other ) noexcept {
if (    otherHitbox == player_top
or otherHitbox == player_bottom
or otherHitbox == player_left
or otherHitbox == player_right)
{
_triggerCallback();
// TODO:
//    tone and blur screen?
//    display text?
//    wait for input?
//    load next level?
}
}





















*/