#ifndef __OBJDESC_INC
#define __OBJDESC_INC

#include <list>
#include <windows.h>
#include "typeDesc.h"
#include "lexeme.h"

typedef enum
{
	OBJ_TYPE_NONE			= 0,	// ничего
	OBJ_TYPE_VARIALE		= 1,	// простая переменная 
	
	OBJ_TYPE_FORCE_DWORD	= 0xffffffff
}objectType;

// этот класс является базовым классом для 
// описания скриптового объекта, коим является 
// переменая, перечисление, класс, структура и т. д.
class scriptObjectDesc
{
public:
	// информация о положении
	scriptLexeme		lexeme;			// лексема описывающая объект

	objectType			objClass;		// класс объекта ...
	scriptTypeDesc		typeDesc;		// описание типа объекта (тип переменной, возвращаемое значение функции и т.д.)
	
	scriptObjectDesc();
	~scriptObjectDesc();
};

// очистить список обектов
void clearObjList( std::list< scriptObjectDesc* > &sodl );

#endif