#include "syntparser.h"

int	getTriadeIndex( sgpTriade *triadeList, int triadeCount, int s1, int s2, int s3 )
{

	int	index;

	for( index = 0; index < triadeCount; index ++ )
	{
		if( s1 == triadeList[ index ].s1 && 
			s2 == triadeList[ index ].s2 && 
			s3 == triadeList[ index ].s3 )
			return index;
	}

	// если не нашли тройку
	return -1;
}

TCHAR	*relationCode2Text( relationCode relCode )
{

	switch( relCode )
	{
	case RELCODE_NONE:
		return TEXT( "o" );
		break;
	case RELCODE_EQUAL:
		return TEXT( "=" );
		break;
	case RELCODE_GREATER:
		return TEXT( ">" );
		break;
	case RELCODE_LESS:
		return TEXT( "<" );
		break;
	case RELCODE_GREATEREQUAL:
		return TEXT( ">=" );
		break;
	case RELCODE_LESSEQUAL:
		return TEXT( "<=" );
		break;
	case RELCODE_GREATERLESS:
		return TEXT( "><" );
		break;
	case RELCODE_LESSGREATER:
		return TEXT( "<>" );
		break;
	case RELCODE_LESSGREATEREQUAL:
		return TEXT( "<>=" );
		break;
	case RELCODE_GREATERLESSEQUAL:
		return TEXT( "><=" );
		break;
	}

	return TEXT( "o" );
}

scriptSyntaxParser::scriptSyntaxParser( )
{
	// очищаем ресурсы и 
	// устанавливаем все по умолчанию
	clearErrorsList( this->errorList );
	this->bError			= false;
	
	this->matrixSize		= 0;
	this->matrix			= NULL;
	
	this->lexemeCount		= 0;
	this->lexemeList		= NULL;

	this->ruleCount			= 0;
	this->ruleLeftPart		= NULL;
	this->rightPartSize		= NULL;
	this->ruleRightPart		= NULL;

	this->leftTriadeCount	= 0;
	this->leftTriadeList	= NULL;

	this->rightTriadeCount	= 0;
	this->rightTriadeList	= NULL;

	this->fpLog				= NULL;
					
}

void scriptSyntaxParser::clear()
{
	
	int						index;
	
	// удаляем матрицу
	if( this->matrix != NULL )
	{
		delete[] this->matrix;
		this->matrix	= NULL;
	}
	
	this->matrixSize		= 0;
	
	// удаляем массив лексем
	if( this->lexemeList != NULL )
	{
		delete[] this->lexemeList;
		this->lexemeList = NULL;
	}
	
	this->lexemeCount		= 0;

	// удаляем левые части правила
	if( this->ruleLeftPart != NULL )
	{
		delete[] this->ruleLeftPart;
		this->ruleLeftPart = NULL;
	}

	// удаляем правые части правила
	if( this->ruleRightPart != NULL )
	{
		for( index = 0; index < this->ruleCount; index ++ )
		{
			if( this->ruleRightPart[index] != NULL )
				delete[] this->ruleRightPart[index];
		}
		delete[] this->ruleRightPart;
		this->ruleRightPart = NULL;
	}

	// удаляем размер правых частей правила
	if( this->rightPartSize != NULL )
	{
		delete[] this->rightPartSize;
		this->rightPartSize = NULL;
	}

	this->ruleCount			= 0;
	
	// удаляем массив левых троек
	if( this->leftTriadeList != NULL )
	{
		delete[] this->leftTriadeList;
		this->leftTriadeList = NULL;
	}

	this->leftTriadeCount	= 0;
	
	// удаляем массив правых троек
	if( this->rightTriadeList != NULL )
	{
		delete[] this->rightTriadeList;
		this->rightTriadeList = NULL;
	}
	
	this->rightTriadeCount = 0 ;

	// закрываем файл отладки
	if( this->fpLog )
	{
		fclose( fpLog );
		fpLog	= NULL;
	}
	
	return ;
}

