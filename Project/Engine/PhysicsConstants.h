//Protection
#pragma once

//Types
#include <glm\glm.hpp>

//This contains a whole lot of default values to be used by the physics engine
//This can easily be overwirrten, but are optional paramters
static class cPhysicsConstants
{
public:
	//These are pretty self explainitory
	const float GRAVITY = 9.8f;
	const float MASS = 10.0f;
	const bool KINEMATIC = false;
	const float BOUNCE = 0.0f;
	const float FRICTION = 0.6f;
	const glm::vec3 CUBE_SIZE = glm::vec3(1, 1, 1);
	const glm::vec3 CENTER = glm::vec3(0, 0, 0);

} PhysicsConstants;