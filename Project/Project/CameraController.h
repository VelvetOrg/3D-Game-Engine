//Include guards
#pragma once

//Needs camera
#include <Engine\Camera.h>

//Needs input class

//Can be static but can still be a class
class CamerController
{
public:
	//Constructor
	CamerController(Camera* cam) : _cam_pointer(cam) { }

	//React to inputs
	void Update()
	{
		//Store position of the mouse
		//glm::vec2 m_pos = Input::mousePos;


	}

private:
	//Member vars
	Camera* _cam_pointer; //Points to a camera

	//Stores a copy of the camera rot
	float _horizontal = 3.14f;
	float _vertical = 0.0f;

	//Movement attributes
	float _mouse_speed = 0.001f;
	float _move_speed = 3.0f;
};