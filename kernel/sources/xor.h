#ifndef XOR_ENCODER
#define XOR_ENCODER

#include <windows.h>
#include <tchar.h>

class XOREncoder{

	BYTE	Key[32];	// ключ длиной 16 байт
	
public:
	
	XOREncoder(){ ZeroMemory(this, sizeof(XOREncoder)); }
	~XOREncoder(){ ZeroMemory(this, sizeof(XOREncoder)); }

	void SetKey( BYTE k[32] ){ memcpy( Key, k, 32 ); }				// установить ключ
	void EncodeFile( TCHAR *lpInFile, TCHAR *lpOutFile );			// кодировать файл
	void ProcessBlock( BYTE kin[32], BYTE kout[32] );			
};

#endif