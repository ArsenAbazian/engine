#include "wave.h"
#include <stdio.h>
#include <tchar.h>
#include <dxerr9.h>

BYTE	checkByte17;
BYTE	checkByte1_20;

WORD	checkWord6;
#define CHECK_WORD6
BYTE	checkByte1_27;
BYTE	checkByte1_40;

WORD	checkWord6_1;
#define CHECK_WORD6_1
BYTE	checkByte1_38;

BYTE	checkByte6;
BYTE	checkByte35;
BYTE	checkByte26;

BYTE	checkByte1_6;

bool GameWaveFile::Load(TCHAR *lpFile){

	Clear();

	FILE *fp;
	_tfopen_s(&fp, lpFile, TEXT("rb"));
	if(fp == NULL)
		return false;

	fseek(fp, 0, SEEK_END);
	dwBufferLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	lpWaveBuffer = new BYTE[dwBufferLen];
	
	if(fread(lpWaveBuffer, sizeof(BYTE), dwBufferLen, fp) != dwBufferLen){
		fclose(fp);
		return false;
	}

	fclose(fp);

	// теперь начинаем устанавливать данные
    lpRIFF = lpWaveBuffer;
	dwFileLength1 = *(DWORD*)(lpWaveBuffer+4);

	lpWaveId = (lpWaveBuffer + 8);
	lpWaveFormatName = (lpWaveBuffer + 12);
	
	dwFileLength2 = *(DWORD*)(lpWaveBuffer + 16);
	lpWaveFormatEx = (WAVEFORMATEX*)(lpWaveBuffer + 20);
	
	
	lpDataName = (lpWaveBuffer + 20 + dwFileLength2);
	dwDataSize = *(DWORD*)(lpWaveBuffer + 24 + dwFileLength2);
	if(dwDataSize > dwBufferLen)
		dwDataSize = dwBufferLen;
	
	lpData = (lpDataName + 8); 
	
	memcpy(&FormatEx, lpWaveFormatEx, sizeof(WAVEFORMATEX));
	FormatEx.cbSize = sizeof(WAVEFORMATEX);
	
	return true;
}
bool GameWaveFile::CopyDataToBuffer(void *lpD, DWORD dwDSize, DWORD *dwReadData){

	// размер прочтенных данных устанавливается размером буффера или меньше
	*dwReadData = dwDSize > dwDataSize? dwDataSize: dwDSize;

	// просто копируем данные
	memcpy(lpD, lpData, *dwReadData);

	return true;
}

void GameWaveFile::Play(DWORD dwFlags){

	// средства windows
	// корректируем флаги для работы с памятью
	dwFlags |= SND_MEMORY;

	PlaySound((LPCWSTR)lpWaveBuffer, NULL, dwFlags);

	return ;
}

void GameWaveFile::Clear(){

	if(lpWaveBuffer)
		delete[] lpWaveBuffer;

	lpWaveBuffer = NULL;

	return ;
}

HRESULT SoundKernel::Initialize(HWND hw, DWORD dwCoopLevel){

	HRESULT rv;
	TCHAR	buf[256];

	Clear();
	
	hWnd = hw;

	rv = DirectSoundCreate8(NULL, &lpSound, NULL);
	if(FAILED(rv)){
	
		_stprintf_s(buf, 256, TEXT("Error: can't create sound kernel (%s)"), DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);
		return rv;

	}

	rv = lpSound->SetCooperativeLevel(hWnd, dwCoopLevel);
	if(FAILED(rv)){
	
		_stprintf_s(buf, 256, TEXT("Error: can't set cooperative level (%s)"), DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);
		return rv;

	}

	return S_OK;
}

HRESULT SoundKernel::SetPrimaryBufferFormat(DWORD dwChannels, DWORD dwFreq, DWORD dwBitRate){

	HRESULT rv;
	TCHAR	buf[256];
	
	
	DSBUFFERDESC	dsbd;

	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;

	if(lpPrimaryBuffer)
		lpPrimaryBuffer->Release();
	
	rv = lpSound->CreateSoundBuffer(&dsbd, &lpPrimaryBuffer, NULL);
	if(FAILED(rv)){
		_stprintf_s(buf, 256, TEXT("Error: can't create primary sound buffer! (%s)"),DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);
		return rv;
	}

	WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) dwChannels; 
    wfx.nSamplesPerSec  = (DWORD) dwFreq; 
    wfx.wBitsPerSample  = (WORD) dwBitRate; 
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

	rv = lpPrimaryBuffer->SetFormat(&wfx);
	if(FAILED(rv)){
		_stprintf_s(buf, 256, TEXT("Error: can't set format to primary sound buffer! (%s)"),DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);
		return rv;
	}

	lpPrimaryBuffer->Release();
	lpPrimaryBuffer = NULL;

	return S_OK;
}

