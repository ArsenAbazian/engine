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

	// ���� �� ����� ������
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
	// ������� ������� � 
	// ������������� ��� �� ���������
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
	
	// ������� �������
	if( this->matrix != NULL )
	{
		delete[] this->matrix;
		this->matrix	= NULL;
	}
	
	this->matrixSize		= 0;
	
	// ������� ������ ������
	if( this->lexemeList != NULL )
	{
		delete[] this->lexemeList;
		this->lexemeList = NULL;
	}
	
	this->lexemeCount		= 0;

	// ������� ����� ����� �������
	if( this->ruleLeftPart != NULL )
	{
		delete[] this->ruleLeftPart;
		this->ruleLeftPart = NULL;
	}

	// ������� ������ ����� �������
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

	// ������� ������ ������ ������ �������
	if( this->rightPartSize != NULL )
	{
		delete[] this->rightPartSize;
		this->rightPartSize = NULL;
	}

	this->ruleCount			= 0;
	
	// ������� ������ ����� �����
	if( this->leftTriadeList != NULL )
	{
		delete[] this->leftTriadeList;
		this->leftTriadeList = NULL;
	}

	this->leftTriadeCount	= 0;
	
	// ������� ������ ������ �����
	if( this->rightTriadeList != NULL )
	{
		delete[] this->rightTriadeList;
		this->rightTriadeList = NULL;
	}
	
	this->rightTriadeCount = 0 ;

	// ��������� ���� �������
	if( this->fpLog )
	{
		fclose( fpLog );
		fpLog	= NULL;
	}
	
	return ;
}

scriptRetCode scriptSyntaxParser::initLog( shString fileName )
{
	
	// ��������� ������ ���� ������
	if( this->fpLog != NULL )
	{
		fclose( fpLog );
		fpLog	= NULL;
	}
	
	// �������� ����������������
	this->logFileName = fileName;

	_tfopen_s( &this->fpLog, this->logFileName.getCString(), TEXT( "wt" ) );
	if( this->fpLog == NULL )
		return SRV_FAIL;

	return SRV_SUCCESS;
}

void scriptSyntaxParser::printStack( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli, relationCode relCode, std::list< scriptLexeme* > scList )
{

	std::list< scriptLexeme* >::iterator	sliStack;
	
	// ���� ���� �� ������ - ������������
	if( this->fpLog == NULL )
		return ;
	
	// �������� ������ �����
	_ftprintf( this->fpLog, TEXT( "forward __LEXEME_STACK_BEGIN  " ) );

	for( sliStack = this->lexemeStack.begin(); sliStack != this->lexemeStack.end(); sliStack ++ ) 
		_ftprintf( this->fpLog, TEXT( "%s  " ), (*sliStack)->getLexemeDesc( scList ).getCString() ); 
	
	// ������ �������� ��������� ����� ��������� 
	_ftprintf( this->fpLog, relationCode2Text( relCode ) );
	// �������� ��������� �������
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

	// ���� ���� �� ������ ������������
	if( this->fpLog == NULL )
		return ;
	
	// ������� �������� �� �������, ��� ����� �� 
	if( sli1 == this->lexemeStack.begin() )
		_ftprintf( this->fpLog, TEXT( "back __LEXEME_STACK_BEGIN  " ) );
	else
	{
		sliPrev = sli1;
		sliPrev --;

		_ftprintf( this->fpLog, TEXT( "back %s  " ), (*sliPrev)->getLexemeDesc( scList ).getCString() );
	}

	// ������ �������� ��������� ����� ��������� 
	_ftprintf( this->fpLog, relationCode2Text( relCode1 ) );
	_ftprintf( this->fpLog, TEXT( "  " ) );

	// �������� ������ ������
	for( sliStack = sli1; sliStack != this->lexemeStack.end(); sliStack ++ ) 
		_ftprintf( this->fpLog, TEXT( "%s  " ), (*sliStack)->getLexemeDesc( scList ).getCString(  ) ); 


	// ������ �������� ��������� ����� ��������� 
	_ftprintf( this->fpLog, relationCode2Text( relCode2 ) );
	
	// �������� ��������� �������
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

	// ���� ���� �� ������ ������������
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
	
	// ��� �� ����� ������
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

	// ���� s1 - ������ ������� �� -1
	if( lex1 == NULL )
		s1 = TRIADE_LEXEME_EMPTY;
	else
	{
		s1 = this->getLexemeIndex( *lex1 );
		if( s1 == -1 ) // ����� ����������� ������� �� ������� � ������
			s1 = -2;
	}
	
	// ���� s2 - ������ ������� �� -1
	if( lex2 == NULL )
		s2 = TRIADE_LEXEME_EMPTY;
	else
	{
		s2 = this->getLexemeIndex( *lex2 );
		if( s2 == -1 ) // ����� ����������� ������� �� ������� � ������
			s2 = -2;
	}

	// ���� s3 - ������ ������� �� -1
	if( lex3 == NULL )
		s3 = TRIADE_LEXEME_EMPTY;
	else
	{
		s3 = this->getLexemeIndex( *lex3 );
		if( s3 == -1 ) // ����� ����������� ������� �� ������� � ������
			s3 = -2;
	}

	return this->getTriadeIndex( s1, s2, s3, left );
}

