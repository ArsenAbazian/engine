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
	SE_NONE							= 0,	// никакой
	SE_SCANER_ERROR					= 1,	// на стадии сканера
	SE_SCANER_FATAL_ERROR			= 2,	
	SE_SCANER_WARNING				= 3, 
	SE_PARSER_ERROR					= 4,	// на стадии анализатора
	SE_PARSER_FATAL_ERROR			= 5,	
	SE_PARSER_WARNING				= 6,
	SE_PREPROCESSOR_ERROR			= 7,	// на стадии препроцессора	
	SE_PREPROCESSOR_FATAL_ERROR		= 8,	
	SE_PREPROCESSOR_WARNING			= 9,	
	SE_SGRAMMAR_ERROR				= 10,	// эти ошибки относятся к построителю грамматики
	SE_SGRAMMAR_FATAL_ERROR			= 11,
	SE_SGRAMMAR_WARNING				= 12,
	SE_FORCE_DWORD					= 0xffffffff
}scriptErrorType;

class scriptError
{
public:
	shString		text;			// текст ошибки
	long			fileIndex;		// индекс файла в котором была обнаружена ошибка
	scriptRetCode	code;			// код ошибки
	scriptErrorType	type;			// тип ошибки
	int				line;			// линия
	int				col;			// позиция
	
	scriptError();					
	~scriptError();

	void			clear();							// очистка
	scriptError&	copy( scriptError& se );			// копирование
	void			print();							// печать ошибки

	void			printFileIndex( );					// печать индекса файла
	void			printFileIndex( TCHAR *buffer );	// ...
	void			printLine( );						// печать линии
	void			printLine( TCHAR *buffer );			// ...
	void			printCol( );						// печать столбца
	void			printCol( TCHAR *buffer );			// ...
	void			printCodeDesc( );					// печать кода
	void			printCodeDesc( TCHAR *buffer );		// ...
	void			printCode( );						// печать кода
	void			printCode( TCHAR *buffer );			// ...
	void			printType( );						// печать типа ошибки
	void			printType( TCHAR *buffer );			// ...
	void			printText( );						// печать текста
	void			printText( TCHAR *buffer );			// ...
};

void clearErrorsList( std::list< scriptError* > &errList );

#endif