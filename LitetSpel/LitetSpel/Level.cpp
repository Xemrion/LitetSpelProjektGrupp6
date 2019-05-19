#include "Level.h"
#include "LevelGoal.h"
#include "Platform.h"

// when to use: when loading a level or dynamically spawning entities in a level.
//
// usage:
//            LevelData level = /*...*/;
//            // ...
//
//            // either:
//            auto enemy = std::make_unique<Enemy>(/*some_constructor_params*/);
//            level.add( {10.0f, .0f, .0f}, std::move(enemy) }; // transfer ownership to level
//
//            // or:
//            level.add( {10.0f, .0f, .0f}, std::make_unique<Enemy>(/*some_constructor_params*/) );
//
// TODO: make it so that not all IObjects are IColliders and account for the edge case where they're not
void Level::add(std::unique_ptr<IObject> object) noexcept {
    Player *player = dynamic_cast<Player*>(object.get());
    if ( player ) {
        if ( this->player )
            assert( false and "The level already contains a player!" );
        else this->player = player;
    }
    LevelGoal *goal = dynamic_cast<LevelGoal*>(object.get());
    if (goal) {
        goal->setWinCallback( std::bind(&Level::notifyOfVictory, this) );
    }

    // add statics:
    Platform *platform = dynamic_cast<Platform*>(object.get());
    if ( platform )
        staticBoxes.push_back( platform->getBox() );

    collisionManager.add( *object );
    scene.push_back({ std::move(object) });
}

Level::Level() {}

void Level::onFrameStart() {
    // TODO: let IRepresentables have mutable handles to a GraphicsManager instead!
    dynamicBoxes.clear();
    spheres.clear();
}

// when to use: when a level in an entity is destroyed by game logic (e.g. an enemy dies)
//
// usage:
//     level.remove( *this );
bool Level::remove( IObject &target ) noexcept {
    // TODO: handle Player edge case
    auto unary_predicate = [&target]( auto const &e ) { 
        return *e == target;
    };
    return std::remove_if( scene.begin(), scene.end(), unary_predicate ) != scene.end() 
        or collisionManager.remove(target);
}

Player& Level::getPlayer() noexcept {
    return *player;
}

CollisionManager & Level::getCollisionManager() noexcept {
    return collisionManager;
}

Level::Boxes & Level::getDynamicBoxes() noexcept {
    return dynamicBoxes;
}

Level::Boxes & Level::getStaticBoxes() noexcept {
    return staticBoxes;
}


Level::Spheres & Level::getSpheres() noexcept {
    return spheres;
}

Level::SceneEntries & Level::getScene() noexcept {
    return scene;
}

void Level::notifyOfVictory() noexcept {
    getPlayer().win();
}
