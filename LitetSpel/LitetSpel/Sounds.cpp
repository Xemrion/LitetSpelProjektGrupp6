#include"Sounds.h"


Sounds::Sounds() {
	this->directSound = nullptr;
	this->firstBuffer = nullptr;
	this->secondBuffer = nullptr;
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
name = "sound02.wav";
fileName = new char[name.size()];
fileName = name.data();

results = LoadWaveFile(fileName, &secondBuffer);
if(results == false){
	return false;
}

results = PlayWaveFile(secondBuffer);
if (results == false) {
	return false;
}

return true;
}
void Sounds::Shutdown() {
	ShutdownWaveFile(&secondBuffer);
	ShutdownDirectSound();

	if (fileName != nullptr) {
		delete fileName;
		fileName = nullptr;
	}
}

bool Sounds::LoadWaveToBuffer(char* fileName, IDirectSoundBuffer8** soundBuffer) {
	if (!LoadWaveFile(fileName, soundBuffer)) {
		return false;
	}
	return true;
}
bool Sounds::PlayWaveSound(IDirectSoundBuffer8* sound) {
	if (!PlayWaveFile(sound)) {
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

	hr = temp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondBuffer);
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

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) {
		delete[]waveData;
		waveData = nullptr;
		return false;
	}

	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}

	hr = (secondBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(hr)) {
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	hr = (secondBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(hr)) {
		return false;
	}

	delete[]waveData;
	waveData = nullptr;

	return true;
}
void Sounds::ShutdownWaveFile(IDirectSoundBuffer8** soundBuffer) {
	if (secondBuffer) {
		secondBuffer->Release();
		secondBuffer = nullptr;
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