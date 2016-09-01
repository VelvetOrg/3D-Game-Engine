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

	//Make sure events are passed though engine input manager
	glfwSetKeyCallback(win, Input::keyCallback);
	glfwSetCursorPosCallback(win, Input::cursorCallback);
	glfwSetMouseButtonCallback(win, Input::mouseClickCallback);
	
	//Make OPEN GL context
	glfwMakeContextCurrent(win);

	//Set double buffer interval
	glfwSwapInterval(1);
	
	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();

	if (glewStatus != GLEW_OK) Console::error("GLEW failed to setup.");
	
	//Parse the mesh renderer mesh data from the externs header
	_mesh_renderer.elements = ELEMENT_DATA;
	_mesh_renderer.verticies = VERT_DATA;

	//Create buffers
	//Graphics::createBuffers(&vao, &vbo, &ebo, VERT_DATA, ELEMENT_DATA);

	/*ULTRA TEMP!!!

	//Create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Create the vertex buffer object
	glGenBuffers(1, &vbo); //Tell OPEN GL of vbo existance
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //Store VBO in array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERT_DATA), VERT_DATA, GL_STATIC_DRAW); //Parse vertex data to array buffer

	//Bind element buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ELEMENT_DATA), ELEMENT_DATA, GL_STATIC_DRAW);
	//END ULTRA TEMP!!*/

	Graphics::createBuffers(&vao, &vbo, &ebo, sizeof(VERT_DATA), VERT_DATA, sizeof(ELEMENT_DATA), ELEMENT_DATA);

	//Load shaders
	GLuint vertex_shader = Shader::load(VERTEX_PATH, GL_VERTEX_SHADER);
	GLuint fragment_shader = Shader::load(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

	shader_program = Shader::bind(vertex_shader, fragment_shader);


	//  -------- Get uniforms from shader -------- 
	
	vertex_pos_location = glGetAttribLocation(shader_program, "vertPosition");
	vertex_col_location = glGetAttribLocation(shader_program, "vertColour");

	glEnableVertexAttribArray(vertex_pos_location);
	glVertexAttribPointer(vertex_pos_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) 0);

	glEnableVertexAttribArray(vertex_col_location);
	glVertexAttribPointer(vertex_col_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (sizeof(GLfloat) * 2));

	// -------- Done -------- 

	//State can now be changed
	state = programState::Running;

	//Test console message
	Console::message("Started program...");
}

//Start of update
void Manager::clear()
{
	//Recreate viewport
	int width, height;

	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);

	//Clear colours on screen
	//glClearColor(GAME_BG.r, GAME_BG.g, GAME_BG.b, GAME_BG.a);
	glClearColor(0.0f, 25.0f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Get input from user
void Manager::input()
{
	//Close on escape
	if (Input::getKey(GLFW_KEY_ESCAPE).released) quit();
}

//Main game logic
void Manager::logic() {}

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

	//Make sure all events are read
	glfwPollEvents();
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