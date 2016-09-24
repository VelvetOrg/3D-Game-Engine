//Needs header file
#include "Manager.h"
#include "Externs.h"

//Engine specific
#include <Engine\Time.h>
#include <Engine\Input.h>
#include <Engine\Loader.h>
#include <Engine\Shader.h>
#include <Engine\Console.h>
#include <Engine\Graphics.h>
#include <Engine\Primitives.h>
#include <Engine\SoundManager.h>

//GLM
#include <glm\common.hpp>

//Math
#include <Engine\Mathf.h>
#include <time.h>

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

	if (!init_status) Console.error("Could not initialize GLFW.");

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
	glfwSetErrorCallback(&(cConsole::glfwError));

	//Create window based on externs
	win = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, GAME_TITLE, NULL, NULL);
	if (!win) Console.error("Could not create the game window.");

	//Make OPEN GL context
	glfwMakeContextCurrent(win);

	//No mouse should be visible
	Input.initCalbacks(win, GAME_WIDTH / 2, GAME_HEIGHT / 2);

	//Set double buffer interval
	glfwSwapInterval(1);

	//Allow for back face culling
	glEnable(GL_CULL_FACE);

	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();

	if (glewStatus != GLEW_OK) Console.error("GLEW failed to setup.");
	
	//Create the camera
	cam.Init(glm::vec3(0, 20, 10));
	cam.far_clipping = 300;
	cam.pitch = -20;
	cam.yaw = -90;

	//Initialize the randomizer
	Mathf::seed = time(NULL);
	Random::seed();

	//Create a noise map
	Mathf::createMap();

	//Setup the cube model
	Mesh box_mesh = Loader::loadModel(BOX_MODEL_FILE);
	GLuint checker_id = Loader::loadTexture(CHECKER_TEX);

	//Generate an array of boxes
	boxes = new GameObject[LEVEL_WIDTH * LEVEL_HEIGHT];

	for (int y = 0; y < LEVEL_HEIGHT; y++)
	{
		for (int x = 0; x < LEVEL_WIDTH; x++)
		{
			//Find the current list index
			int index = y * LEVEL_WIDTH + x;

			int nx = x - LEVEL_WIDTH / 2;
			int ny = y - LEVEL_HEIGHT / 2;

			//Set general properties
			boxes[index].meshRenderer.colour = glm::vec3(0, 1, 0.5);
			boxes[index].meshRenderer.tex_index = checker_id;
			boxes[index].meshRenderer.mesh = box_mesh;

			//Set the position based
			boxes[index].transform.position.x = nx;
			boxes[index].transform.position.z = ny;

			//Find y position based on noise map
			boxes[index].transform.position.y = (GLint)(Mathf::perlinNoise(((GLfloat)x) / LEVEL_WIDTH, ((GLfloat)y) / LEVEL_HEIGHT) * 10.0f);
		}
	}

	//Create object buffers
	//Also loads up textures
	Graphics::createBuffers();

	//Load shaders
	GLuint vertex_shader = Shader::load(VERTEX_PATH, GL_VERTEX_SHADER);
	GLuint fragment_shader = Shader::load(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

	shader_program = Shader::bind(vertex_shader, fragment_shader);

	Graphics::bindShaderData(shader_program);

	SoundManager::Init();
	//SoundManager::Play(SONG_PATH, true);

	//State can now be changed
	state = programState::Running;

	//Test console message
	Console.message("Started program...");
}

//Start of update
void Manager::early()
{
	//Timer timers need to be updated
	Time::start();
}

//Get input from user
void Manager::input()
{
	//Close on escape
	if (Input.getKey(GLFW_KEY_ESCAPE).released) quit();
	Input.lockCustorToPos(win, glm::vec2(GAME_WIDTH / 2, GAME_HEIGHT / 2));

	//Find delta between mouse position
	glm::vec2 m_pos = Input.getmousePos() - glm::vec2(GAME_WIDTH / 2, GAME_HEIGHT / 2);

	//Affect cameras rotation
	cam.pitch -= m_pos.y * MOUSE_SENSITIVITY;
	cam.yaw += m_pos.x * MOUSE_SENSITIVITY;

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
	if (Input.getKey(GLFW_KEY_A).held) horizontal -= 1.0f;
	if (Input.getKey(GLFW_KEY_D).held) horizontal += 1.0f;
	if (Input.getKey(GLFW_KEY_W).held) vertical += 1.0f;
	if (Input.getKey(GLFW_KEY_S).held) vertical -= 1.0f;
	if (Input.getKey(GLFW_KEY_SPACE).held) depth += 1.0f;
	if (Input.getKey(GLFW_KEY_LEFT_SHIFT).held) depth -= 1.0f;

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
}

//Main game logic
void Manager::logic()
{
	//Test the math class
	//box.transform.position.x = Mathf::cos(Mathf::lerp(-Mathf::PI, Mathf::PI, Mathf::smoothstep((Mathf::bounce(Time::seconds))))) * 5.0f;
	//box.transform.position.z = Mathf::sin(Mathf::lerp(-Mathf::PI, Mathf::PI, Mathf::smoothstep((Mathf::bounce(Time::seconds))))) * 5.0f;
	
	//Show fps
	glfwSetWindowTitle(win, ("3D Game, FPS: " + std::to_string(Time::fps)).c_str());
}

//Draw the game using engine
void Manager::draw()
{
	//Set the shader uniforms
	Graphics::view_projection_mat_value = cam.getViewProjection(GAME_WIDTH / GAME_HEIGHT); //Set view matrix based on camera object

	//Draw
	Graphics::draw(shader_program, glm::vec2(GAME_WIDTH, GAME_HEIGHT));
}

//Any post drawing things
void Manager::late()
{
	//Set stae of program
	if (glfwWindowShouldClose(win)) quit();

	//Update input manager
	Input.update();

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

	//Destroy class objects
	glfwDestroyWindow(win);

	//Close window
	glfwTerminate();

	//Set staet to break out of loop
	state = programState::Closing;
}