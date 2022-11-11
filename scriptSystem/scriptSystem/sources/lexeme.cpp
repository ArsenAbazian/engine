#include "lexeme.h"
#include "scaner.h"
#include <float.h>

// очищает лексему
scriptLexeme::scriptLexeme()
{
	
	this->type				= LT_NONE;
	this->constType			= LCT_NONE;
	this->fileIndex			= -1;
	this->filePos			= -1;
	this->col				= -1;
	this->line				= -1;
	this->dwordValue		= 0;
	this->doubleValue		= 0.0;
	this->isUnsigned		= false;
	this->useInGrammar		= false;
}

scriptLexeme::scriptLexeme( scriptLexeme &lex )
{
	this->copy( lex );	
}

scriptLexeme::~scriptLexeme()
{
	this->clear();
}

void scriptLexeme::clear()
{

	this->type				= LT_NONE;
	this->constType			= LCT_NONE;
	this->fileIndex			= -1;
	this->filePos			= -1;
	this->col				= -1;
	this->line				= -1;
	this->dwordValue		= 0;
	this->doubleValue		= 0.0;
	this->isUnsigned	= false;

	this->name.clear();
	
	return ;
}

void scriptLexeme::print( std::list< scriptLexeme* > &lexList )
{
	
	_tprintf_s( TEXT( "fileIndex = %d;\n" ), this->fileIndex );
	_tprintf_s( TEXT( "filePos = %d;\n" ), this->filePos );
	_tprintf_s( TEXT( "line = %d;\n" ), this->line );
	_tprintf_s( TEXT( "col  = %d;\n" ), this->col );

	if( this->type == LT_ID && this->name.getCString() == NULL )
	{
		_tprintf_s( TEXT( "type = LT_ID;\n" ) );
		_tprintf_s( TEXT( "unsigned = %d;\n" ), this->isUnsigned );
		
		if( this->name.getLength() != 0 )
			_tprintf_s( TEXT( "id name = '%s';\n" ), this->name.getCString() );
		else 
			_tprintf_s( TEXT( "id name = '%s'  table index = %d;\n" ), this->getLexemeDesc( lexList ).getCString(), this->index );
	}
	else
		this->print();	

	return ;
}

