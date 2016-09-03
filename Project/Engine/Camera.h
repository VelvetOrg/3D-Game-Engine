//Include guards
#pragma once

//Needs glm for matricies and maths
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\matrix.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>

//Needs transform component
#include "Transform.h"

//One can create an object of the camera
class Camera
{
public:
	//Holds a transform
	Transform transform;

	//Is it an orthographic or perspective camera?
	enum cameraType { Perspective, Orthographic };
	cameraType type;

	//Properties of a camera
	float fov;
	float near_clipping;
	float far_clipping;

	//Takes params and set members
	Camera() :
		transform(Transform()),
		type(cameraType::Perspective),
		near_clipping(0.1f),
		far_clipping(100.0f),
		fov(60) {}

	void Init(glm::vec3 pos, glm::vec3 rot, cameraType t = cameraType::Perspective, float f = 60, float n = 0.1f, float c = 100.0f)
	{
		transform.position = pos;
		transform.rotation = rot;
		near_clipping = n;
		far_clipping = c;
		type = t;
		fov = f;
	}

	//Will find the model view projection matrix for this camera
	glm::mat4 getViewProjection()
	{
		//Find projection matrix
		if (type == cameraType::Perspective) _projection_mat = glm::perspective(glm::radians(fov), (float)(1280 / 720), near_clipping, far_clipping);
		if (type == cameraType::Orthographic) _projection_mat = glm::ortho(-fov, fov, -fov, fov, near_clipping, far_clipping);

		//Create the camera matrix
		//_view_mat = glm::lookAt(position, rotation, glm::vec3(0, 1, 0));
		_view_mat = glm::translate(_view_mat, transform.position);

		//Done
		_current_MVP = _projection_mat * _view_mat;

		//Reset for next call - temp
		transform.position = glm::vec3(0, 0, 0);
		transform.rotation = glm::vec3(0, 0, 0);

		//Return
		return _current_MVP;
	};

	//Member vars
private:
	glm::mat4 _current_MVP;

	glm::mat4 _projection_mat;
	glm::mat4 _view_mat;
};