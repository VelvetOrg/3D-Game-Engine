#include "SoundManager.h"
namespace SoundManager
{
	void Init();
	void Play(const char* path, bool loop);
	void Play(const char* path, bool loop, vec3df position);
	void Pause();
	void Resume();
	void Kill();
	ISoundEngine *Engine;

	void Init()
	{
		Engine = createIrrKlangDevice();
	}

	void Play(const char* path, bool loop)
	{
		Engine->play2D(path, loop);
	}

	void Play(const char* path, bool loop, vec3df position)
	{
		Engine->play3D(path, position, loop);
	}

	void Pause()
	{
	}

	void Resume()
	{

	}

	void Kill()
	{

	}
}