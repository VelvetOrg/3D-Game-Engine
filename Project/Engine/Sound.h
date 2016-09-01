/*
#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>
#include <unordered_map>
#include "Console.h"
#include <libvorbis\vorbisfile.h>
using std::string;
using std::unordered_map;

using std::vector;
class Sound
{
private:
	ALCdevice *device;
	ALCcontext *context;
	ALint state;
	ALuint sourceID;
	ALenum format;
	ALsizei frequency;

	bool audioFileWasSuccessfullyLoaded;
	bool OGGLoaded;
	string OGGextension = ".ogg";
	string sound_name;

public:
	Sound();
	~Sound();
	void PlaySong(string path);
	void PlaySFX(string path);
	void PlayMusic(string path, bool loop = true);
	
	bool LoadOGG(string path, vector<char> &buffer, ALenum &format, ALsizei &frequency);
};

*/