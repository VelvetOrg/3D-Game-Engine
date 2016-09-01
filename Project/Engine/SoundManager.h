#pragma once
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Console.h"
#include "Vector3.h"
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <libvorbis\vorbisfile.h>
using std::string;
using std::unordered_map;
using std::vector;
namespace SoundManager
{
#define MAX_AUDIO_BUFFERS 64
#define MAX_AUDIO_SOURCES 16
#ifndef _SOUND_MANAGER_H
#define _SOUND_MANAGER_H
	typedef __int16 SAMPLES;
	static  bool isInit;
	static  ALCdevice *Device;
	static  ALCcontext *Context;
	extern string AudioPath;
	static  bool isSoundOn;
	extern ALfloat Position[3];
	extern ALfloat Velocity[3];
	extern ALfloat Orientation[6];

	extern unsigned int AudioSourcesInUseCount;
	extern vector<ALuint> AudioSources;
	extern vector<bool> AudioSourcesInUse;
	
	extern unsigned int AudioBuffersInUseCount;
	extern vector<ALuint> AudioBuffers;
	extern vector<bool> AudioBuffersInUse;
	extern vector<string> AudioBufferFileName;

	extern int LocateAudioBuffer(string path);
	extern int LoadAudioToBuffer(string path);
	extern bool LoadOGG(string path, ALuint destinationAudioBuffer);

	extern void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up);
	static void Kill();
	
	static inline bool IsSoundOn() { return isSoundOn; };
	static void SetAudioPath(string path) { AudioPath = path; };
	
	extern string ListAvailableDevices();

	extern bool LoadAudio(string path, unsigned int *audioID, bool loop);
	extern bool ReleaseAudio(unsigned int audioID);
	extern bool PlayAudio(unsigned int audioID, bool forceRestart);
	extern bool StopAudio(unsigned int audioID);
	extern bool StopAllAudio();

	extern bool PauseAudio(unsigned int audioID);
	extern bool PauseAllAudio();
	extern bool ResumeAudio(unsigned int audioID);
	extern bool ResumeAllAudio();
	extern bool SetSoundPosition(unsigned int audioID, Vec3 position);
	extern bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction);
	extern bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool loop, float minGain);
	extern bool SetListenerPosition(Vec3 position, Vec3 velocity, Vec3 orientation);
#endif
}