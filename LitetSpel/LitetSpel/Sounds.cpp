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


//results = PlayWaveFile(secondBuffer);
//if (results == false) {
//	return false;
//}

return true;
}
void Sounds::Shutdown() {
	ShutdownWaveFile(&test01);
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
bool Sounds::PlayLandSound01() {
	if (!PlayWaveFile(land01)) {
		return false;
	}
	return true;
}
bool Sounds::PlayLandSound02() {
	if (!PlayWaveFile(land02)) {
		return false;
	}
	return true;
}
bool Sounds::PlayLandSound03() {
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
bool Sounds::PlayTestSound01() {
	if (!PlayWaveFile(test01)) {
		return false;
	}
	return true;
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

	hr = sound->SetVolume(DSBVOLUME_MAX);
	if (FAILED(hr)) {
		return false;
	}
	
	hr = sound->Play(0, 0, 0);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}