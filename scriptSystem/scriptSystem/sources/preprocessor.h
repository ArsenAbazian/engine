#ifndef __PREPROCESSOR_INC
#define __PREPROCESSOR_INC

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <list>

#include "hstring.h"
#include "lexeme.h"
#include "scrmacro.h"
#include "screrror.h"

#define SPP_FILE_MAX_COUNT	1024	// ���� ������ ���������� ������� ������� ������������ ������ 

class scriptPreProcessor
{
private:
	// ������������
	std::list< shString >					includeDirList;					// ��������� ��� ���������� ������
	shString								programmPath;
	// �������� ����������
	fileInfo								files[ SPP_FILE_MAX_COUNT ];	// ����� � �����
	shString								outFileName;					// ��� ����� ������
	FILE									*fpOut;							// ���� ������
	int										fileCount;						// ����� �������� ������
	
	// �������
	std::list< scriptMacros >				macros;							// ������ ��������
		
	// �������
	std::list< scriptError* >				errorList;						// ������ ������ � ��������������	
	std::list< scriptError* >				warningList;						

	// ��������� ������
	TCHAR									tempBuffer[ 2048 ];				// 2 ��������� ��� ��������� ������

public:
	scriptPreProcessor();													// �����������
	~scriptPreProcessor();													// ����������
	
	scriptRetCode	initialize( std::list< shString > &incDir, shString &progPath );	// ������������� - ���������� ������������ ������ ���� ���������� ���������
	scriptRetCode	clear();												// ������� ���������� ������������
	scriptRetCode	openInputFile( TCHAR *fileName, bool inSystem );		// ������� ���� ��� ������
	scriptRetCode	closeInputFile();
	scriptRetCode	openOutputFile( TCHAR *fileName );						// ������� ���� ��� ������
	scriptRetCode	preProcess();											// ������������� ��

	scriptRetCode	getFullPathName( shString &directory, shString &fileName, shString &fullPathName );
	bool			isFileOpened( shString &fileName );						// ������� ������ �� ��� ����� ����
	void			runUntilEOL( fileInfo &fInfo );							// ���������� ������� �� ����� ������ ��� �����

	bool			isMacros( fileInfo &fInfo, scriptMacros &mac );			// �������, ���� �� � ������ ��������, ������ � ����� ������

};

#endif