#ifndef __MODULE_DESC_INC
#define __MODULE_DESC_INC

#include "scaner.h"
#include "syntparser2.h"
#include "objDesc.h"
#include "scopeDesc.h"

// объявляем класс дескиптора программ
class scriptProgramm;

class scriptModule
{
	
	// указатель на программу, которой принадлежит данный модуль
	scriptProgramm			*programm;	
	
	// область видимости программы
	scriptScopeDesc			scope;
	
	// сканер для данного модуля
	scriptScaner			scaner;
	// синтаксический анализатор для данного модуля
	scriptSyntaxParser2		parser;
	
	// индекс модуля в списке модулей
	int						index;	

public:
	// как обычно, конструкторы и деструкторы
	scriptModule( scriptProgramm *prg );
	~scriptModule(){ this->clear(); }

	// очищаем модуль
	void				clear();

	// инициализация модуля
	scriptRetCode		initialize( int ind );
	
	// компилляция модуля
	scriptRetCode		compile( shString );

};

void clearModuleList( std::list< scriptModule* > &ml );

#endif