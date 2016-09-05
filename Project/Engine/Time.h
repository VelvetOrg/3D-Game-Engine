//Include guards
#pragma once

//For time
#include <GLFW\glfw3.h>

//Make static
namespace Time
{
	//Values to return
	int framesPassed = 0;
	float delta = 0.0f;
	float time = 0.0f;
	float fps = 0.0f;

	//Calculating delta time
	float last_frame = 0.0f;
	float current_frame = 0.0f;

	//Called at the start of the main loop
	void start()
	{
		//Find delta time
		current_frame = (float)glfwGetTime();
		delta = current_frame - last_frame;

		//Update time
		time = (float)glfwGetTime();

		//Find FPS
		fps = framesPassed / time;

		//Done
		framesPassed++;
		last_frame = current_frame;
	}
}