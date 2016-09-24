//Header
#include "Input.h"
#include "Console.h"
#include <string>

/* -- Callbacks -- */

//Create some temp callback functions
void mouseClickCallbackWrapper(GLFWwindow* w, int b, int a, int m) { Input.mouseClickCallback(w, b, a, m); }
void keyCallbackWrapper(GLFWwindow* w, int k, int s, int a, int m) { Input.keyCallback(w, k, s, a, m); }
void cursorCallbackWrapper(GLFWwindow* w, double x, double y)      { Input.cursorCallback(w, x, y); }

//Calls the callback functions
void cInput::initCalbacks(GLFWwindow* window, float winwidth, float winheight)
{
	glfwSetCursorPos(window, winwidth, winheight);
	Input.cursorCallback(window, winwidth, winheight);

	//Make sure events are passed though engine input manager
	glfwSetKeyCallback(window, keyCallbackWrapper);
	glfwSetCursorPosCallback(window, cursorCallbackWrapper);
	glfwSetMouseButtonCallback(window, mouseClickCallbackWrapper);
}

//Called by GLFW when a keyboard event occurs
void cInput::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Add key data to map
	keyEvent result(key, scancode, action, mods);
	Input._key_map[key] = result;
}

//Called by GLFW when the mouse moves
void cInput::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	//Set
	Input._mouse_pos.x = (float)((xpos)* 1);
	Input._mouse_pos.y = (float)((ypos)* 1);

	_moved_mouse = true;
}

//Getter
glm::vec2 cInput::getmousePos() { return Input._mouse_pos; }

//Called when a mouse button is clicked
void cInput::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	//Set unordered_map position
	mouseEvent result(button, action, mods);
	Input._mouse_map[button] = result;
}

//Input updates at the end of the frame
void cInput::update()
{
	//No longer moving mouse
	_moved_mouse = false;

	//Clean out mouse events
	for (auto m : Input._mouse_map)
	{
		//Change
		if (m.second.down) m.second.down = false;
		if (m.second.up) m.second.up = false;

		//Update
		Input._mouse_map[m.first] = m.second;
	}

	for (auto k : Input._key_map)
	{
		//Modify single shot events
		if (k.second.down) k.second.down = false;
		if (k.second.released) k.second.released = false;

		//Set
		Input._key_map[k.first] = k.second;
	}
}

//Generic functions

//Gets keys
keyEvent cInput::getKey(int keycode)
{
	//Check if item exists first
	auto it = Input._key_map.find(keycode);

	//Return data
	if (it != Input._key_map.end()) { return it->second; }
	else { return keyEvent(); }
}

//Get mouse button
mouseEvent cInput::getMouseClick(int button)
{
	//Chcek if it is in the map
	auto it = Input._mouse_map.find(button);

	//Return data
	if (it != Input._mouse_map.end()) return it->second;
	else return mouseEvent();
}

//Stick to position on screen
void cInput::lockCustorToPos(GLFWwindow* win, glm::vec2 pos)
{
	glfwSetCursorPos(win, (double)pos.x, (double)pos.y);
}