HRESULT SoundKernel::CreateSound(TCHAR *lpWaveFile, DWORD dwFlags, DWORD dwNumBuffers, GameSound **lpGameSound){

	
	HRESULT rv;
	DWORD	i;
	TCHAR	buf[256];

	DWORD				dwSBufferSize;
	GameWaveFile		*lpWave;
	DSBUFFERDESC		dsbd;	
	
	if(lpSound == NULL)
		return CO_E_NOTINITIALIZED;
	if(lpWaveFile == NULL || dwNumBuffers < 1)
		return E_INVALIDARG;

	*lpGameSound = new GameSound;
	
	// создаем буфферы в gamesound
	rv = (*lpGameSound)->CreateBuffers(dwNumBuffers);
	if(FAILED(rv)){
		_stprintf_s(buf, 256, TEXT("Error: can't create buffers for GameSound! (%s)"),DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);	
		delete *lpGameSound;
		return rv;
	}
	
	// загружаем файл
	lpWave = new GameWaveFile;

	lpWave->Load(lpWaveFile);
	if(lpWave->dwBufferLen == 0){
	
		delete lpWave;
		delete *lpGameSound;
		return E_FAIL;
	
	}
	
	dwSBufferSize = lpWave->dwDataSize;
	
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize				= sizeof(DSBUFFERDESC);
	dsbd.dwFlags			= dwFlags;
	dsbd.dwBufferBytes		= dwSBufferSize;
	dsbd.lpwfxFormat		= &lpWave->FormatEx;
	dsbd.guid3DAlgorithm	= GUID_NULL;

	(*lpGameSound)->SetKernel(this);
	(*lpGameSound)->SetDescription(&dsbd);
	
	rv = (*lpGameSound)->InitializeSoundBuffer(0);
	if(rv != DS_NO_VIRTUALIZATION && FAILED(rv)){
		_stprintf_s(buf, 256, TEXT("Error: can't initialize buffer[0] for GameSound! (%s) buffer size = %d"),DXGetErrorString9(rv), dwSBufferSize);
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);	
		delete lpWave;
		delete *lpGameSound;
		return rv;	
	}
	
	if( (dwFlags & DSBCAPS_CTRLFX) == 0 ){
        for( i=1; i<dwNumBuffers; i++ ){
            
			rv = (*lpGameSound)->DuplicateSoundBuffer(0, i);
			if(FAILED(rv)){
				_stprintf_s(buf, 256, TEXT("Error: can't duplicate buffer[%d]  from buffer[0] for GameSound! (%s)"),i, DXGetErrorString9(rv));
				MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);	
				delete lpWave;
				delete *lpGameSound;
				return rv;	
			}
        }
    }
    else {
        for( i=1; i<dwNumBuffers; i++ ){
            rv = (*lpGameSound)->InitializeSoundBuffer(i);
			if(FAILED(rv)){
				_stprintf_s(buf, 256, TEXT("Error: can't initialize buffer[%d] for GameSound! (%s)"),i, DXGetErrorString9(rv));
				MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);	
				delete lpWave;
				delete *lpGameSound;
				return rv;	
			}
        }
    }

	// заполянем данными
	rv = (*lpGameSound)->SetData(lpWave);
	delete lpWave;

	if(FAILED(rv)){
		_stprintf_s(buf, 256, TEXT("Error: can't set data for GameSound! (%s)"),DXGetErrorString9(rv));
		MessageBox(hWnd, buf, TEXT("SoundKernel"), MB_ICONERROR);	
		delete *lpGameSound;
		return rv;	
	}

	return S_OK;

}

