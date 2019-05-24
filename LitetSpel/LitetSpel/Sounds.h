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

	bool PlayMenuHighlightSound();
	bool PlayMenuClickSound();
	bool PlayMenuBackSound();

	bool PlayTestSound01();
	bool StartMenuMusic();
	bool StopMenuMusic();
	bool StartGameMusic();
	bool StopGameMusic();
	bool ContinueGameMusic();
	bool StopAllSFX();
	bool ContinueAllSFX();
	bool StartPlayerMoveLoop();
	bool StopPlayerMoveLoop();

	bool PlayEnemyJumpSound(int index, float distance);
	bool InitializeEnemySounds(int nrOfEnemies);

	void setLimits(float inner, float outer);
	void setMasterVolume(UINT master);
	void setSFXVolume(UINT sfx);
	void setMusicVolume(UINT music);
	UINT getMasterVolume() const { return this->masterVolume; };
	UINT getSFXVolume() const { return this->sfxVolume; };
	UINT getMusicVolume() const { return this->musicVolume; };

	bool RestartSounds();

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
	bool PlayMusicWaveFile(IDirectSoundBuffer8* sound);
	bool PlaySFXWaveFile(IDirectSoundBuffer8* sound);
	bool PlayLoopingMusicWaveFile(IDirectSoundBuffer8* sound);
	bool PlayLoopingSFXWaveFile(IDirectSoundBuffer8* sound);
	bool StopLoopingWaveFile(IDirectSoundBuffer8* sound);
	bool ContinueLoopingWaveFile(IDirectSoundBuffer8* sound);
	bool SetVolume(IDirectSoundBuffer8* sound, LONG volume);
	bool StopSFX(IDirectSoundBuffer8* sound);
	bool ContinueSFX(IDirectSoundBuffer8* sound);
	void ShutdownEnemySounds();

private:
	UINT masterVolume;
	UINT sfxVolume;
	UINT musicVolume;
	float innerLimit;
	float outerLimit;
	bool gameMusicPlaying, menuMusicPlaying;
	bool playerMoving;

	IDirectSound8* directSound;
	IDirectSoundBuffer* firstBuffer;

	//Test
	IDirectSoundBuffer8* test01;
	//Music
	IDirectSoundBuffer8* menu01; //Menu sound loop
	IDirectSoundBuffer8* game01; //During game sound loop
	IDirectSoundBuffer8* pStart; //Start of level sound
	IDirectSoundBuffer8* pGoal; //End of level sound

	//SFX
	IDirectSoundBuffer8* jump01; //Jump for different power ups
	IDirectSoundBuffer8* jump02; //Heavy
	IDirectSoundBuffer8* jump03; //Bouncy

	IDirectSoundBuffer8* land01; //Landing for different power ups
	IDirectSoundBuffer8* land02; //Heavy
	IDirectSoundBuffer8* land03; //Bouncy

	IDirectSoundBuffer8* blob01; //Shoot
	IDirectSoundBuffer8* blob02; //Landing
	IDirectSoundBuffer8* blob03; //Recall

	IDirectSoundBuffer8* enmy01; //Death
	IDirectSoundBuffer8* enmy02;
	IDirectSoundBuffer8* enmy03; //Enemy Stuck

	IDirectSoundBuffer8* sorb01; //Absorb blob
	IDirectSoundBuffer8* sorb02; //Absorb enemy
	IDirectSoundBuffer8* sorb03; //Absorb power up

	IDirectSoundBuffer8* pDeath; //Player Death
	IDirectSoundBuffer8* pDmg01; //High health
	IDirectSoundBuffer8* pDmg02; //Lower Health
	IDirectSoundBuffer8* playerMoveLoop;

	//GUI SFX
	IDirectSoundBuffer8* menuHighlightButton;
	IDirectSoundBuffer8* menuClickButton;
	IDirectSoundBuffer8* menuBack;

	std::vector<IDirectSoundBuffer8*> enemies;
	int nrOfEnemies;

};

#endif // !SOUNDS_H
