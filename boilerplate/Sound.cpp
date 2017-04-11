#include "Sound.h"
using namespace std;
using namespace Sound;

wavInfo song;

wavInfo PlayerFront;
wavInfo PlayerBack;

namespace Sound 
{
	string fileSong = "\\test.wav";
	string fileEngine = "\\car_v.wav";
	ALuint setSource(ALuint source);
	void checkError();
	wavInfo openWavFile(string fileName, wavInfo toPlay);
	ALenum formatSound(short channels, short samples);

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
		
		if (!alcMakeContextCurrent(ctx))
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
		alGenSources(1, &song.source);
		checkError();
		alGenSources(1, &PlayerBack.source);
		checkError();
		alGenSources(1, &PlayerFront.source);
		checkError();

		song.source = setSource(song.source);
		checkError();
		PlayerBack.source = setSource(PlayerBack.source);
		checkError();
		PlayerFront.source = setSource(PlayerFront.source);
		checkError();

		/*Buffer Generation this holds the raw audio stream*/
		alGenBuffers(1, &song.buffer);
		alGenBuffers(1, &PlayerBack.buffer);
		alGenBuffers(1, &PlayerFront.buffer);
		checkError();

		char* curDir = _getcwd(NULL, 0);
		string filename = curDir + fileSong;
		song = openWavFile(filename, song);

		song.format = formatSound(song.channels, song.bitsPerSample);

		filename = curDir + fileEngine;
		PlayerBack = openWavFile(filename, PlayerBack);

		PlayerBack.format = formatSound(PlayerBack.channels, PlayerBack.bitsPerSample);


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


	/*Initializes the wav file to be read*/
	wavInfo openWavFile(string fileName, wavInfo toPlay)
	{
		wavInfo wav = toPlay;

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


		cout << "bits " << bitsPerSample << endl;

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


	ALenum formatSound(short channels, short samples)
	{
		bool stereo = (channels > 1);

		cout << "samples " << samples << endl;

		switch (samples) {
		case 16:
			if (stereo)
				return AL_FORMAT_STEREO16;
			else
				return AL_FORMAT_MONO16;
		case 8:
			if (stereo)
				return AL_FORMAT_STEREO8;
			else
				return AL_FORMAT_MONO8;
		default: 
			return -1;
		}
	}


	void updateSources()
	{
		PxTransform frontPos = Game::aventador0->actor->getGlobalPose();
		alSource3f(1, AL_POSITION, (ALfloat)frontPos.p.x, (ALfloat)frontPos.p.y, (ALfloat)frontPos.p.z);

		float dist = glm::distance(glm::vec3(frontPos.p.x, frontPos.p.y, frontPos.p.z), glm::vec3(1));

		alDistanceModel(AL_INVERSE_DISTANCE);
		ALfloat vol = 4 / (4 +  2  * (dist)-4);



		alGetSourcef(1, AL_GAIN, &vol);
		alSourcef(1, AL_GAIN, vol + .6);



		//alSourcef(1, AL_PITCH, 1.0f);
		//checkError();


		//alSourcef(1, AL_GAIN, .2);
		//checkError();

		//alSourcef(2, AL_GAIN, 2);
		//checkError();



		PxTransform backPos = Game::aventador0->actor->getGlobalPose();
		alSource3f(2, AL_POSITION, backPos.p.x, backPos.p.y, backPos.p.z);

	}

	void updateListener()
	{
		PxTransform backPos = Game::aventador1->actor->getGlobalPose();

		alListener3f(AL_POSITION, backPos.p.x, backPos.p.y, backPos.p.z);
	}


	void playSound(ALuint a)
	{
		wavInfo toPlay;
		switch (a)
		{
		case 1:
			toPlay = song;
			break;
		case 2:
			toPlay = PlayerBack;

		default:
			break;

		}

		ALint source_state;



		alGetSourcei(toPlay.source, AL_SOURCE_STATE, &source_state);
		checkError();
		if (source_state == AL_PLAYING)
		{
			return;
		}
		else
		{
			cout << "I LOVE JOHNNBI" << endl;
			alBufferData(toPlay.buffer, toPlay.format, (ALvoid*)toPlay.songBuf, (ALsizei)toPlay.dataSize, (ALsizei)toPlay.sampleRate);
			checkError();


			alSourcei(toPlay.source, AL_BUFFER, toPlay.buffer);
			checkError();
			alSourcePlay(toPlay.source);
		}
	} 
	void stopSound(ALuint a)
	{
		alSourceStop(a);
	}

} 