void scriptLexeme::print()
{
	
	_tprintf_s( TEXT( "fileIndex = %d;\n" ), this->fileIndex );
	_tprintf_s( TEXT( "filePos = %d;\n" ), this->filePos );
	_tprintf_s( TEXT( "line = %d;\n" ), this->line );
	_tprintf_s( TEXT( "col  = %d;\n" ), this->col );
	
	switch( this->type )
	{
	case LT_NONE:
		_tprintf_s( TEXT( "type = LT_NONE;\n" ) );
		break;

	case LT_ERROR:
		_tprintf_s( TEXT( "type = LT_ERROR;\n" ) );
		break;

	case LT_PARSER:
		_tprintf_s( TEXT( "type = LT_PARSER;\n" ) );
		break;

	case LT_ID:
		_tprintf_s( TEXT( "type = LT_ID;\n" ) );
		break;

	case LT_RWORD:
		_tprintf_s( TEXT( "type = LT_RWORD;\n" ) );
		break;

	case LT_CONST:
		_tprintf_s( TEXT( "type = LT_CONST;\n" ) );
		break;

	case LT_STRING:
		_tprintf_s( TEXT( "type = LT_STRING;\n" ) );
		break;

	case LT_BAD_CHARACTER:
		_tprintf_s( TEXT( "type = LT_BAD_CHARACTER;\n" ) );
		break;
	
	case LT_NTERMINAL:
		_tprintf_s( TEXT( "type = LT_NTERMINAL" ) );
		break;
	
	default:
		_tprintf_s( TEXT( "type = LT_NONE;\n" ) );
		break;
	}

	switch( this->constType )
	{
	case LCT_NONE:
		_tprintf_s( TEXT( "constType = LCT_NONE;\n" ) );
		break;	

	case LCT_BOOL:
		_tprintf_s( TEXT( "constType = LCT_BOOL;\n" ) );
		if( this->boolValue )
			_tprintf_s( TEXT( "boolValue = true;\n" ) );
		else
			_tprintf_s( TEXT( "boolValue = false;\n" ) );
		break;

	case LCT_BINARY:
		_tprintf_s( TEXT( "constType = LCT_BINARY;\n" ) );
		if( this->isUnsigned )
			_tprintf_s( TEXT( "dwordValue = %d;\n" ), this->dwordValue );
		else 
			_tprintf_s( TEXT( "longValue = %d;\n" ), this->longValue );
		break;

	case LCT_OCTAL:
		_tprintf_s( TEXT( "constType = LCT_OCTAL;\n" ) );
		if( this->isUnsigned )
			_tprintf_s( TEXT( "dwordValue = %o;\n" ), this->dwordValue );
		else 
			_tprintf_s( TEXT( "longValue = %o;\n" ), this->longValue );
		break;

	case LCT_DECIMAL:
		_tprintf_s( TEXT( "constType = LCT_DECIMAL;\n" ) );
		if( this->isUnsigned )
			_tprintf_s( TEXT( "dwordValue = %d;\n" ), this->dwordValue );
		else 
			_tprintf_s( TEXT( "longValue = %d;\n" ), this->longValue );
		break;

	case LCT_HEX:
		_tprintf_s( TEXT( "constType = LCT_HEX;\n" ) );
		if( this->isUnsigned )
			_tprintf_s( TEXT( "dwordValue = 0x%8.8x;\n" ), this->dwordValue );
		else 
			_tprintf_s( TEXT( "longValue = 0x%8.8x;\n" ), this->longValue );
		break;

	case LCT_SYMBOL:
		_tprintf_s( TEXT( "constType = LCT_SYMBOL;\n" ) );
		_tprintf_s( TEXT( "byteVal   = %c ( 0x%2.2x );\n" ),this->byteValue, this->byteValue );
		break;

	case LCT_FLOAT:
		_tprintf_s( TEXT( "constType = LCT_FLOAT;\n" ) );
		_tprintf_s( TEXT( "floatValue = %f;\n" ), this->floatValue );
		break;

	case LCT_DOUBLE:
		_tprintf_s( TEXT( "constType = LCT_DOUBLE;\n" ) );
		_tprintf_s( TEXT( "doubleValue = %g;\n" ), this->doubleValue );
		break;
	
	default:
		_tprintf_s( TEXT( "constType = LCT_NONE;\n" ) );
		break;
	}

	_tprintf_s( TEXT( "unsigned = %d;\n" ), this->isUnsigned );
	
	if( this->type == LT_ID )
	{
		if( this->name.getLength() != 0 )
			_tprintf_s( TEXT( "id name = '%s';\n" ), this->name.getCString() );
		else 
			_tprintf_s( TEXT( "id table index = %d;\n" ), this->index );
	}
	else if( this->type == LT_NTERMINAL )
	{
		_tprintf_s( TEXT( "nt name = '%s';\n" ), this->name.getCString() );
	}

	else if( this->type == LT_RWORD )
		_tprintf_s( TEXT( "rword name = '%s';\n" ), scriptRWordArray[ this->rwordType ] );
	
	else if( this->type == LT_STRING )
	{
		if( this->name.getLength() != 0 )
			_tprintf_s( TEXT( "string text = '%s';\n" ), this->name.getCString() );
		else
			_tprintf_s( TEXT( "string table index = %d;\n" ), this->index );
	}
	
	else if( this->type == LT_PARSER )
		_tprintf_s( TEXT( "parser = '%s';\n" ), scriptParserNameArray[ this->parser ] );

	else if( this->type == LT_BAD_CHARACTER )
		_tprintf_s( TEXT( "character = '%c'\n" ), this->byteValue );

	return ;
}

