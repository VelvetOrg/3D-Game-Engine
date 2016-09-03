//Include guards
#pragma once

//Needs vectos
#include <glm\glm.hpp>

//Stores space attributes about a given GameObject
class Transform
{
public:
	//Temporary
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 pivot;

	//Reset on load
	Transform() : position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), scale(glm::vec3(1, 1, 1)) { pivot = glm::vec3(0, 0, 0); }
	Transform(glm::vec3 p, glm::vec3 r, glm::vec3 s) : position(p), rotation(r), scale(s) { pivot = glm::vec3(0, 0, 0); }
};