scriptRetCode scriptSyntaxParser::initLog( shString fileName )
{
	
	// закрываем старый файл отчета
	if( this->fpLog != NULL )
	{
		fclose( fpLog );
		fpLog	= NULL;
	}
	
	// пытаемся инициализировать
	this->logFileName = fileName;

	_tfopen_s( &this->fpLog, this->logFileName.getCString(), TEXT( "wt" ) );
	if( this->fpLog == NULL )
		return SRV_FAIL;

	return SRV_SUCCESS;
}

void scriptSyntaxParser::printStack( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli, relationCode relCode, std::list< scriptLexeme* > scList )
{

	std::list< scriptLexeme* >::iterator	sliStack;
	
	// если файл не открыт - игнорировать
	if( this->fpLog == NULL )
		return ;
	
	// печатаем начало стека
	_ftprintf( this->fpLog, TEXT( "forward __LEXEME_STACK_BEGIN  " ) );

	for( sliStack = this->lexemeStack.begin(); sliStack != this->lexemeStack.end(); sliStack ++ ) 
		_ftprintf( this->fpLog, TEXT( "%s  " ), (*sliStack)->getLexemeDesc( scList ).getCString() ); 
	
	// теперь печатаем отношение между лексемами 
	_ftprintf( this->fpLog, relationCode2Text( relCode ) );
	// печатаем последнюю лексему
	if( sli == lexList.end() )
		_ftprintf( this->fpLog, TEXT( "  __LEXEME_LIST_END\n" ) );
	else 
		_ftprintf( this->fpLog, TEXT( "  %s\n" ), (*sli)->getLexemeDesc( scList ).getCString() );

	fflush( this->fpLog );

	return ;
}

void scriptSyntaxParser::printStack( std::list< scriptLexeme* >::iterator &sli1, std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli2, relationCode relCode1, relationCode relCode2, std::list< scriptLexeme* > scList )
{

	std::list< scriptLexeme* >::iterator	sliPrev;
	std::list< scriptLexeme* >::iterator	sliStack;

	// если файл не открыт игнорировать
	if( this->fpLog == NULL )
		return ;
	
	// сначала печатаем ту лексему, что стоит за 
	if( sli1 == this->lexemeStack.begin() )
		_ftprintf( this->fpLog, TEXT( "back __LEXEME_STACK_BEGIN  " ) );
	else
	{
		sliPrev = sli1;
		sliPrev --;

		_ftprintf( this->fpLog, TEXT( "back %s  " ), (*sliPrev)->getLexemeDesc( scList ).getCString() );
	}

	// теперь печатаем отношение между лексемами 
	_ftprintf( this->fpLog, relationCode2Text( relCode1 ) );
	_ftprintf( this->fpLog, TEXT( "  " ) );

	// печатаем список правил
	for( sliStack = sli1; sliStack != this->lexemeStack.end(); sliStack ++ ) 
		_ftprintf( this->fpLog, TEXT( "%s  " ), (*sliStack)->getLexemeDesc( scList ).getCString(  ) ); 


	// теперь печатаем отношение между лексемами 
	_ftprintf( this->fpLog, relationCode2Text( relCode2 ) );
	
	// печатаем последнюю лексему
	if( sli2 == lexList.end() )
		_ftprintf( this->fpLog, TEXT( "  __LEXEME_LIST_END\n" ) );
	else 
		_ftprintf( this->fpLog, TEXT( "  %s\n" ), (*sli2)->getLexemeDesc( scList ).getCString() );

	fflush( this->fpLog );
	
	return ;
}

