//Include guards
#pragma once

//Needs GLFW
#include <GLFW\glfw3.h>

//STD
#include <unordered_map>

//Uses vector's
#include <glm\vec2.hpp>

//Pre define classes
class keyEvent;
class mouseEvent;

//Put in namespace instead of static class
static class cInput
{
public:
	//Setups the callbacks
	void initCalbacks(GLFWwindow* window, float winwidth, float winheight);

	//Input updates at the end of the frame
	void update();

	//Generic functions:
	void lockCustorToPos(GLFWwindow* win, glm::vec2 pos); //Locks the cursor position
	mouseEvent getMouseClick(int button); //Get mouse button
	keyEvent getKey(int keycode); //Gets keys

	//Simple getters
	glm::vec2 getmousePos();

	//Note: All callbacks must be static
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); //Called by GLFW when a keyboard event occurs
	void mouseClickCallback(GLFWwindow* window, int button, int action, int mods); //Called when a mouse button is clicked
	void cursorCallback(GLFWwindow* window, double xpos, double ypos); //Called by GLFW when the mouse moves

private:
	//Stores the mouse position
	glm::vec2 _mouse_pos;
	bool _moved_mouse;

	//A hash map of key events where:
	//First: keycode
	//Second: key data
	std::unordered_map<int, keyEvent> _key_map;

	//A hash map of mouse button events
	//First: button
	//Second: mouseEvent
	std::unordered_map<int, mouseEvent> _mouse_map;

} Input;

//Holds key events
//Holds data about a key event
class keyEvent
{
public:
	//Properties
	int key;
	int scancode;
	int action;
	int mods;

	bool held;
	bool down;
	bool released;

	//Set on constructor
	keyEvent() : key(-1), scancode(0), action(0), mods(0) { held = false; down = false; released = false; }

	keyEvent(int k, int s, int a, int m) : key(k), scancode(s), action(a), mods(m)
	{
		if (action == GLFW_PRESS) { down = true; released = false; held = true; }
		if (action == GLFW_REPEAT) { down = false; released = false;  held = true; }
		if (action == GLFW_RELEASE) { down = false; released = true;  held = false; }
	}
};

//States of the mouse
class mouseEvent
{
public:
	//Specific
	int button;
	int action;
	int mods;

	//States
	bool up;
	bool down;
	bool held;

	//Constructors
	mouseEvent() : button(-1), action(-1), mods(0) { up = false; down = false; held = false; }
	mouseEvent(int b, int a, int m) : button(b), action(a), mods(m)
	{
		if (action == GLFW_PRESS) { down = true; up = false; held = true; }
		if (action == GLFW_RELEASE) { down = false; up = true; held = false; }
		//else { down = false; up = false; }
	}
};