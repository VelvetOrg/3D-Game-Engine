#pragma once
#include <ambiera\irrKlang.h>
#include "Console.h"

using namespace irrklang;
namespace SoundManager
{
	extern void Init();
	extern void Play(const char* path, bool loop);
	extern void Play(const char* path, bool loop, vec3df position);
	extern void Pause();
	extern void Resume();
	extern void Kill();
	extern ISoundEngine *Engine;
}