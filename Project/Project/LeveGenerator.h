//Portection
#pragma once

//Needs the engine fils to create the obejcts
#include <Engine\GameObject.h>
#include <Engine\Texture.h>
#include <Engine\Loader.h>
#include <Engine\Mesh.h>

//For colours
#include <glm\glm.hpp>

//This is a class used bu the manager to generate static gameobjects in the scene
static class cLevelGenerator
{
public:
	//Creates the level made up of an array of gameobjects
	GameObject* generate();

protected:
	//Texture path
	const char* TEX_PATH = "../../Assets/Prefs/Grid.png";

	//Holds mesh data paths
	const char* HUGE_BLOCK_PATH =		"../../Assets/Prefs/HugeBlock.obj";
	const char* LARGE_BLOCK_PATH =		"../../Assets/Prefs/LargeBlock.obj";
	const char* BLOCK_PATH =			"../../Assets/Prefs/Block.obj";
	const char* SMALL_BLOCK_PATH =		"../../Assets/Prefs/SmallBlock.obj";
	const char* TINY_BLOCK_PATH =		"../../Assets/Prefs/TinyBlock.obj";
	const char* FLOOR_PATH =			"../../Assets/Prefs/Floor.obj";
	const char* COIN_PATH =				"../../Assets/Prefs/Coin.obj";
	const char* RAMP_PATH =				"../../Assets/Prefs/Ramp.obj";
	const char* STEPS_PATH =			"../../Assets/Prefs/Steps.obj";
	const char* PILLAR_PATH =			"../../Assets/Prefs/Pillar.obj";
	const char* LARGE_PLATFORM_PATH =	"../../Assets/Prefs/PlatformLarge.obj";
	const char* MEDIUM_PLATFORM_PATH =	"../../Assets/Prefs/PlatformMedium.obj";
	const char* SMALL_PLATFORM_PATH =	"../../Assets/Prefs/PlatformSmall.obj";

	//Defines the colour pallet
	const glm::vec3 COL_WHITE =		glm::vec3(255 / 255, 255 / 255, 255 / 255);
	const glm::vec3 COL_GREY =		glm::vec3( 32 / 255,  44 / 255,  55 / 255);
	const glm::vec3 COL_BLACK =		glm::vec3( 25 / 255,  29 / 255,  34 / 255);
	const glm::vec3 COL_RED =		glm::vec3(255 / 255,   0 / 255, 102 / 255);
	const glm::vec3 COL_ORANGE =	glm::vec3(255 / 255, 127 / 255,  51 / 255);
	const glm::vec3 COL_YELLOW =	glm::vec3(255 / 255, 246 / 255,   0 / 255);
	const glm::vec3 COL_PURPLE =	glm::vec3(184 / 255,  60 / 255, 130 / 255);
	const glm::vec3 COL_CYAN =		glm::vec3(  0 / 255, 204 / 255, 204 / 255);
	const glm::vec3 COL_GREEN =		glm::vec3( 25 / 255, 227 / 255, 177 / 255);

} LevelGenerator;