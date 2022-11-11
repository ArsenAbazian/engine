#ifndef __SCRIPT_MACRO_INC
#define __SCRIPT_MACRO_INC

#include <windows.h>
#include <tchar.h>
#include <list>
#include "fileinfo.h"
#include "hstring.h"
#include "lexeme.h"
#include "scrcode.h"

// описание последовательности лексем во входном параметре макроса
typedef struct
{
	std::list< scriptLexeme* >::iterator	begin;
	std::list< scriptLexeme* >::iterator	end;
	int										size;
}macrosInputParam;

class scriptMacros	// класс описатель макроса
{
public:

	shString									name;			// имя макроса
	long										fileIndex;		// индекс файла в котором был обнаружен макрос
	int											line;			// линия на которой нашли макрос
	int											col;			// позиция на которой нашли макрос
	std::list< scriptLexeme* >					params;			// параметры макроса
	std::list< scriptLexeme* >					body;			// тело макроса
	macrosInputParam							inputList;		// входной список макроса
	std::list< macrosInputParam >				input;			// входные параметры в макрос
	bool										called;			// истина если мы генерируем данный макрос
	bool										isSystem;		// макрос который был добавлен в коммандной строке 

	scriptMacros();
	~scriptMacros();

	void						clear();						// очистка макроса 
	scriptMacros&				copy( scriptMacros &sm );		// копирование макроса
	bool						isParam( scriptLexeme &lex, std::list< macrosInputParam >::iterator &mipi );	// возвращает истину, если указанная лексема - параметр макроса 
	
};

#endif