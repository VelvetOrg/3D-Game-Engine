#pragma once
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Console.h"
#include "Vector3.h"
#include <glm\gtc\quaternion.hpp>
#include <glm\glm.hpp>
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
	static bool isInit;
	static  ALCdevice *Device;
	static  ALCcontext *Context;
	static string AudioPath;
	static  bool isSoundOn;
	static ALfloat Position[3];
	static ALfloat Velocity[3];
	static ALfloat Orientation[6];

	static unsigned int AudioSourcesInUseCount;
	static vector<ALuint> AudioSources;
	static vector<bool> AudioSourcesInUse;

	static unsigned int AudioBuffersInUseCount;
	static vector<ALuint> AudioBuffers;
	static vector<bool> AudioBuffersInUse;
	static vector<string> AudioBufferFileName;

	static int LocateAudioBuffer(string path);
	static int LoadAudioToBuffer(string path);
	static bool LoadOGG(string path, ALuint destinationAudioBuffer);

	static void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up);
	static void Kill();

	static inline bool IsSoundOn() { return isSoundOn; };
	static void SetAudioPath(string path) { AudioPath = path; };

	static string ListAvailableDevices();

	static bool LoadAudio(string path, unsigned int *audioID, bool loop);
	static bool ReleaseAudio(unsigned int audioID);
	static bool PlayAudio(unsigned int audioID, bool forceRestart);
	static bool StopAudio(unsigned int audioID);
	static bool StopAllAudio();

	static bool PauseAudio(unsigned int audioID);
	static bool PauseAllAudio();
	static bool ResumeAudio(unsigned int audioID);
	static bool ResumeAllAudio();
	static bool SetSoundPosition(unsigned int audioID, Vec3 position);
	static bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction);
	static bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool loop, float minGain);
	static bool SetListenerPosition(Vec3 position, Vec3 velocity, glm::quat orientation);
#endif
}