void scriptLexeme::copy( scriptLexeme &lex )
{
	
	// очищаем
	this->clear();
	
	this->type			= lex.type;
	this->constType		= lex.constType;
	this->fileIndex		= lex.fileIndex;
	this->filePos		= lex.filePos;
	this->col			= lex.col;
	this->line			= lex.line;
	this->name			= lex.name;
	
	if( this->type == LT_CONST )
	{
		switch( this->constType )
		{
		case LCT_BOOL:
			this->boolValue		= lex.boolValue;
			break;
		case LCT_BINARY:
		case LCT_OCTAL:
		case LCT_HEX:
			this->dwordValue	= lex.dwordValue;
			break;
		case LCT_DECIMAL:
			this->longValue		= lex.longValue;
			break;
		case LCT_SYMBOL:
			this->byteValue		= lex.byteValue;
			break;
		case LCT_FLOAT:
			this->floatValue	= lex.floatValue;
			break;
		case LCT_DOUBLE:
			this->doubleValue	= lex.doubleValue;
			break;
		}
	}
	else if( this->type == LT_PARSER )
		this->parser = lex.parser;
	else if( this->type == LT_RWORD )
		this->rwordType = lex.rwordType;
	else if( this->type == LT_BAD_CHARACTER )
		this->byteValue = lex.byteValue;

	return ;
}

void scriptLexeme::setParser( BYTE pars )
{

	switch( pars )
	{
	case '~':
		parser = LPT_BITWISE_NOT_OR_DESTRUCTOR;
		break;
	case '!':
		parser = LPT_LOGICAL_NOT;
		break;
	case '#':
		parser = LPT_PREPROC_DIRECTIVE;
		break;
	case '$':
		parser = LPT_DOLLAR;
		break;
	case '%':
		parser = LPT_REMAINDER;
		break;
	case '^':
		parser = LPT_BITWISE_EXCLUSIVE_OR;
		break;
	case '&':
		parser = LPT_BITWISE_AND;
		break;
	case '*':
		parser = LPT_MULTIPLICATION;
		break;
	case '(':
		parser = LPT_LEFT_BRACKET;
		break;
	case ')':
		parser = LPT_RIGHT_BRACKET;
		break;
	case '-':
		parser = LPT_SUBTRACTION;
		break;
	case '+':
		parser = LPT_ADDITION;
		break;
	case '=':
		parser = LPT_ASSIGNMENT;
		break;
	case '[':
		parser = LPT_LEFT_SQUARE_BRACKET;
		break;
	case ']':
		parser = LPT_RIGHT_SQUARE_BRACKET;
		break;
	case '{':
		parser = LPT_LEFT_FIGURE_BRACKET;
		break;
	case '}':
		parser = LPT_RIGHT_FIGURE_BRACKET;
		break;
	case ':':
		parser = LPT_DOT2;
		break;
	case ';':
		parser = LPT_DOTCOMMA;
		break;
	case '|':
		parser = LPT_BITWISE_OR;
		break;
	case '\\':
		parser = LPT_BACKSLASH;
		break;
	case ',':
		parser = LPT_COMMA;
		break;
	case '.':
		parser = LPT_DOT;
		break;
	case '?':
		parser = LPT_QUESTION;
		break;
	case '/':
		parser = LPT_DIVISION;
		break;
	case '<':
		parser = LPT_LESS_THAN;
		break;
	case '>':
		parser = LPT_GREATER_THAN;
		break;
	default:
		parser = LPT_NONE;
		break;
	}
	
	return ;
}

shString& scriptLexeme::getLexemeName()
{
	if( this->type == LT_RWORD )
		return scriptRWordArray[ this->rwordType ];
	
	else if(this->type == LT_ID) { 
		if(this->name.getLength() > 0)
			return this->name;
		else if(this->scaner != NULL)
			return this->getLexemeName(this->scaner->getIdsList());
	}
	else if(this->type == LT_PARSER || this->type == LT_OPERATOR) 
		return scriptParserNameArray[ this->parser ];

	return this->name;
}

