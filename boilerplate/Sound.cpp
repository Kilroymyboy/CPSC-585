#include "Sound.h"
using namespace std;
using namespace Sound;
namespace Sound 
{
	wavFile wav, wav2, wav3, wav4; //creative names later
	void init()
	{
		/*Open a device(In this case the default device)*/
		ALCdevice *device = alcOpenDevice(NULL);
		if (!device)
		{
			fprintf(stderr, "Oops\n");
		}


		/*Check to see if there are other devices to access*/
		ALboolean enumeration;
		enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
		{
			fprintf(stderr, "Enumeration not supported\n");
		}
		else
		{
			fprintf(stderr, "Lets Enumerate\n");
			list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
		}

		/*Reset the error stack has a depth of 1 error.  Do this to help determine where the errors are*/
		checkError();


		/*Initializing a context to render an audio scene*/
		ALCcontext *ctx = alcCreateContext(device, NULL);
		
		if (alcMakeContextCurrent(ctx))
		{
			fprintf(stderr, "Oops2\n");
		}
		checkError();

		/*Configuring a listener*/
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alListener3f(AL_POSITION, 0, 0, 1.0f);
		checkError();

		alListener3f(AL_VELOCITY, 0, 0, 0);
		checkError();

		alListenerfv(AL_ORIENTATION, listenerOri);
		checkError();


		/*Source Generation creating an audio source object which is the origin of the sound*/

		alGenSources(1, &wav.source);
		checkError();

		alGenSources(1, &wav2.source);
		checkError();

		alGenSources(1, &wav3.source);
		checkError();

		alGenSources(1, &wav4.source);
		checkError();


		wav.source = setSource(wav.source);
		wav2.source = setSource(wav2.source);
		wav3.source = setSource(wav3.source);
		wav4.source = setSource(wav4.source);



		/*Buffer Generation this holds the raw audio stream*/

		alGenBuffers(1, &wav.buffer);
		checkError();

		alGenBuffers(1, &wav2.buffer);
		checkError();

		alGenBuffers(1, &wav3.buffer);
		checkError();

		alGenBuffers(1, &wav4.buffer);
		checkError();
		
		
		string firstWav = "\\test.wav";
		string secondWav = "\\test2.wav";
		string thirdWav = "\\test3.wav";
		string fourthWav = "\\test4.wav";

		char* pathToWav = findPathToFile();

		firstWav = pathToWav + firstWav;
		secondWav = pathToWav + secondWav;
		thirdWav = pathToWav + thirdWav;
		fourthWav = pathToWav + fourthWav;


		wav = openWavFile(firstWav, wav);
		wav2 = openWavFile(secondWav, wav2);
		wav3 = openWavFile(thirdWav, wav3);
		wav4 = openWavFile(fourthWav, wav4);

		wav.format = formatWav(wav);
		wav2.format = formatWav(wav2);
		wav3.format = formatWav(wav3);
		wav4.format = formatWav(wav4);

	}


	void destroy()
	{
		//alDeleteSources(1, &source);
		//alDeleteBuffers(1, &buffer);
		//device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		//alcDestroyContext(context);
		//alcCloseDevice(device);
	}

	void setListener()
	{
		/*Configuring a listener*/
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		alListener3f(AL_POSITION, 0, 0, 1.0f);
		checkError();
		alListener3f(AL_VELOCITY, 0, 0, 0);
		checkError();
		alListenerfv(AL_ORIENTATION, listenerOri);
		checkError();
	}

	/*Initializes the wav file to be read*/
	wavFile openWavFile(string fileName, wavFile toPlay)
	{
		wavFile wav = toPlay;

		char type[4];
		DWORD size, chunkSize;
		short formatType, channels;
		DWORD sampleRate, avgBytesPerSec;
		short bytesPerSample, bitsPerSample;
		DWORD dataSize;
		unsigned char* buf;



		FILE *fp = nullptr;
		fopen_s(&fp, fileName.c_str(), "rb");


		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
		{
			cout << "RIFF PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
			cout << "No RIFF" << endl;
		}

		fread(&size, sizeof(DWORD), 1, fp);

		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
		{
			cout << "WAVE PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
			cout << "Not a WAVE" << endl;
		}

		fread(type, sizeof(char), 4, fp);

		if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
		{
			cout << "FMT PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
			cout << "No fmt " << endl;
		}

		fread(&chunkSize, sizeof(DWORD), 1, fp);
		fread(&formatType, sizeof(short), 1, fp);
		fread(&channels, sizeof(short), 1, fp);
		fread(&sampleRate, sizeof(DWORD), 1, fp);
		fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
		fread(&bytesPerSample, sizeof(short), 1, fp);
		fread(&bitsPerSample, sizeof(short), 1, fp);

		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
		{
			cout << "DATA PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
			cout << "No DATA" << endl;
		}

		fread(&dataSize, sizeof(DWORD), 1, fp);

		buf = new unsigned char[dataSize];
		fread(buf, sizeof(BYTE), dataSize, fp);

		wav.size = size;
		wav.chunkSize = chunkSize;
		wav.formatType = formatType;
		wav.channels = channels;
		wav.sampleRate = sampleRate;
		wav.avgBytesPerSec = avgBytesPerSec;
		wav.bytesPerSample = bytesPerSample;
		wav.bitsPerSample = bitsPerSample;
		wav.dataSize = dataSize;
		wav.songBuf = buf;
		return wav;

	}


	ALenum formatWav(wavFile wav)
	{
		ALenum format;
		if (wav.bitsPerSample == 8)
		{
			if (wav.channels == 1)
				format = AL_FORMAT_MONO8;
			else if (wav.channels == 2)
				format = AL_FORMAT_STEREO8;
		}
		if (wav.bitsPerSample == 16)
		{
			if (wav.channels == 1)
				format = AL_FORMAT_MONO16;
			else if (wav.channels == 2)
				format = AL_FORMAT_STEREO16;
		}
		return format;

	}

	/*Modify volume, pitch distance*/
	ALuint setSource(ALuint source)
	{
		alSourcef(source, AL_PITCH, 1.0f);
		checkError();

		alSourcef(source, AL_GAIN, 1.0f);
		checkError();

		alSource3f(source, AL_POSITION, 0, 0, 0);
		checkError();

		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		checkError();

		alSourcei(source, AL_LOOPING, AL_FALSE);
		checkError();

		return source;
	}

	/*Not nessessary, more for checking the lists*/
	void list_audio_devices(const ALCchar *devices)
	{
		const ALCchar *device = devices, *next = devices + 1;
		size_t len = 0;

		fprintf(stdout, "Devices list:\n");
		fprintf(stdout, "----------\n");
		while (device && *device != '\0' && next && *next != '\0') {
			fprintf(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		fprintf(stdout, "----------\n");
	}

	void checkError()
	{
		ALCenum error;
		error = alGetError();
		if (error != AL_NO_ERROR)
			perror("CHECK ERROR");
	}


	char* findPathToFile()
	{
		char* currDir;

		// Get the current working directory:   
		if ((currDir = _getcwd(NULL, 0)) == NULL)
		{
			perror("_getcwd error");
			return false;
		}
		else
		{
			return currDir;
		}
	}
}