//Needs header file
#include "Manager.h"
#include "Externs.h"

//For error handling
#include <Engine\Console.h>

//Create memory for window
GLFWwindow *win;

//Setup all of the program
void Manager::init() 
{
	//Init GLFW
	GLboolean init_status = glfwInit();

	if (!init_status) Console::error("Could not initialize GLFW.");

	//All GLFW error messages go though error class
	glfwSetErrorCallback(Console::glfwError);

	//Create window based on externs
	win = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, GAME_TITLE, NULL, NULL);
	if (!win) Console::error("Could not create the game window.");

	//Make OPEN GL context
	glfwMakeContextCurrent(win);

	//State can now be changed
	state = programState::Running;

	//Test console message
	Console::message("Started program...");
}

//Start of update
void Manager::clear() {}

//Main game logic
void Manager::logic() {}

//Get input from user
void Manager::input() 
{
	//Make sure all events are read
	glfwPollEvents();
}

//Draw the game using engine
void Manager::draw() {}

//Any post drawing things
void Manager::late()
{
	if (glfwWindowShouldClose(win)) state = programState::Closing;
}

//Close and clean up memory
void Manager::quit() 
{
	//Destroy class objects
	glfwDestroyWindow(win);

	//Close window
	glfwTerminate();
}