shString& scriptLexeme::getLexemeName( std::list< scriptLexeme* > &lexList )
{

	scriptLexeme	*lex2;

	// если идентификатор и нет имени 
	// взять имя из таблицы
	if( this->type == LT_ID && this->name.getCString() == NULL )
	{
		lex2 = getLexFromList( lexList, this->index );		
		return lex2->name;
	}
	else
		return this->getLexemeName();
}

shString& scriptLexeme::getLexemeDesc( std::list< scriptLexeme*> &lexList )
{
	
	if( this->type == LT_ID && this->name.getCString() == NULL )
	{
		scriptLexemeDescription = this->getLexemeName( lexList );
		return scriptLexemeDescription;
	}
	
	return this->getLexemeDesc();	
}

shString& scriptLexeme::getLexemeDesc()
{
		
	// по типу лексемы
	switch( this->type )
	{
	case LT_ID:
	case LT_NTERMINAL:
	case LT_RWORD:
		scriptLexemeDescription = this->getLexemeName();
		break;
	case LT_PARSER:
		//scriptLexemeDescription = TEXT( "'" );
		scriptLexemeDescription = scriptParserNameArray[ this->parser ];
		//scriptLexemeDescription += TEXT( "'" );
		break;
	case LT_STRING:
		scriptLexemeDescription = this->name;
		break;
	case LT_CONST:
		switch( this->constType )
		{
		case LCT_BOOL:
			if( this->boolValue == true )
				scriptLexemeDescription = TEXT( "true" );
			else if( this->boolValue == false )
				scriptLexemeDescription = TEXT( "false" );
			break;
		case LCT_BINARY:
		case LCT_OCTAL:
		case LCT_DECIMAL:
		case LCT_HEX:
			if( this->isUnsigned )
				_stprintf_s( scriptLexemeTempBuffer, TEXT( "%ud" ), this->dwordValue );
			else
				_stprintf_s( scriptLexemeTempBuffer, TEXT( "%d" ), this->longValue );
			scriptLexemeDescription = scriptLexemeTempBuffer;
			break;
		case LCT_SYMBOL:
			_stprintf_s( scriptLexemeTempBuffer, TEXT( "%c ( 0x%2.2x )" ), this->byteValue, this->byteValue );
			scriptLexemeDescription = scriptLexemeTempBuffer;
			break;
		case LCT_FLOAT:
			_stprintf_s( scriptLexemeTempBuffer, TEXT( "%f" ), this->floatValue );
			scriptLexemeDescription = scriptLexemeTempBuffer;
			break;
		case LCT_DOUBLE:
			_stprintf_s( scriptLexemeTempBuffer, TEXT( "%g" ), this->doubleValue );
			scriptLexemeDescription = scriptLexemeTempBuffer;
			break;
		default:
			scriptLexemeDescription	= TEXT( "(const type undefined)" );
			break;
		}
		break;
	default:
		scriptLexemeDescription	= TEXT( "(type undefined)" );
	}

	return scriptLexemeDescription;
}

bool scriptLexeme::isParsersNear( scriptLexeme &lex )
{
	int dist = this->col - lex.col;
	
	if( dist < 0 )
		dist = - dist;

	if( this->type != LT_PARSER || lex.type != LT_PARSER )
		return false;
	
	if( this->line == lex.line && dist == 1 )
		return true;

	return false;
}

bool scriptLexeme::isNumericConst()
{
	
	if( this->type == LT_CONST && ( this->constType == LCT_BINARY || this->constType == LCT_OCTAL || this->constType == LCT_DECIMAL || this->constType == LCT_HEX ) )
		return true;	

	return false;
}


