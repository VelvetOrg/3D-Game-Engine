//Needs header file
#include "Manager.h"
#include "Externs.h"

//Engien specific
#include <Engine\Input.h>
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

	//Make sure events are passed though engine input manager
	glfwSetKeyCallback(win, Input::keyCallback);

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

	//Check for space bar
	if (Input::getKey(GLFW_KEY_SPACE).down) printf("Space down!\n");
	if (Input::getKey(GLFW_KEY_SPACE).held) printf("Space held!\n");
	if (Input::getKey(GLFW_KEY_SPACE).released) printf("Space released!\n\n");
}

//Draw the game using engine
void Manager::draw() {}

//Any post drawing things
void Manager::late()
{
	//Set stae of program
	if (glfwWindowShouldClose(win)) state = programState::Closing;

	//Update input manager
	Input::update();
}

//Close and clean up memory
void Manager::quit() 
{
	//Destroy class objects
	glfwDestroyWindow(win);

	//Close window
	glfwTerminate();
}