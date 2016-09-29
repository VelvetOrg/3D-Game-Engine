//Include guards
#pragma once

//For time
#include <GLFW\glfw3.h>

//Make static
static class cTime
{
private:
	//Values to return
	int frames_passed = 0;
	float seconds = 0.0f;
	float delta = 0.0f;
	float time = 0.0f;
	float fps = 0.0f;

	//Calculating delta time
	float last_frame = 0.0f;
	float current_frame = 0.0f;

public:
	//Called at the start of the main loop
	void update()
	{
		//Find delta time
		Time.current_frame = (float)glfwGetTime();
		Time.delta = Time.current_frame - Time.last_frame;

		//Update time
		Time.time = (float)glfwGetTime();

		//Find FPS
		Time.fps = 1 / Time.delta;

		//Done
		Time.frames_passed++;
		Time.seconds += Time.delta;
		Time.last_frame = Time.current_frame;
	}

	//Since all values should be read only they need getters
	float getCurrentFrame() { return Time.current_frame; }
	int getFramesPassed() { return Time.frames_passed; }
	float getLastFrame() { return Time.last_frame; }
	float getSeconds() { return Time.seconds; }
	float getDelta() { return Time.delta; }
	float getTime() { return Time.time; }
	float getFps() { return Time.fps; }

} Time;