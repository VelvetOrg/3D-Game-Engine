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
	const float RADIUS = 10;
	const float HEIGHT = 30;
	const float MASS = 10.0f;
	const float DIAMETER = 20;
	const float BOUNCE = 0.0f;
	const float GRAVITY = 9.81f;
	const float FRICTION = 0.6f;
	const bool KINEMATIC = false;
	const glm::vec3 CENTER = glm::vec3(0, 0, 0);
	const glm::vec3 INERTIA = glm::vec3(0, 0, 0);
	const glm::vec3 CUBE_SIZE = glm::vec3(1, 1, 1);

} PhysicsConstants;