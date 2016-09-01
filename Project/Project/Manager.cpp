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
	
	//Create the camera
	_cam.Init(glm::vec3(4, 3, 3), //Starting position
		glm::vec3(0, 0, 0),       //Looking at the origin
		_cam.Perspective,		  //Use perspective matrix
		FOV,					  //Degrees FOV
		NEAR_CLIPPING,			  //Closest distance allowed to the camera
		FAR_CLIPPING);			  //Furthest distance allowed to the camera

	//Parse the mesh renderer mesh data from the externs header
	_mesh_renderer.elements = ELEMENT_DATA;
	_mesh_renderer.verticies = VERT_DATA;

	Graphics::createBuffers(&vao, &vbo, &ebo, sizeof(VERT_DATA), VERT_DATA, sizeof(ELEMENT_DATA), ELEMENT_DATA);

	//Load shaders
	GLuint vertex_shader = Shader::load(VERTEX_PATH, GL_VERTEX_SHADER);
	GLuint fragment_shader = Shader::load(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

	shader_program = Shader::bind(vertex_shader, fragment_shader);


	//  -------- Get uniforms from shader -------- 
	
	vertex_pos_location = glGetAttribLocation(shader_program, "vertPosition"); //Vertex position input
	vertex_col_location = glGetAttribLocation(shader_program, "vertColour"); //Vertex colour input
	model_view_projection_location = glGetUniformLocation(shader_program, "mvp"); //Model view projection

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	//Set the shader uniforms
	glUniformMatrix4fv(model_view_projection_location, 1, GL_FALSE, &_cam.getMVP()[0][0]); //Set view matrix based on camera object

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