scriptRetCode scriptSyntaxParser::applyRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int ruleIndex )
{
	
	// �� ����� ������ �������, �� ������� ������ ��������
	// ��� ��� ����� ������������� �������� ������� 
	// � �� ��������� �� ������� ������� � ������ ������� �� ����� �����
	
	
	// ���������� ������� �������
	std::list< scriptLexeme* >::iterator		sli;
	std::list< int >::iterator					sliIndex;

	// ������� ����� ����������� ����� ����� ���������� �������
	scriptLexeme *left = new scriptLexeme( this->lexemeList[ this->ruleLeftPart[ ruleIndex ] ] );
	
	// �������� ������� � �����
	left->fileIndex	= (*sliBegin)->fileIndex;
	left->filePos	= (*sliBegin)->filePos;
	left->line		= (*sliBegin)->line;
	left->col		= (*sliBegin)->col;

	for( sli = sliBegin; ; sli ++ )
	{
		// ���� ��������� ������� ����������, 
		// �� �� ���� ����������� �������, ��������� ������ ��� ��� �������� �� ��������, � ����
		// ��������� �� �� �������
		if( (*sli)->type == LT_NTERMINAL )
			delete *sli;
	
		// ���� ����� �� ����� �������
		if( sli == sliEnd )
			break;

	}

	// ����������� �������
	sliEnd ++;
	sliEndIndex ++;
	
	// ������� �����
	this->lexemeStack.erase( sliBegin, sliEnd );
	this->indexStack.erase( sliBeginIndex, sliEndIndex );
		
	// ��������� ��� ������� � �� ������ � ���� ������ � ��������	
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

	// ��������� ������ �������
	for( sli = sliBegin, thisRuleSize = 1; sli != sliEnd; sli ++, thisRuleSize ++ );
	
	
	// ������� ���� �� ����� ������
	for( ruleIndex = 0; ruleIndex < this->ruleCount; ruleIndex ++ )
	{
		
		// ������� - ��������� �� ������� �� �������
		if( this->rightPartSize[ ruleIndex ] != thisRuleSize )
			continue;
		
		// ������� �� �����������
		// ������� ���� �� ��� ���, ���� 
		for( lexemeIndex = 0, sli = sliBegin, sliIndex = sliBeginIndex; lexemeIndex < thisRuleSize; lexemeIndex ++, sliIndex ++, sli ++ )
		{
			// ���� �� ������� �� ���
			if( (*sliIndex) != this->ruleRightPart[ruleIndex][lexemeIndex] )
				break ;
		}
			
		// ���� ������� �����
		if( lexemeIndex == this->rightPartSize[ ruleIndex ] )
		{
			// ��������� �������
			// this->applyRule( );	
			// ��� ������ ���������� ���������

			// ���������� �������
			return SRV_SUCCESS;
		}
	}

	// ���� �� ��������� �����, �� �� �� ����� ������� ���������� ��������� ������
	
	return SRV_FAIL;

}

