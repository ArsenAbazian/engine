#include "lexeme.h"
#include "syntparser2.h"
#include "moduleDesc.h"

scriptSyntaxParser2::scriptSyntaxParser2() {

	// изначально ошибок нет
	this->bError			= false;
	
	clearErrorsList( this->errorList );

	// файл не открыт
	this->fpLog				= NULL;
	
	// модуля пока нет
	this->module			= NULL;

	// параметров тоже пока нет
	this->scaner			= NULL;
	
	this->lexemeList		= NULL;
	this->idsList			= NULL;
	this->stringList		= NULL;
	this->fileList			= NULL;
	
	this->skipSymantic		= false;

	// стек области имен
	this->scopeList.clear();
}

bool scriptSyntaxParser2::initializeLog( shString fileName ) {

	// закрываем предыдущий файл
	this->closeLog();

	// запоминаем имя
	this->logFileName = fileName;

	// открываем файл
	this->fpLog = _tfopen( this->logFileName.getCString(), TEXT( "wt" ) );

	if( this->fpLog == NULL )
		return false;

	return true;
}

void scriptSyntaxParser2::closeLog() {
	
	// закрываем файл
	if( this->fpLog != NULL )
		fclose( this->fpLog );

	this->fpLog = NULL;

	// очищаем имя
	this->logFileName.clear();
	
	return ;
}

void scriptSyntaxParser2::clear() {

	// изначально ошибок нет
	this->bError			= false;
	
	clearErrorsList( this->errorList );

	// закрыть файл если есть
	this->closeLog();

	// модуля пока нет
	this->module			= NULL;

	// стек области имен
	this->scopeList.clear();
	
	return ;
}

bool	scriptSyntaxParser2::getVariableInScope( scriptScopeDesc &scope, shString idName, int *objIndex ) {
	
	return false;

	//std::list< scriptObjectDesc* >::iterator	soi;
	//scriptObjectDesc							*obj;
	//int											objInd;
	//scriptLexeme								*lex;

	//for( soi = this->module->scope.objectList.begin(), objInd = 0; soi = this->module->scope.objectList.end(); soi ++, objInd ++ )
	//{
	//	// берем переменную
	//	obj = *soi;

	//	// если у него есть имя
	//	if( obj->lexeme.name.getLength() != 0 )
	//	{
	//		if( obj->lexeme.name == idName )
	//		{
	//			*objIndex = objInd;
	//			return true;
	//		}
	//	}
	//	// если имени нет
	//	else
	//	{
	//		// получаем лексему по индексу
	//		lex = getLexFromList( this->strngList, obj->lexeme.index );
	//		if( lex != NULL && lex->name == idName )
	//		{
	//			*objIndex = objInd;
	//			return true;
	//		}
	//	}
	//}
	//
	//return false;
}

bool	scriptSyntaxParser2::getVariableInScope( scriptScopeDesc &scope, int idIndex, int *objIndex ) {
	
	return false;
	
	//std::list< scriptObjectDesc* >::iterator	soi;
	//scriptObjectDesc							*obj;
	//int											objInd;
	//scriptLexeme								*lex;

	//for( soi = this->module->scope.objectList.begin(), objInd = 0; soi = this->module->scope.objectList.end(); soi ++, objInd ++ )
	//{
	//	// берем переменную
	//	obj = *soi;

	//	// получаем лексему по индексу
	//	if( idIndex == obj->lexeme.index )
	//	{
	//		*objIndex = objInd;
	//		return true;
	//	}
	//}
	//
	//return false;
}

scriptRetCode scriptSyntaxParser2::parse( scriptScaner &scn ) {

	// запоминаем параметры
	this->scaner		= &scn;
	
	this->lexemeList	= & this->scaner->getLexemesList();
	this->idsList		= & this->scaner->getIdsList();
	this->stringList	= & this->scaner->getStringsList();
	this->fileList		= & this->scaner->getFilesList();
			
	currentLexeme		= lexemeList->begin();
	
	
	// инициализируем по умолчанию внутренние переменные программы
	
	// инициализируем имя области видимости
	// заносим стек как глобальную область видимости модуля
	//this->module->scope.name = TEXT("");
	//this->scopeList.push_back( &this->module->scope );
	
	// проверяем главное правило - programmModule
	this->programmModule( );
		
	return SRV_SUCCESS;
}