void clearLexList( std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli;

	for( sli = lexList.begin(); sli != lexList.end(); sli++ )
		delete *sli;

	lexList.clear();
	
	return ;
}

scriptLexeme* getLexFromList( std::list< scriptLexeme* > &lexList, int tableIndex )
{
	
	std::list< scriptLexeme* >::iterator	sli;
	int	i;

	for( i=0, sli = lexList.begin(); sli != lexList.end(); sli++, i++ )
	{
		if( i == tableIndex )
			return *sli;
	}

	return NULL;
}

bool scriptLexeme::convertIfRWord()
{
	DWORD rwIndex;

	int count = this->useInGrammar? scriptRWCount : LRW_SGRAMMAR_IDENTIFIER;
	for( rwIndex = 0; rwIndex < count ; rwIndex++ )
	{
		if( this->name == scriptRWordArray[ rwIndex ] )
		{
			this->type		= LT_RWORD;
			this->rwordType = ( lexRWordType ) rwIndex;	
			this->name.clear();
			return true;
		}
	}
	
	return false;
}

bool scriptLexeme::isParserNearId( scriptLexeme &lex )
{
	
	int		dist;
	
	if( this->line != lex.line )
		return false;
		
	// получаем расстояние между концом первой лексемы и начала второй
	if( this->type == LT_ID )
		dist = lex.col - this->col - this->name.getLength();
	else 
		dist = lex.col - this->col - scriptRWordArray[ this->rwordType ].getLength();
	
	// если расстояние больше 1 символа
	if( dist )
		return false;
	
	return true;
}

bool scriptLexeme::isEqual( scriptLexeme &lex )
{

	// не совпадают типы
	if( this->type != lex.type )
		return false;

	// не совпадают имена идентификаторов
	if( this->type == LT_ID && this->name != lex.name )
		return false;

	// не совпадают зарезервированые слова
	if( this->type == LT_RWORD && this->rwordType != lex.rwordType )
		return false;
	
	return true;
}

// проверяется тип лексемы	
bool scriptLexeme::isEqualTypeEx( scriptLexeme &lex )
{
	
	scriptLexeme	*lex1, *lex2;
	
	// выясняем у кого реальный тип, а у кого зарезервированное слово
	if( this->type == LT_ID || this->type == LT_STRING || this->type == LT_CONST )
	{
		lex1 = this;
		lex2 = &lex;
	}
	else 
	{
		lex1 = &lex;
		lex2 = this;
	}

	// если найден разделитель
	if( lex1->type == LT_PARSER && lex2->type == LT_PARSER && lex1->parser == lex2->parser )
		return true ;

	// если нетерминал
	else if( lex1->type == LT_NTERMINAL && lex2->type == LT_NTERMINAL && lex1->name == lex2->name )
		return true ;

	// если настоящее зарезервированное слово для С++
	// или зарезервированное слово 
	else if( lex1->type == LT_RWORD && lex2->type == LT_RWORD && lex.rwordType == this->rwordType )
		return true ;

	// поскольку идентификатор это и имя класса и имя структуры и перечисления и имя перемнной или константы
	// то сравниваем ID с не только с другим ID, но и с зарезервированными словами
	else if( lex1->type == LT_ID && lex2->type == LT_ID )
		return true ; 
	else if( lex1->type == LT_ID && lex2->type == LT_RWORD && lex2->rwordType == LRW_SGRAMMAR_IDENTIFIER /*&&
																lex2->rwordType != LRW_SGRAMMAR_STRING && 
																lex2->rwordType != LRW_SGRAMMAR_CONST_VALUE */ )
		return true;
		
	// если строка
	else if( lex1->type == LT_STRING && lex2->type == LT_STRING )
		return true;
	else if( lex1->type == LT_STRING && lex2->type == LT_RWORD && lex2->rwordType == LRW_SGRAMMAR_STRING )
		return true;

	// если числовая константа
	else if( lex1->type == LT_CONST && lex2->type == LT_CONST )
		return true; 
	else if( lex1->type == LT_CONST && lex2->type == LT_RWORD && lex2->rwordType == LRW_SGRAMMAR_CONST_VALUE )
		return true;

	return false; 
}

