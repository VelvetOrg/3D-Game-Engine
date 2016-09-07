#include "SoundManager.h"

namespace SoundManager {
	//The maximum number of buffers and audio sources our program will need
#define MAX_AUDIO_BUFFERS 64
#define MAX_AUDIO_SOURCES 16

	//Useless typedef for samples in decoding ogg's
	typedef short SAMPLES;
	//Boolean for whether the SoundManager has been initialised
	 bool isInit;
	//The OpenAL Device
	 ALCdevice *Device;
	//The OpenAL Context
	 ALCcontext *Context;
	//Path at which the audio is found at
	 string AudioPath;
	//Boolean for whether sound is currently playing
	 bool isSoundOn;
	//Initial position of the listener
	 ALfloat Position[3];
	//Initial velocity of the listener
	 ALfloat Velocity[3];
	//Initial orientation of the listener, and holds 6 items: orientation + orientation's direction UP
	 ALfloat Orientation[6];

	 //How many audio sources are being used
	  unsigned int AudioSourcesInUseCount;
	 //Vector of all the audio sources
	  ALuint AudioSources[MAX_AUDIO_SOURCES];
	 //Vector of the audio sources of which are in use
	  bool AudioSourcesInUse[MAX_AUDIO_SOURCES];

	 //How many audio buffers are being used
	  unsigned int AudioBuffersInUseCount;
	 //Vector of all the audio buffers
	  ALuint AudioBuffers[MAX_AUDIO_BUFFERS];
	 //Vector of the audio buffers of which are in use
	  bool AudioBuffersInUse[MAX_AUDIO_BUFFERS];
	 //Holds the file names (paths) of a sound in the buffer
	  char AudioBufferFileName[MAX_AUDIO_BUFFERS][120];

	//Function to locate whether a sound has already been loaded to a buffer
	 int LocateAudioBuffer(string path);
	//Functino to load audio into an OpenAL buffer
	 int LoadAudioToBuffer(string path);
	//Function to decode a .ogg file and put that into a buffer, takes in a destination buffer 
	 bool LoadOGG(string path, ALuint destinationAudioBuffer);
	//Incomplete function to load a .wav file and put that into a buffer, takes in a destination buffer
	 bool LoadWAV(string path, ALuint destinationAudioBuffer);

