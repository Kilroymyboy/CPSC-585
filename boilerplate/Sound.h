#pragma once
#include <al.h>
#include <alc.h>
#include <windows.h>
#include <cstring>
#include <string>
#include <direct.h>
#include <cstdlib>
#include <iostream>

namespace Sound
{
	void init();
	void destroy();
	void setListener();
	void list_audio_devices(const ALCchar *devices);
	void checkError();
	char* findPathToFile();
	ALenum formatWav(wavFile wav);
	wavFile openWavFile(string fileName, wavFile toPlay);


	extern ALCdevice device;
	extern ALCcontext *ctx;
	
};


struct wavFile {

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;
	ALuint buffer;
	ALuint source;
	ALenum format;
	unsigned char* songBuf;

};
