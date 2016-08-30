//STD
#include <unordered_map>
#include <map>

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

	//A hash map of key events where:
	//First: keycode
	//Second: key data
	std::unordered_map<int, keyEvent> _keyMap;

	//Called by GLFW when a keyboard event occurs
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//Add key data to map
		keyEvent result(key, scancode, action, mods);

		_keyMap[key] = result;
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

	//Input updates at the end of the frame
	static void update()
	{
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