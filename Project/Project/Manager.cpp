//Needs header file
#include "Manager.h"
#include "Externs.h"

//Engine specific
#include <Engine\Input.h>
#include <Engine\Shader.h>
#include <Engine\Console.h>
#include <Engine\Graphics.h>

//Create memory for externs that are not set, oly declared
GLFWwindow *win;
GLuint vao;
GLuint vbo;
GLuint ebo;
GLuint shader_program;

//Setup all of the program
void Manager::init() 
{
	//Init GLFW
	GLboolean init_status = glfwInit();

	if (!init_status) Console::error("Could not initialize GLFW.");

	//Try to use OPEN GL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//For compiling on mac - uncomment this:
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Only use modern OPEN GL (All legacy functions will return an error)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Non resizable window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//All GLFW error messages go though error class
	glfwSetErrorCallback(Console::glfwError);

	//Create window based on externs
	win = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, GAME_TITLE, NULL, NULL);
	if (!win) Console::error("Could not create the game window.");

	//Make OPEN GL context
	glfwMakeContextCurrent(win);
	
	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();

	if (glewStatus != GLEW_OK) Console::error("GLEW failed to setup.");

	//Make sure events are passed though engine input manager
	glfwSetKeyCallback(win, Input::keyCallback);
	glfwSetCursorPosCallback(win, Input::cursorCallback);
	glfwSetMouseButtonCallback(win, Input::mouseClickCallback);

	//Create OPEN GL viewport
	glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
	
	//Parse the mesh renderer mesh data from the externs header
	_mesh_renderer.elements = ELEMENT_DATA;
	_mesh_renderer.verticies = VERT_DATA;

	//Create buffers
	Graphics::createBuffers(vao, vbo, ebo, _mesh_renderer.verticies, _mesh_renderer.elements);

	//Load shaders
	GLuint vertex_shader = Shader::load(VERTEX_PATH, GL_VERTEX_SHADER);
	GLuint fragment_shader = Shader::load(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

	shader_program = Shader::bind(vertex_shader, fragment_shader);


	//  -------- Get uniforms from shader -------- 
	
	GLint positionAttribute = glGetAttribLocation(shader_program, "position");
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint colourAttribute = glGetAttribLocation(shader_program, "colour");
	glEnableVertexAttribArray(colourAttribute);
	glVertexAttribPointer(colourAttribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	// -------- Done -------- 

	
	
	SoundManager::Init(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 0));
	cout << SoundManager::ListAvailableDevices() << endl;
	SoundManager::SetAudioPath("Assets/");
	



	//State can now be changed
	state = programState::Running;

	//Test console message
	Console::message("Started program...");
}

//Start of update
void Manager::clear() 
{
	//Clear colours on screen
	//glClearColor(GAME_BG.r, GAME_BG.g, GAME_BG.b, GAME_BG.a);
	glClearColor(0.0f, 25.0f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Main game logic
void Manager::logic() {}

//Get input from user
void Manager::input() 
{
	//Make sure all events are read
	glfwPollEvents();

	//Close on escape
	if (Input::getKey(GLFW_KEY_ESCAPE).released) quit();
}

//Draw the game using engine
void Manager::draw() 
{
	//Rendering functions go here...
	//...
	//...

	//Draw
	Graphics::draw(vbo, shader_program);
}

//Any post drawing things
void Manager::late()
{
	//Set stae of program
	if (glfwWindowShouldClose(win)) quit();

	//Update input manager
	Input::update();

	//Swap the OPEN GL buffers:
	//Uese double buffering to prevent flickers
	glfwSwapBuffers(win);
}

//Close and clean up memory
void Manager::quit() 
{
	//Clean buffers
	glDeleteProgram(shader_program);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	//Destroy class objects
	glfwDestroyWindow(win);

	//Close window
	glfwTerminate();

	//Set staet to break out of loop
	state = programState::Closing;
}