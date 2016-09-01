/*
#include "SoundManager.h"

void SoundManager::Init(Vec3 listener_pos = Vec3(0, 0, 0), Vec3 listener_velocity = Vec3(0, 0, 0), Vec3 listener_orientation = Vec3(0, 0, -1))
{
	context = NULL;
	device = alcOpenDevice(0);
	if (device == nullptr) Console::error("OpenAL Device was not created!");
	context = alcCreateContext(device, 0);
	if (context == nullptr)
	{
		alcCloseDevice(device);
		Console::error("OpenAL Context was not created! OpenAL Device has been deleted.");
	}
	alListener3f(AL_POSITION, listener_pos.x, listener_pos.y, listener_pos.z);
	alListener3f(AL_VELOCITY, listener_velocity.x, listener_velocity.y, listener_velocity.z);
	alListener3f(AL_ORIENTATION, listener_orientation.x, listener_orientation.y, listener_orientation.z);
}

bool SoundManager::AddSFX(string key, string path)
{
	ALint state;                            // The state of the sound source
	ALuint bufferID;                        // The OpenAL sound buffer ID
	ALuint sourceID;                        // The OpenAL sound source
	ALenum format;                          // The sound data format
	ALsizei frequency;                      // The frequency of the sound data
	vector<char> bufferData;                // The sound buffer data from file
	ALvoid* data;
	ALsizei size;
	if (device == nullptr) Console::error("OpenAL Device not found!");
	if (context == nullptr) Console::error("OpenAL Context not found!");
	if (path.empty()) Console::error("The specified Sound filepath could not be found or the filepath was empty!");
	alGenBuffers(1, &bufferID);
	//Enter de-coding and conversion to raw audio data
	if (path.length() >= oggextension.length()) { // if it is a .ogg
		if (0 == path.compare(path.length() - oggextension.length(), oggextension.length(), oggextension))
		{
			LoadOGG(path.c_str(), bufferData, format, frequency);
			alBufferData(bufferID, format, &bufferData, static_cast<ALsizei>(buffer.size()), frequency);
			Console::message("WAV is in memory! :)");
			//alBufferData(bufferID, format, ConvertOGG(path, format, frequency), buffer.size(), frequency);
		}
	}
	else // if it is not a .ogg
	{
		//alutLoadWAVFile((ALbyte*)path.c_str(), &format, &data, &size, &frequency, (ALboolean*)false);
		//alBufferData(bufferID, format, data, size, frequency);
		bufferID = alutCreateBufferFromFile(path.c_str());
		if (!bufferID) Console::error("WAV File Buffer did not get created!");
	}
	//Continue flow
	//sfx_map.insert({ key,  bufferID});
	sfx_map[key] = bufferID;
	alDeleteBuffers(1, &bufferID);
	return true;
}

bool SoundManager::AddSong(string key, string path)
{
	
	return true;
}

void SoundManager::PlaySFX(string sfx_key, Vec3 position_sfx = Vec3(0, 0, 0), Vec3 velocity_sfx = Vec3(0, 0, 0), bool is3D = true, float pitch = 1, float gain = 1)
{
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, sfx_map[sfx_key]);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSource3f(source, AL_POSITION, position_sfx.x, position_sfx.y, position_sfx.z);
	alSource3f(source, AL_VELOCITY, velocity_sfx.x, velocity_sfx.y, velocity_sfx.z);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_SOURCE_RELATIVE, (is3D == true) ? AL_FALSE : AL_TRUE);
	alSourcePlay(source);
	alDeleteSources(1, &source);
}

void SoundManager::PlaySong(string song_key, Vec3 position_song = Vec3(0, 0, 0), Vec3 velocity_song = Vec3(0, 0, 0), bool loop = true, bool is3D = true, float pitch = 1, float gain = 1)
{
	ALuint source;
	alGenSources(1, &source);

	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSource3f(source, AL_POSITION, position_song.x, position_song.y, position_song.z);
	alSource3f(source, AL_VELOCITY, velocity_song.x, velocity_song.y, velocity_song.z);
	alSourcei(source, AL_LOOPING, (loop == true) ? AL_TRUE : AL_FALSE);
	alSourcei(source, AL_SOURCE_RELATIVE, (is3D == true) ? AL_FALSE : AL_TRUE);
	alSourcePlay(source);
	alDeleteSources(1, &source);

}

ALvoid* SoundManager::ConvertOGG(string path, ALenum &format, ALsizei &frequency)
{
	int endian = NULL;
	int i = 0;
	int bitStream; // Bit stream
	long audioBytes; // Bytes of audio
	char array[32768]; // Local fixed size array
	FILE *oggFile; // File
	oggFile = fopen(path.c_str(), "r+");
	if (oggFile == nullptr) Console::warning("Could not open the file for reading!");

	//Variables for libvorbis SDK
	vorbis_info *oggFileInformation; // Audio file information
	OggVorbis_File oggAudioFile;
	//Pass the filepath and the actual file to the SDK
	ov_open(oggFile, &oggAudioFile, 0, 0);
	//Get information about the OGG File
	oggFileInformation = ov_info(&oggAudioFile, -1);
	//Use 16-bit channels
	format = AL_FORMAT_MONO16;
	//Sampling rate's frequency
	frequency = oggFileInformation->rate;

	//Decode the OGG into audio data
	do
	{
		//Read a buffers decoded sound data
		audioBytes = ov_read(&oggAudioFile, array, 32768, endian, 2, 1, &bitStream);
		//Append data to buffer's end
		buffer.insert(buffer.end(), array, array + audioBytes);
	} while (audioBytes > 0);

	//Clean up
	ov_clear(&oggAudioFile);
	return (ALvoid*)buffer[i++];
}

void SoundManager::LoadOGG(string fileName, vector<char> &buffer, ALenum &format, ALsizei &freq)
{
	int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[32768];                // Local fixed size array
	FILE *f;

	// Open for binary reading
	f = fopen(fileName.c_str(), "rb");

	if (f == NULL)
	{
		Console::error("FILE is equal to nullptr");
		exit(-1);
	}
	// end if

	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	// Try opening the given file
	if (ov_open(f, &oggFile, NULL, 0) != 0)
	{
		Console::error("Could not openn FILE for de-encoding");
		exit(-1);
	}
	// end if

	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	// end if

	// The frequency of the sampling rate
	freq = pInfo->rate;

	// Keep reading until all is read
	do
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, array, 32768, endian, 2, 1, &bitStream);

		if (bytes < 0)
		{
			ov_clear(&oggFile);
			Console::error("Error while de-coding the FILE");
			exit(-1);
		}
		// end if

		// Append to end of buffer
		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);

	// Clean up!
	ov_clear(&oggFile);
}

void SoundManager::Kill()
{
	alcMakeContextCurrent(0);
	alcDestroyContext(context);
	alcCloseDevice(device);
	Console::message("SoundManger ended. OpenAL Device has been closed and the OpenAL Context has been destroyed");
}
*/