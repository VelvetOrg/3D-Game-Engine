//Protection
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Static class
namespace Shader
{
	//This will load, compile and error check a shader from a file
	//Returns the compiled shader program
	GLuint load(const char* filePath, GLenum shaderType);

	//Compiles the whole shader program, made of many shaders
	GLuint bind(GLuint vertex, GLuint fragment);
};