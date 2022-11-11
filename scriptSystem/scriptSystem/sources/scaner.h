#ifndef __SCRIPT_SCANER_INC
#define __SCRIPT_SCANER_INC

#include "scrcode.h"
#include "fileinfo.h"
#include "hstring.h"
#include "lexeme.h"
#include "screrror.h"
#include "scrmacro.h"
#include <stack>

typedef enum _scriptScanerState		// описание состояний сканера
{
	SSS_NONE							= 0,	// свободное состояние
	SSS_ID_RWORD						= 1,	// состояние считывания идентификатора
	SSS_NUMERIC_CONST					= 2,	// состояние считывания численной константы
	SSS_COMMENT_LINE					= 3,	// комментарий линии
	SSS_COMMENT_BLOCK					= 4,	// комментарий блока
	SSS_STRING							= 5,	// строковая константа
	SSS_ERROR							= 6,	// состояние плохого идентификатора
	SSS_CHAR_CONST						= 7,	// символьная константа
	SSS_FLOAT_CONST						= 8,	// вещественная константа
	SSS_FLOAT_CONST2					= 9,	// продолжение считывания вещественной константы
	SSS_FLOAT_CONST3					= 10,	// продолжение считывания вещественной константы
	SSS_FLOAT_CONST4					= 11,	// окончание считывания вещественной константы
	SSS_SCAN_INCLUDE					= 12,	// состояние в котором считывается строка в угловых скобках < ... >
	SSS_FIND_DIRECTIVE_SYMBOL			= 13,	// состояние в котором пропускаются все символы а остается только #
	SSS_WAIT_FOR_ENDIF_ANALOG			= 14,	// состояние в котором мы ожидаем директиву endif else и elif
	SSS_SCAN_ENDIF_ANALOG				= 15,	// состояние в котором мы считываем директиву
	SSS_FORCE_DWORD						= 0xffffffff
}scriptScanerState;

typedef enum						// описание типа пропуска сканирования
{
	SST_DO_NOT_SKIP_SCANNING		= 0,
	SST_SKIP_UNTIL_NEW_CONDITION	= 1,
	SST_SKIP_UNTIL_END				= 2,
	SST_FORCE_DWORD					= 0xffffffff
}scanSkipType;


class scriptScaner	// сканер - лексический анализатор текста
{

	// конфигурация сканера
	std::list< shString >					includeDirList;					// директоии для включаемых файлов
	shString								programmDirectory;				// путь программы
	shString								programmFileName;				// имя файла программы
	shString								programmFullPathName;			// полный путь файла

	// список макросов
	std::list< scriptMacros* >				macros;							
		
	// списки ошибок и предупреждений
	std::list< scriptError* >				errorList;						
		
	// информация о файле и файловом буффере
	fileInfo								fInfo;
	
	// состояние сканера
	scriptScanerState						state;
	
	// 2 кила под временный буффер
	TCHAR									tempBuffer[ 2048 ];	

	// для обеспечения рекурсии сканирования
	scriptScaner							*parent;		// родительский сканер
	scriptScaner							*prevScaner;	// предыдущий сканер
	scriptScaner							*nextScaner;	// следующий сканер
	
	// список лексем полученных в результате сканирования
	std::list< scriptLexeme* >				lexemes;										
	// список идентификаторов полученных в результате сканирования
	std::list< scriptLexeme* >				ids;	
	// список строковых констант полученных в результате сканирования
	std::list< scriptLexeme* >				constStrings;

	// список лексем в директиве препроцессора
	// а также список параметров макроса
	std::list< scriptLexeme* >				directiveLexemes;
	
	// входной и выходной список лексем макросов
	std::list< scriptLexeme* >				inputMacrosList;
	std::list< scriptLexeme* >				outputMacrosList;
	
	// вектор отсканировавших файлов		
	std::list< fileNameInfo >				scanedFiles;		