bool scriptLexeme::isZeroConst()
{
	
	if( this->constType == LCT_BOOL && this->boolValue == false )
		return true;
	if( ( this->constType == LCT_BINARY || this->constType == LCT_OCTAL || this->constType == LCT_DECIMAL || this->constType == LCT_HEX ) && this->dwordValue == 0 )
		return true;
	if( this->constType == LCT_SYMBOL && this->byteValue == 0 )
		return true;
	if( this->constType == LCT_FLOAT && this->floatValue == 0.0f )
		return true;
	if( this->constType == LCT_DOUBLE && this->doubleValue == 0.0 )
		return true;

	return false;

}

void scriptLexeme::getResultType( scriptLexeme &rightOp, lexConstType &resConstType, bool &resUnsigned )
{ 
	resUnsigned		= this->isUnsigned || rightOp.isUnsigned;
	resConstType	= lexConstTypeConv[ this->constType ][ rightOp.constType ]; 
	
	return ;
}

void scriptLexeme::convertData( lexConstType newConstType, bool newUnsigned )
{

	// вычисляем адрес - согласно входным параметрам
	(this->*lexConvArray[newUnsigned][this->isUnsigned][newConstType][this->constType])();
	// новый параметры лексемы
	this->constType  = newConstType;
	this->isUnsigned = newUnsigned; 

	return ;
}

/************************************************************************/
/* это функции преобразования типов                                     */
/************************************************************************/
void scriptLexeme::nullToNull() { return; }

void scriptLexeme::boolToBool() { return; }
void scriptLexeme::boolToLong() { this->longValue = this->boolValue; return; }
void scriptLexeme::boolToDword() { this->dwordValue = this->boolValue; return; }
void scriptLexeme::boolToByte() { this->byteValue = this->boolValue; return; }
void scriptLexeme::boolToFloat() { this->floatValue = this->boolValue; return; }
void scriptLexeme::boolToDouble() { this->doubleValue = this->boolValue; return; }

void scriptLexeme::longToBool() { this->boolValue = this->longValue != 0; return; }
void scriptLexeme::longToLong() { return; }
void scriptLexeme::longToDword() { this->dwordValue = this->longValue; return; }
void scriptLexeme::longToByte() { this->byteValue = (BYTE) this->longValue; return; }
void scriptLexeme::longToFloat() { this->floatValue = (float)this->longValue; return; }
void scriptLexeme::longToDouble() { this->doubleValue = this->longValue; return; }

void scriptLexeme::dwordToBool() { this->boolValue = this->dwordValue != 0; return; }
void scriptLexeme::dwordToLong() { this->longValue = this->dwordValue; return; }
void scriptLexeme::dwordToDword() { return; }
void scriptLexeme::dwordToByte() { this->byteValue = (BYTE) this->dwordValue; return; }
void scriptLexeme::dwordToFloat() { this->floatValue = (float) this->dwordValue; return; }
void scriptLexeme::dwordToDouble() { this->doubleValue = this->dwordValue; return; }

void scriptLexeme::byteToBool() { this->boolValue = this->byteValue != 0; return; }
void scriptLexeme::byteToLong() { this->longValue = this->byteValue; return; }
void scriptLexeme::byteToDword() { this->dwordValue = this->byteValue; return; }
void scriptLexeme::byteToByte() { this->byteValue = this->byteValue; return; }
void scriptLexeme::byteToFloat() { this->floatValue = this->byteValue; return; }
void scriptLexeme::byteToDouble() { this->doubleValue = this->byteValue; return; }

