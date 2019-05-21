#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

// TODO: put global constants and config values in a global namespace

char const * const LEVELS[] = { "test2.png",
                                "test3.png",
                                "test.png"  };
size_t constexpr NUMBER_OF_LEVELS = sizeof(LEVELS)/sizeof(*LEVELS);

float constexpr PHYSICS_TIME_STEP    = 0.001f;

// debug constants
bool  constexpr SHOW_HITBOXES        = false;

// player constants                                // {3.0f, 3.0f, 0.5f}
struct BLOB_ANIM_AMPLITUDE { static constexpr float X=2.4f, Y=1.7f, Z=.8f; };
float constexpr PLAYER_JUMP_FORCE    = 4000.00f;
float constexpr PLAYER_MASS          =   50.00f;
float constexpr PLAYER_SPEED         =  200.00f;

// movement constants
float constexpr GRAVITY_CONSTANT     =  150.00f;
float constexpr MAX_SPEED            =  100.00f;
float constexpr FLOAT_REDUCTION      =     .05f; // lower = more reduction
float constexpr DEFAULT_FRICTION     =     .90f; // higher = less

// door
float constexpr DOOR_MOVEMENT_SPEED  = .5f; // higher = faster

// blob constants
float constexpr BLOB_SHOOT_SPEED     =  150.00f;
float constexpr BLOB_RECALL_SPEED    =  800.00f;
float constexpr BLOB_ACTIVE_RADIUS   =    2.00f;
float constexpr BLOB_INACTIVE_RADIUS =    2.00f;

// cooldown constants
float constexpr  JUMP_CD = .3f;
float constexpr POWER_CD = .2f;
float constexpr SHOOT_CD = .5f;

// level
size_t constexpr LEVEL_SCENE_INITIAL_RESERVATIONS = 32;

// colour constants
const glm::vec4 red		= glm::vec4( 1.00f, 0.00f, 0.00f, 1.0f );
const glm::vec4 green	= glm::vec4( 0.00f, 1.00f, 0.00f, 1.0f );
const glm::vec4 blue	= glm::vec4( 0.00f, 0.00f, 1.00f, 1.0f );
const glm::vec4 yellow	= glm::vec4( 1.00f, 1.00f, 0.00f, 1.0f );
const glm::vec4 magenta	= glm::vec4( 1.00f, 0.00f, 1.00f, 1.0f );
const glm::vec4 purple	= glm::vec4( 0.50f, 0.00f, 0.50f, 1.0f );
const glm::vec4 cyan	= glm::vec4( 0.00f, 1.00f, 1.00f, 1.0f );
const glm::vec4 black	= glm::vec4( 0.00f, 0.00f, 0.00f, 1.0f );
const glm::vec4 white	= glm::vec4( 1.00f, 1.00f, 1.00f, 1.0f );
const glm::vec4 grey    = glm::vec4( 0.77f, 0.77f, 0.77f, 1.0f );
const glm::vec4 olive	= glm::vec4( 0.50f, 0.50f, 0.50f, 1.0f );

enum class ColliderType { player,
                          enemy,
                          level_goal,
                          platform,
                          wall,
                          gate,
                          button,
                          powerup_bouncy,
                          powerup_sticky,
                          powerup_heavy,
                          powerup_none,
                          blob };

// Adding more requires change in PowerUp class
enum class PowerType { none,
                       bouncy,
                       heavy,
                       sticky };


// Use player.status as index
const glm::vec4 PowerTypeColor[] = {
    glm::vec4( 0.85f, 0.05f, 0.75f, 0.0f ),
    glm::vec4( 1.00f, 0.75f, 0.05f, 0.0f ),
    glm::vec4( 0.05f, 0.05f, 1.00f, 0.0f ),
    glm::vec4( 0.75f, 0.75f, 0.75f, 0.1f )
};
