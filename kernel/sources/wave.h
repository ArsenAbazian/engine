#ifndef ARS_WAVE_INC	
#define ARS_WAVE_INC

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>


class GameWaveFile{
	
public:
	BYTE			*lpWaveBuffer;
	DWORD			dwBufferLen;

	BYTE			*lpRIFF;
	DWORD			dwFileLength1;
	
	BYTE			*lpWaveId;
	BYTE			*lpWaveFormatName;
	DWORD			dwFileLength2;
	WAVEFORMATEX	*lpWaveFormatEx;
	WAVEFORMATEX	FormatEx;

	BYTE			*lpDataName;
	BYTE			*lpData;
	DWORD			dwDataSize;

	GameWaveFile(){ ZeroMemory(this, sizeof(GameWaveFile)); }
	~GameWaveFile(){ Clear(); }

	bool Load(TCHAR *lpFile);
	void Play(DWORD dwFlags);
	void Clear();
	bool CopyDataToBuffer(void *lpData, DWORD dwDataSize, DWORD *dwDataRead);
};

class GameSound;
class SoundKernel{

public:
	LPDIRECTSOUND8			lpSound;	// указатель на интерфейс	
	LPDIRECTSOUNDBUFFER		lpPrimaryBuffer;
	HWND					hWnd;

	SoundKernel(){ ZeroMemory(this, sizeof(SoundKernel)); }
	~SoundKernel(){ Clear(); }

	void Clear(){ 
		
		if(lpSound)
			lpSound->Release();

		lpSound = NULL;
	
		if(lpPrimaryBuffer)
			lpPrimaryBuffer->Release();

		lpPrimaryBuffer = NULL;
	}

	HRESULT SetPrimaryBufferFormat(DWORD dwChannels, DWORD dwFreq, DWORD dwBitRate);
	HRESULT Initialize(HWND hw, DWORD dwCoopLevel);

	HRESULT CreateSound(TCHAR *lpWaveFile, DWORD dwFlags, DWORD dwNumBuffers, GameSound **lpGameSound);
};

class GameSound{
	
	SoundKernel				*lpKernel;

	LPDIRECTSOUNDBUFFER		*lpBuffers;
	DWORD					dwBufferCount;

	DSBUFFERDESC			Desc;

public:

	GameSound(){ ZeroMemory(this, sizeof(GameSound)); }
	~GameSound(){ Clear(); }

	void Clear(){
	
		if(lpBuffers){
			for(DWORD i=0; i < dwBufferCount; i++){
				if(lpBuffers[i]){
					lpBuffers[i]->Release();
					lpBuffers[i] = NULL;
				}
			}
			delete[] lpBuffers;
			lpBuffers = NULL;
		}
		if(Desc.lpwfxFormat)
			delete Desc.lpwfxFormat;

		Desc.lpwfxFormat = NULL;
	}

	void		SetKernel(SoundKernel *krn){ lpKernel = krn; }
	void		SetDescription(DSBUFFERDESC *lpDesc){ 
					memcpy(&Desc, lpDesc, sizeof(DSBUFFERDESC)); 
					Desc.lpwfxFormat = new WAVEFORMATEX;
					memcpy(Desc.lpwfxFormat, lpDesc->lpwfxFormat, sizeof(WAVEFORMATEX));
				} 
	DWORD		GetBufferCount(){ return dwBufferCount; }
	HRESULT		CreateBuffers(DWORD dwbc){
					Clear();

					dwBufferCount = dwbc;
					lpBuffers = new LPDIRECTSOUNDBUFFER[dwbc];
					ZeroMemory(lpBuffers, sizeof(LPDIRECTSOUNDBUFFER) * dwBufferCount);

					return S_OK;	
				}
	HRESULT		InitializeSoundBuffer(DWORD dwIndex);
	HRESULT		DuplicateSoundBuffer(DWORD dwIndex1, DWORD dwIndex2);
	HRESULT		SetData(GameWaveFile *lpWave);
	HRESULT		RestoreBuffer(DWORD dwIndex);

	HRESULT		Play(DWORD dwPriority, DWORD dwFlags);
	HRESULT		Stop();
	HRESULT     Reset();
	HRESULT		SetVolume(LONG Volume);
	bool		IsStop();
};

#endif