void scriptLexeme::floatToBool() { this->boolValue = false; return; }
void scriptLexeme::floatToLong() { this->longValue = (long) this->floatValue; return; }
void scriptLexeme::floatToDword() { this->dwordValue = (DWORD) this->floatValue; return; }
void scriptLexeme::floatToByte() { this->byteValue = (BYTE) this->floatValue; return; }
void scriptLexeme::floatToFloat() { return; }
void scriptLexeme::floatToDouble() { this->doubleValue = this->floatValue; return; }

void scriptLexeme::doubleToBool() { this->boolValue = false; return; }
void scriptLexeme::doubleToLong() { this->longValue = (long) this->doubleValue; return; }
void scriptLexeme::doubleToDword() { this->dwordValue = (DWORD) this->doubleValue; return; }
void scriptLexeme::doubleToByte() { this->byteValue = (BYTE) this->doubleValue; return; }
void scriptLexeme::doubleToFloat() { this->floatValue = (float) this->doubleValue; return; }
void scriptLexeme::doubleToDouble() { return; }

DWORD scriptLexeme::getShiftValue( scriptLexeme &rightOp )
{
	// если булевая переменная
	if( rightOp.constType == LCT_BOOL )
		return (DWORD) rightOp.boolValue;
	// если символьная константа
	else if( rightOp.constType == LCT_SYMBOL )
		return (DWORD) rightOp.byteValue;
	// если обычное целое число
	// ( беззнаковое и знаковое )
	else if( rightOp.constType == LCT_BINARY || 
			rightOp.constType == LCT_OCTAL || 
			rightOp.constType == LCT_DECIMAL ||
			rightOp.constType == LCT_HEX )
		return (DWORD) rightOp.dwordValue; 
	// если вещственное число
	// но вещественное число как число сдвига являетс ошибкой
	// тем не менне возвратим хотя бы 0
	else 
		return 0;
	
}

void scriptLexeme::operatorPlus( scriptLexeme &rightOp )
{
	
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue != rightOp.boolValue; // сложение по модулю 2 :)
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue += rightOp.dwordValue;
		else
			this->longValue += rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue += rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->floatValue += rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->doubleValue += rightOp.doubleValue;
		break;
	}
}

void scriptLexeme::operatorMinus( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue != rightOp.boolValue; // вычитание по модулю 2
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue -= rightOp.dwordValue;
		else
			this->longValue -= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue -= rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->floatValue -= rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->doubleValue -= rightOp.doubleValue;
		break;
	}
}

void scriptLexeme::operatorMul( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue && rightOp.boolValue; // умножение по модулю 2
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue *= rightOp.dwordValue;
		else
			this->longValue *= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue *= rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->floatValue *= rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->doubleValue *= rightOp.doubleValue;
		break;
	}	
}

void scriptLexeme::operatorDiv( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue && rightOp.boolValue; // деление по модулю 2
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue /= rightOp.dwordValue;
		else
			this->longValue /= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue /= rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->floatValue /= rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->doubleValue /= rightOp.doubleValue;
		break;
	}	
}

void scriptLexeme::operatorModulus( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue && rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue %= rightOp.dwordValue;
		else
			this->longValue %= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue %= rightOp.byteValue;
		break;
	}	
}

