/*
#include "SoundManager.h"

namespace SoundManager {

	bool isInit;
	ALCdevice *Device;
	ALCcontext *Context;
	string AudioPath;
	bool isSoundOn;
	ALfloat Position[3];
	ALfloat Velocity[3];
	ALfloat Orientation[6];

	unsigned int AudioSourcesInUseCount;
	vector<ALuint> AudioSources;
	vector<bool> AudioSourcesInUse;

	unsigned int AudioBuffersInUseCount;
	vector<ALuint> AudioBuffers;
	vector<bool> AudioBuffersInUse;
	vector<string> AudioBufferFileName;

	int LocateAudioBuffer(string path);
	int LoadAudioToBuffer(string path);
	bool LoadOGG(string path, ALuint destinationAudioBuffer);

	void Init(Vec3 listener_pos, Vec3 listener_velocity, Vec3 listener_orientation_dir, Vec3 listener_orientation_dir_up);
	void Kill();

	bool IsSoundOn();
	void SetAudioPath(string path);

	string ListAvailableDevices();

	bool LoadAudio(string path, unsigned int *audioID, bool loop);
	bool ReleaseAudio(unsigned int audioID);
	bool PlayAudio(unsigned int audioID, bool forceRestart);
	bool StopAudio(unsigned int audioID);
	bool StopAllAudio();

	bool PauseAudio(unsigned int audioID);
	bool PauseAllAudio();
	bool ResumeAudio(unsigned int audioID);
	bool ResumeAllAudio();
	bool SetSoundPosition(unsigned int audioID, Vec3 position);
	bool SetSoundPosition(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction);
	bool SetSound(unsigned int audioID, Vec3 position, Vec3 velocity, Vec3 direction, float maxDistance, bool playNow, bool loop, float minGain);
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
		Context = alcCreateContext(Device, NULL);
		if (!Context)Console::error("OpenAL Context was not created!");
		alcMakeContextCurrent(Context);

		for (int i = 0; i < MAX_AUDIO_BUFFERS; i++)
		{
			ALuint bufferID = AudioBuffers[i];
			alGenBuffers(1, &bufferID);
		}

		for (int i = 0; i < MAX_AUDIO_SOURCES; i++)
		{
			ALuint sourceID = AudioSources[i];
			alGenBuffers(1, &sourceID);
		}

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
		for (int i = 0; i < AudioSources.size(); i++)
		{
			alDeleteSources(1, &AudioSources[i]);
		}

		for (int i = 0; i < AudioBuffers.size(); i++)
		{
			alDeleteSources(1, &AudioBuffers[i]);
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

	bool LoadAudio(string path, unsigned int *audioID, bool loop)
	{
		if (path.empty())
			Console::warning("Audio not loaded. Filepath is empty."); return false;

		if (AudioSourcesInUseCount == MAX_AUDIO_SOURCES)
			Console::error("Out of Audio Source slots! Increase macro size in SoundManager.h to fit more!"); return false;

		int bufferID, sourceID;
		bufferID = sourceID = -1;
		alGetError();

		bufferID = LocateAudioBuffer(path);
		if (bufferID < 0)
		{
			bufferID = LoadAudioToBuffer(path);
			if (bufferID < 0)return false;
		}

		sourceID = 0;
		while (AudioSourcesInUse[sourceID] == true)
			sourceID++;

		*audioID = sourceID;
		AudioSourcesInUse[sourceID] = true;
		AudioSourcesInUseCount++;

		alSourcei(AudioSources[sourceID], AL_BUFFER, AudioBuffers[bufferID]);
		alSourcei(AudioSources[sourceID], AL_LOOPING, ((loop == true) ? AL_TRUE : AL_FALSE));

		return true;
	}

	int LocateAudioBuffer(string path)
	{
		for (unsigned int i = 0; i < MAX_AUDIO_BUFFERS; i++)
		{
			if (path == AudioBufferFileName[i])
			{
				return (int)i;
			}
		}
		Console::message("File does not already exist in OpenAL buffer");
		return -1;
	}

	int LoadAudioToBuffer(string path)
	{
		if (path.empty())
		{
			return -1;
		}

		if (AudioBuffersInUseCount == MAX_AUDIO_BUFFERS)
		{
			Console::warning("No more available slots for OpenAL Audio Buffers! Increase macro value in SoundManager.h for more slots!");
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
		AudioBufferFileName[bufferID] = path;
		AudioBuffersInUseCount++;
		return bufferID;
	}

	bool PlayAudio(unsigned int audioID, bool forceRestart)
	{
		if (audioID >= MAX_AUDIO_SOURCES || AudioSourcesInUse[audioID])
		{
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
			return false;
		}
		int sourceAudioState = 0;
		alGetError();
		alGetSourcei(AudioSources[audioID], AL_SOURCE_STATE, &sourceAudioState);
		if (sourceAudioState == AL_PLAYING)
		{
			if (forceRestart)
			{
				StopAudio(audioID);
			}
			else
			{
				return false;
			}
		}

		alSourcePlay(AudioSources[audioID]);
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

		for (int i = 0; i < AudioSources.size(); i++)
		{
			alSourcePause(AudioSources[i]);
		}

		return true;
	}

	bool ResumeAudio(unsigned int audioID)
	{
		if (audioID >= MAX_AUDIO_SOURCES || !AudioSourcesInUse[audioID])
		{
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
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
			Console::warning("No more audio source slots available. Increase macro value in SoundManager.h for more slots!");
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
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
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
			Console::warning("No more audio source slots available. Increase macro value in SoundManager.h for more slots!");
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
			Console::error("Audio ID is not valid and usable OR there are no more audio sources available. Increase macro value in SoundManager.h for more slots!");
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

		alBufferData(destinationAudioBuffer, format, &samples[0], ov_pcm_total(&oggfile, -1), fileInformation->rate);

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

} */