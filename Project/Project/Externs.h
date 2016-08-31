//Libs for variables
#include <GLFW\glfw3.h>

//STD
#include <string>

//Window properties
extern const int GAME_WIDTH = 1280;
extern const int GAME_HEIGHT = 720;
extern const char* GAME_TITLE = "3D";

//Shape properties - temp
extern GLfloat vbo_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

//Declare externs and game consts
//Things that are global to the app
extern GLFWwindow *win;
extern GLuint vao;
extern GLuint vbo;