void scriptSyntaxParser::printFoundRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list< scriptLexeme* >::iterator &sliEnd, std::list< int >::iterator &sliBeginIndex, std::list<int>::iterator &sliEndIndex, int ruleIndex, std::list< scriptLexeme* > scList )
{

	std::list< scriptLexeme* >::iterator	sliStack;
	std::list< int >::iterator				sliIndexStack;
				
	int						rLexIndex;				

	// если файл не открыт игнорировать
	if( this->fpLog == NULL )
		return ;
								   
	_ftprintf( this->fpLog, TEXT( "------------------------------------found rule----------------------------------\n" ) );
	
	sliStack		= sliBegin;
		
	_ftprintf( this->fpLog, TEXT( "stack indicies\n%d ::= " ), this->ruleLeftPart[ ruleIndex ] );
	for( sliIndexStack	= sliBeginIndex; ; sliIndexStack ++ )
	{
		_ftprintf( this->fpLog, TEXT( "%d  " ), *sliIndexStack );
		if( sliIndexStack == sliEndIndex )
			break;
	}
	_ftprintf( this->fpLog, TEXT( "\n" ) );

	_ftprintf( this->fpLog, TEXT( "rule indicies\n%d ::= " ), this->ruleLeftPart[ ruleIndex ] );
	for( rLexIndex = 0; rLexIndex < this->rightPartSize[ ruleIndex ]; rLexIndex ++ )
		_ftprintf( this->fpLog, TEXT( "%d  " ), this->ruleRightPart[ ruleIndex ][ rLexIndex ] );
	_ftprintf( this->fpLog, TEXT( "\n" ) );
	
	_ftprintf( this->fpLog, TEXT( "stack lexemes\n%s ::= " ), this->lexemeList[ this->ruleLeftPart[ ruleIndex ] ].getLexemeDesc().getCString() );
	for( sliStack	= sliBegin; ; sliStack ++ )
	{
		_ftprintf( this->fpLog, TEXT( "%s  " ), (*sliStack)->getLexemeDesc( scList ).getCString() );
		if( sliStack == sliEnd )
			break;
	}
	_ftprintf( this->fpLog, TEXT( "\n" ) );
	
	_ftprintf( this->fpLog, TEXT( "rule lexemes\n%s ::= " ), this->lexemeList[ this->ruleLeftPart[ ruleIndex ] ].getLexemeDesc().getCString() );
	for( rLexIndex = 0; rLexIndex < this->rightPartSize[ ruleIndex ]; rLexIndex ++ )
		_ftprintf( this->fpLog, TEXT( "%s  " ), this->lexemeList[ this->ruleRightPart[ ruleIndex ][ rLexIndex ] ].getLexemeDesc( scList ).getCString() );
	_ftprintf( this->fpLog, TEXT( "\n" ) );
	
	_ftprintf( this->fpLog, TEXT( "--------------------------------------------------------------------------------\n" ) );
	
	fflush( this->fpLog );

	return ;
}

int scriptSyntaxParser::getLexemeIndex( scriptLexeme &lex )
{
	
	int		lexemeIndex;
	
	// цкл по числу лексем
	for( lexemeIndex = 0; lexemeIndex < this->lexemeCount; lexemeIndex ++ )
	{
		if( this->lexemeList[ lexemeIndex ].isEqualTypeEx( lex ) )
			return lexemeIndex;
	}
	
	return -1;
}

int	scriptSyntaxParser::getTriadeIndex( int s1, int s2, int s3, bool left )
{

	sgpTriade	*triadeList;
	int			triadeCount;

	if( left )
	{
		triadeList		= this->leftTriadeList;
		triadeCount		= this->leftTriadeCount;
	}
	else
	{
		triadeList		= this->rightTriadeList;
		triadeCount		= this->rightTriadeCount;
	}

	return ::getTriadeIndex( triadeList, triadeCount, s1, s2, s3 );
}