// �������� ������ � ������ ������
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

	// ��������� �� �������
	std::list< scriptLexeme* >::iterator		sliCurrent, sliPrev, sliNext, sliEnd;
	// � �� �������
	std::list< int >::iterator					sliCurrentIndex, sliPrevIndex, sliNextIndex, sliEndIndex;

	// ������� ������
	int					currentLexemeIndex, prevLexemeIndex, nextLexemeIndex;
	
	// ������ �������
	int					ruleIndex;
	
	// ��� ���������
	relationCode		relCode;
	
	// �������� ������� �����
	sliCurrent = this->lexemeStack.end();
	// � ��������� �������
	sliCurrent --;

	// � ����� �� ������
	sliCurrentIndex = this->indexStack.end();
	sliCurrentIndex --;

	// ���������� ��������� �������
	sliEnd = sliCurrent;
	// � �� ������
	sliEndIndex = sliCurrentIndex;

	// ���������, ���� �� ������ �� ����� �����
	while( 1 )
	{
		
		// ���� ��� ������� ������, �� ��� ����������� ��������� <
		if( sliCurrent == this->lexemeStack.begin() )
		{
			relCode		= RELCODE_LESS;
			this->printStack( sliCurrent, scaner.getLexemesList(), sliGreater, relCode, RELCODE_GREATER, scaner.getIdsList() );  
		}
		// ����� �������� ���������� �������
		// � �������� �������� ��������� ����� ���������� � �������
		else
		{
					
			// �������� ������ ������� �������
			currentLexemeIndex = *sliCurrentIndex;
			
			// �������� ���������� �������
			sliPrev = sliCurrent;
			sliPrev --;

			sliPrevIndex = sliCurrentIndex;
			sliPrevIndex --;
			
			//� �� ������ 
			prevLexemeIndex = *sliPrevIndex;
			
			// ������ �������� ��� ���������
			relCode = (relationCode)this->matrix[ prevLexemeIndex ][ currentLexemeIndex ];

			this->printStack( sliCurrent, scaner.getLexemesList(), sliGreater, relCode, RELCODE_GREATER, scaner.getIdsList() );  
			
			// ����� ��������� �� < � �� ��� ��������� 
			if( relCode == RELCODE_NONE )
			{
				_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliPrev)->getLexemeDesc().getCString() );
				_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s" ), (*sliCurrent)->getLexemeDesc().getCString() );
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_NO_RELATION_FOUND, this->tempBuffer, *(*sliPrev) ); 	
				return SRV_FAIL;
			}
			else if( relCode == RELCODE_GREATER || relCode == RELCODE_GREATEREQUAL )
				relCode = RELCODE_EQUAL;
			// ���� ������� �������� <= �� ���� ��������� ��������
			else if( relCode == RELCODE_LESSEQUAL )
			{
				// �������� ��������� �������
				sliNext = sliCurrent;
				sliNext ++;

				// � �� ������
				sliNextIndex = sliCurrentIndex;
				sliNextIndex ++;
				
				// ���� ������� ������� ��������� �� ������� �����
				if( sliNext == this->lexemeStack.end() )
					nextLexemeIndex = TRIADE_LEXEME_EMPTY;
				else
					// �������� �� ������
					nextLexemeIndex = *sliNextIndex;
							
				// ������ �������� ����� ����� ������
				if( this->getTriadeIndex( prevLexemeIndex, currentLexemeIndex, nextLexemeIndex, true ) != -1 )
					relCode = RELCODE_EQUAL;
				// ����� �������� ����������� � ������� <
				else
					relCode = RELCODE_LESS;
			}
		}
		
		// ������ �������� �������� 
		if( relCode == RELCODE_EQUAL )
		{
			// ���� ������
			sliCurrent --;
			sliCurrentIndex --;
		}
		else if( relCode == RELCODE_LESS )
		{
			// �� ����� ����� ������ � ����� �������
			// ������ ���� ������ �������
			// ���� ������ �� ���������, ������ �� ��������� ������
			// ��������� ���������� ������ �� ���� �� �����
			if( this->findRule( sliCurrent, sliCurrentIndex, sliEnd, sliEndIndex, ruleIndex ) == SRV_SUCCESS )
			{
				
				// �������� ��������� �������
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
		// ������������ ���������
		else 
		{
			// ������� ������������ ���������
			_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliPrev)->getLexemeDesc().getCString() );
			_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s ( relation <> or <>= )" ), (*sliCurrent)->getLexemeDesc().getCString() );
				
			this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRESOLVED_RELATION, this->tempBuffer, *(*sliPrev) ); 	
				
			return SRV_FAIL;	
		}

	} // ����������� ���� 
	
	return SRV_SUCCESS;
}