	// вспмогательные переменые
	bool									skipLine;			// показывает, что надо пропустить все лексемы до конца строки
	int										skipLineIndex;		// номер пропускаемой линии

	std::stack< scanSkipType >				skipScanning;		// пропускать сканирование лексем, пока не будет выполнено условие трансляции

	// индикатор ошибки в результате сканирования
	bool									bError;			

public:
	// управляющие флаги
	bool									enableFormatString;		// форматировать строку по принципу c++
	bool									skipPreprocDirectives;	// пропускать директивы
	bool									useInGrammar;			// сканировать для грамматики

	scriptScaner();					// конструктор 
	~scriptScaner();				// деструктор

	// инициализация 
	scriptRetCode					initialize( std::list< shString > &incDir );
	scriptRetCode					initialize( scriptScaner *prev );
	scriptRetCode					initialize(  );

	// файловые операции
	// открыть начальный файл для чтения ( обычно *.cpp файл )
	scriptRetCode					openFile( TCHAR *fileName );							
	// открыть файл в результате выполнения директивы include
	scriptRetCode					openIncludeFile( shString &fileName, bool inSystem );		
	
	// определяет используется ли указанный файл другим сканером
	bool							isFileOpened( shString &fileName );	

	// проинициализировать сканер уже готовым буффером
	scriptRetCode					addBuffer( BYTE *buffer, DWORD bufLength, int line, int col );				
	scriptRetCode					addBuffer( BYTE *buffer, DWORD bufLength, int line, int col, scriptScaner *owner );
	
	// операции лексического анализатора
	scriptRetCode					getNextLexeme( scriptLexeme	&lex );				// выделить следующую лексему из файла
	scriptRetCode					scan();											// полностью отсканировать файл
	scriptRetCode					scanDirective();								// сканировать директиву
	scriptRetCode					scanMacros( scriptLexeme &lex, scriptMacros& scrMacro );	// сканируем макрос
	scriptRetCode					scanMacros( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList, scriptMacros& scrMacro );	// сканируем макрос который находится в другом макросе 
	scriptRetCode					skipUntilDirective();							// пропустить сканирование пока не будет найдена директива endif elif или else 
	
	// разбор директив
	scriptRetCode					parseDirective();								// анализ директивы
	scriptRetCode					parseIncludeDirective();						// анализ директивы include
	scriptRetCode					parseDefineDirective();							// анализ директивы define 
	scriptRetCode					parseDefineParams( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli );	// разобрать параметры макроса
	scriptRetCode					parseDefineBody( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli );	// разобрать тело макроса
	scriptRetCode					parseIfDefDirective();							// анализ директивы ifdef
	scriptRetCode					parseIfNdefDirective();							// анализ директивы ifndef
	scriptRetCode					parseEndIfDirective();							// анализ директивы endif
	scriptRetCode					parseIfDirective();
	scriptRetCode					parseElseDirective();							// анализ директивы else
	scriptRetCode					parseUndefDirective();							// анализ директивы undef
	scriptRetCode					parseErrorDirective();							// анализ директивы error
	scriptRetCode					parseMacros( scriptLexeme &lex, scriptMacros &scrMacro );									// анализируем макрос									
	