void scriptLexeme::operatorGreater( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue > rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue > rightOp.dwordValue;
		else
			this->boolValue = this->longValue > rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue > rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue > rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue > rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorGreaterEqual( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue >= rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue >= rightOp.dwordValue;
		else
			this->boolValue = this->longValue >= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue >= rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue >= rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue >= rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorLess( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue < rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue < rightOp.dwordValue;
		else
			this->boolValue = this->longValue < rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue < rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue < rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue < rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorLessEqual( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue <= rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue <= rightOp.dwordValue;
		else
			this->boolValue = this->longValue <= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue <= rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue <= rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue <= rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorEquality( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue == rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue == rightOp.dwordValue;
		else
			this->boolValue = this->longValue == rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue == rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue == rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue == rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorInEquality( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue != rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue != rightOp.dwordValue;
		else
			this->boolValue = this->longValue != rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue != rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue != rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue != rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorLogAnd( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue && rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue && rightOp.dwordValue;
		else
			this->boolValue = this->longValue && rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue && rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue && rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue && rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorLogOr( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue = this->boolValue || rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->boolValue = this->dwordValue || rightOp.dwordValue;
		else
			this->boolValue = this->longValue || rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->boolValue = this->byteValue || rightOp.byteValue;
		break;
	case LCT_FLOAT:
		this->boolValue = this->floatValue || rightOp.floatValue;
		break;
	case LCT_DOUBLE:
		this->boolValue = this->doubleValue || rightOp.doubleValue;
		break;
	}	

	this->isUnsigned = false;
	this->constType  = LCT_BOOL;
}

void scriptLexeme::operatorRightShift( scriptLexeme &rightOp )
{
	
	DWORD	shiftVal = this->getShiftValue( rightOp );
	
	switch( this->constType )
	{
	case LCT_BOOL:
		if(shiftVal > 0) this->boolValue = 0;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue >>= shiftVal;
		else
			this->longValue >>= shiftVal;
		break;
	case LCT_SYMBOL:
		this->byteValue >>= shiftVal;
		break;
	}	
}

void scriptLexeme::operatorLeftShift( scriptLexeme &rightOp )
{
	DWORD	shiftVal = this->getShiftValue( rightOp );
	
	switch( this->constType )
	{
	case LCT_BOOL:
		if(shiftVal > 0)
			this->boolValue = 0;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue <<= shiftVal;
		else
			this->longValue <<= shiftVal;
		break;
	case LCT_SYMBOL:
		this->byteValue <<= shiftVal;
		break;
	}	
}

void scriptLexeme::operatorBitAnd( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue &= rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue &= rightOp.dwordValue;
		else
			this->longValue &= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue &= rightOp.byteValue;
		break;
	}	
}

void scriptLexeme::operatorBitOr( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue |= rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue |= rightOp.dwordValue;
		else
			this->longValue |= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue |= rightOp.byteValue;
		break;
	}	
}

void scriptLexeme::operatorBitXor( scriptLexeme &rightOp )
{
	switch( this->constType )
	{
	case LCT_BOOL:
		this->boolValue ^= rightOp.boolValue;
		break;
	case LCT_BINARY:
	case LCT_OCTAL:
	case LCT_DECIMAL:
	case LCT_HEX:
		if( this->isUnsigned )
			this->dwordValue ^= rightOp.dwordValue;
		else
			this->longValue ^= rightOp.longValue;
		break;
	case LCT_SYMBOL:
		this->byteValue ^= rightOp.byteValue;
		break;
	}	
}

bool scriptLexeme::isId( shString &id ) { 

	if( this->type != LT_ID ) return false;
	if( this->name != id ) return false;
		
	return true;
}

int	getLexemeIndex( scriptLexeme *lexArray, int arraySize, scriptLexeme &lex )
{
	
	int		index;

	for( index = 0; index < arraySize; index ++ )
	{
		if( lexArray[ index ].isEqual( lex ) )
			return index;
	}
	
	return -1;
}

float scriptLexeme::const2Float() { 
	
	if(this->type != LT_CONST) return FLT_MAX;
	if(this->isNumericConst()) return (float)this->longValue;
	if(this->isFloatConst()) return this->floatValue;
	if(this->isDoubleConst()) return (float)this->doubleValue;
	return FLT_MAX;
}

int scriptLexeme::const2Int() { 
	
	if(this->type != LT_CONST) return 0;
	if(this->isNumericConst()) return this->longValue;
	if(this->isFloatConst()) return (int)this->floatValue;
	if(this->isDoubleConst()) return (int)this->doubleValue;
	return 0;
}
