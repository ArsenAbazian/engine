#ifndef __SCRIPT_SYNTAX_PARSER_INC
#define __SCRIPT_SYNTAX_PARSER_INC

#include "scaner.h"
#include "rulepproc.h"
#include "objDesc.h"
#include "scopeDesc.h"

// записать файл если он открыт
#define SAFE_LOG( X ) if( this->fpLog ) X

typedef enum {

	RR_FALSE	= 0,
	RR_TRUE		= 1,
	RR_WRONG	= 2

}ruleResult;

class scriptSyntaxParser2;

typedef ruleResult ( scriptSyntaxParser2::*rulePointer ) ();

// перед описанием класса надо объ€вить
class scriptModule;

// описание класса синтаксического анализатора
// провер€ющего сверху вниз рекурсивным спуском
class scriptSyntaxParser2
{
	
	std::list< scriptError*	>				errorList;			// список ошибок
	bool									bError;				// индикатор ошибки
	
	bool									ruleError;			// ошибка в процессе правила
	bool									skipSymantic;		// пропустить семантическую проверку

	// 2 кила под временный буффер
	TCHAR									tempBuffer[ 2048 ];	

	// ƒл€ отладочной информации
	FILE									*fpLog;
	shString								logFileName;		// им€ файла отладки

	// сканер, который производил лексическую поверку
	scriptScaner							*scaner;
	
	// список лексем сканера
	std::list< scriptLexeme* >				*lexemeList;
	
	// список идентификаторов и строк
	std::list< scriptLexeme* >				*idsList;
	std::list< scriptLexeme* >				*stringList;
	
	// список файлов
	std::list< fileNameInfo >				*fileList;
	
	// текуща€ лексема
	std::list< scriptLexeme* >::iterator	currentLexeme;

	std::list< scriptLexeme* >::iterator	tmpSli1;
	
	// модуль в который будет записыватьс€ вс€ информаци€
	scriptModule							*module;			
	
	// пост обработка правила
	rulePostProcessor						rulepp;

	// описание объекта
	scriptObjectDesc						objDesc;
	
	// описание типа
	scriptTypeDesc							typeDesc;
	
	// стек области имен
	std::list< scriptScopeDesc* >			scopeList;

public:

	scriptSyntaxParser2();
	~scriptSyntaxParser2(){ this->clear(); }

	// инициализируем модуль
	void								setModule( scriptModule *mod ){ this->module = mod; }
	// инициализируем файл отчета
	bool								initializeLog( shString fileName );
	// закрыт файл отчета
	void								closeLog();
	// очистить все
	void								clear();

	// анализировать код
	scriptRetCode						parse( scriptScaner &scaner );
	
	// достигнут ли конец списка лексем
	bool								isEnd(){ return this->currentLexeme == this->lexemeList->end(); }
	
	// возвращает истину если в указанной области видимости имен найдет идентификатор
	bool								getVariableInScope( scriptScopeDesc &scope, shString idName, int *objIndex );
	bool								getVariableInScope( scriptScopeDesc &scope, int idIndex, int *objIndex );

	// пошли правила рекурсивного спуска согласно грамматике!!!
	
	// вспомогательные значени€
	ruleResult							isConstValue();
	ruleResult							isParser( lexParserType &pt );
	ruleResult							isParser( const lexParserType &pt );
	ruleResult							isParser( int pCount, lexParserType *pt );
	ruleResult							isId( );
	ruleResult							isRword( lexRWordType &rw );
	ruleResult							isRword( const lexRWordType &rw );
	ruleResult							isBuildInTypeName();
	ruleResult							isOneOfRules( int ruleCount, rulePointer *rulesArray );	// одно из альтернатив
	ruleResult							isTypedefName( );				// какое то им€ typedef
	ruleResult							isClassName( );					// это им€ класса	
	ruleResult							isTemplateClassName( );			// это им€ шаблона класса
	ruleResult							isTypedefClassName( );			// это им€ typedef объвлени€
	ruleResult							isTypedefTemplateClassName( );	// это им€ typedef объ€влени€ шаблона класса
	ruleResult							isNamespaceName();				// это им€ пространства имен
	ruleResult							isTypedefNamespaceName();		// это typedef им€ пространства имен
	ruleResult							isMemberFunction();				// это член функци€
	ruleResult							isMemberOperator();				// это член оператор
	ruleResult							isConstructor();				// может это конструктор
	ruleResult							isOperator();					// оператор
	
	// самое главное правило - программа
	ruleResult							programmModule( );
};

#endif