int scriptSyntaxParser::getTriadeIndex( scriptLexeme *lex1, scriptLexeme *lex2, scriptLexeme *lex3, bool left )
{
	
	int	s1, s2, s3;

	// если s1 - пустая лексема то -1
	if( lex1 == NULL )
		s1 = TRIADE_LEXEME_EMPTY;
	else
	{
		s1 = this->getLexemeIndex( *lex1 );
		if( s1 == -1 ) // чтобы ненайденная лексема не совпала с пустой
			s1 = -2;
	}
	
	// если s2 - пустая лексема то -1
	if( lex2 == NULL )
		s2 = TRIADE_LEXEME_EMPTY;
	else
	{
		s2 = this->getLexemeIndex( *lex2 );
		if( s2 == -1 ) // чтобы ненайденная лексема не совпала с пустой
			s2 = -2;
	}

	// если s3 - пустая лексема то -1
	if( lex3 == NULL )
		s3 = TRIADE_LEXEME_EMPTY;
	else
	{
		s3 = this->getLexemeIndex( *lex3 );
		if( s3 == -1 ) // чтобы ненайденная лексема не совпала с пустой
			s3 = -2;
	}

	return this->getTriadeIndex( s1, s2, s3, left );
}

scriptRetCode scriptSyntaxParser::applyRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int ruleIndex )
{
	
	// мы знаем индекс правила, на которое должны заменить
	// так что после семантической проверки правила 
	// и ее выплнения мы удаляем лексемы и ставим лексему из левой части
	
	
	// собственно очистка правила
	std::list< scriptLexeme* >::iterator		sli;
	std::list< int >::iterator					sliIndex;

	// создаем копию нетерминала левой части найденного правила
	scriptLexeme *left = new scriptLexeme( this->lexemeList[ this->ruleLeftPart[ ruleIndex ] ] );
	
	// копируем позицию в файле
	left->fileIndex	= (*sliBegin)->fileIndex;
	left->filePos	= (*sliBegin)->filePos;
	left->line		= (*sliBegin)->line;
	left->col		= (*sliBegin)->col;

	for( sli = sliBegin; ; sli ++ )
	{
		// если следующая лексема нетерминал, 
		// то ее надо обязательно удалить, поскольку память для нее выделена не сканером, а нами
		// остальное мы не трогаем
		if( (*sli)->type == LT_NTERMINAL )
			delete *sli;
	
		// если дошли до конца правила
		if( sli == sliEnd )
			break;

	}

	// увеличиваем границы
	sliEnd ++;
	sliEndIndex ++;
	
	// очищаем стеки
	this->lexemeStack.erase( sliBegin, sliEnd );
	this->indexStack.erase( sliBeginIndex, sliEndIndex );
		
	// добавляем эту лексему и ее индекс в стек лексем и индексов	
	this->lexemeStack.push_back( left );
	this->indexStack.push_back( this->ruleLeftPart[ ruleIndex ] );

	return SRV_SUCCESS;
}

scriptRetCode scriptSyntaxParser::findRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int &ruleIndex )
{
	
	std::list< scriptLexeme* >::iterator	sli;
	std::list< int >::iterator				sliIndex;
	
	int										lexemeIndex;	
	int										thisRuleSize;

	// посчитаем размер правила
	for( sli = sliBegin, thisRuleSize = 1; sli != sliEnd; sli ++, thisRuleSize ++ );
	
	
	// пускаем цикл по числу правил
	for( ruleIndex = 0; ruleIndex < this->ruleCount; ruleIndex ++ )
	{
		
		// смотрим - совпадают ли правила по размеру
		if( this->rightPartSize[ ruleIndex ] != thisRuleSize )
			continue;
		
		// смотрим по содержимому
		// пускаем цикл до тех пор, пока 
		for( lexemeIndex = 0, sli = sliBegin, sliIndex = sliBeginIndex; lexemeIndex < thisRuleSize; lexemeIndex ++, sliIndex ++, sli ++ )
		{
			// если не совпало то все
			if( (*sliIndex) != this->ruleRightPart[ruleIndex][lexemeIndex] )
				break ;
		}
			
		// если правило нашли
		if( lexemeIndex == this->rightPartSize[ ruleIndex ] )
		{
			// применяем правило
			// this->applyRule( );	
			// нет просто возвращаем результат

			// нормальный возврат
			return SRV_SUCCESS;
		}
	}

	// если мы оказались здесь, то мы не нашли правило необходимо исправить ошибку
	
	return SRV_FAIL;

}

