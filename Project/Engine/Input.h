//Include guards
#pragma once

//STD
#include <unordered_map>
#include <map>

//Uses vector's
#include <glm\vec2.hpp>

//Needs GLFW
#include <GLFW\glfw3.h>

//Put in namespace instead of static class
namespace Input
{
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

	//Stores the mouse position
	glm::vec2 mousePos(0.0f, 0.0f);
	bool movedMouse = false;

	//A hash map of key events where:
	//First: keycode
	//Second: key data
	std::unordered_map<int, keyEvent> _keyMap;

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

	//A hash map of mouse button events
	//First: button
	//Second: mouseEvent
	std::unordered_map<int, mouseEvent> _mouseMap;

	//Called by GLFW when a keyboard event occurs
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//Add key data to map
		keyEvent result(key, scancode, action, mods);

		_keyMap[key] = result;
	}

	//Called by GLFW when the mouse moves
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		//Set
		mousePos.x = (float)xpos;
		mousePos.y = (float)ypos;

		movedMouse = true;
	}

	//Locks the cursor position
	static void lockCustorToPos(GLFWwindow* win, glm::vec2 pos)
	{
		glfwSetCursorPos(win, (double)pos.x, (double)pos.y);
	}

	//Called when a mouse button is clicked
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
	{
		//Set unordered_map position
		mouseEvent result(button, action, mods);
		_mouseMap[button] = result;
	}

	//Gets keys
	static keyEvent getKey(int keycode)
	{
		//Check if item exists first
		auto it = _keyMap.find(keycode);

		//Return data
		if (it != _keyMap.end()) return it->second;
		else return keyEvent();
	}

	//Get mouse button
	static mouseEvent getMouseClick(int button)
	{
		//Chcek if it is in the map
		auto it = _mouseMap.find(button);

		//Return data
		if (it != _mouseMap.end()) return it->second;
		else return mouseEvent();
	}

	//Input updates at the end of the frame
	static void update()
	{
		//No longer moving mouse
		movedMouse = false;

		//Clean out mouse events
		for (auto m : _mouseMap)
		{
			//Change
			if (m.second.down) m.second.down = false;
			if (m.second.up) m.second.up = false;

			//Update
			_mouseMap[m.first] = m.second;
		}

		//Clean out all down and release events
		for (auto k : _keyMap)
		{
			//Modify single shot events
			if (k.second.down) k.second.down = false;
			if (k.second.released) k.second.released = false;

			//Set
			_keyMap[k.first] = k.second;
		}
	}
}