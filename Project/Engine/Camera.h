//Include guards
#pragma once

//Needs glm for matricies and maths
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\matrix.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GL\glew.h>

//Print
#include <stdio.h>

//Needs transform component
#include "Transform.h"

//One can create an object of the camera
class Camera
{
public:
	//Is it an orthographic or perspective camera?
	enum cameraType { Perspective, Orthographic };
	cameraType type;

	//Holds the vector direction of the camera
	glm::vec3 relativeForward;
	glm::vec3 relativeRight;
	glm::vec3 relativeUp;

	glm::vec3 forward = glm::vec3(0, 0, 0 - 1);
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);

	glm::vec3 transformPos = glm::vec3(0, 0, 5);

	//Rotation
	float pitch; //Up and down
	float yaw; //Left and right

	//Properties of a camera
	float fov;
	float near_clipping;
	float far_clipping;

	//Takes params and set members
	Camera() :
		type(cameraType::Perspective),
		near_clipping(0.1f),
		far_clipping(100.0f),
		fov(60)
	{
		relativeForward = forward;
		relativeRight = right;
		relativeUp = up;
	}

	void Init(glm::vec3 pos, float y = 0.0f, float p = 0.0f, cameraType t = cameraType::Perspective, float f = 60, float n = 0.1f, float c = 100.0f)
	{
		transformPos = pos;
		pitch = p;
		yaw = y;

		near_clipping = n;
		far_clipping = c;
		type = t;
		fov = f;

		relativeForward = forward;
		relativeRight = right;
		relativeUp = up;
	}

	//Holds a transform
	//Transform transform;

	//Will find the model view projection matrix for this camera
	glm::mat4 getViewProjection()
	{
		//Find projection matrix
		if (type == cameraType::Perspective) _projection_mat = glm::perspective(glm::radians(fov), (float)(720 / 720), near_clipping, far_clipping);
		if (type == cameraType::Orthographic) _projection_mat = glm::ortho(-fov, fov, -fov, fov, near_clipping, far_clipping);

		/*Create the camera matrix
		_view_mat = glm::translate(-(transform.pivot - _previous_transform.pivot)) * //Move to the pivot point, so adjustment can be made around a position
		//glm::mat4_cast(glm::quat(transform.rotation - _previous_transform.rotation)) *
		glm::rotate(_view_mat, glm::radians(yaw), glm::vec3(1, 0, 0)) *
		glm::translate(transform.pivot - _previous_transform.pivot) * //Actualy move based on transform.position
		glm::translate(_view_mat, transform.position - _previous_transform.position); //No longer move to privot point
		*/

		//Convert yaw and pitch to a vector
		glm::vec3 temp_front;

		//Use equation
		temp_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		temp_front.y = sin(glm::radians(pitch));
		temp_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		//Normalize sot he greatest value can only be one
		relativeForward = glm::normalize(temp_front);

		//Apply to view matrix
		_view_mat = glm::lookAt(transformPos, transformPos + relativeForward, up);

		//Done
		_current_MVP = _projection_mat * _view_mat;

		//Return
		return _current_MVP;
	};

	//Member vars
private:
	//Matricies
	glm::mat4 _current_MVP;

	glm::mat4 _projection_mat;
	glm::mat4 _view_mat;

	//Needs to store previous values
	Transform _previous_transform;
};