// добавить ошибку в список ошибок
scriptError& scriptSyntaxParser::addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col )
{
	
	scriptError	*scrError = new scriptError;

	scrError->fileIndex	= fileIndex;
	scrError->type		= type;
	scrError->code		= code;
	scrError->text		= text;
	scrError->line		= line;
	scrError->col		= col;

	this->errorList.push_back( scrError );
	
	return *scrError;	
	
}

scriptError& scriptSyntaxParser::addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex )
{
	return this->addError( type, code, text, lex.fileIndex, lex.line, lex.col );
}

scriptError& scriptSyntaxParser::addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex )
{
	return this->addError( type, code, lex.getLexemeDesc().getCString(), lex.fileIndex, lex.line, lex.col );
}

scriptRetCode scriptSyntaxParser::parseBack( scriptScaner &scaner, std::list< scriptLexeme *>::iterator &sliGreater )
{

	// указатели на лексемы
	std::list< scriptLexeme* >::iterator		sliCurrent, sliPrev, sliNext, sliEnd;
	// и их индексы
	std::list< int >::iterator					sliCurrentIndex, sliPrevIndex, sliNextIndex, sliEndIndex;

	// индексы лексем
	int					currentLexemeIndex, prevLexemeIndex, nextLexemeIndex;
	
	// индекс правила
	int					ruleIndex;
	
	// код отношения
	relationCode		relCode;
	
	// получаем вершину стека
	sliCurrent = this->lexemeStack.end();
	// и последнюю лексему
	sliCurrent --;

	// а также ее индекс
	sliCurrentIndex = this->indexStack.end();
	sliCurrentIndex --;

	// запоминаем последнюю лексему
	sliEnd = sliCurrent;
	// и ее индекс
	sliEndIndex = sliCurrentIndex;

	// считываем, пока не дойдем до конца стека
	while( 1 )
	{
		
		// если эта лексема первая, то уже установлено отношение <
		if( sliCurrent == this->lexemeStack.begin() )
		{
			relCode		= RELCODE_LESS;
			this->printStack( sliCurrent, scaner.getLexemesList(), sliGreater, relCode, RELCODE_GREATER, scaner.getIdsList() );  
		}
		// иначе получаем предыдущую лексему
		// и пытаемся выяснить отношения между предыдущей и текущей
		else
		{
					
			// получаем индекс текущей лексемы
			currentLexemeIndex = *sliCurrentIndex;
			
			// получаем предыдущую лексему
			sliPrev = sliCurrent;
			sliPrev --;

			sliPrevIndex = sliCurrentIndex;
			sliPrevIndex --;
			
			//и ее индекс 
			prevLexemeIndex = *sliPrevIndex;
			
			// теперь получаем код отношения
			relCode = (relationCode)this->matrix[ prevLexemeIndex ][ currentLexemeIndex ];

			this->printStack( sliCurrent, scaner.getLexemesList(), sliGreater, relCode, RELCODE_GREATER, scaner.getIdsList() );  
			
			// делим отношение на < и на все остальные 
			if( relCode == RELCODE_NONE )
			{
				_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliPrev)->getLexemeDesc().getCString() );
				_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s" ), (*sliCurrent)->getLexemeDesc().getCString() );
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_NO_RELATION_FOUND, this->tempBuffer, *(*sliPrev) ); 	
				return SRV_FAIL;
			}
			else if( relCode == RELCODE_GREATER || relCode == RELCODE_GREATEREQUAL )
				relCode = RELCODE_EQUAL;
			// если найдено отношени <= то надо разрешить конфликт
			else if( relCode == RELCODE_LESSEQUAL )
			{
				// получаем следующую лексему
				sliNext = sliCurrent;
				sliNext ++;

				// и ее индекс
				sliNextIndex = sliCurrentIndex;
				sliNextIndex ++;
				
				// если текущая лексема находится на вершине стека
				if( sliNext == this->lexemeStack.end() )
					nextLexemeIndex = TRIADE_LEXEME_EMPTY;
				else
					// получаем ее индекс
					nextLexemeIndex = *sliNextIndex;
							
				// теперь пытаемся найти левую тройку
				if( this->getTriadeIndex( prevLexemeIndex, currentLexemeIndex, nextLexemeIndex, true ) != -1 )
					relCode = RELCODE_EQUAL;
				// иначе конфликт разрешается в сторону <
				else
					relCode = RELCODE_LESS;
			}
		}
		
		// теперь проводим проверку 
		if( relCode == RELCODE_EQUAL )
		{
			// идем дальше
			sliCurrent --;
			sliCurrentIndex --;
		}
		else if( relCode == RELCODE_LESS )
		{
			// мы нашли вроде начало и конец правила
			// теперь ищем индекс правила
			// если ничего не получится, значит мы возвратим ошибку
			// поскольку исправлять ошибки мы пока не будем
			if( this->findRule( sliCurrent, sliCurrentIndex, sliEnd, sliEndIndex, ruleIndex ) == SRV_SUCCESS )
			{
				
				// печатаем найденное правило
				this->printFoundRule( sliCurrent, sliEnd, sliCurrentIndex, sliEndIndex, ruleIndex, scaner.getIdsList() );

				if( this->applyRule( sliCurrent, sliCurrentIndex, sliEnd, sliEndIndex, ruleIndex ) == SRV_SUCCESS )
				{
					
					// if( this->checkSymantic() )
					return SRV_SUCCESS;
				}
				else 
					return SRV_FAIL;
			}
			else
				return SRV_FAIL;
		}
		// недопустимое отношение
		else 
		{
			// найдено недопустимое отношение
			_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliPrev)->getLexemeDesc().getCString() );
			_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s ( relation <> or <>= )" ), (*sliCurrent)->getLexemeDesc().getCString() );
				
			this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRESOLVED_RELATION, this->tempBuffer, *(*sliPrev) ); 	
				
			return SRV_FAIL;	
		}

	} // бесконечный цикл 
	
	return SRV_SUCCESS;
}

