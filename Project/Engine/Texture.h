//Protection
#pragma once

//Glew
#include <GL\glew.h>

//Type of a texture
class Texture
{
public:
	int width;
	int height;

	GLuint* value; //Buffer ID
	const char* file_path; //Location on disk
	unsigned char* image_data; //Stores pixel image in memory

	//Set on constructor
	Texture() { value = 0; file_path = "\0"; width = 1; height = 1; }
};