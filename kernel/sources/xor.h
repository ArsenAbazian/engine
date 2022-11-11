#ifndef XOR_ENCODER
#define XOR_ENCODER

#include <windows.h>
#include <tchar.h>

class XOREncoder{

	BYTE	Key[32];	// ���� ������ 16 ����
	
public:
	
	XOREncoder(){ ZeroMemory(this, sizeof(XOREncoder)); }
	~XOREncoder(){ ZeroMemory(this, sizeof(XOREncoder)); }

	void SetKey( BYTE k[32] ){ memcpy( Key, k, 32 ); }				// ���������� ����
	void EncodeFile( TCHAR *lpInFile, TCHAR *lpOutFile );			// ���������� ����
	void ProcessBlock( BYTE kin[32], BYTE kout[32] );			
};

#endif