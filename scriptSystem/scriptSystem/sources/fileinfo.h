#ifndef __FILE_INFO_INC
#define __FILE_INFO_INC

#include "hstring.h"
#include "scrcode.h"
#include "ppstate.h"
#include <stdio.h>

// ��������� ����� �����
typedef struct {
	shString				directory;
	shString				fileName;				
	shString				fullPathName;
}fileNameInfo;		

class fileInfo // ��������� ������ ���������� � �����
{
public:
	shString				directory;		// ����������
	shString				fileName;		// ��� �����
	shString				fullPathName;	// ������ ���� ����� ������ � �����������
	BYTE					*fp;			// ������ ������ �����
	DWORD					fileLength;		// ������ ����� �����
	DWORD					filePos;		// ������� � �����
	DWORD					line;			// ����� � �����
	DWORD					col;			// ������� � �����
	bool					isOwnBuffer;	// ���� ������

	long					fileIndex;		// ������ �����
	preProcessorState		ppState;

	fileInfo();
	~fileInfo();	

	DWORD					getFileSize( FILE *filep );
	void					clear();		// �������� ���������
	scriptRetCode			openFile( TCHAR *fileName );		// ������� ��� ������
	scriptRetCode			openFile( shString &dir, shString &file, shString &filePath, FILE *filep );
	scriptRetCode			openBuffer( BYTE *buffer, DWORD bufLength, int ln, int cl );
};

#endif