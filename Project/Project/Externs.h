//Libs for variables
#include <GLFW\glfw3.h>

//STD
#include <string>

//Window properties
extern const int GAME_WIDTH = 1280;
extern const int GAME_HEIGHT = 720;
extern const char* GAME_TITLE = "3D";

//Location of shader files
extern const char* VERTEX_PATH = "vertex.txt";
extern const char* FRAGMENT_PATH = "fragment.txt";

//Shape properties - temp
extern GLfloat VERT_DATA[] = {
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
};

extern GLuint ELEMENT_DATA[] = {
	0, 1, 2, // Left tri
	2, 3, 0 // Right tri
};

//Declare externs and game consts
//Things that are global to the app
extern GLFWwindow *win; //GLFW window
extern GLuint vao; //Vertex array object
extern GLuint vbo; //Vertex buffer object
extern GLuint ebo; //Element buffer object
extern GLuint shader_program; //Compiled shaders