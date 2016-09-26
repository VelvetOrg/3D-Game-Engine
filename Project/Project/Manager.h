//Include guards
#pragma once

//Needs to have glfw and glew
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//Engine
#include <Engine\Camera.h>
#include <Engine\GameObject.h>
#include <Engine\MeshRenderer.h>

#include <Engine\Collider.h>
#include <Engine\Rigidbody.h>
#include <Engine\Physics.h>

//Enum shows all the possible states of the program
enum programState { Starting, Running, Closing };

class Manager
{
	//All methods control the flow of the program
public:
	//Constructor, sets state
	Manager() { state = programState::Starting; }

	void init(); //Setup all of the program
	void early(); //Start of update
	void input(); //Get input from user
	void logic(); //Main game code and logic
	void draw(); //Draw the game using engine
	void late(); //Any post drawing things
	void quit(); //Close and clean up memory

	//Stores the state of the program
	programState state;

private:
	//Needs a camera
	Camera cam;

	//Testing
	GameObject* boxes;
	GameObject torus, tor2;

	const int LEVEL_WIDTH = 25;
	const int LEVEL_HEIGHT = 25;
};