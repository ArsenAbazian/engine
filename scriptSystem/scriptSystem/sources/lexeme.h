#ifndef __SCRIPT_LEXEME_INC
#define __SCRIPT_LEXEME_INC

#include <windows.h>
#include <tchar.h>
#include <list>
#include "fileinfo.h"
#include "hstring.h"

// класс символов
// 0 - плохой символ
// 1 - цифры 0 1 2 3 4 ...
// 2 - буквы
// 3 - разделитель + - / ...

static BYTE symbolClass[ 256 ] = 
				{
//					0 1 2 3 4 5 6 7 8 9 a b c d e f
					0,0,0,0,0,0,0,0,0,3,3,0,0,3,0,0, //0
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1
					3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, //2
					1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3, //3
					3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //4
					2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,2, //5
					3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //6
					2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,0, //7
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //8
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //9
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //a
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //b
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //c
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //d
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //e
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  //f
				};

typedef enum _symbolClassCode	// код класса символа
{
	SCC_BAD_SYMBOL		= 0,
	SCC_DIGIT			= 1,
	SCC_LETTER			= 2,
	SCC_PARSER			= 3
}symbolClassCode;

typedef enum _lexType	// тип лексемы
{
	LT_NONE				= 0,	// ничего нет
	LT_ERROR			= 1,	// ошибка
	LT_PARSER			= 2,	// разделитель пока еще не оператор
	LT_ID				= 3,	// идентификатор
	LT_RWORD			= 4,	// зарезервированное слово
	LT_CONST			= 5,	// числовая или символьная константа
	LT_STRING			= 6,	// строковая константа
	LT_BAD_CHARACTER	= 7,	// плохой символ
	LT_OPERATOR			= 8,	// оператор
	LT_NTERMINAL		= 9,	// нетерминал
	LT_FORCE_DWORD		= 0xffffffff
}lexType;

// если будешь менять порядок перечисления
// или добавлять новую величину знай, что
// тогда необходимо внести изменения в массив 
// conversion ОБЯЗАТЕЛЬНО!!!!!!!!!!!!!!!!!!!!!
typedef enum _lexConstType	// тип константы
{	
	LCT_NONE		= 0,
	LCT_BOOL		= 1,	// булевая константа
	LCT_BINARY		= 2,	// типа 0b111011 или 0B111011
	LCT_OCTAL		= 3,	// восьмиричная
	LCT_DECIMAL		= 4,	// целая константа в десятеричной системе счисления
	LCT_HEX			= 5,	// константа в шестнадцатеричной системе счисления
	LCT_SYMBOL		= 6,	// символьная константа
	LCT_FLOAT		= 7,	// вещественная константа
	LCT_DOUBLE		= 8,	// вещественная константа повышенной точности
	LCT_FORCE_DWORD	= 0xffffffff
}lexConstType;

