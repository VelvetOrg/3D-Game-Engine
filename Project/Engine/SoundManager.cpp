#include "SoundManager.h"
namespace SoundManager
{
    //Functions re-declared
	void Init();
	void Play(const char* path, bool loop);
	void Play(const char* path, bool loop, glm::vec3 position);
	void Pause();
	void Resume();
	void Kill();
    //Engine re-declared
	ISoundEngine *Engine;

	void Init()
	{
        //Create irrKlangDevice
		Engine = createIrrKlangDevice();
	}

	void Play(const char* path, bool loop = false)
	{
        //Plays the sound with the Engine
		Engine->play2D(path, loop);
	}

	void Play(const char* path, bool loop, glm::vec3 position)
	{
        //Convert glm::vec3 to vec3df (used by irrKlang)
        vec3df tempPos = vec3df(position.x, position.y, position.z);
        //Plays the sound with the Engine
		Engine->play3D(path, tempPos, loop);
	}

	void Pause()
	{
        //Should pause the sound. Incomplete.   
	}

	void Resume()
	{
        //Should resume the sound. Incomplete.
	}

	void Kill()
	{
        //Should destroy and kill the SoundManager. Incomplete.
	}
}