scriptRetCode scriptSyntaxParser::parse( scriptScaner &scaner )
{

	std::list< scriptLexeme* >::iterator		sliCurrent, sliStackTop, sliStackTop_1;		
	std::list< int >::iterator					sliStackTopIndex, sliStackTopIndex_1;
	// для повышения скорости
	std::list< scriptLexeme* >					&inputLexemeList = scaner.getLexemesList();

	// индексы лексем
	int		stackTopIndex, stackTopIndex_1, currentLexemeIndex;
	
	// код отношения
	relationCode	relCode;	
	
	// код возврата из функций
	scriptRetCode	retCode;

	// получаем начало списка
	sliCurrent = inputLexemeList.begin();
	
	// в бесконечном цикле считываем цепочку до тех пор, пока она не закончится
	while( 1 )
	{
		
		// если стек лексем пуст - просто получить лексему и отправить ее в стек
		if( this->lexemeStack.size() == 0 )
		{
			this->lexemeStack.push_back( *sliCurrent );
			
			currentLexemeIndex = this->getLexemeIndex( *(*sliCurrent) );
			if( currentLexemeIndex < 0 || currentLexemeIndex >= this->lexemeCount )
			{
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliCurrent) ); 	
				return SRV_FAIL;
			} // если лексема на вершине стека не распознана 

			this->indexStack.push_back( currentLexemeIndex );
			
			// переходим к следующей лексеме
			sliCurrent ++;
			continue ; 
		}
		// стек не пуст надо посмотреть
		// какое отношение между вершиной стека и следующей лексемой
		else 
		{
			// получаем указатель на вершину стека
			sliStackTop = this->lexemeStack.end();
			sliStackTop --;
			
			sliStackTopIndex = this->indexStack.end();
			sliStackTopIndex --;

			// а также индекс этой лексемы в списке лексем анализатора
			stackTopIndex = *sliStackTopIndex;	
			
			// если достигнут конец списка, то отношение заведомо >
			if( sliCurrent == inputLexemeList.end() )
			{
				relCode = RELCODE_GREATER;
			}
			// если нет, то надо получить следующую лексему и найти отношение
			else
			{
			
				// получаем индекс текущей лексемы
				currentLexemeIndex = this->getLexemeIndex( *(*sliCurrent) );

				if( currentLexemeIndex < 0 || currentLexemeIndex >= this->lexemeCount )
				{
					this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliCurrent) ); 	
					return SRV_FAIL;
				} // если лексема на вершине стека не распознана 

				relCode = (relationCode)this->matrix[ stackTopIndex ][ currentLexemeIndex ];
			}
				
			this->printStack( inputLexemeList, sliCurrent, relCode, scaner.getIdsList() );
			
			// теперь смотрим отношение
			// пытаемся разделить отношения на > и =
			if( relCode == RELCODE_NONE )
			{
				_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliStackTop)->getLexemeDesc().getCString() );
				_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s" ), (*sliCurrent)->getLexemeDesc().getCString() );
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_NO_RELATION_FOUND, this->tempBuffer, *(*sliStackTop) ); 	
				return SRV_FAIL;
			}
			else if( relCode == RELCODE_LESS || 
					 relCode == RELCODE_LESSEQUAL )
			{
				relCode = RELCODE_EQUAL;
			}
			else if( relCode == RELCODE_GREATEREQUAL )
			{
				// конфликт отношений - необходимо решить его с помошью
				// таблицы левых и правых троек
				
				// получаем лексему которая лежит под лексемой вверху стека
				// если ее нет, то ее индекс равен TRIADE_LEXEME_EMPTY
				if( sliStackTop == this->lexemeStack.begin() )
					stackTopIndex_1 = TRIADE_LEXEME_EMPTY;
				// иначе пытаемся получить ее индекс
				else 
				{
					sliStackTop_1 = sliStackTop;
					sliStackTop_1 --;

					sliStackTopIndex_1 = sliStackTopIndex; 
					sliStackTopIndex_1 --;

					// определяем индекс
					stackTopIndex_1 = *sliStackTopIndex_1;

					// смотрим а нашли ли мы индекс
					if( stackTopIndex_1 < 0 || stackTopIndex_1 >= this->lexemeCount )
					{
						this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliStackTop_1) ); 	
						return SRV_FAIL;
					} // если лексема перед лексемой на вершине стека не распознана 
				}
			
				// распознали индекс - теперь пытаемся найти правую тройку
				if( this->getTriadeIndex( stackTopIndex_1, stackTopIndex, currentLexemeIndex, false ) != -1 )
					relCode = RELCODE_EQUAL;
				// иначе конфликт разрешается в сторону >
				else
					relCode = RELCODE_GREATER;	
			}
			
			// отношения разделии
			// теперь проверяем два отношения > и =
			
			// отношение =
			if( relCode == RELCODE_EQUAL )
			{
				// надо просто добавить лексему в стек лексем
				this->lexemeStack.push_back( *sliCurrent );
				// а также ее индекс
				this->indexStack.push_back( currentLexemeIndex );
				// переходим к следующей лексеме
				sliCurrent ++;
			}
			// отношение >
			else if( relCode == RELCODE_GREATER )
			{
				// разбираем в обратном порядке
				retCode = this->parseBack( scaner, sliCurrent );
				// если не все нормально разобрали
				if( retCode != SRV_SUCCESS )
					return retCode ;
			}
			else
			{
				// найдено недопустимое отношение
				_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliStackTop)->getLexemeDesc().getCString() );
				_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s ( relation <> or <>= )" ), (*sliCurrent)->getLexemeDesc().getCString() );
				
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRESOLVED_RELATION, this->tempBuffer, *(*sliStackTop) ); 	
				
				return SRV_FAIL;
			}

		} // в стеке есть лексемы

	} // while( 1 )
	
	return SRV_SUCCESS;
}
