#pragma once
#include <al.h>
#include <alc.h>
#include <windows.h>
#include <cstring>
#include <string>
#include <direct.h>
#include <cstdlib>
#include <iostream>
#include "Game.h"

namespace Sound
{
	struct wavInfo {

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
	void list_audio_devices(const ALCchar *devices);
	void playSound(ALuint a);
	void stopSound(ALuint a);
	void updateSources();
	void updateListener();

	extern ALCdevice device;
	extern ALCcontext *ctx;
	
};