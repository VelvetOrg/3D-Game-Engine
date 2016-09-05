/*Include guards
#pragma once

//To use deprecated functions such as fopen()
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//Includes
#include "Console.h" // Console messages, errors and warnings
#include "Vector3.h" // Vector class
#include <glm\gtc\quaternion.hpp> // GLM Quaternion 
#include <glm\glm.hpp> // GLM 
//Includes for OpenAL
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>
#include <vector>
#include <string>
//LibVorbis for de-coding ogg files
#include <libvorbis\vorbisfile.h>
using std::string;
using std::vector;
namespace SoundManager
{
	//The maximum number of buffers and audio sources our program will need
#define MAX_AUDIO_BUFFERS 64
#define MAX_AUDIO_SOURCES 16

	//Useless typedef for samples in decoding ogg's
	typedef short SAMPLES;
	//Boolean for whether the SoundManager has been initialised
	extern bool isInit;
	//The OpenAL Device
	extern ALCdevice *Device;
	//The OpenAL Context
	extern ALCcontext *Context;
	//Path at which the audio is found at
	extern string AudioPath;
	//Boolean for whether sound is currently playing
	extern bool isSoundOn;
	//Initial position of the listener
	extern ALfloat Position[3];
	//Initial velocity of the listener
	extern ALfloat Velocity[3];
	//Initial orientation of the listener, and holds 6 items: orientation + orientation's direction UP
	extern ALfloat Orientation[6];

	//How many audio sources are being used
	extern unsigned int AudioSourcesInUseCount;
	//Vector of all the audio sources
	extern vector<ALuint> AudioSources;
	//Vector of the audio sources of which are in use
	extern vector<bool> AudioSourcesInUse;

	//How many audio buffers are being used
	extern unsigned int AudioBuffersInUseCount;
	//Vector of all the audio buffers
	extern vector<ALuint> AudioBuffers;
	//Vector of the audio buffers of which are in use
	extern vector<bool> AudioBuffersInUse;
	//Holds the file names (paths) of a sound in the buffer
	extern vector<string> AudioBufferFileName;

	//Function to locate whether a sound has already been loaded to a buffer
	extern int LocateAudioBuffer(string path);
	//Functino to load audio into an OpenAL buffer
	extern int LoadAudioToBuffer(string path);
	//Function to decode a .ogg file and put that into a buffer, takes in a destination buffer 
	extern bool LoadOGG(string path, ALuint destinationAudioBuffer);
	//Incomplete function to load a .wav file and put that into a buffer, takes in a destination buffer
	extern bool LoadWAV(string path, ALuint destinationAudioBuffer);

	//Initialises the SoundManager singleton class, takes the position, velocity and orientation of the listener
	extern void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up);
	//Destroys the SoundManager
	extern void Kill();

	//Returns whether sound is playing
	extern bool IsSoundOn();
	//Sets the audio path for where audio files are found
	extern void SetAudioPath(string path);

	//Lists the available OpenAL devices
	extern string ListAvailableDevices();

	//Loads the audio, takes in the path at which the file is found, an uint for the ID and a bool for whether you want to loop the audio
	extern bool LoadAudio(string path, unsigned int *audioID, bool loop);
	//Releases and deletes the audio from OpenAL memory
	extern bool ReleaseAudio(unsigned int audioID);
	//Plays an audio, takes the audio to play and a bool for whether you want to start playing from the start, if set to false, it just continues from where it was paused
	extern bool PlayAudio(unsigned int audioID, bool forceRestart);
	//Stops an audio, takes the audio to stop
	extern bool StopAudio(unsigned int audioID);
	//Loops through all audio sources and stops them all
	extern bool StopAllAudio();

	//Pauses the audio, takes the audio to be paused
	extern bool PauseAudio(unsigned int audioID);
	//Loops through each audio source and pauses all of them
	extern bool PauseAllAudio();
	//Resumes a specific audio, takes an audio to be resumed
	extern bool ResumeAudio(unsigned int audioID);
	//Loops through each audio source and the resumes all of them
	extern bool ResumeAllAudio();
	//Sets the audio source position for a certain audio, takes the audio, and a Vec3 for the position you want to move it to
	extern bool SetSoundPosition(unsigned int audioID, Vec3 position);
	//Sets the audio source position for a certain audio, takes the audio, and a Vec3 for the position you want to move it to, the velocity of the source and the direction of the source
	extern bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction);
	//Sets a audio source at a position, takes the sound and Vec3s for position, velocity, direction. Takes a float for the maximum distance from the listener it can be heard at, a boolean for whether to play the audio, a boolean for whether to loop the audio, and a float for the minimum gain of the audio emitted by the source
	extern bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool loop, float minGain);
	//Sets the position of the OpenAL listener, takes in Vec3s for position, velocity and a GLM Quaternion for the orientation
	extern bool SetListenerPosition(Vec3 position, Vec3 velocity, glm::quat orientation);
}
*/