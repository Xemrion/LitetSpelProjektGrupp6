#include"DamagePlatform.h"

DamagePlatform::DamagePlatform( Box box, float friction ): Platform( box, friction ) {}
DamagePlatform::~DamagePlatform() {}
void DamagePlatform::collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept {}
