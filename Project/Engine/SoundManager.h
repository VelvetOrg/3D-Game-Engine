//Include guards
#pragma once

//Includes
#include <ambiera\irrKlang.h>
#include <glm\glm.hpp>

//Lazy
using namespace irrklang;

//Static class
static class pSoundManager
{
public:
	void Init(); // Initialises the SoundManager and creates a SoundEngine
	void Play(const char* path, bool loop); // Plays a 2D Sound, takes in the path and whether to loop the audio
	void Play(const char* path, bool loop, glm::vec3 position); // Plays a 3D Sound
	
protected:
	//The ISoundEngine
	ISoundEngine *Engine;
} SoundManager;