HRESULT GameSound::InitializeSoundBuffer(DWORD dwIndex){

	HRESULT rv;

	if(lpKernel == NULL || lpKernel->lpSound == NULL)
		return E_INVALIDARG;
	
	if(dwIndex >= dwBufferCount)
		return E_INVALIDARG;
	
	if(lpBuffers[dwIndex])
		lpBuffers[dwIndex]->Release();

	lpBuffers[dwIndex] = NULL;
	
	rv = lpKernel->lpSound->CreateSoundBuffer(&Desc, &lpBuffers[dwIndex], NULL);	
	if(FAILED(rv))
		return rv;
	
	return S_OK;
}

HRESULT GameSound::DuplicateSoundBuffer(DWORD dwIndex1, DWORD dwIndex2){

	HRESULT rv;

	if(lpKernel == NULL || lpKernel->lpSound == NULL)
		return E_INVALIDARG;
	
	if(dwIndex1 >= dwBufferCount || dwIndex2 >= dwBufferCount)
		return E_INVALIDARG;
	
	if(lpBuffers[dwIndex2])
		lpBuffers[dwIndex2]->Release();

	lpBuffers[dwIndex2] = NULL;

	rv = lpKernel->lpSound->DuplicateSoundBuffer(lpBuffers[dwIndex1], &lpBuffers[dwIndex2]);
	if(FAILED(rv))
		return rv;

	return S_OK;    
}

HRESULT	GameSound::RestoreBuffer(DWORD dwIndex){

	HRESULT rv;

	// нечего восстанавливать
	if(lpBuffers == NULL || lpBuffers[dwIndex] == NULL)
		return S_OK;

	DWORD dwStatus;

	rv = lpBuffers[dwIndex]->GetStatus(&dwStatus);
	if(FAILED(rv))
		return rv;

	if(dwStatus & DSBSTATUS_BUFFERLOST){ // если потеряли
	
		// в будущем будем упорно восстанавливать до бесконечности ;)
		rv = lpBuffers[dwIndex]->Restore();
		if(FAILED(rv))
			return rv;

	}
	
	return S_OK;
}

HRESULT GameSound::SetData(GameWaveFile *lpWave){

	HRESULT rv;
	BYTE	*lpData;
	DWORD	dwDataSize;
	DWORD	dwDataRead;

	if(lpBuffers == NULL || lpBuffers[0] == NULL)
		return E_INVALIDARG;
	
	// пытаемся восстановить буффер
	rv = RestoreBuffer(0);
	if(FAILED(rv))
		return rv;

	// открываем достпуп к буфферу
	rv = lpBuffers[0]->Lock(0, Desc.dwBufferBytes, (LPVOID*)&lpData, &dwDataSize, NULL ,NULL, 0); 
	if(FAILED(rv))
		return rv;

    // заполняем буффер данными
	if(!lpWave->CopyDataToBuffer(lpData, dwDataSize, &dwDataRead)){
		lpBuffers[0]->Unlock(lpData, dwDataSize, NULL, 0);
		return S_FALSE;
	}	
	
	// заполняем оставшуюся часть тишиной
	BYTE value = lpWave->FormatEx.wBitsPerSample == 8? 128: 0;
	FillMemory(&lpData[dwDataRead], dwDataSize - dwDataRead, value);

	// закрываем буффер
	rv = lpBuffers[0]->Unlock(lpData, dwDataSize, NULL, 0);
	if(FAILED(rv))
		return rv;
	
	return S_OK;
}

HRESULT GameSound::Play(DWORD dwPriority, DWORD dwFlags){

	Stop();
	
	if(lpBuffers == NULL || lpBuffers[0] == NULL)
		return S_FALSE;
	
	lpBuffers[0]->SetCurrentPosition(0);
	
	return lpBuffers[0]->Play(0, dwPriority, dwFlags);

}

HRESULT GameSound::Stop(){
	
	if(lpBuffers == NULL || lpBuffers[0] == NULL)
		return S_FALSE;
	
	return lpBuffers[0]->Stop();

}

HRESULT GameSound::Reset(){

	return lpBuffers[0]->Restore();

}

HRESULT GameSound::SetVolume(LONG Volume){

	return lpBuffers[0]->SetVolume(Volume);
	
}

bool GameSound::IsStop(){

	DWORD		St;
	HRESULT		rv;

	St = 0;
	rv = lpBuffers[0]->GetStatus(&St);
	if(FAILED(rv))
		return false;

	if(St & DSBSTATUS_PLAYING){
		return false;
	}

	return true;
}