static lexConstType lexConstTypeConv[9][9] = 
{
	/* LCT_NONE */     { LCT_NONE,     LCT_BOOL,     LCT_BINARY,   LCT_OCTAL,   LCT_DECIMAL, LCT_HEX,    LCT_SYMBOL,  LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_BOOL */     { LCT_BOOL,     LCT_BOOL,     LCT_BINARY,   LCT_OCTAL,   LCT_DECIMAL, LCT_HEX,    LCT_SYMBOL,  LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_BINARY */   { LCT_BINARY,   LCT_BINARY,   LCT_BINARY,   LCT_OCTAL,   LCT_DECIMAL, LCT_HEX,    LCT_BINARY,  LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_OCTAL */    { LCT_OCTAL,    LCT_OCTAL,    LCT_OCTAL,    LCT_OCTAL,   LCT_DECIMAL, LCT_HEX,    LCT_OCTAL,   LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_DECIMAL */  { LCT_DECIMAL,  LCT_DECIMAL,  LCT_DECIMAL,  LCT_DECIMAL, LCT_DECIMAL, LCT_HEX,    LCT_DECIMAL, LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_HEX */      { LCT_HEX,      LCT_HEX,      LCT_HEX,      LCT_HEX,     LCT_HEX,     LCT_HEX,    LCT_HEX,     LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_SYMBOL */   { LCT_SYMBOL,   LCT_SYMBOL,   LCT_BINARY,   LCT_OCTAL,   LCT_DECIMAL, LCT_HEX,    LCT_SYMBOL,  LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_FLOAT */    { LCT_FLOAT,    LCT_FLOAT,    LCT_FLOAT,    LCT_FLOAT,   LCT_FLOAT,   LCT_FLOAT,  LCT_FLOAT,   LCT_FLOAT,  LCT_DOUBLE },
	/* LCT_DOUBLE */   { LCT_DOUBLE,   LCT_DOUBLE,   LCT_DOUBLE,   LCT_DOUBLE,  LCT_DOUBLE,  LCT_DOUBLE, LCT_DOUBLE,  LCT_DOUBLE, LCT_DOUBLE }
};

typedef enum _lexParserType
{
	LPT_NONE									= 0,
	LPT_BITWISE_NOT_OR_DESTRUCTOR				= 1,
	LPT_LOGICAL_NOT								= 2,
	LPT_PREPROC_DIRECTIVE						= 3,
	LPT_DOLLAR									= 4,
	LPT_REMAINDER								= 5,
	LPT_BITWISE_EXCLUSIVE_OR					= 6,
	LPT_BITWISE_AND								= 7,
	LPT_MULTIPLICATION							= 8,
	LPT_LEFT_BRACKET							= 9,
	LPT_RIGHT_BRACKET							= 10,
	LPT_SUBTRACTION								= 11,
	LPT_ADDITION								= 12,
	LPT_ASSIGNMENT								= 13,
	LPT_LEFT_SQUARE_BRACKET						= 14,
	LPT_RIGHT_SQUARE_BRACKET					= 15,
	LPT_LEFT_FIGURE_BRACKET						= 16,
	LPT_RIGHT_FIGURE_BRACKET					= 17,
	LPT_DOT2									= 18,
	LPT_DOTCOMMA								= 19,
	LPT_BITWISE_OR								= 20,
	LPT_BACKSLASH								= 21,
	LPT_COMMA									= 22,
	LPT_DOT										= 23,
	LPT_QUESTION								= 24,
	LPT_DIVISION								= 25,
	
	LPT_SCOPE_RES								= 26,
	LPT_MEMBER_SEL_POINTER						= 27,
	LPT_INC										= 28,
	LPT_DEC										= 29,
	LPT_APPLY_POINTER_TO_CLASS_MEMBER			= 30,
	LPT_DEREFERENCE_POINTER_TO_CLASS_MEMBER		= 31,
	LPT_LEFT_SHIFT								= 32,
	LPT_RIGHT_SHIFT								= 33,
	LPT_LESS_THAN								= 34,
	LPT_GREATER_THAN							= 35,
	LPT_LESS_THAN_OR_EQUAL_TO					= 36,
	LPT_GREATER_THAN_OR_EQUAL_TO				= 37,
	LPT_EQUALITY								= 38,
	LPT_INEQUALITY								= 39,
		
	LPT_LOGICAL_AND								= 40,
	LPT_LOGICAL_OR								= 41,
	
	LPT_MUL_ASSIGNMENT							= 42,
	LPT_DIV_ASSIGNMENT							= 43,
	LPT_MOD_ASSIGNMENT							= 44,
	LPT_ADD_ASSIGNMENT							= 45,
	LPT_SUB_ASSIGNMENT							= 46,
	LPT_LEFT_SHIFT_ASSIGNMEN					= 47,
	LPT_RIGHT_SHIFT_ASSIGNMENT					= 48,
	LPT_BITWISE_AND_ASSIGNMENT					= 49,
	LPT_BITWISE_OR_ASSIGNMENT					= 50,
	LPT_BITWISE_EXCLUSIVE_OR_ASSIGNMENT			= 51,
	
	LPT_UNARY_MINUS								= 52,
	LPT_UNARY_PLUS								= 53,
	LPT_DOT3									= 54,

	LPT_FORCE_DWORD								= 0xffffffff
}lexParserType;

// массив разделителей
static shString scriptParserNameArray[] = 
{

	shString( TEXT( "unrecognized" ) ),
	shString( TEXT( "~" ) ),
	shString( TEXT( "!" ) ),
	shString( TEXT( "#" ) ),
	shString( TEXT( "$" ) ),
	shString( TEXT( "%" ) ),
	shString( TEXT( "^" ) ),
	shString( TEXT( "&" ) ),
	shString( TEXT( "*" ) ),
	shString( TEXT( "(" ) ),
	shString( TEXT( ")" ) ),
	shString( TEXT( "-" ) ),
	shString( TEXT( "+" ) ),
	shString( TEXT( "=" ) ),
	shString( TEXT( "[" ) ),
	shString( TEXT( "]" ) ),
	shString( TEXT( "{" ) ),
	shString( TEXT( "}" ) ),
	shString( TEXT( ":" ) ),
	shString( TEXT( ";" ) ),
	shString( TEXT( "|" ) ),
	shString( TEXT( "\\" ) ),
	shString( TEXT( "," ) ),
	shString( TEXT( "." ) ),
	shString( TEXT( "?" ) ),
	shString( TEXT( "/" ) ),
	
	shString( TEXT( "::" ) ),
	shString( TEXT( "->" ) ),
	shString( TEXT( "++" ) ),
	shString( TEXT( "--" ) ),
	shString( TEXT( ".*" ) ),
	shString( TEXT( "->*" ) ),
	shString( TEXT( "<<" ) ),
	shString( TEXT( ">>" ) ),
	shString( TEXT( "<" ) ),
	shString( TEXT( ">" ) ),
	shString( TEXT( "<=" ) ),
	shString( TEXT( ">=" ) ),
	shString( TEXT( "==" ) ),
	shString( TEXT( "!=" ) ),
		
	shString( TEXT( "&&" ) ),
	shString( TEXT( "||" ) ),
	
	shString( TEXT( "*=" ) ),
	shString( TEXT( "/=" ) ),
	shString( TEXT( "%=" ) ),
	shString( TEXT( "+=" ) ),
	shString( TEXT( "-=" ) ),
	shString( TEXT( "<<=" ) ),
	shString( TEXT( ">>=" ) ),
	shString( TEXT( "&=" ) ),
	shString( TEXT( "|=" ) ),
	shString( TEXT( "^=" ) ),

	shString( TEXT( "u-" ) ),
	shString( TEXT( "u+" ) ),
	shString( TEXT( "..." ) )

};

// массив зарезервированных слов языка 
static shString	scriptRWordArray[] = 
{
	shString ( TEXT( "unavailableRword" ) ),
	shString ( TEXT( "abstract" ) ), 
	shString ( TEXT( "as" ) ),
	shString ( TEXT( "base" ) ),
	shString ( TEXT( "bool" ) ),
	shString ( TEXT( "break" ) ),
	shString ( TEXT( "byte" ) ),
	shString ( TEXT( "case" ) ),
	shString ( TEXT( "catch" ) ),
	shString ( TEXT( "char" ) ),
	shString ( TEXT( "checked" ) ),
	shString ( TEXT( "class" ) ),
	shString ( TEXT( "const" ) ),
	shString ( TEXT( "continue" ) ),
	shString ( TEXT( "decimal" ) ),
	shString ( TEXT( "default" ) ),
	shString ( TEXT( "delegate" ) ), 
	shString ( TEXT( "do" ) ),
	shString ( TEXT( "double" ) ),
	shString ( TEXT( "else" ) ),
	shString ( TEXT( "enum" ) ), 
	shString ( TEXT( "event" ) ),
	shString ( TEXT( "explicit" ) ),
	shString ( TEXT( "extern" ) ), 
	shString ( TEXT( "false" ) ),
	shString ( TEXT( "finally" ) ),
	shString ( TEXT( "fixed" ) ),
	shString ( TEXT( "float" ) ),
	shString ( TEXT( "for" ) ),
	shString ( TEXT( "foreach" ) ),
	shString ( TEXT( "get" ) ),
	shString ( TEXT( "goto" ) ),
	shString ( TEXT( "if" ) ),
	shString ( TEXT( "implicit" ) ),
	shString ( TEXT( "in" ) ),
	shString ( TEXT( "int" ) ),
	shString ( TEXT( "interface" ) ),
	shString ( TEXT( "internal" ) ),
	shString ( TEXT( "is" ) ),
	shString ( TEXT( "lock" ) ), 
	shString ( TEXT( "long" ) ),
	shString ( TEXT( "namespace" ) ),
	shString ( TEXT( "new" ) ),
	shString ( TEXT( "null" ) ),
	shString ( TEXT( "object" ) ), 
	shString ( TEXT( "out" ) ),
	shString ( TEXT( "override" ) ),
	shString ( TEXT( "params" ) ),
	shString ( TEXT( "private" ) ),
	shString ( TEXT( "protected" ) ),
	shString ( TEXT( "public" ) ),
	shString ( TEXT( "readonly" ) ),
	shString ( TEXT( "ref" ) ),
	shString ( TEXT( "return" ) ),
	shString ( TEXT( "sbyte" ) ),
	shString ( TEXT( "sealed" ) ),
	shString ( TEXT( "set" ) ),
	shString ( TEXT( "short" ) ),
	shString ( TEXT( "sizeof" ) ),
	shString ( TEXT( "stackalloc" ) ),
	shString ( TEXT( "static" ) ),
	shString ( TEXT( "string" ) ),
	shString ( TEXT( "struct" ) ),
	shString ( TEXT( "switch" ) ),
	shString ( TEXT( "this" ) ),
	shString ( TEXT( "throw" ) ),
	shString ( TEXT( "true" ) ),
	shString ( TEXT( "try" ) ),
	shString ( TEXT( "typeof" ) ),
	shString ( TEXT( "uint" ) ),
	shString ( TEXT( "ulong" ) ),
	shString ( TEXT( "unchecked" ) ),
	shString ( TEXT( "unsafe" ) ),
	shString ( TEXT( "ushort" ) ),
	shString ( TEXT( "using" ) ),
	shString ( TEXT( "virtual" ) ),
	shString ( TEXT( "void" ) ),
	shString ( TEXT( "volatile" ) ),
	shString ( TEXT( "while" ) ),

	// директивы препроцессора
	shString ( TEXT( "include" ) ),
	shString ( TEXT( "import" ) ),
	shString ( TEXT( "define" ) ),
	shString ( TEXT( "elif" ) ),
	shString ( TEXT( "endif" ) ),
	shString ( TEXT( "ifdef" ) ),
	shString ( TEXT( "ifndef" ) ),
	shString ( TEXT( "undef" ) ),
	shString ( TEXT( "defined" ) ),
	shString ( TEXT( "error" ) ),
	shString ( TEXT( "pragma" ) ),

	// вспомогательные зарезервированные слова 
	shString ( TEXT( "__sgrammar_identifier" ) ),		// просто идентификатор
	shString ( TEXT( "__sgrammar_typeName" ) ),			// имя типа 
	shString ( TEXT( "__sgrammar_className" ) ),		// имя класса
	shString ( TEXT( "__sgrammar_structName" ) ),		// имя структуры
	shString ( TEXT( "__sgrammar_enumName" ) ),			// имя перечисления
	shString ( TEXT( "__sgrammar_variableName" ) ),		// имя переменной 
	shString ( TEXT( "__sgrammar_constName" ) ),		// имя константы
	shString ( TEXT( "__sgrammar_functionName" ) ),		// имя функции
	shString ( TEXT( "__sgrammar_variableClass" ) ),	// имя переменной - класса
	shString ( TEXT( "__sgrammar_variablStruct" ) ),	// имя переменной - структуры
	shString ( TEXT( "__sgrammar_variableEnum" ) ),		// имя переменной - перечисления
	shString ( TEXT( "__sgrammar_templateName" ) ),		// имя шаблона
	shString ( TEXT( "__sgrammar_variableTemplate" ) ),	// имя переменной шаблона
	shString ( TEXT( "__sgrammar_string" ) ),
	shString ( TEXT( "__sgrammar_stringName" ) ),
	shString ( TEXT( "__sgrammar_constValue" ) ),
	shString ( TEXT( "__sgrammar_enumItemName" ) )
};

// число зарезервированных слов
static DWORD	scriptRWCount		= sizeof( scriptRWordArray ) / sizeof( shString );
static DWORD	scriptParserCount	= sizeof( scriptParserNameArray ) / sizeof( shString );

typedef enum _lexRWordType	// тип зарезервированного слова лексемы
{

	LRW_NONE = 0,
	LRW_ABSTRACT = 1, 
	LRW_AS = 2,
	LRW_BASE = 3,
	LRW_BOOL = 4,
	LRW_BREAK = 5,
	LRW_BYTE = 6,
	LRW_CASE = 7,
	LRW_CATCH = 8,
	LRW_CHAR = 9,
	LRW_CHECKED = 10,
	LRW_CLASS = 11,
	LRW_CONST = 12,
	LRW_CONTINUE = 13,
	LRW_DECIMAL = 14,
	LRW_DEFAULT = 15,
	LRW_DELEGATE = 16, 
	LRW_DO = 17,
	LRW_DOUBLE = 18,
	LRW_ELSE = 19,
	LRW_ENUM = 20,  
	LRW_EVENT = 21,
	LRW_EXPLICIT = 22,
	LRW_EXTERN = 23, 
	LRW_FALSE = 24,
	LRW_FINALLY = 25,
	LRW_FIXED = 26,
	LRW_FLOAT = 27,
	LRW_FOR = 28,
	LRW_FOREACH = 29,
	LRW_GET = 30,
	LRW_GOTO = 31,
	LRW_IF = 32,
	LRW_IMPLICIT = 33,
	LRW_IN = 34,
	LRW_INT = 35,
	LRW_INTERFACE = 36,
	LRW_INTERNAL = 37,
	LRW_IS = 38,
	LRW_LOCK = 39, 
	LRW_LONG = 40,
	LRW_NAMESPACE = 41,
	LRW_NEW = 42,
	LRW_NULL = 43,
	LRW_OBJECT = 44, 
	LRW_OUT = 45,
	LRW_OVERRIDE = 46,
	LRW_PARAMS = 47,
	LRW_PRIVATE = 48,
	LRW_PROTECTED = 49,
	LRW_PUBLIC = 50,
	LRW_READONLY = 51,
	LRW_REF = 52,
	LRW_RETURN = 53,
	LRW_SBYTE = 54,
	LRW_SEALED = 55,
	LRW_SET = 56,
	LRW_SHORT = 57,
	LRW_SIZEOF = 58,
	LRW_STACKALLOC = 59,
	LRW_STATIC = 60,
	LRW_STRING = 61,
	LRW_STRUCT = 62,
	LRW_SWITCH = 63,
	LRW_THIS = 64,
	LRW_THROW = 65,
	LRW_TRUE = 66,
	LRW_TRY = 67,
	LRW_TYPEOF = 68,
	LRW_UINT = 69,
	LRW_ULONG = 70,
	LRW_UNCHECKED = 71,
	LRW_UNSAFE = 72,
	LRW_USHORT = 73,
	LRW_USING = 74,
	LRW_VIRTUAL = 75,
	LRW_VOID = 76,
	LRW_VOLATILE = 77,
	LRW_WHILE = 78,
	
	LRW_INCLUDE						= 79,
	LRW_IMPORT						= 80,
	LRW_DEFINE						= 81,
	LRW_ELIF						= 82,
	LRW_ENDIF						= 83,
	LRW_IFDEF						= 84,
	LRW_IFNDEF						= 85,
	LRW_UNDEF						= 86,
	LRW_DEFINED						= 87,
	LRW_ERROR						= 88,
	LRW_PRAGMA						= 89,

	LRW_SGRAMMAR_IDENTIFIER			= 90,	// идентификатор
	LRW_SGRAMMAR_TYPE_NAME			= 91,	// идентификатор, который является именем типа
	LRW_SGRAMMAR_CLASS_SNAME		= 92,	// идентификатор, который является именем класса
	LRW_SGRAMMAR_STRUCT_NAME		= 93,	// ... именем структуры
	LRW_SGRAMMAR_ENUM_NAME			= 94,	// ... именем перечисления
	LRW_SGRAMMAR_VARIABLE_NAME		= 95,	// ... именем переменной
	LRW_SGRAMMAR_CONST_NAME			= 96,	// ... именем константы
	LRW_SGRAMMAR_FUNCTION_NAME		= 97,	// ... именем функции
	LRW_SGRAMMAR_VARIABLE_CLASS		= 98,	// ... именем объекта класса тип Matrix a a - то самое
	LRW_SGRAMMAR_VARIABLE_STRUCT	= 99,	// ... именем переменной структуры
	LRW_SGRAMMAR_VARIABLE_ENUM		= 100,	// ... именем перечисления
	LRW_SGRAMMAR_TEMPLATE_NAME		= 101,	// ... имя шаблона
	LRW_SGRAMMAR_VARIABLE_TEMPLATE	= 102,	// ... имя переменной шаблона
	LRW_SGRAMMAR_STRING				= 103,	// строка
	LRW_SGRAMMAR_STRING_NAME		= 104,	// имя переменной строки
	LRW_SGRAMMAR_CONST_VALUE		= 105,	// константа
	LRW_SGRAMMAR_ENUM_ITEM_NAME		= 106,	// имя пункта перечисления

	LRW_FORCEDWORD					= 0xffffffff
}lexRWordType;	

/************************************************************************/
/* временные переменные                                                 */
/************************************************************************/
static shString		scriptLexemeDescription;
static TCHAR		scriptLexemeTempBuffer[ 128 ];

class scriptScaner;
class scriptLexeme	// представление лексемы полученной от сканера
{
	
	static shString	description;	// временное описание лексемы - одно описание
	static TCHAR	tempBuffer[128];// временный буффер
public:
	scriptScaner	*scaner;

	bool			useInGrammar;

	lexType			type;			// тип лексемы
	lexConstType	constType;		// тип константы лексемы
	long			fileIndex;		// индекс файла в котором была обнаружена лексема
	long			filePos;		// позиция в файле
	long			line;			// линия на которой была обнаружена лексема
	long			col;			// позиция на которой была обнаружена лексема
	
	bool			isUnsigned;		// если беззнаковое
	
	shString		name;			// имя идентификатора, если нет таблицы идентификаторов

	union							// в зависимости от типа будут разные данные
	{
		bool			boolValue;				
		BYTE			byteValue;
		long			longValue;
		DWORD			dwordValue;
		float			floatValue;
		double			doubleValue;

		lexRWordType	rwordType;	// код зарезервированного слова
		int				index;		// индекс в таблице 
		lexParserType	parser;		// код разделителя	
	};

	// конструктор
	scriptLexeme();									
	// копирующий конструктор	
	scriptLexeme( scriptLexeme &lex );				
	// деструктор
	~scriptLexeme();								

	// напечатать лексему
	void			print();
	void			print( std::list< scriptLexeme* > &lexList ); 
	
	// очистить лексему	
	void			clear();						
	// копирование	
	void			copy( scriptLexeme &lex );		
	// переименовать разделитель в код
	void			setParser( BYTE pars );			
	
	// получить текст лексемы
	shString&		getLexemeName();
	shString&		getLexemeName( std::list< scriptLexeme*> &lexList );

	// получить описание лексемы
	shString&		getLexemeDesc();
	shString&		getLexemeDesc( std::list< scriptLexeme*> &lexList );
	
	// является ли имя зарезервиованным словом
	// если да то производится преобразование лексемы
	bool			convertIfRWord();
	bool			convertStringToParser();

	bool			isId( shString &id );
	// следует ли данный разделитель непосредственно еще одним разделителем
	bool			isParsersNear( scriptLexeme &lex );	
	// истина если разделитель идет сразу же за идентификатором
	bool			isParserNearId( scriptLexeme &lex ); 
	// целочисленная константа ( двоичная, восьмеричная, десятеричная, 
	// шестнадцатеричная, знаковая или без неважно )
	bool			isNumericConst();
	bool			isFloatConst() { return this->type == LT_CONST && this->constType == LCT_FLOAT; }
	bool			isDoubleConst() { return this->type == LT_CONST && this->constType == LCT_DOUBLE; }
	// одинаковые ли лексемы ( верно только для идентификаторов и для зарезервированных слов )
	// проверяется только имена 
	bool			isEqual( scriptLexeme &lex );	
	// проверяется тип лексемы	
	bool			isEqualTypeEx( scriptLexeme &lex );
	// является ли лексема разделителем
	bool			isParser( lexParserType p ) { return this->type == LT_PARSER && this->parser == p; }
	// константа равна нулю
	bool			isZeroConst();					
	// на основе своего и ct типа константы
	// вернуть тип результата, полученого по обычным C шным законам приведения констант
	void			getResultType( scriptLexeme &rightOp, lexConstType &resConstType, bool &resUnsigned ); 
	// преобразовать данные согласно новому типу констант 
	void			convertData( lexConstType newConstType, bool newUnsigned );	
	// это все функции преобразования
	void			nullToNull();
	void			boolToBool();
	void			boolToByte();
	void			boolToLong();
	void			boolToDword();
	void			boolToFloat();
	void			boolToDouble();
	void			byteToBool();
	void			byteToByte();
	void			byteToLong();
	void			byteToDword();
	void			byteToFloat();
	void			byteToDouble();
	void			longToBool();
	void			longToByte();
	void			longToLong();
	void			longToDword();
	void			longToFloat();
	void			longToDouble();
	void			dwordToBool();
	void			dwordToByte();
	void			dwordToLong();
	void			dwordToDword();
	void			dwordToFloat();
	void			dwordToDouble();
	void			floatToBool();
	void			floatToByte();
	void			floatToLong();
	void			floatToDword();
	void			floatToFloat();
	void			floatToDouble();
	void			doubleToBool();
	void			doubleToByte();
	void			doubleToLong();
	void			doubleToDword();
	void			doubleToFloat();
	void			doubleToDouble();

	DWORD			getShiftValue( scriptLexeme &rightOp );
	
	void			operatorPlus( scriptLexeme &rightOp );
	void			operatorMinus( scriptLexeme &rightOp );
	void			operatorMul( scriptLexeme &rightOp );
	void			operatorDiv( scriptLexeme &rightOp );
	void			operatorModulus( scriptLexeme &rightOp );
	
	void			operatorGreater( scriptLexeme &rightOp );
	void			operatorGreaterEqual( scriptLexeme &rightOp );
	void			operatorLess( scriptLexeme &rightOp );
	void			operatorLessEqual( scriptLexeme &rightOp );
	void			operatorEquality( scriptLexeme &rightOp );
	void			operatorInEquality( scriptLexeme &rightOp );
	void			operatorLogAnd( scriptLexeme &rightOp );
	void			operatorLogOr( scriptLexeme &rightOp );

	void			operatorLeftShift( scriptLexeme &rightOp );
	void			operatorRightShift( scriptLexeme &rightOp );
	void			operatorBitAnd( scriptLexeme &rightOp );
	void			operatorBitOr( scriptLexeme &rightOp );
	void			operatorBitXor( scriptLexeme &rightOp );

	float			const2Float();
	int				const2Int();
};

// вспомогательные функции для работы с лексемами
void clearLexList( std::list< scriptLexeme* > &lexList ); // очистить список лексем
void clearLexLists( std::list< std::list< scriptLexeme* >* > &lexLists );	// очистить списки лексем
scriptLexeme*	getLexFromList( std::list< scriptLexeme* > &lexList, int tableIndex );	// получить лексему из списка по индексу
scriptLexeme*	getLexFromList( scriptLexeme &lex, std::list< scriptLexeme* > &lexList );	// получить лексему по имени
int				getLexemeIndex( scriptLexeme &lex, std::list< scriptLexeme* > &lexList );	// получить номер лексемы в списке лексем

int	getLexemeIndex( scriptLexeme *lexArray, int arraySize, scriptLexeme &lex );	// получить индекс лексемы в массиве лексем

scriptLexeme*	getLexFromList( std::list< scriptLexeme* >::iterator &sliBegin, std::list< scriptLexeme* >::iterator &sliEnd, scriptLexeme &lex );	// получить лексему из списка
bool			compareLexLists( std::list< scriptLexeme* >::iterator &sliBegin1, std::list< scriptLexeme* >::iterator &sliEnd1, std::list< scriptLexeme* >::iterator &sliBegin2, std::list< scriptLexeme* >::iterator &sliEnd2 );
bool			findLexemes( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* >::iterator &sliEnd, scriptLexeme &lex1, scriptLexeme &lex2 ); // ищет 2 подряд идущие лексемы, и если таковые есть останавливается на позиции lex1

typedef void (scriptLexeme::*lexConvFp)();

/************************************************************************/
/* вспомогательный массив в котором содержаться указатели				*/
/* на функции преобразования                                            */
/* входные параметры 
/* unsigned
/* твой тип
/* твой unsigned 
/* тип нового преобразования
/************************************************************************/

static lexConvFp lexConvArray[2][2][9][9] = 
{	
	// new unsigned = false
	{
		// old unsigned = false
		{
		
			// new = NONE
			{ &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull },
			// new = BOOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::byteToBool, &scriptLexeme::floatToBool, &scriptLexeme::doubleToBool },
			// new = BINARY
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = OCTAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = DECIMAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = HEX
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::longToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = SYMBOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::byteToByte, &scriptLexeme::floatToByte, &scriptLexeme::doubleToByte },
			// new = FLOAT
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::byteToFloat, &scriptLexeme::floatToFloat, &scriptLexeme::doubleToFloat },
			// new = DOUBLE
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::byteToDouble, &scriptLexeme::floatToDouble, &scriptLexeme::doubleToDouble }
		},
		// old unsigned = true
		{
			// new = NONE
			{ &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull },
				// new = BOOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::byteToBool, &scriptLexeme::floatToBool, &scriptLexeme::doubleToBool },
			// new = BINARY
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = OCTAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = DECIMAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = HEX
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::dwordToLong, &scriptLexeme::byteToLong, &scriptLexeme::floatToLong, &scriptLexeme::doubleToLong },
			// new = SYMBOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::byteToByte, &scriptLexeme::floatToByte, &scriptLexeme::doubleToByte },
			// new = FLOAT
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::byteToFloat, &scriptLexeme::floatToFloat, &scriptLexeme::doubleToFloat },
			// new = DOUBLE
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::byteToDouble, &scriptLexeme::floatToDouble, &scriptLexeme::doubleToDouble }			
		}
	},
	// new unsigned = true
	{
		// old nsigned = false
		{
			// new = NONE
			{ &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull },
			// new = BOOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::longToBool, &scriptLexeme::byteToBool, &scriptLexeme::floatToBool, &scriptLexeme::doubleToBool },
			// new = BINARY
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = OCTAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = DECIMAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = HEX
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::longToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = SYMBOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::longToByte, &scriptLexeme::byteToByte, &scriptLexeme::floatToByte, &scriptLexeme::doubleToByte },
			// new = FLOAT
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::longToFloat, &scriptLexeme::byteToFloat, &scriptLexeme::floatToFloat, &scriptLexeme::doubleToFloat },
			// new = DOUBLE
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::longToDouble, &scriptLexeme::byteToDouble, &scriptLexeme::floatToDouble, &scriptLexeme::doubleToDouble }	
		},
		// old unsigned = true
		{
			// new = NONE
			{ &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull, &scriptLexeme::nullToNull },
				// new = BOOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::dwordToBool, &scriptLexeme::byteToBool, &scriptLexeme::floatToBool, &scriptLexeme::doubleToBool },
			// new = BINARY
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = OCTAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = DECIMAL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = HEX
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::dwordToDword, &scriptLexeme::byteToDword, &scriptLexeme::floatToDword, &scriptLexeme::doubleToDword },
			// new = SYMBOL
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::dwordToByte, &scriptLexeme::byteToByte, &scriptLexeme::floatToByte, &scriptLexeme::doubleToByte },
			// new = FLOAT
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::dwordToFloat, &scriptLexeme::byteToFloat, &scriptLexeme::floatToFloat, &scriptLexeme::doubleToFloat },
			// new = DOUBLE
			{ &scriptLexeme::nullToNull, &scriptLexeme::boolToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::dwordToDouble, &scriptLexeme::byteToDouble, &scriptLexeme::floatToDouble, &scriptLexeme::doubleToDouble }	
		}
	}	
};

#endif