scriptRetCode scriptSyntaxParser::parse( scriptScaner &scaner )
{

	std::list< scriptLexeme* >::iterator		sliCurrent, sliStackTop, sliStackTop_1;		
	std::list< int >::iterator					sliStackTopIndex, sliStackTopIndex_1;
	// ��� ��������� ��������
	std::list< scriptLexeme* >					&inputLexemeList = scaner.getLexemesList();

	// ������� ������
	int		stackTopIndex, stackTopIndex_1, currentLexemeIndex;
	
	// ��� ���������
	relationCode	relCode;	
	
	// ��� �������� �� �������
	scriptRetCode	retCode;

	// �������� ������ ������
	sliCurrent = inputLexemeList.begin();
	
	// � ����������� ����� ��������� ������� �� ��� ���, ���� ��� �� ����������
	while( 1 )
	{
		
		// ���� ���� ������ ���� - ������ �������� ������� � ��������� �� � ����
		if( this->lexemeStack.size() == 0 )
		{
			this->lexemeStack.push_back( *sliCurrent );
			
			currentLexemeIndex = this->getLexemeIndex( *(*sliCurrent) );
			if( currentLexemeIndex < 0 || currentLexemeIndex >= this->lexemeCount )
			{
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliCurrent) ); 	
				return SRV_FAIL;
			} // ���� ������� �� ������� ����� �� ���������� 

			this->indexStack.push_back( currentLexemeIndex );
			
			// ��������� � ��������� �������
			sliCurrent ++;
			continue ; 
		}
		// ���� �� ���� ���� ����������
		// ����� ��������� ����� �������� ����� � ��������� ��������
		else 
		{
			// �������� ��������� �� ������� �����
			sliStackTop = this->lexemeStack.end();
			sliStackTop --;
			
			sliStackTopIndex = this->indexStack.end();
			sliStackTopIndex --;

			// � ����� ������ ���� ������� � ������ ������ �����������
			stackTopIndex = *sliStackTopIndex;	
			
			// ���� ��������� ����� ������, �� ��������� �������� >
			if( sliCurrent == inputLexemeList.end() )
			{
				relCode = RELCODE_GREATER;
			}
			// ���� ���, �� ���� �������� ��������� ������� � ����� ���������
			else
			{
			
				// �������� ������ ������� �������
				currentLexemeIndex = this->getLexemeIndex( *(*sliCurrent) );

				if( currentLexemeIndex < 0 || currentLexemeIndex >= this->lexemeCount )
				{
					this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliCurrent) ); 	
					return SRV_FAIL;
				} // ���� ������� �� ������� ����� �� ���������� 

				relCode = (relationCode)this->matrix[ stackTopIndex ][ currentLexemeIndex ];
			}
				
			this->printStack( inputLexemeList, sliCurrent, relCode, scaner.getIdsList() );
			
			// ������ ������� ���������
			// �������� ��������� ��������� �� > � =
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
				// �������� ��������� - ���������� ������ ��� � �������
				// ������� ����� � ������ �����
				
				// �������� ������� ������� ����� ��� �������� ������ �����
				// ���� �� ���, �� �� ������ ����� TRIADE_LEXEME_EMPTY
				if( sliStackTop == this->lexemeStack.begin() )
					stackTopIndex_1 = TRIADE_LEXEME_EMPTY;
				// ����� �������� �������� �� ������
				else 
				{
					sliStackTop_1 = sliStackTop;
					sliStackTop_1 --;

					sliStackTopIndex_1 = sliStackTopIndex; 
					sliStackTopIndex_1 --;

					// ���������� ������
					stackTopIndex_1 = *sliStackTopIndex_1;

					// ������� � ����� �� �� ������
					if( stackTopIndex_1 < 0 || stackTopIndex_1 >= this->lexemeCount )
					{
						this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRECOGNIZED_TOKEN, *(*sliStackTop_1) ); 	
						return SRV_FAIL;
					} // ���� ������� ����� �������� �� ������� ����� �� ���������� 
				}
			
				// ���������� ������ - ������ �������� ����� ������ ������
				if( this->getTriadeIndex( stackTopIndex_1, stackTopIndex, currentLexemeIndex, false ) != -1 )
					relCode = RELCODE_EQUAL;
				// ����� �������� ����������� � ������� >
				else
					relCode = RELCODE_GREATER;	
			}
			
			// ��������� ��������
			// ������ ��������� ��� ��������� > � =
			
			// ��������� =
			if( relCode == RELCODE_EQUAL )
			{
				// ���� ������ �������� ������� � ���� ������
				this->lexemeStack.push_back( *sliCurrent );
				// � ����� �� ������
				this->indexStack.push_back( currentLexemeIndex );
				// ��������� � ��������� �������
				sliCurrent ++;
			}
			// ��������� >
			else if( relCode == RELCODE_GREATER )
			{
				// ��������� � �������� �������
				retCode = this->parseBack( scaner, sliCurrent );
				// ���� �� ��� ��������� ���������
				if( retCode != SRV_SUCCESS )
					return retCode ;
			}
			else
			{
				// ������� ������������ ���������
				_stprintf( this->tempBuffer, TEXT( "%s" ), (*sliStackTop)->getLexemeDesc().getCString() );
				_stprintf( &this->tempBuffer[ _tcslen( this->tempBuffer ) ], TEXT( " and %s ( relation <> or <>= )" ), (*sliCurrent)->getLexemeDesc().getCString() );
				
				this->addError( SE_PARSER_FATAL_ERROR, SPRV_UNRESOLVED_RELATION, this->tempBuffer, *(*sliStackTop) ); 	
				
				return SRV_FAIL;
			}

		} // � ����� ���� �������

	} // while( 1 )
	
	return SRV_SUCCESS;
}
