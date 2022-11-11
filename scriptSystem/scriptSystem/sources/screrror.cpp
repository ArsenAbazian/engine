#include "screrror.h"

scriptError::scriptError()
{

	this->code		= SRV_SUCCESS;
	this->fileIndex = -1;
	this->col		= -1;
	this->line		= -1;
	this->type		= SE_NONE;
		
	return ;

}

scriptError::~scriptError()
{
	this->clear();
}

void scriptError::clear()
{
	
	text.clear();
	    
	this->code		= SRV_SUCCESS;
	this->fileIndex = -1;
	this->col		= -1;
	this->line		= -1;
	this->type		= SE_NONE;

	return ;	

}

scriptError& scriptError::copy( scriptError& se )
{
	
	this->text			= se.text;
	this->fileIndex		= se.fileIndex;
	this->code			= se.code;
	this->line			= se.line;
	this->col			= se.col;
	this->type			= se.type;

	return *this;
}

void scriptError::print()
{
	// сначала печатаем имя файла
	_tprintf( TEXT( "file id = " ) );
	this->printFileIndex();
	_tprintf( TEXT( " : " ) );
	
	// затем строку и столбец
	_tprintf( TEXT( "line = %d, col = %d : " ), this->line, this->col );
	
	// затем тип ошибки
	this->printType( );
	_tprintf( TEXT( " : " ) );

	// затем код ошибки
	this->printCode();
		
	// текст ошибки
	_tprintf( TEXT( " : %s : " ), this->text );

	// описание ошибки
	this->printCodeDesc();
	
	return ;
}

void scriptError::printFileIndex( )
{
	_tprintf( TEXT( "%d" ), this->fileIndex );
	return ;
}

void scriptError::printFileIndex( TCHAR *buffer )
{
	_stprintf_s( buffer, 16,TEXT( "%d" ), this->fileIndex );
	return ;
}

void scriptError::printLine( )
{
	_tprintf( TEXT( "%d" ), this->line );
	return ;
}

void scriptError::printLine( TCHAR *buffer )
{
	_stprintf_s( buffer, 16, TEXT( "%d" ), this->line );
	return ;
}

void scriptError::printCol( )
{
	_tprintf( TEXT( "%d" ), this->col );
	return ;
}

void scriptError::printCol( TCHAR *buffer )
{
	_stprintf_s( buffer, 16, TEXT( "%d" ), this->col );
	return ;
}

void scriptError::printCodeDesc( )
{
	TCHAR buffer[ 512 ];
	
	this->printCode( buffer );
	
	_tprintf( TEXT( "%s" ), buffer );
	return ;
}

