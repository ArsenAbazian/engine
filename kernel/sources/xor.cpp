#include "xor.h"
#include <stdio.h>

void XOREncoder::ProcessBlock( BYTE kin[32], BYTE kout[32] ){

	int i;

	for(i=0; i<32; i++)
		kout[i] = kin[i] ^ Key[i];
	
	return ;
}

void XOREncoder::EncodeFile( TCHAR *lpInFile, TCHAR *lpOutFile ){

	FILE	*fp_in, *fp_out;

	_tfopen_s(&fp_in, lpInFile, TEXT("rb"));
	if( fp_in == NULL )
		return ;
	
	 _tfopen_s(&fp_out, lpOutFile, TEXT("wb"));
	if( fp_out == NULL )
	{
		fclose( fp_in );
		return ;
	}
	
	DWORD file_len, i;

	fseek(fp_in, 0, SEEK_END);
	file_len = ftell(fp_in);
	fseek(fp_in, 0, SEEK_SET);

	BYTE	kin[32];
	BYTE	kout[32];

	// кодируем весь файл
	for( i = 0; i < file_len-32; i+=32 )
	{
	
		fread( kin, 1, 32, fp_in );
		ProcessBlock( kin, kout );
		fwrite( kout, 1, 32, fp_out );
        
	}
	
	fread( kin, 1, file_len - i, fp_in );
	ProcessBlock( kin, kout );
	fwrite( kout, 1, file_len - i, fp_out );
	fclose( fp_in );
	fclose( fp_out );

	return ;
}