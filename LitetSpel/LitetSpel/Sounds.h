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

	bool PlayLandingSound01();
	bool PlayLandingSound02();
	bool PlayLandingSound03();

	bool PlayBlobSound01();
	bool PlayBlobSound02();
	bool PlayBlobSound03();

	bool PlayEnemySound01();
	bool PlayEnemySound02();
	bool PlayEnemySound03();

	bool PlayAbsorbSound01();
	bool PlayAbsorbSound02();
	bool PlayAbsorbSound03();

	bool PlayPlayerDeathSound();
	bool PlayDamagedSound01();
	bool PlayDamagedSound02();
	bool PlayStartOfLevelSound();
	bool PlayEndOfLevelSound();

	bool PlayTestSound01();
	bool StartMenuMusic();
	bool StopMenuMusic();
	bool StartGameMusic();
	bool StopGameMusic();
	bool ContinueGameMusic();
	bool StopAllSFX();
	bool ContinueAllSFX();

	void setMasterVolume(UINT master);
	void setSFXVolume(UINT sfx);
	void setMusicVolume(UINT music);
	UINT getMasterVolume() const { return this->masterVolume; };
	UINT getSFXVolume() const { return this->sfxVolume; };
	UINT getMusicVolume() const { return this->musicVolume; };
	void applyVolumes();

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
	bool PlayLoopingWaveFile(IDirectSoundBuffer8* sound);
	bool StopLoopingWaveFile(IDirectSoundBuffer8* sound);
	bool ContinueLoopingWaveFile(IDirectSoundBuffer8* sound);
	bool SetVolume(IDirectSoundBuffer8* sound, LONG volume);
	bool StopSFX(IDirectSoundBuffer8* sound);
	bool ContinueSFX(IDirectSoundBuffer8* sound);

private:
	UINT masterVolume;
	UINT sfxVolume;
	UINT musicVolume;
	bool defaultVolume = true;

	IDirectSound8* directSound;
	IDirectSoundBuffer* firstBuffer;

	//Test
	IDirectSoundBuffer8* test01;
	//Music
	IDirectSoundBuffer8* menu01;
	IDirectSoundBuffer8* game01;
	IDirectSoundBuffer8* pStart;
	IDirectSoundBuffer8* pGoal;

	//SFX
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

	IDirectSoundBuffer8* sorb01;
	IDirectSoundBuffer8* sorb02;
	IDirectSoundBuffer8* sorb03;

	IDirectSoundBuffer8* pDeath;
	IDirectSoundBuffer8* pDmg01; //High health
	IDirectSoundBuffer8* pDmg02; //Lower Health
};

#endif // !SOUNDS_H
