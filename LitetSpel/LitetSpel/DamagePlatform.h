#pragma once

#include "Platform.h"

class DamagePlatform : public Platform {
public:
    DamagePlatform( Box box, float friction=DEFAULT_FRICTION );
    virtual ~DamagePlatform();
    virtual void collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept override;
};