	// выполнение директив
	scriptRetCode					executeIfDirective();
	scriptRetCode					executeIfDirective( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList );	// выполнить директиву if
	scriptRetCode					executeErrorDirective();						// выполнение директивы error
	scriptRetCode					executeIncludeDirective( shString &str, bool inSysDir );	// выполнение директивы include
	scriptRetCode					executeMacros( scriptLexeme &lex, scriptMacros &scrMacro, std::list< scriptLexeme* >	&lexList );		// выполнение макроподстановки
	scriptRetCode					executeDefinedOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );		// выполнить операцию defined macroname
	scriptRetCode					executeUnaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );	// выполнить операцию ! или ~
	scriptRetCode					executeBinaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );	// выполнить бинарные операции
	scriptRetCode					executeQuickIfOp( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList ); // выполнить быстрый if

	// оптимизация на уровне сканера
	scriptRetCode					optimizeMacroUnaryMinusAndPlus( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList );	// очень печально но эту функцию надо вызывать каждый раз после преобразований
	scriptRetCode					optimizeParsers( std::list< scriptLexeme* > &lexList );		// оптимизировать разделители ( превратить несколько стоящих рядом простых разделителей в комплексные )						
	scriptRetCode					optimizeStrings( std::list< scriptLexeme* > &lexList );		// соединение строк и занесение строк в таблицу
	scriptRetCode					optimizeIds( std::list< scriptLexeme* > &lexList );			// заносим все идентификаторы в таблицу
	scriptRetCode					optimize( std::list< scriptLexeme* > &lexList );			// оптимизировать лескемы в указанном списке
	scriptRetCode					optimize();													// оптимизировать список лексем полученные в результате сканирования

	// очистка ресурсов
	scriptRetCode					clear();									// очистить сканер
	scriptRetCode					clearScanerResult();						// очистить результат

	// вспомогательные функции
	bool							idExist( shString &name, int &index );		// проверяет, занесен ли идентификатор в таблицу идентификаторов
	bool							stringExist( shString &name, int &index );	// проверяет, занесена ли строка в таблицу строк
	scriptMacros*					macroExist( shString &name );				// проверяет, существует ли такой макрос
	void							createIdReference( scriptLexeme &lex );		// заносит идентификатор в таблицу идентификаторов и заменяет имя на его индекс в таблице
	void							createStringReference( scriptLexeme &lex );	// заносит строку в таблицу строк и заменяет текст строки на ее индкс в таблице 
	shString&						getLexemeName( scriptLexeme &lex );			// получить имя идентификатора или текст строки
	bool							isSystemIncludeDir( scriptLexeme &lex );	// найдена ли запись # include <						// возвращает истину если сканер обнаружил #include < 
	DWORD							toDword( TCHAR *szString, DWORD dwLength );
	bool							isEscapeSequence( TCHAR *szString, DWORD dwLength );	// если это ковычка внутри строки
	scriptRetCode					formatString( TCHAR *szString, TCHAR *szString2, DWORD dwLength, scriptLexeme &scrLex );		// форматируем строку
		
	// обработка списка файлов
	int								isFileScaned( shString fullPathName );		// был ли открыт уже такой файл
	void							getFileIndex( fileInfo &fi, int *fileInd );				// получить индекс файла если есть, если нет, то добавить а потом получить индекс файла
	void							addFileToList( fileInfo &fi );
	
	// выполнение математических операций препроцессора
	scriptRetCode					ppExecuteArithmeticOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );
	scriptRetCode					ppExecuteLogicalOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );
	scriptRetCode					ppExecuteBinaryOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );

	// обработка ошибок
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col );	// добавить ошибку в список ошибок	
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, int line, int col );	
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex );
	
	// функции доступа к членам класса
	std::list< scriptLexeme* >&		getLexemesList(){ return lexemes; }
	std::list< scriptLexeme* >&		getIdsList(){ return ids; }
	std::list< scriptLexeme* >&		getStringsList(){ return constStrings; }
	std::list< scriptError* >&		getErrorsList(){ return errorList; }
	std::list< scriptMacros* >&		getMacrosList(){ return macros; } 
	std::list< fileNameInfo >&		getFilesList(){ return scanedFiles; }	

	// события сканера
	void							onError();			// вызывается когда сканер выдает ошибку
	void							onNewLine();		// вызывается когда сканер переходит на следующую строку
	void							onNewFile();		// вызывается когда сканер открывает новый файл
	void							onGetNewLexeme();	// вызывается когда сканер находит новую лексему

	bool							isError(){ return this->bError; };

	fileNameInfo*					getFileNameInfo(int fileIndex);	// получить инфо файла по индексу
	
	bool							checkLex(std::list<scriptLexeme*>::iterator &sli) { return sli != this->lexemes.end(); }
};

#endif
