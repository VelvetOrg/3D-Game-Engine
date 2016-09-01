//Protection
#pragma once

//TO prevent arning from stopping compilation
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

//Engine files
#include "Console.h"
#include "Vector3.h"

//OPEN AL files
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>

//SDT
#include <unordered_map>

//Storing oggs
#include <libvorbis\vorbisfile.h>

//Namespaces - please don't use namespaces like this!
using std::string;
using std::unordered_map;
using std::vector;

//Static so it can be inside a namesapce
namespace SoundManager
{
	//Holds file extension
	string oggextension = ".ogg";
	vector<char> buffer;
	
	//Has map of sound buffers
	unordered_map<string, ALuint> song_map;
	unordered_map<string, ALuint> sfx_map;
	
	//Open AL buffers
	ALCcontext *context;
	ALCdevice *device;
	
	//Adds file to the hash maps
	bool AddSong(string key, string path);
	bool AddSFX(string key, string path);
	
	//Play the loaded sounds - Ruchir: Please use optional paramters for this
	void PlaySong(string song_key, Vec3 position_song, Vec3 velocity_song, bool loop, bool is3D, float pitch, float gain);
	void PlaySFX(string sfx_key, Vec3 position_sfx, Vec3 velocity_sfx, bool is3D, float pitch, float gain);
	
	//OGG loading
	ALvoid* ConvertOGG(string path, ALenum &format, ALsizei &frequency);
	void LoadOGG(string fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);
	
	//Setup
	void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation);
	
	//Clean up
	void Kill();
}
