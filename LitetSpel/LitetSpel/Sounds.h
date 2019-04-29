#ifndef SOUNDS_H
#define SOUNDS_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<stdio.h>
#include<string>
#include<vector>

class Sounds
{
public:
	Sounds();
	Sounds(const Sounds &sound);
	~Sounds();

	bool InitializeSound(HWND handle);
	void Shutdown();

	bool PlayJumpSound01();
	bool PlayJumpSound02();
	bool PlayJumpSound03();

	bool PlayLandSound01();
	bool PlayLandSound02();
	bool PlayLandSound03();

	bool PlayBlobSound01();
	bool PlayBlobSound02();
	bool PlayBlobSound03();

	bool PlayEnemySound01();
	bool PlayEnemySound02();
	bool PlayEnemySound03();

	bool PlayTestSound01();
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

	IDirectSoundBuffer8* test01;
	IDirectSoundBuffer8* jump01;
	IDirectSoundBuffer8* jump02;
	IDirectSoundBuffer8* jump03;

	IDirectSoundBuffer8* land01;
	IDirectSoundBuffer8* land02;
	IDirectSoundBuffer8* land03;

	IDirectSoundBuffer8* blob01;
	IDirectSoundBuffer8* blob02;
	IDirectSoundBuffer8* blob03;

	IDirectSoundBuffer8* enmy01;
	IDirectSoundBuffer8* enmy02;
	IDirectSoundBuffer8* enmy03;
	//Sounds to add
	/*
	More jump sounds
	Landing
	Shooting blobs, recalling
	Enemy jump
	Music?
	Ambient sound?
	*/
};

#endif // !SOUNDS_H
