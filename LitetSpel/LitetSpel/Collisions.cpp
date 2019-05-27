#include "Collisions.h"
#include <cassert>
#include <algorithm>

bool CollisionObject::operator==(CollisionObject const &other) const noexcept {
    return id == other.id;
}

bool CollisionObject::operator!=(CollisionObject const &other) const noexcept {
    return id != other.id;
}

CollisionManager::CollisionManager() {
    staticBoxes.reserve( 200 );
    mobileBoxes.reserve(  25 );
}

void CollisionManager::registerEntry(CollisionObject &parent, ColliderType id, Box const &hitbox, bool isStatic) noexcept {
    HitboxEntry e = { &parent, &hitbox, id };

	if (isStatic)
        staticBoxes.push_back(e);
	else
        mobileBoxes.push_back(e);
}

// bool CollisionManager::unregisterEntry(CollisionObject const &parent) noexcept {
//     auto unary_predicate = [&parent]( auto const &e ) { return !((e.object) != &parent); };
//     return std::remove_if( staticBoxes.begin(), staticBoxes.end(), unary_predicate ) != staticBoxes.end()
//         or std::remove_if( mobileBoxes.begin(), mobileBoxes.end(), unary_predicate ) != staticBoxes.end();
// }

void CollisionManager::unregisterEntry( CollisionObject const &target ) noexcept {
    auto unary_predicate = [&target]( HitboxEntry const &e ) { return *(e.object) == target; };
    mobileBoxes.erase( std::remove_if( mobileBoxes.begin(), mobileBoxes.end(), unary_predicate ) );
    staticBoxes.erase( std::remove_if( staticBoxes.begin(), staticBoxes.end(), unary_predicate ) );
}


bool CollisionManager::intersect(Box const &a, Box const &b) noexcept {
    float const dx      = fabsf(a.center.x - b.center.x); // midpoint delta x
    float const dy      = fabsf(a.center.y - b.center.y); // midpoint delta y
    float const cWidth  = (a.halfLengths.x + b.halfLengths.x); // combined width
    float const cHeight = (a.halfLengths.y + b.halfLengths.y); // combined height
    return dx < cWidth and dy < cHeight;
}

void CollisionManager::update() noexcept {
    // test each mobile hitbox for intersections:
    for (auto &m : mobileBoxes) {
		// test against all static environment hitboxes
		for (auto &environment : staticBoxes)
			if (intersect((*m.hitbox), (*environment.hitbox)))
				m.object->collide(m.colliderType, environment);
        // test against all other mobile hitboxes (that belong to another object)
        for (auto &other : mobileBoxes)
            if (glm::distance(m.hitbox->center, other.hitbox->center) < 40.0 && *(m.object) != *(other.object) and intersect((*m.hitbox), (*other.hitbox)))
                m.object->collide(m.colliderType, other);
    }
}

void CollisionManager::clean()
{
	for(int i = 0; i < mobileBoxes.size(); i++)
	{
		delete mobileBoxes.at(i).hitbox;
		delete mobileBoxes.at(i).object;
	}
	mobileBoxes = std::vector<HitboxEntry>();
	for (int i = 0; i < staticBoxes.size(); i++)
	{
		delete staticBoxes.at(i).hitbox;
		delete staticBoxes.at(i).object;
	}
	staticBoxes = std::vector<HitboxEntry>();
}
