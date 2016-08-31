#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include "Console.h"
#include "Vector3.h"
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>
#include <unordered_map>
#include <libvorbis\vorbisfile.h>
using std::string;
using std::unordered_map;
using std::vector;
namespace SoundManager
{
	string oggextension = ".ogg";
	vector<char> buffer;
	unordered_map<string, ALuint> song_map;
	unordered_map<string, ALuint> sfx_map;
	ALCcontext *context;
	ALCdevice *device;
	bool AddSong(string key, string path);
	bool AddSFX(string key, string path);
	void PlaySong(string song_key, Vec3 position_song, Vec3 velocity_song, bool loop, bool is3D, float pitch, float gain);
	void PlaySFX(string sfx_key, Vec3 position_sfx, Vec3 velocity_sfx, bool is3D, float pitch, float gain);
	ALvoid* ConvertOGG(string path, ALenum &format, ALsizei &frequency);
	void LoadOGG(string fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);
	void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation);
	void Kill();
}