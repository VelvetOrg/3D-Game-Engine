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

//Shape properties - temp
//Formatting is: x, y, z....
extern GLfloat VERT_DATA[] = {
	//Back face - yellow
	-1, -1, -1, 1, 1, 0,
	+1, -1, -1, 1, 1, 0,
	+1, +1, -1, 1, 1, 0,
	-1, +1, -1, 1, 1, 0,

	//Front face
	-1, -1, +1, 0, 0, 1,
	+1, -1, +1, 0, 0, 1,
	+1, +1, +1, 0, 0, 1,
	-1, +1, +1, 0, 0, 1,

	//Left face
	-1, -1, -1, 0, 1, 1,
	-1, +1, -1, 0, 1, 1,
	-1, +1, +1, 0, 1, 1,
	-1, -1, +1, 0, 1, 1,

	//Right face
	+1, -1, -1, 1, 0, 0,
	+1, +1, -1, 1, 0, 0,
	+1, +1, +1, 1, 0, 0,
	+1, -1, +1, 1, 0, 0,

	//Bottom face
	-1, -1, -1, 1, 0, 1,
	-1, -1, +1, 1, 0, 1,
	+1, -1, +1, 1, 0, 1,
	+1, -1, -1, 1, 0, 1,

	//Top face
	-1, +1, -1, 0, 1, 0,
	-1, +1, +1, 0, 1, 0,
	+1, +1, +1, 0, 1, 0,
	+1, +1, -1, 0, 1, 0
};

//Contain faces, by joining verts
extern GLushort ELEMENT_DATA[] = {
	//Back face
	0, 1, 2,
	0, 2, 3,

	//Front face
	4, 5, 6,
	4, 6, 7,

	//Left face
	8, 9, 10,
	8, 10, 11,

	//Right face
	12, 13, 14,
	12, 14, 15,

	//Bottom face
	16, 17, 18,
	16, 18, 19,

	//Top face
	20, 21, 22,
	20, 22, 23
};

//Declare externs and game consts
//Things that are global to the app
extern GLFWwindow *win; //GLFW window
extern GLuint vbo; //Vertex buffer object
extern GLuint ebo; //Element buffer object
extern GLuint shader_program; //Compiled shaders