ruleResult	scriptSyntaxParser2::programmModule( ) {

	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isParser( const lexParserType &pt ) {
	
	lexParserType	pt2 = pt;
	return	this->isParser( pt2 );
}

ruleResult	scriptSyntaxParser2::isParser( lexParserType &pt ) {

	if( this->isEnd() ) 
		return RR_WRONG;
	
	if( (*this->currentLexeme)->type != LT_PARSER || (*this->currentLexeme)->parser != pt )
		return RR_WRONG;

	this->currentLexeme ++;
	
	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isParser( int pCount, lexParserType *pt ) {

	int			alt;
	ruleResult	rr;

	for( alt = 0; alt < pCount; alt ++ )
	{
		rr = this->isParser( pt[ alt ] );
		if( rr == RR_FALSE )
			return RR_FALSE;
		
		if( rr == RR_TRUE )
			break ;
	} 

	return rr;
}

ruleResult	scriptSyntaxParser2::isOneOfRules( int ruleCount, rulePointer *rulesArray ) {
	
	int			alt;
	ruleResult	rr;

	for( alt = 0; alt < ruleCount; alt ++ )
	{
		rr = (this->*rulesArray[ alt ])() ;
		if( rr == RR_FALSE )
			return RR_FALSE;
		
		if( rr == RR_TRUE )
			break ;
	} 

	return rr;
}

ruleResult	scriptSyntaxParser2::isBuildInTypeName( ) {
	
	if( this->isEnd() )
		return RR_WRONG;
	
	scriptLexeme *lexeme = *this->currentLexeme;
	
	if( lexeme->type != LT_RWORD )
		return RR_WRONG;

	// смотрим что за зарезервированное слово
	switch( lexeme->rwordType )
	{
	case LRW_VOID:
		this->typeDesc.objectType = ST_VOID;
		break;
	case LRW_BOOL:
		this->typeDesc.objectType = ST_BOOL;
		break;
	case LRW_CHAR:
		this->typeDesc.objectType = ST_CHAR;
		break;
	case LRW_SHORT:
		this->typeDesc.objectType = ST_SHORT;
		break;
	case LRW_INT:
		this->typeDesc.objectType = ST_INT;
		break;
	case LRW_FLOAT:
		this->typeDesc.objectType = ST_FLOAT;
		break;
	case LRW_STRING:
		this->typeDesc.objectType = ST_STRING;
		break;
	case LRW_COM_OBJECT:
		this->typeDesc.objectType = ST_COM_OBJECT;
		break;
	default:
		return RR_WRONG;
		break;
	}
	
	this->currentLexeme ++;

	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isId( ) {
	
	if( this->isEnd() )
		return RR_WRONG;
	
	if( (*this->currentLexeme)->type != LT_ID )
		return RR_WRONG;

	this->currentLexeme ++;

	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isConstValue( ) {
	
	if( this->isEnd() )
		return RR_WRONG;
	
	if( (*this->currentLexeme)->type != LT_CONST )
		return RR_WRONG;

	this->currentLexeme ++;

	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isRword( lexRWordType &rw ) {
	
	if ( this->isEnd() )
		return RR_WRONG;

	if( (*this->currentLexeme)->type != LT_RWORD || (*this->currentLexeme)->rwordType != rw )
		return RR_WRONG;

	this->currentLexeme ++;
        
	return RR_TRUE;
}

ruleResult	scriptSyntaxParser2::isRword( const lexRWordType &rw ) {
	
	lexRWordType rw2 = rw;
	return this->isRword( rw2 );
}