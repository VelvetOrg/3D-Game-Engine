//Needs header file
#include "Manager.h"
#include "Externs.h"

//Engine specific
#include <Engine\Time.h>
#include <Engine\Input.h>
#include <Engine\Shader.h>
#include <Engine\Console.h>
#include <Engine\Graphics.h>

//GLM
#include <glm\common.hpp>

//Math
#include <math.h>

//Create memory for externs that are not set, oly declared
GLFWwindow *win;
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

	//No mouse should be visible
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(win, GAME_WIDTH / 2, GAME_HEIGHT / 2);
	Input::cursorCallback(win, GAME_WIDTH / 2, GAME_HEIGHT / 2);

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
	cam.Init(glm::vec3(0, 0, 5));

	//Alert mesh renderer of the gameobjecft transform
	box.meshRenderer.objectTransform = &box.transform;

	//Parse the mesh renderer mesh data from the externs header
	box.meshRenderer.elements = ELEMENT_DATA;
	box.meshRenderer.verticies = VERT_DATA;

	box.meshRenderer.vertSize = sizeof(VERT_DATA);
	box.meshRenderer.elementSize = sizeof(ELEMENT_DATA);

	box.meshRenderer.tris = 12;

	//Create render buffers
	Graphics::createBuffers(&vbo, &ebo, &box.meshRenderer);

	//Load shaders
	GLuint vertex_shader = Shader::load(VERTEX_PATH, GL_VERTEX_SHADER);
	GLuint fragment_shader = Shader::load(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

	shader_program = Shader::bind(vertex_shader, fragment_shader);

	//  -------- Get uniforms from shader -------- 

	vertex_pos_location = glGetAttribLocation(shader_program, "vertPosition"); //Vertex position input
	vertex_col_location = glGetAttribLocation(shader_program, "vertColour"); //Vertex colour input
	view_projection_location = glGetUniformLocation(shader_program, "viewProjection"); //Projection and view mat
	model_matrix_projection = glGetUniformLocation(shader_program, "model"); //Model matrix

	glEnableVertexAttribArray(vertex_pos_location);
	glVertexAttribPointer(vertex_pos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(vertex_col_location);
	glVertexAttribPointer(vertex_col_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

	// -------- Done -------- 

	//State can now be changed
	state = programState::Running;

	//Test console message
	Console::message("Started program...");
}

//Start of update
void Manager::clear()
{
	//Timer goes first
	Time::start();

	//Recreate viewport
	int width, height;

	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);

	//Clear colours on screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
}

//Get input from user
void Manager::input()
{
	//Close on escape
	if (Input::getKey(GLFW_KEY_ESCAPE).released) quit();
	Input::lockCustorToPos(win, glm::vec2(GAME_WIDTH / 2, GAME_HEIGHT / 2));

	//Find delta between mouse position
	glm::vec2 m_pos = Input::mousePos - glm::vec2(GAME_WIDTH / 2, GAME_HEIGHT / 2);

	//Affect cameras rotation
	cam.pitch -= m_pos.y * Time::delta * MOUSE_SENSITIVITY;
	cam.yaw += m_pos.x * Time::delta * MOUSE_SENSITIVITY;

	//Fix that strange bug
	if (cam.pitch > 89.0f) cam.pitch = 89.0f;
	if (cam.pitch < -89.0f) cam.pitch = -89.0f;

	//Move based on input relative to camera rotation
	float speed = CAMERA_MOVE_SPEED * Time::delta;

	//Stores the movement axis
	float horizontal = 0.0f;
	float vertical = 0.0f;
	float depth = 0.0f;
	
	//Determine the direction of each exis
	if (Input::getKey(GLFW_KEY_A).held) horizontal -= 1.0f;
	if (Input::getKey(GLFW_KEY_D).held) horizontal += 1.0f;
	if (Input::getKey(GLFW_KEY_W).held) vertical += 1.0f;
	if (Input::getKey(GLFW_KEY_S).held) vertical -= 1.0f;
	if (Input::getKey(GLFW_KEY_SPACE).held) depth += 1.0f;
	if (Input::getKey(GLFW_KEY_LEFT_SHIFT).held) depth -= 1.0f;

	//Adjust speed based on how many keys are down
	float total_val = abs(vertical) + abs(horizontal) + abs(depth);

	//Because diagonal movement should be slower
	//E.G When two input are pressed speed will be multiplied by 0.7..
	float direction_mod = 1.0f;
	if (total_val > 0) direction_mod = 1 / sqrt(total_val);

	//Move camera By the axis
	cam.transformPos += cam.relativeForward * speed * vertical * direction_mod;
	cam.transformPos += glm::cross(cam.relativeForward, cam.up) * speed * horizontal * direction_mod;
	cam.transformPos += cam.relativeUp * speed * depth * direction_mod;

	//Move box on input
	if (Input::getKey(GLFW_KEY_UP).held) box.transform.position.y += Time::delta * BOX_MOVE_SPEED;
	if (Input::getKey(GLFW_KEY_DOWN).held) box.transform.position.y -= Time::delta * BOX_MOVE_SPEED;
	if (Input::getKey(GLFW_KEY_LEFT).held) box.transform.position.x += Time::delta * BOX_MOVE_SPEED;
	if (Input::getKey(GLFW_KEY_RIGHT).held) box.transform.position.x -= Time::delta * BOX_MOVE_SPEED;
}

//Main game logic
void Manager::logic()
{
	//Move
	box.transform.rotation.x += Time::delta;

	//Show fps
	glfwSetWindowTitle(win, ("3D Game, FPS: " + std::to_string(Time::fps)).c_str());
}

//Draw the game using engine
void Manager::draw()
{
	//Rendering functions go here...
	//...
	//...

	//Set the shader uniforms
	glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, &cam.getViewProjection()[0][0]); //Set view matrix based on camera object
	glUniformMatrix4fv(model_matrix_projection, 1, GL_FALSE, &(box.meshRenderer.genModelMatrix()[0][0])); //Set model matrix

	//Draw
	Graphics::draw(shader_program);
	box.meshRenderer.draw();
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

	glDeleteVertexArrays(1, &box.meshRenderer.vao);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	//Destroy class objects
	glfwDestroyWindow(win);

	//Close window
	glfwTerminate();

	//Set staet to break out of loop
	state = programState::Closing;
}