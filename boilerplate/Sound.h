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

	void init();
	void destroy();
	void setListener();

	extern ALCdevice device;
	extern ALCcontext *ctx;
	
};