	//Initialises the SoundManager singleton class, takes the position, velocity and orientation of the listener
	 void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up);
	//Destroys the SoundManager
	 void Kill();

	//Returns whether sound is playing
	 bool IsSoundOn();
	//Sets the audio path for where audio files are found
	 void SetAudioPath(string path);

	//Lists the available OpenAL devices
	 string ListAvailableDevices();

	//Loads the audio, takes in the path at which the file is found, an uint for the ID and a bool for whether you want to loop the audio
	 bool LoadAudio(string path, unsigned int *audioID, bool loop);
	//Releases and deletes the audio from OpenAL memory
	 bool ReleaseAudio(unsigned int audioID);
	//Plays an audio, takes the audio to play and a bool for whether you want to start playing from the start, if set to false, it just continues from where it was paused
	 bool PlayAudio(unsigned int audioID, bool forceRestart);
	//Stops an audio, takes the audio to stop
	 bool StopAudio(unsigned int audioID);
	//Loops through all audio sources and stops them all
	 bool StopAllAudio();

	//Pauses the audio, takes the audio to be paused
	 bool PauseAudio(unsigned int audioID);
	//Loops through each audio source and pauses all of them
	 bool PauseAllAudio();
	//Resumes a specific audio, takes an audio to be resumed
	 bool ResumeAudio(unsigned int audioID);
	//Loops through each audio source and the resumes all of them
	 bool ResumeAllAudio();
	//Sets the audio source position for a certain audio, takes the audio, and a Vec3 for the position you want to move it to
	 bool SetSoundPosition(unsigned int audioID, Vec3 position);
	//Sets the audio source position for a certain audio, takes the audio, and a Vec3 for the position you want to move it to, the velocity of the source and the direction of the source
	 bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction);
	//Sets a audio source at a position, takes the sound and Vec3s for position, velocity, direction. Takes a float for the maximum distance from the listener it can be heard at, a boolean for whether to play the audio, a boolean for whether to loop the audio, and a float for the minimum gain of the audio emitted by the source
	 bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool loop, float minGain);
	//Sets the position of the OpenAL listener, takes in Vec3s for position, velocity and a GLM Quaternion for the orientation
	 bool SetListenerPosition(Vec3 position, Vec3 velocity, glm::quat orientation);


	void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up)
	{
		isInit = false;
		isSoundOn = false;
		Device = NULL;
		Context = NULL;
		AudioPath = "";

		Position[0] = listener_pos.x;
		Position[1] = listener_pos.y;
		Position[2] = listener_pos.z;

		Velocity[0] = listener_velocity.x;
		Velocity[1] = listener_velocity.y;
		Velocity[2] = listener_velocity.z;

		Orientation[0] = listener_orientation_dir.x;
		Orientation[1] = listener_orientation_dir.y;
		Orientation[2] = listener_orientation_dir.z;
		Orientation[3] = listener_orientation_dir_up.x;
		Orientation[4] = listener_orientation_dir_up.y;
		Orientation[5] = listener_orientation_dir_up.z;

		AudioBuffersInUseCount = 0;
		AudioSourcesInUseCount = 0;

		for (int i = 0; i < MAX_AUDIO_SOURCES; i++)
		{
			AudioSources[i] = 0;
			AudioSourcesInUse[i] = false;
		}

		for (int i = 0; i < MAX_AUDIO_BUFFERS; i++)
		{
			AudioBuffers[i] = 0;
			AudioBuffersInUse[i] = false;
		}

		Device = alcOpenDevice(0);
		if (!Device) Console::error("OpenAL Device was not created!");
		if (Device != nullptr) Console::message("OpenAL Device created.");
		Context = alcCreateContext(Device, NULL);
		if (!Context)Console::error("OpenAL Context was not created!");
		if (Device != nullptr) Console::message("OpenAL Context created.");
		alcMakeContextCurrent(Context);

		/*for (int i = 0; i < MAX_AUDIO_BUFFERS; i++)
		{
			ALuint bufferID = AudioBuffers[i];
			strcpy(AudioBufferFileName[i], "--");
			alGenBuffers(1, &bufferID);
		}

		for (int i = 0; i < MAX_AUDIO_SOURCES; i++)
		{
			ALuint sourceID = AudioSources[i];
			alGenBuffers(1, &sourceID);
		}*/

		for (int i = 0; i < MAX_AUDIO_BUFFERS; i++)
		{
			AudioBuffers[i] = 0;
			strcpy(AudioBufferFileName[i], "--");
			AudioBuffersInUse[i] = false;
		}

		for (int i = 0; i < MAX_AUDIO_SOURCES; i++)
		{
			AudioSources[i] = 0;
			AudioSourcesInUse[i] = false;
		}

		alGenBuffers(MAX_AUDIO_BUFFERS, AudioBuffers);

		alGenSources(MAX_AUDIO_SOURCES, AudioSources);		
		
		alListenerfv(AL_POSITION, Position);

		alListenerfv(AL_VELOCITY, Velocity);

		alListenerfv(AL_ORIENTATION, Orientation);

		alListenerf(AL_GAIN, 1.0);

		alDopplerFactor(1.0);
		alDopplerVelocity(343.0f);

		isInit = true;
		isSoundOn = true;

		Console::message("SoundManager initialised.");
	}

	void Kill()
	{
		for (int i = 0; i < sizeof(AudioSources) / sizeof(AudioSources[0]); i++)
		{
			alDeleteSources(1, &AudioSources[i]);
		}

		for (int i = 0; i < sizeof(AudioBuffers) / sizeof(AudioBuffers[0]); i++)
		{
			alDeleteBuffers(1, &AudioBuffers[i]);
		}

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		alcMakeContextCurrent(0);
		alcDestroyContext(Context);
		if (Device)
		{
			alcCloseDevice(Device);
		}
		Console::message("SoundManager killed.");
	}

	string ListAvailableDevices()
	{
		string deviceList = "Sound Devices Available: ";
		if (alcIsExtensionPresent(0, "AL_ENUMERATION_EXT") == AL_TRUE)
		{
			deviceList = "List of devices: ";
			deviceList += (char*)alcGetString(0, ALC_DEVICE_SPECIFIER);
			deviceList += "\n";
		}
		else
		{
			deviceList += "enumeration error.\n";
		}
		return deviceList;
	}
