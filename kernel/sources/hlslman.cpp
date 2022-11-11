#include "hlslman.h"
#include "dxerr9.h"

HRESULT HLSLManager::createEffectFromFile(LPDIRECT3DDEVICE9 lpDevice, shString &fileName) 
{ 
	LPD3DXBUFFER lpBuffer;

	HRESULT rv = D3DXCreateEffectFromFile( lpDevice, fileName.getCString(), NULL, NULL, 0, NULL, &lpEffect, &lpBuffer );
	if(lpBuffer != NULL) { 
		char  *buf = (char*)lpBuffer->GetBufferPointer();
		TCHAR *tbuf = new TCHAR[lpBuffer->GetBufferSize() + 1];
		strToWCS(buf, tbuf);
		this->result.initialize(tbuf);
		delete[] tbuf;
		lpBuffer->Release();
	}
	else if(FAILED(rv)) {
		result = DXGetErrorString9( rv );
	}
	else
		result = TEXT("compilation successfull!");

	return rv;
}

HRESULT	HLSLManager::createEffectFromString(LPDIRECT3DDEVICE9 lpDevice, shString &effect) { 
	
	char *eff = new char[ effect.getLength() + 1 ];
	effect.copyToChar( eff );

	HRESULT rv = this->createEffectFromString( lpDevice, eff );
	delete[] eff;

	return rv;
}

HRESULT	HLSLManager::createEffectFromString(LPDIRECT3DDEVICE9 lpDevice, char *effect) {
	
	LPD3DXBUFFER lpBuffer;

	HRESULT rv = D3DXCreateEffect( lpDevice, effect, (UINT)strlen( effect ), NULL, NULL, 0, NULL, &lpEffect, &lpBuffer );
	if(lpBuffer != NULL) { 
		char  *buf = (char*)lpBuffer->GetBufferPointer();
		TCHAR *tbuf = new TCHAR[lpBuffer->GetBufferSize() + 1];
		strToWCS(buf, tbuf);
		this->result.initialize(tbuf);
		delete[] tbuf;
		lpBuffer->Release();
	}
	else if(FAILED(rv)) {
		result = DXGetErrorString9( rv );
	}
	else
		result = TEXT("compilation successfull!");

	return rv;
}
