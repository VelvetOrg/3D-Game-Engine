//Header
#include "SoundManager.h"

//Create irrKlangDevice
void pSoundManager::Init() { Engine = createIrrKlangDevice(); }

//Plays the sound with the Engine
void pSoundManager::Play(const char* path, bool loop = false) { Engine->play2D(path, loop); }

//Plays the sound with the Engine
void pSoundManager::Play(const char* path, bool loop, glm::vec3 position)
{
	Engine->play3D(path, vec3df(position.x, position.y, position.z), loop);
}