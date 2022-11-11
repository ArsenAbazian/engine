#ifndef __SCRIPT_SYNTAX_PARSER_INC
#define	__SCRIPT_SYNTAX_PARSER_INC

#include "scaner.h"
#include "rulepproc.h"

// эта часть кода вставлена из файла grmParser.h проекта scGrammar

// ------------>

typedef enum _relationCode
{
	RELCODE_NONE				= 0,
	RELCODE_EQUAL				= 1,
	RELCODE_GREATER				= 2,
	RELCODE_LESS				= 3,
	RELCODE_GREATEREQUAL		= 4,
	RELCODE_LESSEQUAL			= 5,
	RELCODE_GREATERLESS			= 6,
	RELCODE_LESSGREATER			= 7,
	RELCODE_LESSGREATEREQUAL	= 8,
	RELCODE_GREATERLESSEQUAL	= 9
}relationCode;

// вспомогательная функция возвращает текст отношения
TCHAR	*relationCode2Text( relationCode relcode );

// для левых и правых троек
#define TRIADE_LEXEME_EMPTY		(-1)

// описание тройки
typedef struct
{
	int							s1;		// первая лексема
	int							s2;		// вторая лексема
	int							s3;		// третья лексема
}sgpTriade;

// вспомогательная функция для поиска индекс тройки в массиве
int	getTriadeIndex( sgpTriade *triadeList, int triadeCount, int s1, int s2, int s3 );

// <------------

class scriptModule;	// объявление класса модуля 

class scriptSyntaxParser
{
	std::list< scriptError*	>		errorList;			// список ошибок
	bool							bError;				// индикатор ошибки

	int								matrixSize;			// размер матрицы
	BYTE							**matrix;			// сама матрица предшествования

	int								lexemeCount;		// число лексем, найденных в грамматике
	scriptLexeme					*lexemeList;		// список лексем, найденных в грамматике
	
	int								ruleCount;			// число правил, найденных в грамматике
	int								*ruleLeftPart;		// левые части правил
	int								*rightPartSize;		// размер правой части правил, по числу лексем в ней
	int								**ruleRightPart;	// правые части правил
	
	int								leftTriadeCount;	// число левых троек
	sgpTriade						*leftTriadeList;	// список левых троек

	int								rightTriadeCount;	// число правых троек
	sgpTriade						*rightTriadeList;	// список правых троек

	
	std::list< scriptLexeme* >		lexemeStack;		// стек для разбора списка лексем	
	std::list< int >				indexStack;			// стек индексов для разбора лексем

	// 2 кила под временный буффер
	TCHAR							tempBuffer[ 2048 ];	

	// Для отладочной информации
	FILE							*fpLog;
	shString						logFileName;		// имя файла отладки

	// модуль в который будет записываться вся информация
	scriptModule					*module;			
	// пост обработка правила
	rulePostProcessor				rulepp;

public:

	scriptSyntaxParser(  );								// конструктор
	~scriptSyntaxParser(){ clear(); }					// деструктор					

	void							clear();			// очистить ресурсы класса
	
	// инициализация анализатора - тело этой функции будет генерироваться приложением
	// scGrammarBuilder
	void							initialize(  );		// инициализация
	
	// инициализируем модуль
	void							setModule( scriptModule *mod ){ this->module = mod; }

	// вспомогательные функции для разбора
	// получить индекс лексемы
	int								getLexemeIndex( scriptLexeme &lex );
	// найти тройку лексем 
	int								getTriadeIndex( int s1, int s2, int s3, bool left );
	// то же самое только с помощью лексем
	int								getTriadeIndex( scriptLexeme *lex1, scriptLexeme *lex2, scriptLexeme *lex3, bool left );
	// применить правило
	scriptRetCode					findRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int &ruleIndex );
	scriptRetCode					applyRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int ruleIndex );

	// обработка ошибок
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col );	// добавить ошибку в список ошибок
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex );

	// разбор списка лексем
	scriptRetCode					parse( scriptScaner &scaner );	
	// просмотр стека в обратном порядке в поисках отношения <
	scriptRetCode					parseBack( scriptScaner &scaner, std::list< scriptLexeme *>::iterator &sliGreater );

	// отладочная информация
	
	// инициализирует файл отладки
	scriptRetCode					initLog( shString fileName );

	// печатает стек лексем и следующую лексему
	void							printStack( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli, relationCode relCode, std::list< scriptLexeme* > scList );
	// печатает правило, зажатое между лексемами
	void							printStack( std::list< scriptLexeme* >::iterator &sli1, std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli2, relationCode relCode1, relationCode relCode2, std::list< scriptLexeme* > scList );
	// печатает найденное правило
	void							printFoundRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list< scriptLexeme* >::iterator &sliEnd, std::list< int >::iterator &sliBeginIndex, std::list<int>::iterator &sliEndIndex, int ruleIndex, std::list< scriptLexeme* > scList );
};

#endif