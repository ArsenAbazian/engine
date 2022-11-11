#ifndef __FILE_INFO_INC
#define __FILE_INFO_INC

#include "hstring.h"
#include "scrcode.h"
#include "ppstate.h"
#include <stdio.h>

// описатель имени файла
typedef struct {
	shString				directory;
	shString				fileName;				
	shString				fullPathName;
}fileNameInfo;		

class fileInfo // описатель класса информации о файле
{
public:
	shString				directory;		// директория
	shString				fileName;		// имя файла
	shString				fullPathName;	// полный путь файла вместе с директорией
	BYTE					*fp;			// полный буффер файла
	DWORD					fileLength;		// полная длина файла
	DWORD					filePos;		// позиция в файле
	DWORD					line;			// линия в файле
	DWORD					col;			// позиция в линии
	bool					isOwnBuffer;	// свой буффер

	long					fileIndex;		// индекс файла
	preProcessorState		ppState;

	fileInfo();
	~fileInfo();	

	DWORD					getFileSize( FILE *filep );
	void					clear();		// очистить структуру
	scriptRetCode			openFile( TCHAR *fileName );		// открыть для чтения
	scriptRetCode			openFile( shString &dir, shString &file, shString &filePath, FILE *filep );
	scriptRetCode			openBuffer( BYTE *buffer, DWORD bufLength, int ln, int cl );
};

#endif