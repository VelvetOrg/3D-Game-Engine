//Include guards
#pragma once
//Includes
#include <ambiera\irrKlang.h> // irrKlang
#include "Console.h" // Console printing
#include <glm\glm.hpp> // To use GLM Vector3 class
using namespace irrklang;
namespace SoundManager
{
	extern void Init(); // Initialises the SoundManager and creates a SoundEngine
	extern void Play(const char* path, bool loop); // Plays a 2D Sound, takes in the path and whether to loop the audio
	extern void Play(const char* path, bool loop, glm::vec3 position); // Plays a 3D Sound, takes in the path, whether the loop the // audio and a vector to play the sound at
	extern void Pause(); // Pauses an audio. Incomplete.
	extern void Resume(); // Resumes an audio. Incomplete.
	extern void Kill(); // Destroys and cleans up the SoundManager
	extern ISoundEngine *Engine; // The ISoundEngine
}
