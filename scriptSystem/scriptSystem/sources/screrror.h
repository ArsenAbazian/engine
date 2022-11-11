#ifndef __SCRIPT_ERROR_INC
#define __SCRIPT_ERROR_INC

#include <windows.h>
#include <tchar.h>
#include "fileinfo.h"
#include "hstring.h"
#include "scrcode.h"
#include <list>

typedef enum
{
	SE_NONE							= 0,	// �������
	SE_SCANER_ERROR					= 1,	// �� ������ �������
	SE_SCANER_FATAL_ERROR			= 2,	
	SE_SCANER_WARNING				= 3, 
	SE_PARSER_ERROR					= 4,	// �� ������ �����������
	SE_PARSER_FATAL_ERROR			= 5,	
	SE_PARSER_WARNING				= 6,
	SE_PREPROCESSOR_ERROR			= 7,	// �� ������ �������������	
	SE_PREPROCESSOR_FATAL_ERROR		= 8,	
	SE_PREPROCESSOR_WARNING			= 9,	
	SE_SGRAMMAR_ERROR				= 10,	// ��� ������ ��������� � ����������� ����������
	SE_SGRAMMAR_FATAL_ERROR			= 11,
	SE_SGRAMMAR_WARNING				= 12,
	SE_FORCE_DWORD					= 0xffffffff
}scriptErrorType;

class scriptError
{
public:
	shString		text;			// ����� ������
	long			fileIndex;		// ������ ����� � ������� ���� ���������� ������
	scriptRetCode	code;			// ��� ������
	scriptErrorType	type;			// ��� ������
	int				line;			// �����
	int				col;			// �������
	
	scriptError();					
	~scriptError();

	void			clear();							// �������
	scriptError&	copy( scriptError& se );			// �����������
	void			print();							// ������ ������

	void			printFileIndex( );					// ������ ������� �����
	void			printFileIndex( TCHAR *buffer );	// ...
	void			printLine( );						// ������ �����
	void			printLine( TCHAR *buffer );			// ...
	void			printCol( );						// ������ �������
	void			printCol( TCHAR *buffer );			// ...
	void			printCodeDesc( );					// ������ ����
	void			printCodeDesc( TCHAR *buffer );		// ...
	void			printCode( );						// ������ ����
	void			printCode( TCHAR *buffer );			// ...
	void			printType( );						// ������ ���� ������
	void			printType( TCHAR *buffer );			// ...
	void			printText( );						// ������ ������
	void			printText( TCHAR *buffer );			// ...
};

void clearErrorsList( std::list< scriptError* > &errList );

#endif