#include"Sounds.h"


Sounds::Sounds() {
	this->directSound = nullptr;
	this->firstBuffer = nullptr;
	this->test01 = nullptr;
	this->jump01 = nullptr;
	this->jump02 = nullptr;
	this->jump03 = nullptr;
	this->land01 = nullptr;
	this->land02 = nullptr;
	this->land03 = nullptr;
	this->blob01 = nullptr;
	this->blob02 = nullptr;
	this->blob03 = nullptr;
	this->enmy01 = nullptr;
	this->enmy02 = nullptr;
	this->enmy03 = nullptr;
}
Sounds::Sounds(const Sounds &sound) {

}
Sounds::~Sounds() {
	Shutdown();
}

bool Sounds::InitializeSound(HWND handle) {
	bool results;

results = InitializeDirectSound(handle);
if (results == false) {
	return false;
}
std::string name = "test01.wav";
char testFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10]};


results = LoadWaveFile(testFileName, &test01);
if(results == false){
	return false;
}

name = "menu01.wav";
char menuMusicFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(menuMusicFileName, &menu01);
if (results == false) {
	return false;
}

name = "game01.wav";
char gameMusicFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(gameMusicFileName, &game01);
if (results == false) {
	return false;
}


name = "jump01.wav";
char jumpFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10]};

results = LoadWaveFile(jumpFileName, &jump01);
if (results == false) {
	return false;
}

name = "jump02.wav";
jumpFileName[5] = name[5];

results = LoadWaveFile(jumpFileName, &jump02);
if (results == false) {
	return false;
}

name = "jump03.wav";
jumpFileName[5] = name[5];

results = LoadWaveFile(jumpFileName, &jump03);
if (results == false) {
	return false;
}

name = "land01.wav";
char landFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(landFileName, &land01);
if (results == false) {
	return false;
}

name = "land02.wav";
landFileName[5] = name[5];

results = LoadWaveFile(landFileName, &land02);
if (results == false) {
	return false;
}

name = "land03.wav";
landFileName[5] = name[5];

results = LoadWaveFile(landFileName, &land03);
if (results == false) {
	return false;
}

name = "blob01.wav";
char blobFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(blobFileName, &blob01);
if (results == false) {
	return false;
}

name = "blob02.wav";
blobFileName[5] = name[5];

results = LoadWaveFile(blobFileName, &blob02);
if (results == false) {
	return false;
}

name = "blob03.wav";
blobFileName[5] = name[5];

results = LoadWaveFile(blobFileName, &blob03);
if (results == false) {
	return false;
}

name = "enmy01.wav";
char enemyJumpFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(enemyJumpFileName, &enmy01);
if (results == false) {
	return false;
}

name = "enmy02.wav";
enemyJumpFileName[5] = name[5];

results = LoadWaveFile(enemyJumpFileName, &enmy02);
if (results == false) {
	return false;
}

name = "enmy03.wav";
enemyJumpFileName[5] = name[5];

results = LoadWaveFile(enemyJumpFileName, &enmy03);
if (results == false) {
	return false;
}

name = "sorb01.wav";
char absorbFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(absorbFileName, &sorb01);
if (results == false) {
	return false;
}

name = "sorb02.wav";
absorbFileName[5] = name[5];

results = LoadWaveFile(absorbFileName, &sorb02);
if (results == false) {
	return false;
}

name = "sorb03.wav";
absorbFileName[5] = name[5];

results = LoadWaveFile(absorbFileName, &sorb03);
if (results == false) {
	return false;
}

name = "pdeath.wav";
char playerDeathFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(playerDeathFileName, &pDeath);
if (results == false) {
	return false;
}

name = "pdmg01.wav";
char playerDamagedFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(playerDamagedFileName, &pDmg01);
if (results == false) {
	return false;
}

name = "pdmg01.wav";
playerDamagedFileName[5] = name[5];

results = LoadWaveFile(playerDamagedFileName, &pDmg02);
if (results == false) {
	return false;
}

name = "pstart.wav";
char startLevelFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(startLevelFileName, &pStart);
if (results == false) {
	return false;
}

name = "p-goal.wav";
char reachedGoalFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(reachedGoalFileName, &pGoal);
if (results == false) {
	return false;
}

