#include "scrmacro.h"

scriptMacros::scriptMacros()
{
	
	this->fileIndex = -1;
	this->line		= -1;
	this->col		= -1;
	this->called	= false;
	this->isSystem	= false;

}

scriptMacros::~scriptMacros()
{
	this->clear();
}

void scriptMacros::clear()
{
	
	// очищаем имя
	name.clear();
		
	// положение макроса
	this->fileIndex = -1;
	this->line		= -1;
	this->col		= -1;
	
	// очищаем список параметров
	clearLexList( this->params );

	// очищаем список тела макроса
	clearLexList( this->body ); 
	
	// очищаем список входных параметров макроса
	this->input.clear();
	
	this->called	= false;
	this->isSystem	= false;
	
	return ;
}

scriptMacros& scriptMacros::copy( scriptMacros &sm )
{

	std::list< scriptLexeme* >::iterator		sli, sli2;
	std::list< macrosInputParam >::iterator		mipi;
	scriptLexeme								*lex;
	macrosInputParam							par;
	
	this->clear();
			
	this->name		= sm.name;
	this->fileIndex = sm.fileIndex;
	this->line		= sm.line;
	this->col		= sm.col;

	// копируем список параметров
	for( sli = sm.params.begin(); sli != sm.params.end(); sli ++ )
	{
		lex = new scriptLexeme( *(*sli) );
		this->params.push_back( lex );
	}
	
	// копируем тело макроса
	for( sli = sm.body.begin(); sli != sm.body.end(); sli ++ )
	{
		lex = new scriptLexeme( *(*sli) );
		this->body.push_back( lex );
	}

	// копируем входную строку макроса
	this->inputList.begin	= sm.inputList.begin;
	this->inputList.end		= sm.inputList.end;
	this->inputList.size	= sm.inputList.size;
	
	// копируем описание параметров 
	for( mipi = sm.input.begin(); mipi != sm.input.end(); mipi ++ )
		this->input.push_back( *mipi );
	
	this->called	= sm.called;
	this->isSystem	= sm.isSystem;

	return *this;
}

bool scriptMacros::isParam( scriptLexeme &lex, std::list< macrosInputParam >::iterator &mipi )
{

	std::list< scriptLexeme* >::iterator		sli;
	std::list< macrosInputParam >::iterator		mipi2;

	if( lex.type != LT_RWORD && lex.type != LT_ID )
		return false;

	if( this->params.size() == 0 )
		return false;

	for( sli = this->params.begin(), mipi2 = this->input.begin(); sli != this->params.end(); sli ++ )
	{
		// если зарезервированное слово совпало с параметром
		if( (*sli)->isEqual( lex ) )
		{
			mipi = mipi2;
			break;
		}
	
		// каждому параметру соответствует его входное значение,
		// если только пользователь его указал
		if( mipi2 != this->input.end() )
			mipi2 ++;
	}

	if( sli == this->params.end() )
		return false;

	return true;
}
