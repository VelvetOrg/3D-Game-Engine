//Protection
#pragma once

//Engine
#include "Mesh.h"
#include "SoundManager.h"

//This is a static singleton for loading anything
static class cLoader
{
public:
	GLuint loadTexture(const char* path); //Adds an index to the graphics list of textures
	Mesh loadModel(const char* path); //This will convert an OBJ file into a Mesh object

	void Play(const char* path, bool loop); //This will play a 2D sound
	void Play(const char* path, bool loop, glm::vec3 position); //This will play a 3D sound
} Loader;