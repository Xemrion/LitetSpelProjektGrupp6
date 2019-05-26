#include <cassert>
#include <functional>
// TODO: rename to Game.cpp|h
#include "game.h"
#include "Globals.h"

char const * const getNextLevelName() {
    static size_t currentLevelNo = 0;
    if ( currentLevelNo < NUMBER_OF_LEVELS )
        return LEVELS[currentLevelNo++];
    else {
        assert(false and "You beat the game!"); // TODO: handle victory properly
        return *LEVELS;
    }
}

void Game::loadLevel() { // temp
    level = levelLoader.load( getNextLevelName() );
    level->getPlayer().attachInput(keyboard, mouse);
    level->getPlayer().attachCamera( camera );
    state = State::level;
    level->setLevelChangeFunction( std::bind(&Game::loadLevel, this) );
}

Game::Game( KeyboardInput &keyboard, MouseInput &mouse, Graphics &graphics ):
    physicsSimTime ( .0        ),
    time           ( .0        ),
    level          ( nullptr   ),
    keyboard       ( &keyboard ),
    mouse          ( &mouse    ),
    graphics       ( &graphics ),
    levelLoader    (  graphics )
{
    assert( this->keyboard and "Keyboard mustn't be null!" );
    assert( this->mouse    and "Mouse mustn't be null!"    );
    assert( this->graphics and "Graphics mustn't be null!" );
    menuLoad();
}

void Game::init() noexcept {
    loadLevel();
}

Level& Game::getLevel() noexcept {
    return *level;
}

Game::State Game::getState() const noexcept {
    return state;
}

void Game::update( double dt_s )  {
    level->onFrameStart(); // TEMP: clears dynamic boxes & spheres
    Game::time += dt_s; // NOTE: Should be redundant (but would require some revamping)
    updateInput();
    updateLogic(dt_s);
    updatePhysics();
    updateAnimations(dt_s);
    updateGraphics();
}

// Call first of all per frame updates
void Game::updateInput() {
    level->getPlayer().updateInput();
}

void Game::updateLogic( double dt_s ) {
    for ( auto &e : level->getScene() )
        e->updateLogic(dt_s);
}

// Catches up the physics simulation time to the actual game time
// Call last of all logic updates (but before graphics)
void Game::updatePhysics() {
    while ( physicsSimTime < Game::time ) {
        for ( auto &e : level->getScene() ) {
            e->updatePhysics(PHYSICS_TIME_STEP);
        }
		level->getCollisionManager().update();
        physicsSimTime += PHYSICS_TIME_STEP ;
	}
}

void Game::updateAnimations( double dt_s ) noexcept {
    for ( auto &e : level->getScene() )
        e->updateAnimations( dt_s, Game::time );
}

// TODO: SphereHandle gfxManager::requestSphere();
// TODO: BoxHandle    gfxManager::requestBox();

// Call after all other per frame updates
void Game::updateGraphics() { // TODO: refactor into level?
    // TODO: make it update elements instead of repopulate vectors?
    for ( auto const &e : level->getScene() ) {
        e->updateGraphics();
        auto const &gfx = e->getRepresentation();
        auto platform = dynamic_cast<Platform const *>( e.get() );
        if ( platform )
            ; // do nothing; statics are already loaded!
        else if ( std::holds_alternative<IRepresentable::Boxes>(gfx) )
            for ( auto const *p : std::get<IRepresentable::Boxes>(gfx) )
                level->getDynamicBoxes().push_back( *p );
        else if ( std::holds_alternative<IRepresentable::Spheres>(gfx) )
            for ( auto const *p : std::get<IRepresentable::Spheres>(gfx) )
                level->getSpheres().push_back( *p );
        e->updateHitboxes();
    }
    if constexpr ( SHOW_HITBOXES )
        showHitboxes();
}

void Game::showHitboxes() noexcept {
    auto &dynamics = level->getDynamicBoxes();
    auto &statics  = level->getStaticBoxes();
    for ( auto const &e : level->getScene() ) {
        auto platform = dynamic_cast<Platform const *>( e.get() );
        if ( platform )
            for ( auto const &hitbox : e->getHitboxes() )
                statics.push_back(hitbox.box);
        else for ( auto const &hitbox : e->getHitboxes() )
            dynamics.push_back(hitbox.box);
    }
}

void Game::menuLoad() {
    menuBg  = { {    .0f,    .0f,  5.0f,  .0f },   // position
                { 100.0f, 100.0f,   .0f,  .0f },   // dimensions
                {    .0f,    .0f,  1.0f,  .0f } }; // color

    menuYes = { {  30.0f,  10.0f,   .0f,  .0f },   // position
                {  20.0f,  20.0f,   .0f,  .0f },   // dimensions
                {    .0f,   1.0f,   .0f,  .0f } }; // color
                                   
    menuNo  = { { -30.0f,  10.0f,   .0f,  .0f },   // position
                {  20.0f,  20.0f,   .0f,  .0f },   // dimensions
                {   1.0f,    .0f,   .0f,  .0f } }; // color

    state = State::menu;
}
