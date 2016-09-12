//Protection
#pragma once

//Engine
#include "Mesh.h"
#include "Texture.h"
#include "Graphics.h"
#include "ModelLoader.h"
#include "SoundManager.h"

//This is a static singleton for loading anything
namespace Loader
{
	GLuint loadTexture(const char* path); //Adds an index to the graphics list of textures
	Mesh loadModel(const char* path); //This will convert an OBJ file into a Mesh object

	void Play(const char* path, bool loop); //This will play a 2D sound
	void Play(const char* path, bool loop, vec3df position); //This will play a 3D sound
}