void scriptError::printCodeDesc( TCHAR *buffer )
{
	// затем распознавание кода ошибки ошибки
	switch( this->code )
	{
	case SRV_FAIL:
		_stprintf_s( buffer, 256, TEXT( "internal error!" ) );
		break;
	case SRV_SUCCESS:
	case SSRV_SUCCESS:
		_stprintf_s( buffer, 256, TEXT( "no error" ) ); 
		break;
	case SPPRV_CANT_OPEN_READ_FILE:
		_stprintf_s( buffer, 256, TEXT( "preprocessor cannot open file for reading" ) );
		break;
	case SPPRV_CANT_OPEN_WRITE_FILE:
		_stprintf_s( buffer, 256, TEXT( "preprocessor cannot open file for writing" ) );
		break;
	case SPPRV_FILE_LIMIT_COUNT:
		_stprintf_s( buffer, 256, TEXT( "'include' directive deep count limit" ) );	
		break;
	case SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR:
		_stprintf_s( buffer, 256, TEXT( "no such file found in system include directories" ) );	
		break;
	case SPPRV_FILE_RECURSION:
		_stprintf_s( buffer, 256, TEXT( "found file recursion in 'include' directive" ) );
		break;
	case SPPRV_NO_SUCH_FILE_IN_USER_INCLUDE_DIR:
		_stprintf_s( buffer, 256, TEXT( "no such file found in user include directories" ) );	
		break;
	case SPPRV_INVALID_FILE_NAME:
		_stprintf_s( buffer, 256, TEXT( "invalid file name" ) );
		break;
	case SPPRV_OUTPUT_FILE_NOT_OPENED:
		_stprintf_s( buffer, 256, TEXT( "output file not opened" ) );
		break;
	case SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST:
		_stprintf_s( buffer, 256, TEXT( "preprocessor command must start as first nonwhite space" ) );
        break;
	case SSRV_CANT_OPEN_FILE:
		_stprintf_s( buffer, 256, TEXT( "scaner cannot open file" ) );
		break;
	case SSRV_END_OF_FILE:
		_stprintf_s( buffer, 256, TEXT( "scaner returned end of file" ) );
		break;
	case SSRV_FILE_NOT_OPENED:
		_stprintf_s( buffer, 256, TEXT( "file not opened" ) );
		break;
	case SSRV_COMMENT_BLOCK_NOT_FINISHED:
		_stprintf_s( buffer, 256, TEXT( "unexpected end of file in comment block" ) );
		break;
	case SSRV_NEWLINE_IN_STRING_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "new line in string constant" ) );
		break;
	case SSRV_EOF_IN_STRING_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "end of file in string constant" ) );
		break;
	case SSRV_NEWLINE_IN_CHAR_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "new line in character constant" ) );
		break;
	case SSRV_EOF_IN_CHAR_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "end of file in character constant" ) );
		break;
	case SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "more than one character in character constant" ) );
		break;
	case SSRV_EMPTY_CHAR_CONSTANT:
		_stprintf_s( buffer, 256, TEXT( "empty char constant" ) );
		break;
	case SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE:
		_stprintf_s( buffer, 256, TEXT( "unrecognized character escape sequence" ) );
		break;
	case SSRV_TOO_BIG_FOR_CHARACTER:
		_stprintf_s( buffer, 256, TEXT( "to big for character" ) );
		break;
	case SSRV_BAD_CHARACTER:
		_stprintf_s( buffer, 256, TEXT( "unrecognized character in file" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_BINARY_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "bad number suffix or unrecognized binary format ( must begin from 0b or 0B )" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_OCTAL_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "bad number suffix or unrecognized octal format ( must begin from 0o or 0O )" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "bad number suffix or unrecognized hexadecimal format ( must begin from 0x or 0X )" ) );
		break;
	case SSRV_UNRECOGNIZED_BINARY_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "unrecognized binary format ( must begin from 0b or 0B and then 0 or 1 digits )" ) );
		break;
	case SSRV_UNRECOGNIZED_OCTAL_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "unrecognized octal format ( must begin from 0o or 0O and then from 0 to 7 digits )" ) );
		break;
	case SSRV_INVALID_SUFFIX_POSITION:
		_stprintf_s( buffer, 256, TEXT( "invalid number suffix position ( suffix must goes after all digits )" ) );
		break;
	case SSRV_INVALID_U_SUFFIX_POSITION:
		_stprintf_s( buffer, 256, TEXT( "invalid 'u' suffix position ( must not goes after another suffix )" ) );
		break;
	case SSRV_INVALID_L_SUFFIX_POSITION:
		_stprintf_s( buffer, 256, TEXT( "invalid 'l' suffix position ( duplicate 'l' suffixs )" ) );
		break;
	case SSRV_UNRECOGNIZED_NUMBER_FORMAT:
		_stprintf_s( buffer, 256, TEXT( "unrecognized number format or bad suffix" ) );
		break;
	case SSRV_INVALID_E_SUFFIX_POSITION:
		_stprintf_s( buffer, 256, TEXT( "invalid 'e' suffix position ( must goes after digit )" ) );
		break;
	case SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX:
		_stprintf_s( buffer, 256, TEXT( "unrecognized float value format or bad suffix" ) );
		break;
	case SSRV_EXPECTED_EXPONENT_VALUE:
		_stprintf_s( buffer, 256, TEXT( "expected exponent value" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected '<' symbol or string constant after 'include' directive but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "expected new line but found token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR03:
		_stprintf_s( buffer, 256, TEXT( "expected '<' symbol or string constant after 'include' but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR04:
		_stprintf_s( buffer, 256, TEXT( "expected filename without path but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR05:
		_stprintf_s( buffer, 256, TEXT( "expected filename without path but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR06:
		_stprintf_s( buffer, 256, TEXT( "expected '>' but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR07:
		_stprintf_s( buffer, 256, TEXT( "expected '>' but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR08:
		_stprintf_s( buffer, 256, TEXT( "expected newline but found another token" ) );
		break;
	case SPPRV_CANT_EXECUTE_DIRECTIVE:
		_stprintf_s( buffer, 256, TEXT( "can't execute preprocessor directive" ) );
		break;
	case SPPRV_DEFINE_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected macro name but found newline" ) );
		break;
	case SPPRV_DEFINE_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "expected macro name ( id or rword ) but found another token" ) );
		break;
	case SPPRV_MACRO_REDEFINITION:
		_stprintf_s( buffer, 256, TEXT( "macro redefinition" ) );
		break;
	case SPPRV_ENDIF_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected new line after 'endif' but found another token" ) );
		break;
	case SPPRV_ENDIF_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "using 'endif' without 'if', 'ifdef' or 'ifndef'" ) );
		break;
	case SPPRV_ELSE_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected new line after 'else' but found another token" ) );
		break;
	case SPPRV_ELSE_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "using 'else' without 'if', 'ifdef' or 'ifndef'" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected macro name after 'undef' but found newline" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "expected macro name but found another token" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR03:
		_stprintf_s( buffer, 256, TEXT( "expected new line after macro name but found another token" ) );
		break;
	case SPPRV_UNDEFINED_MACRO:
		_stprintf_s( buffer, 256, TEXT( "undefined macro's name" ) );
		break;
	case SPPRV_ERROR_DIRECTIVE:
		_stprintf_s( buffer, 256, TEXT( "'error' directive" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "macro parameter list - expected id but found another token" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "macro parameter list - expected ',' but found another token" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR03:
		_stprintf_s( buffer, 256, TEXT( "macro parameter list - expected ')' but found newline" ) );
		break;
	case SPPRV_EOF_IN_MACRO_PARAMS:
		_stprintf_s( buffer, 256, TEXT( "unexpected end of file while scanning macro arguments" ) );
		break;
	case SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO:
		_stprintf_s( buffer, 256, TEXT( "ecpected '(' for macro parameter list but found another token" ) );
		break;
	case SPPRV_EXPECTED_RIGHT_BRACKET_IN_MACRO:
		_stprintf_s( buffer, 256, TEXT( "ecpected ')' for macro parameter list" ) );
		break;
	case SPPRV_NOT_ENOUGH_ACTUAL_PARAMS_FOR_MACRO:
		_stprintf_s( buffer, 256, TEXT( "not enough actual parameters for macro" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "expected expression after '(' but found nothing" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: count of '(' is more than count of ')'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR03:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: () - no token in brackets" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR04:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: expected name after defined but found another token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR05:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: expected name after defined but found newline" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR06:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: expected const value but found newline" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR07:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: expected const value but found anoter token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_WARNING01:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: unsafe applying of '~' operator to bool constant." ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR08:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: '~' : illegal on operands of type 'float'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR09:
		_stprintf_s( buffer, 256, TEXT( "invalid expression in preprocessor directive: '~' : illegal on operands of type 'double'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR10:
		_stprintf_s( buffer, 256, TEXT( "expected operand from binary left side but found nothing" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR11:
		_stprintf_s( buffer, 256, TEXT( "expected operand from binary right side but found nothing" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR12:
		_stprintf_s( buffer, 256, TEXT( "expected num const operand from binary left side but found another token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR13:
		_stprintf_s( buffer, 256, TEXT( "expected num const operand from binary right side but found another token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_WARNING02:
		_stprintf_s( buffer, 256, TEXT( "unsafe usage of 'bool' type value as left operand" ) );
		break;
	case SPPRV_IF_DIRECTIVE_WARNING03:
		_stprintf_s( buffer, 256, TEXT( "unsafe usage of 'bool' type value as right operand" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR14:
		_stprintf_s( buffer, 256, TEXT( "illegal left operand has type 'float'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR15:
		_stprintf_s( buffer, 256, TEXT( "illegal right operand has type 'float'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR16:
		_stprintf_s( buffer, 256, TEXT( "illegal left operand has type 'double'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR17:
		_stprintf_s( buffer, 256, TEXT( "illegal right operand has type 'double'" ) );
		break;
	case SSRV_APPLY_UNARY_MINUS_TO_UNSIGNED:
		_stprintf_s( buffer, 256, TEXT( "applying unary minus to unsigned constant value. result is signed constant" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR18:
		_stprintf_s( buffer, 256, TEXT( "invalid #if directive condition" ) );
		break;
	case SPPRV_QUICKIF_ERROR01:
		_stprintf_s( buffer, 256, TEXT( "must be expression in conditional operator but found nothing" ) );
		break;
	case SPPRV_QUICKIF_ERROR02:
		_stprintf_s( buffer, 256, TEXT( "must be expression in conditional operator but found another token" ) );
		break;
	case SPPRV_QUICKIF_ERROR03:
		_stprintf_s( buffer, 256, TEXT( "mus be an expression after '?' in conditional operator but found nothing" ) );
		break;
	case SPPRV_QUICKIF_ERROR04:
		_stprintf_s( buffer, 256, TEXT( "must be an expression after '?' in conditional operator but found another token" ) );
		break;
	case SPPRV_QUICKIF_ERROR05:
		_stprintf_s( buffer, 256, TEXT( "expected ':' after expression in conditional operator but found nothing" ) );
		break;
	case SPPRV_QUICKIF_ERROR06:
		_stprintf_s( buffer, 256, TEXT( "expected ':' after expression in conditional operator but found another token" ) );
		break;
	case SPPRV_QUICKIF_ERROR07:
		_stprintf_s( buffer, 256, TEXT( "expected expression after ':' but found nothing" ) );
		break;
	case SPPRV_QUICKIF_ERROR08:
		_stprintf_s( buffer, 256, TEXT( "expected expression after ':' but found another token" ) );
		break;
	case SGRV_UNEXPECTED_END_OF_FILE:
		_stprintf_s( buffer, 256, TEXT( "unexpected end of file" ) );	
		break;
	case SGRV_EXPECTED_RULE_RIGHT_PART:
		_stprintf_s( buffer, 256, TEXT( "expected rule right part but found end of rule" ) );	
		break;
	case SGRV_EMPTY_RIGHT_PART:
		_stprintf_s( buffer, 256, TEXT( "empty right side" ) );	
		break;
	case SGRV_EXPECTED_NTERMINAL:
		_stprintf_s( buffer, 256, TEXT( "expected nterminal (id) in rule left part" ) );
		break;
	case SGRV_EXPECTED_COLON2:
		_stprintf_s( buffer, 256, TEXT( "expected '::=' but found end of file" ) );
		break;
	case SGRV_EXPECTED_COLON2_2:
		_stprintf_s( buffer, 256, TEXT( "expected '::=' but found another token" ) );
		break;
	case SGRV_EXPECTED_ASSIGN:
		_stprintf_s( buffer, 256, TEXT( "expected '::=' but found end of file" ) );
		break;
	case SGRV_EXPECTED_ASSIGN_2:
		_stprintf_s( buffer, 256, TEXT( "expected '::=' but found another token" ) );
		break;
	case SGRV_UNDEFINED_NTERMINAL:
		_stprintf_s( buffer, 256, TEXT( "there is no left part for nterminal " ) );
		break;
	case SGRV_LESS_EQUAL_CONFLICT:
		_stprintf_s( buffer, 256, TEXT( "conflict <=" ) );
		break;
	case SGRV_GREATER_EQUAL_CONFLICT:
		_stprintf_s( buffer, 256, TEXT( "conflict >=" ) );
		break;
	case SGRV_LESS_GREATER_CONFLICT:
		_stprintf_s( buffer, 256, TEXT( "conflict <>" ) );
		break;
	case SGRV_LESS_GREATER_EQUAL_CONFLICT:
		_stprintf_s( buffer, 256, TEXT( "conflict <>=" ) );
		break;
	case SGRV_EQUAL_RIGHT_PART:
		_stprintf_s( buffer, 256, TEXT( "rules have the same right part" ) );
		break;
	case SGRV_RIGHT_TRIADE_COLLISION:
		_stprintf_s( buffer, 256, TEXT( "can't resolve >= conflict because of right triade collision" ) );
		break;
	case SGRV_LEFT_TRIADE_COLLISION:
		_stprintf_s( buffer, 256, TEXT( "can't resolve <= conflict because of left triade collision" ) );
	case SPRV_UNRECOGNIZED_TOKEN:
		_stprintf_s( buffer, 256, TEXT( "unrecognized token" ) );
		break;
    case SPRV_NO_RELATION_FOUND:
		_stprintf_s( buffer, 256, TEXT( "no relation found" ) );
        break;
	case SPRV_UNRESOLVED_RELATION:
		_stprintf_s( buffer, 256, TEXT( "unresolved relation" ) );
        break;
	case SGRV_EXPECTED_ENTER_RULE:
		_stprintf_s( buffer, 256, TEXT( "expected main rule definition" ) );
		break;
	}

	return ;
}

void scriptError::printType( )
{
	TCHAR buffer[ 512 ];
	
	this->printType( buffer );
	
	_tprintf( TEXT( "%s" ), buffer );
	return ;
}


void scriptError::printType( TCHAR *buffer )
{
	// затем тип ошибки
	if( this->type == SE_SCANER_ERROR )
		_stprintf_s( buffer, 256, TEXT( "error" ) );
	else if( this->type == SE_SCANER_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "fatal error") );
	else if( this->type == SE_SCANER_WARNING )
		_stprintf_s( buffer, 256, TEXT( "warning" ) );
	else if( this->type == SE_PARSER_ERROR )
		_stprintf_s( buffer, 256, TEXT( "error" ) );
	else if( this->type == SE_PARSER_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "fatal error" ) );
	else if( this->type == SE_PARSER_WARNING )
		_stprintf_s( buffer, 256, TEXT( "warning" ) );
	else if( this->type == SE_PREPROCESSOR_ERROR )
		_stprintf_s( buffer, 256, TEXT( "error" ) );
	else if( this->type == SE_PREPROCESSOR_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "fatal error" ) );
	else if( this->type == SE_PREPROCESSOR_WARNING )
		_stprintf_s( buffer, 256, TEXT( "warning" ) );
	else if( this->type == SE_SGRAMMAR_ERROR )
		_stprintf_s( buffer, 256, TEXT( "error" ) );
	else if( this->type == SE_SGRAMMAR_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "fatal error" ) );
	else if( this->type == SE_SGRAMMAR_WARNING )
		_stprintf_s( buffer, 256, TEXT( "warning" ) );

	return ;
}

void scriptError::printCode( )
{
	TCHAR buffer[ 512 ];
	
	this->printCode( buffer );
	
	_tprintf( TEXT( "%s" ), buffer );
	return ;
}


void scriptError::printCode( TCHAR *buffer )
{
	
	// затем тип ошибки
	if( this->type == SE_SCANER_ERROR )
		_stprintf_s( buffer, 256, TEXT( "SE%4.4d" ), this->code );
	else if( this->type == SE_SCANER_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "SFE%4.4d" ), this->code );
	else if( this->type == SE_SCANER_WARNING )
		_stprintf_s( buffer, 256, TEXT( "SW%4.4d" ), this->code );
	else if( this->type == SE_PARSER_ERROR )
		_stprintf_s( buffer, 256, TEXT( "PE%4.4d" ), this->code );
	else if( this->type == SE_PARSER_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "PFE%4.4d" ), this->code );
	else if( this->type == SE_PARSER_WARNING )
		_stprintf_s( buffer, 256, TEXT( "PW%4.4d" ), this->code );
	else if( this->type == SE_PREPROCESSOR_ERROR )
		_stprintf_s( buffer, 256, TEXT( "PPE%4.4d" ), this->code );
	else if( this->type == SE_PREPROCESSOR_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "PPFE%4.4d" ), this->code );
	else if( this->type == SE_PREPROCESSOR_WARNING )
		_stprintf_s( buffer, 256, TEXT( "PPW%4.4d" ), this->code );
	else if( this->type == SE_SGRAMMAR_ERROR )
		_stprintf_s( buffer, 256, TEXT( "SGE%4.4d" ), this->code );
	else if( this->type == SE_SGRAMMAR_FATAL_ERROR )
		_stprintf_s( buffer, 256, TEXT( "SGFE%4.4d" ), this->code );
	else if( this->type == SE_SGRAMMAR_WARNING )
		_stprintf_s( buffer, 256, TEXT( "SGW%4.4d" ), this->code );

	return ;
}

void scriptError::printText( )
{
	_tprintf( TEXT( "%s" ), this->text.getCString() );
	return ;
}

void scriptError::printText( TCHAR *buffer )
{
	_stprintf_s( buffer, 256, TEXT( "%s" ), this->text.getCString() );
	return ;
}

void clearErrorsList( std::list< scriptError* > &errList )
{
	std::list< scriptError* >::iterator	sei;	

	for( sei = errList.begin(); sei != errList.end(); sei ++ )
		delete *sei;
	
	errList.clear();
	return ;
}