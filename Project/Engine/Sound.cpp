#include "Sound.h"

Sound::Sound()
{
	context = 0;
	device = alcOpenDevice(0);
	if (device == nullptr) Console::warning("OpenAL Device was not created!");
	context = alcCreateContext(device, 0);
	if (context == nullptr)
	{
		alcCloseDevice(device);
		Console::warning("OpenAL Context was not created! OpenAL Device has been deleted.");
	}

}


void Sound::PlaySFX( string path)
{
	if (device == nullptr) Console::error("OpenAL Device not found!");
	if (context == nullptr) Console::error("OpenAL Context not found!");
	if (path.empty()) Console::error("The specified Sound filepath could not be found or the filepath was empty!");
	if (path.length() >= OGGextension.length()) {
	//	if(0 == path.compare(path.length() - OGGextension.length(), OGGextension.length(), OGGextension)) LoadOGG(); //<--- finish the function calls
	}
	ALCcontext *previousContext = alcGetCurrentContext();
	alcMakeContextCurrent(context);
	ALuint bufferID = NULL;
	alGenBuffers(1, &bufferID);
	if (!bufferID)
	{
		Console::error("OpenAL Buffer was not created!");
		if (previousContext != nullptr) alcMakeContextCurrent(previousContext);
	}
}

bool Sound::LoadOGG(string path, vector<char> &buffer, ALenum &format, ALsizei &frequency)
{
	int endian = NULL;
	int bitStream; // Bit stream
	long audioBytes; // Bytes of audio
	char array[32768]; // Local fixed size array
	FILE *oggFile; // File
	oggFile = fopen(path.c_str(), "r+");
	if (oggFile == nullptr) Console::warning("Could not open the file for reading!");
	
	//Vars for vorbis SDK
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

	//Decode the OGG and put the audio data into the OpenAL buffer
	do
	{
		//Read a buffers decoded sound data
		audioBytes = ov_read(&oggAudioFile, array, 32768, endian, 2, 1, &bitStream);
		//Append data to buffer's end
		buffer.insert(buffer.end(), array, array + audioBytes);
	} while (audioBytes > 0);

	//Clean up
	ov_clear(&oggAudioFile);
	return true;
}

Sound::~Sound()
{
	alcCloseDevice(device);
	alcDestroyContext(context);
}