bool LoadAudio(string path, unsigned int *audioID, bool loop){
	if (path.empty() || path.length() > 120)
		return false;

	if (AudioSourcesInUseCount == MAX_AUDIO_SOURCES)
	{
		Console::error("LoadAudio() : No more audio sources");
		return false;   // out of Audio Source slots!
	}

	int bufferID = -1;   // Identity of the Sound Buffer to use
	int sourceID = -1;   // Identity of the Source Buffer to use

	alGetError();    // Clear Error Code

					 // Check and see if the pSoundFile is already loaded into a buffer
	bufferID = LocateAudioBuffer(path);
	if (bufferID < 0)
	{
		// The sound file isn't loaded in a buffer, lets attempt to load it on the fly
		bufferID = LoadAudioToBuffer(path);
		if (bufferID < 0) return false;   // failed!
	}

	// If you are here, the sound the requester wants to reference is in a buffer.
	// Now, we need to find a free Audio Source slot in the sound system
	sourceID = 0;

	while (AudioSourcesInUse[sourceID] == true) sourceID++;

	// When you are here, 'mSourceID' now represents a free Audio Source slot
	// The free slot may not be at the end of the array but in the middle of it.
	*audioID = sourceID;  // return the Audio Source ID to the caller
	AudioSourcesInUse[sourceID] = true; // mark this Source slot as in use
	AudioSourcesInUseCount++;    // bump the 'in use' counter

								  // Now inform OpenAL of the sound assignment and attach the audio buffer
								  // to the audio source
	alSourcei(AudioSources[sourceID], AL_BUFFER, AudioBuffers[bufferID]);

	// Steven : Not in the original code !!!!!
	alSourcei(AudioSources[sourceID], AL_LOOPING, loop);

	return true;
 }


	int LocateAudioBuffer(string path)
	{
		for (unsigned int b = 0; b < MAX_AUDIO_BUFFERS; b++)
		{
			if (path == AudioBufferFileName[b]) return (int)b; // TODO Careful : converts unsigned to int!
		}
		return -1;      // not found
	}

	int LoadAudioToBuffer(string path)
	{
		if (path.empty())
		{
			return -1;
		}

		if (AudioBuffersInUseCount == MAX_AUDIO_BUFFERS)
		{
			Console::warning("LoadAudioToBuffer() : No more available slots for OpenAL Audio Buffers! Increase macro value in SoundManager.h for more slots!");
			return -1;
		}

		int bufferID = 0;
		while (AudioBuffersInUse[bufferID] == true)
		{
			bufferID;
		}

		if (path.find(".ogg", 0) != string::npos)
		{
			if (!LoadOGG(path, AudioBuffers[bufferID]))
			{
				return -1;
			}
		}

		AudioBuffersInUse[bufferID] = true;
		strcpy(AudioBufferFileName[bufferID], path.c_str());
		AudioBuffersInUseCount++;
		return bufferID;
	}

	bool SoundManager::PlayAudio(unsigned int audioID, bool forceRestart)
	{
		// Make sure the audio source ident is valid and usable
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("PlayAudio() : No more available slots for OpenAL Audio Buffers! Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		int sourceAudioState = 0;

		alGetError();

		// Are we currently playing the audio source?
		alGetSourcei(AudioSources[audioID], AL_SOURCE_STATE, &sourceAudioState);

		if (sourceAudioState == AL_PLAYING)
		{
			if (forceRestart)
				StopAudio(audioID);
			else
				return false; // Not forced, so we don't do anything
		}

		alSourcePlay(AudioSources[audioID]);
		cout << "aID" << audioID << endl;
		return true;
	}

	bool PauseAudio(unsigned int audioID)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			return false;
		}
		alGetError();
		alSourcePause(AudioSources[audioID]);

		return true;
	}

	bool PauseAllAudio()
	{
		if (AudioSourcesInUseCount >= MAX_AUDIO_SOURCES)
		{
			return false;
		}
		alGetError();

		for (int i = 0; i < sizeof(AudioSources) / sizeof(AudioSources[0]); i++)
		{
			alSourcePause(AudioSources[i]);
		}

		return true;
	}

	bool ResumeAudio(unsigned int audioID)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("ResumeAudio() : Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		alGetError();

		alSourcePlay(AudioSources[audioID]);

		return true;
	}

	bool ResumeAllAudio()
	{
		if (AudioSourcesInUseCount >= MAX_AUDIO_SOURCES)
		{
			Console::warning("ResumeAllAudio() : No more audio source slots available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		alGetError();

		int sourceAudioState = 0;

		for (int i = 0; i < AudioSourcesInUseCount; i++)
		{
			alGetSourcei(AudioSources[i], AL_SOURCE_STATE, &sourceAudioState);
			if (sourceAudioState == AL_PAUSED)
			{
				ResumeAudio(i);
			}
		}

		return true;
	}

	bool StopAudio(unsigned int audioID)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("StopAudio() : Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		alGetError();

		alSourceStop(AudioSources[audioID]);

		return true;
	}

	bool StopAllAudio()
	{
		if (AudioSourcesInUseCount >= MAX_AUDIO_SOURCES)
		{
			Console::warning("StopAllAudio() : No more audio source slots available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		alGetError();

		for (int i = 0; i < AudioSourcesInUseCount; i++)
		{
			StopAudio(i);
		}

		return true;
	}

	bool ReleaseAudio(unsigned int audioID)
	{
		if (audioID >= MAX_AUDIO_SOURCES)
		{
			Console::error("ReleaseAudio() : Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}
		alGetError();
		alSourceStop(AudioSources[audioID]);
		AudioSourcesInUse[audioID] = false;
		AudioSourcesInUseCount--;

		return true;
	}

	bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool forceRestart, float minGain)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		ALfloat pos[3] = { position.x, position.y, position.z };
		alSourcefv(AudioSources[audioID], AL_POSITION, pos);

		ALfloat vel[3] = { velocity.x, velocity.y, velocity.z };
		alSourcefv(AudioSources[audioID], AL_VELOCITY, vel);

		ALfloat dir[3] = { direction.x, direction.y, direction.z };
		alSourcefv(AudioSources[audioID], AL_DIRECTION, dir);

		alSourcef(AudioSources[audioID], AL_MAX_DISTANCE, maxDistance);

		alSourcef(AudioSources[audioID], AL_MAX_DISTANCE, maxDistance);

		alSourcef(AudioSources[audioID], AL_MIN_GAIN, minGain);

		alSourcef(AudioSources[audioID], AL_MAX_GAIN, 1.0f);

		alSourcef(AudioSources[audioID], AL_ROLLOFF_FACTOR, 1.0f);

		if (playNow)
		{
			return PlayAudio(audioID, forceRestart);
		}

		return true;
	}

	bool SetSoundPosition(unsigned int audioID, Vec3 position)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		ALfloat pos[3] = { position.x, position.y, position.z };
		alSourcefv(AudioSources[audioID], AL_POSITION, pos);

		return true;
	}

	bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}

		ALfloat pos[3] = { position.x, position.y, position.z };
		alSourcefv(AudioSources[audioID], AL_POSITION, pos);

		ALfloat vel[3] = { velocity.x, velocity.y, velocity.z };
		alSourcefv(AudioSources[audioID], AL_VELOCITY, vel);

		ALfloat dir[3] = { direction.x, direction.y, direction.z };
		alSourcefv(AudioSources[audioID], AL_DIRECTION, dir);

		return true;
	}

	bool SetListenerPosition(Vec3 position, Vec3 velocity, glm::quat orientation)
	{
		Vec3 axis;

		ALfloat pos[3] = { position.x, position.y, position.z };
		alListenerfv(AL_POSITION, pos);

		ALfloat vel[3] = { velocity.x, velocity.y, velocity.z };
		alListenerfv(AL_VELOCITY, vel);

		axis = Vec3(0, 0, 0);
		//USE GLM QUATERNIONS
		//TEMPORARY
		axis.x = orientation.z;
		axis.y = orientation.y;
		axis.z = orientation.x;

		ALfloat dir[3] = { axis.x, axis.y, axis.z };
		alListenerfv(AL_ORIENTATION, vel);

		alListenerf(AL_MAX_DISTANCE, 10000.0f);
		alListenerf(AL_MIN_GAIN, 0.0);
		alListenerf(AL_MIN_GAIN, 1.0);

		return true;
	}

	bool LoadOGG(string path, ALuint destinationAudioBuffer)
	{

		OggVorbis_File oggfile;

		if (ov_fopen(const_cast<char*>(path.c_str()), &oggfile))
		{
			Console::error("OggVorbis_File 'LoadOGG' function ERROR : ov_fopen");
			return false;
		}

		vorbis_info * fileInformation = ov_info(&oggfile, -1);

		ALenum format;

		switch (fileInformation->channels)
		{
		case 1:
			format = AL_FORMAT_MONO16;
			break;

		case 2:
			format = AL_FORMAT_STEREO16;
			break;

		case 4:
			format = alGetEnumValue("AL_FORMAT_QUAD16");
			break;

		case 6:
			format = alGetEnumValue("AL_FORMAT_51CHN16");
			break;

		case 7:
			format = alGetEnumValue("AL_FORMAT_61CHN16");
			break;

		case 8:
			format = alGetEnumValue("AL_FORMAT_71CHN16");
			break;
		default:
			format = 0;
			break;
		}

		vector<short> samples;

		char temporaryBuffer[32768]; // Local temporary array of chars

		int section = 0;
		bool first = true;

		while (true)
		{
			int result = ov_read(&oggfile, temporaryBuffer, 4096, 0, 2, 1, &section);
			if (result > 0)
			{
				first = false;
				samples.insert(samples.end(), temporaryBuffer, temporaryBuffer + (result));
			}
			else
			{
				if (result < 0)
				{
					Console::error("OggVorbis_File 'LoadOGG' function ERROR : Loading OGG sound data into buffer failed!");
				}
				else
				{
					if (first)
					{
						return false;
					}
					break;
				}
			}
		}

		alBufferData(destinationAudioBuffer, format, &samples[0], (ALsizei)ov_pcm_total(&oggfile, -1), fileInformation->rate);

		return true;

	}

	bool IsSoundOn()
	{
		return isSoundOn;
	}

	void SetAudioPath(string path)
	{
		AudioPath = path;
	}

}
