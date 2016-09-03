//Include guards
#pragma once

//Needs to have glfw and glew
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//Engine
#include <Engine\Camera.h>
#include <Engine\GameObject.h>
#include <Engine\MeshRenderer.h>
//#include <Engine\SoundManager.h>

//Enum shows all the possible states of the program
enum programState { Starting, Running, Closing };

class Manager
{
	//All methods control the flow of the program
public:
	//Constructor, sets state
	Manager() { state = programState::Starting; }

	void init(); //Setup all of the program
	void clear(); //Start of update
	void input(); //Get input from user
	void logic(); //Main game code and logic
	void draw(); //Draw the game using engine
	void late(); //Any post drawing things
	void quit(); //Close and clean up memory

	//Stores the state of the program
	programState state;

private:
	//Needs a camera
	Camera _cam;

	//Shader attributes
	GLint vertex_pos_location; //Vertex position input
	GLint vertex_col_location; //Vertex colour input

	GLuint model_matrix_projection; //Stores model position matrix
	GLuint view_projection_location; //Stores view and projection matrix location

	//Create a cube game object
	GameObject box;
};