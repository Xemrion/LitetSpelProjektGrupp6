#pragma once

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

// TODO: put global constants and config values in a global namespace
                                                 // {3.0f, 3.0f, 0.5f}
struct BLOB_ANIM_AMPLITUDE { static constexpr float X=2.4f, Y=1.7f, Z=.8f; };

float constexpr GRAVITY_CONSTANT   =  150.0f; // 100.0f
float constexpr MAX_SPEED          =  100.0f; // temp
float constexpr FLOAT_REDUCTION    =     .05f; // lower = more reduction
float constexpr DEFAULT_FRICTION   =     .5f; // higher = less
float constexpr PLAYER_JUMP_FORCE  = 4000.0f;
float constexpr PLAYER_MASS        =   50.0f;
float constexpr PLAYER_SPEED       =  200.0f;
bool  constexpr SHOW_HITBOXES      =   false;

// cooldown constants
float constexpr JUMP_CD  = .3f;
float constexpr POWER_CD = .2f;
float constexpr SHOOT_CD = .5f;

enum class ColliderType { player,
                          enemy,
                          level_goal,
                          platform,
                          wall,
                          powerup_bouncy,
                          powerup_sticky,
                          blob };

// Adding more requires change in PowerUp class
enum class PowerType { none,
	                   bouncy,
	                   heavy,
	                   sticky };

// Colours
const glm::vec4 red		= glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 green	= glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 blue	= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 yellow	= glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 magenta	= glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 purple	= glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
const glm::vec4 cyan	= glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 black	= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 white	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 grey    = glm::vec4(0.77f, 0.77f, 0.77f, 1.0f);
const glm::vec4 olive	= glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