name = "mhlbtn.wav";
char menuHighlightButtonFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(menuHighlightButtonFileName, &menuHighlightButton);
if (results == false) {
	return false;
}

name = "mclkbt.wav";
char menuClickFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(menuClickFileName, &menuClickButton);
if (results == false) {
	return false;
}

name = "m-back.wav";
char menuBackFileName[11] = { name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7], name[8], name[9], name[10] };

results = LoadWaveFile(menuBackFileName, &menuBack);
if (results == false) {
	return false;
}

return true;
}
void Sounds::Shutdown() {
	ShutdownWaveFile(&test01);
	ShutdownWaveFile(&menu01);
	ShutdownWaveFile(&game01);
	ShutdownWaveFile(&jump01);
	ShutdownWaveFile(&jump02);
	ShutdownWaveFile(&jump03);
	ShutdownWaveFile(&land01);
	ShutdownWaveFile(&land02);
	ShutdownWaveFile(&land03);
	ShutdownWaveFile(&blob01);
	ShutdownWaveFile(&blob02);
	ShutdownWaveFile(&blob03);
	ShutdownWaveFile(&enmy01);
	ShutdownWaveFile(&enmy02);
	ShutdownWaveFile(&enmy03);
	ShutdownWaveFile(&sorb01);
	ShutdownWaveFile(&sorb02);
	ShutdownWaveFile(&sorb03);
	ShutdownWaveFile(&pDeath);
	ShutdownWaveFile(&pDmg01);
	ShutdownWaveFile(&pDmg02);
	ShutdownWaveFile(&pStart);
	ShutdownWaveFile(&pGoal);
	ShutdownWaveFile(&menuHighlightButton);
	ShutdownWaveFile(&menuClickButton);
	ShutdownWaveFile(&menuBack);
	ShutdownDirectSound();
}

