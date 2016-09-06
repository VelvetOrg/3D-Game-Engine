//Protection
#pragma once

//Libs for variables
#include <GLFW\glfw3.h>

//STD
#include <string>

//Window properties
extern const int GAME_WIDTH = 768;
extern const int GAME_HEIGHT = 768;
extern const char* GAME_TITLE = "3D";

//Location of shader files
extern const char* VERTEX_PATH = "vertex.txt";
extern const char* FRAGMENT_PATH = "fragment.txt";

//Camera properties
extern const float FOV = 60.0f;
extern const float NEAR_CLIPPING = 0.1f;
extern const float FAR_CLIPPING = 100.0f;

//Model data
const char* TREE_FILE = "../../Assets/Tree.obj";
const char* TORUS_FILE = "../../Assets/Torus.obj";
const char* BOX_FILE = "../../Assets/Box.obj";

//Camera controller consts
extern const float MOUSE_SENSITIVITY = 0.05f;
extern const float CAMERA_MOVE_SPEED = 5.0f;

//Declare externs and game consts
//Things that are global to the app
extern GLFWwindow *win; //GLFW window
extern GLuint vbo; //Vertex buffer object
extern GLuint ebo; //Element buffer object
extern GLuint shader_program; //Compiled shaders