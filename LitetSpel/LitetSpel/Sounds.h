#ifndef SOUNDS_H
#define SOUNDS_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<stdio.h>

class Sounds
{
public:
	Sounds();
	Sounds(const Sounds &sound);
	~Sounds();

	bool InitializeSound(HWND handle);
	void Shutdown();

	bool LoadWaveToBuffer(char* fileName, IDirectSoundBuffer8** soundBuffer);
	bool PlayWaveSound(IDirectSoundBuffer8* sound);
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	bool InitializeDirectSound(HWND handle);
	void ShutdownDirectSound();
	bool LoadWaveFile(char* fileName, IDirectSoundBuffer8** soundBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** soundBuffer);
	bool PlayWaveFile(IDirectSoundBuffer8* sound);
private:

	IDirectSound8* directSound;
	IDirectSoundBuffer* firstBuffer;
	IDirectSoundBuffer8* secondBuffer;
};

#endif // !SOUNDS_H