bool Sounds::PlayJumpSound01() {
	if (!PlayWaveFile(jump01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayJumpSound02() {
	if (!PlayWaveFile(jump02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayJumpSound03() {
	if (!PlayWaveFile(jump03)) {
		return false;
	}
	return true;
}
bool Sounds::PlayLandingSound01() {
	if (!PlayWaveFile(land01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayLandingSound02() {
	if (!PlayWaveFile(land02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayLandingSound03() {
	if (!PlayWaveFile(land03)) {
		return false;
	}
	return true;
}
bool Sounds::PlayBlobSound01() {
	if (!PlayWaveFile(blob01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayBlobSound02() {
	if (!PlayWaveFile(blob02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayBlobSound03() {
	if (!PlayWaveFile(blob03)) {
		return false;
	}
	return true;
}
bool Sounds::PlayEnemySound01() {
	if (!PlayWaveFile(enmy01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayEnemySound02() {
	if (!PlayWaveFile(enmy02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayEnemySound03() {
	if (!PlayWaveFile(enmy03)) {
		return false;
	}
	return true;
}
bool Sounds::PlayAbsorbSound01() {
	if (!PlayWaveFile(sorb01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayAbsorbSound02() {
	if (!PlayWaveFile(sorb02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayAbsorbSound03() {
	if (!PlayWaveFile(sorb03)) {
		return false;
	}
	return true;
}
bool Sounds::PlayPlayerDeathSound() {
	if (!PlayWaveFile(pDeath)) {
		return false;
	}
	return true;
}
bool Sounds::PlayDamagedSound01() {
	if (!PlayWaveFile(pDmg01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayDamagedSound02() {
	if (!PlayWaveFile(pDmg02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayStartOfLevelSound() {
	if (!PlayWaveFile(pStart)) {
		return false;
	}
	return true;
}
bool Sounds::PlayEndOfLevelSound() {
	if (!PlayWaveFile(pGoal)) {
		return false;
	}
	return true;
}
bool Sounds::PlayMenuHighlightSound() {
	if (!PlayWaveFile(menuHighlightButton)) {
		return false;
	}
	return true;
}
bool Sounds::PlayMenuClickSound() {
	if (!PlayWaveFile(menuClickButton)) {
		return false;
	}
	return true;
}
bool Sounds::PlayMenuBackSound() {
	if (!PlayWaveFile(menuBack)) {
		return false;
	}
	return true;
}
bool Sounds::PlayTestSound01() {
	if (!PlayWaveFile(test01)) {
		return false;
	}
	return true;
}
bool Sounds::StartMenuMusic() {
	if (!PlayLoopingWaveFile(menu01)) {
		return false;
	}
	return true;
}
bool Sounds::StopMenuMusic() {
	if (!StopLoopingWaveFile(menu01)) {
		return false;
	}
	return true;
}
bool Sounds::StartGameMusic() {
	if (!PlayLoopingWaveFile(game01)) {
		return false;
	}
	return true;
}
bool Sounds::StopGameMusic() {
	if (!StopLoopingWaveFile(game01)) {
		return false;
	}
	return true;
}
bool Sounds::ContinueGameMusic() {
	if (ContinueLoopingWaveFile(game01)) {
		return false;
	}
	return true;
}
bool Sounds::StopAllSFX() {
	if (!StopSFX(jump01)) {
		return false;
	}
	if (!StopSFX(jump02)) {
		return false;
	}
	if (!StopSFX(jump03)) {
		return false;
	}
	if (!StopSFX(land01)) {
		return false;
	}
	if (!StopSFX(land02)) {
		return false;
	}
	if (!StopSFX(land03)) {
		return false;
	}
	if (!StopSFX(blob01)) {
		return false;
	}
	if (!StopSFX(blob02)) {
		return false;
	}
	if (!StopSFX(blob03)) {
		return false;
	}
	if (!StopSFX(enmy01)) {
		return false;
	}
	if (!StopSFX(enmy02)) {
		return false;
	}
	if (!StopSFX(enmy03)) {
		return false;
	}
	return true;
}
bool Sounds::ContinueAllSFX() {
	if (!ContinueSFX(jump01)) {
		return false;
	}
	if (!ContinueSFX(jump02)) {
		return false;
	}
	if (!ContinueSFX(jump03)) {
		return false;
	}
	if (!ContinueSFX(land01)) {
		return false;
	}
	if (!ContinueSFX(land02)) {
		return false;
	}
	if (!ContinueSFX(land03)) {
		return false;
	}
	if (!ContinueSFX(blob01)) {
		return false;
	}
	if (!ContinueSFX(blob02)) {
		return false;
	}
	if (!ContinueSFX(blob03)) {
		return false;
	}
	if (!ContinueSFX(enmy01)) {
		return false;
	}
	if (!ContinueSFX(enmy02)) {
		return false;
	}
	if (!ContinueSFX(enmy03)) {
		return false;
	}
	return true;
}

void Sounds::setMasterVolume(UINT master) {
	if (master >= 10) {
		this->masterVolume = 10;
	}
	else if (master <= 0) {
		this->masterVolume = 0;
	}
	else {
		this->masterVolume = master;
	}
}
void Sounds::setSFXVolume(UINT sfx) {
	if (sfx >= 10) {
		this->sfxVolume = 10;
	}
	else if (sfx <= 0) {
		this->sfxVolume = 0;
	}
	else {
		this->sfxVolume = sfx;
	}
}
void Sounds::setMusicVolume(UINT music) {
	if (music >= 10) {
		this->musicVolume = 10;
	}
	else if (music <= 0) {
		this->musicVolume = 0;
	}
	else {
		this->musicVolume = music;
	}
}
void Sounds::applyVolumes() {
	//SFX
	LONG sfx = (masterVolume * sfxVolume * 100) - 10000;
	SetVolume(jump01, sfx);
	SetVolume(jump02, sfx);
	SetVolume(jump03, sfx);
	SetVolume(land01, sfx);
	SetVolume(land02, sfx);
	SetVolume(land03, sfx);
	SetVolume(blob01, sfx);
	SetVolume(blob02, sfx);
	SetVolume(blob03, sfx);
	SetVolume(enmy01, sfx);
	SetVolume(enmy02, sfx);
	SetVolume(enmy03, sfx);
	SetVolume(sorb01, sfx);
	SetVolume(sorb02, sfx);
	SetVolume(sorb03, sfx);
	SetVolume(pDeath, sfx);
	SetVolume(pDmg01, sfx);
	SetVolume(pDmg02, sfx);
	//Music?
	LONG music = (masterVolume * musicVolume * 100) - 10000;
	SetVolume(test01, music);
	SetVolume(menu01, music);
	SetVolume(game01, music);
	SetVolume(pStart, music);
	SetVolume(pGoal, music);

	defaultVolume = false;
}

bool Sounds::InitializeDirectSound(HWND handle) {
	HRESULT hr;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	hr = DirectSoundCreate8(NULL, &directSound, NULL);
	if (FAILED(hr)) {
		return false;
	}

	hr = directSound->SetCooperativeLevel(handle, DSSCL_PRIORITY);
	if (FAILED(hr)) {
		return false;
	}

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	hr = directSound->CreateSoundBuffer(&bufferDesc, &firstBuffer, NULL);
	if (FAILED(hr)) {
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	hr = firstBuffer->SetFormat(&waveFormat);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
void Sounds::ShutdownDirectSound() {
	if (firstBuffer) {
		firstBuffer->Release();
		firstBuffer = nullptr;
	}
	if (directSound) {
		directSound->Release();
		directSound = nullptr;
	}
}
bool Sounds::LoadWaveFile(char* fileName, IDirectSoundBuffer8** soundBuffer) {
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT hr;
	IDirectSoundBuffer* temp;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0) {
		return false;
	}

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F') )
	{
		return false;
	}

	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't'))
	{
		return false;
	}

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM) {
		return false;
	}

	if (waveFileHeader.numChannels != 2) {
		return false;
	}

	if (waveFileHeader.sampleRate != 44100) {
		return false;
	}

	if (waveFileHeader.bitsPerSample != 16) {
		return false;
	}

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	hr = directSound->CreateSoundBuffer(&bufferDesc, &temp, NULL);
	if (FAILED(hr)) {
		return false;
	}

	hr = temp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*soundBuffer);
	if (FAILED(hr)) {
		return false;
	}

	temp->Release();
	temp = nullptr;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) {
		return false;
	}

	count = fread(waveData, 1u, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) {
		delete[]waveData;
		waveData = nullptr;
		return false;
	}

	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}

	hr = (*soundBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(hr)) {
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	hr = (*soundBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(hr)) {
		return false;
	}

	delete[]waveData;

	return true;
}
void Sounds::ShutdownWaveFile(IDirectSoundBuffer8** soundBuffer) {
	if (*soundBuffer) {
		(*soundBuffer)->Release();
		*soundBuffer = nullptr;
	}
}
bool Sounds::PlayWaveFile(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	hr = sound->SetCurrentPosition(0);
	if (FAILED(hr)) {
		return false;
	}

	if (defaultVolume != false) {
		hr = sound->SetVolume(DSBVOLUME_MAX);
		if (FAILED(hr)) {
			return false;
		}
	}
	
	hr = sound->Play(0, 0, 0);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Sounds::PlayLoopingWaveFile(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	hr = sound->SetCurrentPosition(0);
	if (FAILED(hr)) {
		return false;
	}

	if (defaultVolume != false) {
		hr = sound->SetVolume(DSBVOLUME_MAX);
		if (FAILED(hr)) {
			return false;
		}
	}

	hr = sound->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Sounds::StopLoopingWaveFile(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	hr = sound->Stop();
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Sounds::ContinueLoopingWaveFile(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	if (defaultVolume != false) {
		hr = sound->SetVolume(DSBVOLUME_MAX);
		if (FAILED(hr)) {
			return false;
		}
	}

	hr = sound->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Sounds::SetVolume(IDirectSoundBuffer8* sound, LONG volume) {
	HRESULT hr;

	hr = sound->SetVolume(volume);
	if (FAILED(hr)) {
		return false;
	}
	return true;
}
bool Sounds::StopSFX(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	hr = sound->Stop();
	if (FAILED(hr)) {
		return false;
	}

	return true;
}
bool Sounds::ContinueSFX(IDirectSoundBuffer8* sound) {
	HRESULT hr;

	if (defaultVolume != false) {
		hr = sound->SetVolume(DSBVOLUME_MAX);
		if (FAILED(hr)) {
			return false;
		}
	}

	hr = sound->Play(0, 0, 0);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}