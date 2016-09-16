//Protection
#pragma once

//Glew
#include <GL\glew.h>

//Type of a texture
class Texture
{
public:
	int ID;
	int width;
	int height;

	GLuint* value; //Buffer ID
	const char* file_path; //Location on disk
	unsigned char* image_data; //Stores pixel image in memory

	//Set on constructor to white pixel
	Texture() { value = 0; file_path = "\0"; width = 1; height = 1; }
};