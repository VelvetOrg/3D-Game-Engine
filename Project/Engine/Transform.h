#pragma once
#include <GLM\glm.hpp>
namespace Transform {
	class Transform
	{
	public:
		Transform();
		Transform(glm::fvec3 _position, glm::fvec3 _rotation, glm::fvec3 _scale);
		glm::fvec3 position;
		glm::fvec3 rotation;
		glm::fvec3 scale;
	private:

	};

	Transform::Transform()
	{
		position = glm::fvec3(0, 0, 0);
		rotation = glm::fvec3(0, 0, 0);
		scale = glm::fvec3(0, 0, 0);
	}

	Transform::Transform(glm::fvec3 _position, glm::fvec3 _rotation, glm::fvec3 _scale)
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}
}