#include "screrror.h"

scriptError::scriptError()
{

	this->code		= SRV_SUCCESS;
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
	fileName.clear();
	fullPathName.clear();
    
	this->code		= SRV_SUCCESS;
	this->col		= -1;
	this->line		= -1;
	this->type		= SE_NONE;

	return ;	

}

scriptError& scriptError::copy( scriptError& se )
{
	
	this->text			= se.text;
	this->fileName		= se.fileName;
	this->fullPathName	= se.fullPathName;
	this->code			= se.code;
	this->line			= se.line;
	this->col			= se.col;
	this->type			= se.type;

	return *this;
}

void scriptError::print()
{
	// ������� �������� ��� �����
	printf( TEXT( "%s : " ), this->fileName.getCString());
	
	// ����� ������ � �������
	printf( TEXT( "����� = %d, ������� = %d : " ), this->line, this->col );
	
	// ����� ��� ������
	if( this->type == SE_SCANER_ERROR )
		printf( TEXT( "������ : SE%4.4d"), this->code );
	else if( this->type == SE_SCANER_FATAL_ERROR )
		printf( TEXT( "��������� ������ : SFE%4.4d"), this->code );
	else if( this->type == SE_SCANER_WARNING )
		printf( TEXT( "�������������� : SW%4.4d"), this->code );
	else if( this->type == SE_PARSER_ERROR )
		printf( TEXT( "������ : PE%4.4d"), this->code );
	else if( this->type == SE_PARSER_FATAL_ERROR )
		printf( TEXT( "��������� ������ : PFE%4.4d"), this->code );
	else if( this->type == SE_PARSER_WARNING )
		printf( TEXT( "�������������� : PW%4.4d"), this->code );
	else if( this->type == SE_PREPROCESSOR_ERROR )
		printf( TEXT( "������ : PPE%4.4d"), this->code );
	else if( this->type == SE_PREPROCESSOR_FATAL_ERROR )
		printf( TEXT( "��������� ������ : PPFE%4.4d"), this->code );
	else if( this->type == SE_PREPROCESSOR_WARNING )
		printf( TEXT( "�������������� : PPW%4.4d"), this->code );
	
	// ����� ������
	printf( TEXT( " : %s : " ), this->text );

	// ����� ������������� ���� ������ ������
	switch( this->code )
	{
	case SRV_FAIL:
		printf( TEXT( "��������� ������ �����������!" ) );
		break;
	case SRV_SUCCESS:
	case SSRV_SUCCESS:
		printf( TEXT( "��� ������" ) ); 
		break;
	case SPPRV_CANT_OPEN_READ_FILE:
		printf( TEXT( "������������ �� ����� ������� ���� ��� ������" ) );
		break;
	case SPPRV_CANT_OPEN_WRITE_FILE:
		printf( TEXT( "������������ �� ����� ������� ���� ��� ������" ) );
		break;
	case SPPRV_FILE_LIMIT_COUNT:
		printf( TEXT( "���������� ���������� ����� ����������� ��� ��������� 'include'" ) );	
		break;
	case SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR:
		printf( TEXT( "���� � ����� ������ �� ������ � ��������� ������ ������������ ������" ) );	
		break;
	case SPPRV_FILE_RECURSION:
		printf( TEXT( "���������� �������� � ��������� 'include'" ) );
		break;
	case SPPRV_NO_SUCH_FILE_IN_USER_INCLUDE_DIR:
		printf( TEXT( "���� � ����� ������ �� ������ � ���������������� ������" ) );	
		break;
	case SPPRV_INVALID_FILE_NAME:
		printf( TEXT( "������������ ��� �����" ) );
		break;
	case SPPRV_OUTPUT_FILE_NOT_OPENED:
		printf( TEXT( "�������� ���� �� ������" ) );
		break;
	case SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST:
		printf( TEXT( "������ �������� ������������� ������ ���� �������� � ������, �� ������ �������� � �������� ���������" ) );
        break;
	case SSRV_CANT_OPEN_FILE:
		printf( TEXT( "������ �� ����� ������� ����" ) );
		break;
	case SSRV_END_OF_FILE:
		printf( TEXT( "������ ��������� ��� '����� �����'" ) );
		break;
	case SSRV_FILE_NOT_OPENED:
		printf( TEXT( "���� �� ������" ) );
		break;
	case SSRV_COMMENT_BLOCK_NOT_FINISHED:
		printf( TEXT( "����������� ����� ����� � ����� �����������" ) );
		break;
	case SSRV_NEWLINE_IN_STRING_CONSTANT:
		printf( TEXT( "���������� ����� ������ � ��������� ���������" ) );
		break;
	case SSRV_EOF_IN_STRING_CONSTANT:
		printf( TEXT( "��������� ����� ����� � ��������� ���������" ) );
		break;
	case SSRV_NEWLINE_IN_CHAR_CONSTANT:
		printf( TEXT( "���������� ����� ������ � ���������� ���������" ) );
		break;
	case SSRV_EOF_IN_CHAR_CONSTANT:
		printf( TEXT( "��������� ����� ����� � ���������� ���������" ) );
		break;
	case SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT:
		printf( TEXT( "���������� ����� ������ ������� � ���������� ���������" ) );
		break;
	case SSRV_EMPTY_CHAR_CONSTANT:
		printf( TEXT( "empty char constant" ) );
		break;
	case SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE:
		printf( TEXT( "unrecognized character escape sequence" ) );
		break;
	case SSRV_TOO_BIG_FOR_CHARACTER:
		printf( TEXT( "to big for character" ) );
		break;
	case SSRV_BAD_CHARACTER:
		printf( TEXT( "unrecognized character in file" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_BINARY_FORMAT:
		printf( TEXT( "bad number suffix or unrecognized binary format ( must begin from 0b or 0B )" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_OCTAL_FORMAT:
		printf( TEXT( "bad number suffix or unrecognized octal format ( must begin from 0o or 0O )" ) );
		break;
	case SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT:
		printf( TEXT( "bad number suffix or unrecognized hexadecimal format ( must begin from 0x or 0X )" ) );
		break;
	case SSRV_UNRECOGNIZED_BINARY_FORMAT:
		printf( TEXT( "unrecognized binary format ( must begin from 0b or 0B and then 0 or 1 digits )" ) );
		break;
	case SSRV_UNRECOGNIZED_OCTAL_FORMAT:
		printf( TEXT( "unrecognized octal format ( must begin from 0o or 0O and then from 0 to 7 digits )" ) );
		break;
	case SSRV_INVALID_SUFFIX_POSITION:
		printf( TEXT( "invalid number suffix position ( suffix must goes after all digits )" ) );
		break;
	case SSRV_INVALID_U_SUFFIX_POSITION:
		printf( TEXT( "invalid 'u' suffix position ( must not goes after another suffix )" ) );
		break;
	case SSRV_INVALID_L_SUFFIX_POSITION:
		printf( TEXT( "invalid 'l' suffix position ( duplicate 'l' suffixs )" ) );
		break;
	case SSRV_UNRECOGNIZED_NUMBER_FORMAT:
		printf( TEXT( "unrecognized number format or bad suffix" ) );
		break;
	case SSRV_INVALID_E_SUFFIX_POSITION:
		printf( TEXT( "invalid 'e' suffix position ( must goes after digit )" ) );
		break;
	case SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX:
		printf( TEXT( "unrecognized float value format or bad suffix" ) );
		break;
	case SSRV_EXPECTED_EXPONENT_VALUE:
		printf( TEXT( "expected exponent value" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR01:
		printf( TEXT( "expected '<' symbol or string constant after 'include' directive but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR02:
		printf( TEXT( "expected new line but found token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR03:
		printf( TEXT( "expected '<' symbol or string constant after 'include' but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR04:
		printf( TEXT( "expected filename without path but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR05:
		printf( TEXT( "expected filename without path but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR06:
		printf( TEXT( "expected '>' but found new line" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR07:
		printf( TEXT( "expected '>' but found another token" ) );
		break;
	case SPPRV_INCLUDE_DIRECTIVE_ERROR08:
		printf( TEXT( "expected newline but found another token" ) );
		break;
	case SPPRV_CANT_EXECUTE_DIRECTIVE:
		printf( TEXT( "can't execute preprocessor directive" ) );
		break;
	case SPPRV_DEFINE_DIRECTIVE_ERROR01:
		printf( TEXT( "expected macro name but found newline" ) );
		break;
	case SPPRV_DEFINE_DIRECTIVE_ERROR02:
		printf( TEXT( "expected macro name ( id or rword ) but found another token" ) );
		break;
	case SPPRV_MACRO_REDEFINITION:
		printf( TEXT( "macro redefinition" ) );
		break;
	case SPPRV_ENDIF_DIRECTIVE_ERROR01:
		printf( TEXT( "expected new line after 'endif' but found another token" ) );
		break;
	case SPPRV_ENDIF_DIRECTIVE_ERROR02:
		printf( TEXT( "using 'endif' without 'if', 'ifdef' or 'ifndef'" ) );
		break;
	case SPPRV_ELSE_DIRECTIVE_ERROR01:
		printf( TEXT( "expected new line after 'else' but found another token" ) );
		break;
	case SPPRV_ELSE_DIRECTIVE_ERROR02:
		printf( TEXT( "using 'else' without 'if', 'ifdef' or 'ifndef'" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR01:
		printf( TEXT( "expected macro name after 'undef' but found newline" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR02:
		printf( TEXT( "expected macro name but found another token" ) );
		break;
	case SPPRV_UNDEF_DIRECTIVE_ERROR03:
		printf( TEXT( "expected new line after macro name but found another token" ) );
		break;
	case SPPRV_UNDEFINED_MACRO:
		printf( TEXT( "undefined macro's name" ) );
		break;
	case SPPRV_ERROR_DIRECTIVE:
		printf( TEXT( "'error' directive" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR01:
		printf( TEXT( "macro parameter list - expected id but found another token" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR02:
		printf( TEXT( "macro parameter list - expected ',' but found another token" ) );
		break;
	case SPPRV_MACRO_PARAM_LIST_ERROR03:
		printf( TEXT( "macro parameter list - expected ')' but found newline" ) );
		break;
	case SPPRV_EOF_IN_MACRO_PARAMS:
		printf( TEXT( "unexpected end of file while scanning macro arguments" ) );
		break;
	case SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO:
		printf( TEXT( "ecpected '(' for macro parameter list but found another token" ) );
		break;
	case SPPRV_NOT_ENOUGH_ACTUAL_PARAMS_FOR_MACRO:
		printf( TEXT( "not enough actual parameters for macro" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR01:
		printf( TEXT( "expected expression after '(' but found nothing" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR02:
		printf( TEXT( "invalid expression in preprocessor directive: count of '(' is more than count of ')'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR03:
		printf( TEXT( "invalid expression in preprocessor directive: () - no token in brackets" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR04:
		printf( TEXT( "invalid expression in preprocessor directive: expected name after defined but found another token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR05:
		printf( TEXT( "invalid expression in preprocessor directive: expected name after defined but found newline" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR06:
		printf( TEXT( "invalid expression in preprocessor directive: expected const value but found newline" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR07:
		printf( TEXT( "invalid expression in preprocessor directive: expected const value but found anoter token" ) );
		break;
	case SPPRV_IF_DIRECTIVE_WARNING01:
		printf( TEXT( "invalid expression in preprocessor directive: unsafe applying of '~' operator to bool constant." ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR08:
		printf( TEXT( "invalid expression in preprocessor directive: '~' : illegal on operands of type 'float'" ) );
		break;
	case SPPRV_IF_DIRECTIVE_ERROR09:
		printf( TEXT( "invalid expression in preprocessor directive: '~' : illegal on operands of type 'double'" ) );
		break;
	}

	// �������� ������ ��� �����
	printf( TEXT( " : " ) );
	printf( TEXT( "%s" ), this->fullPathName.getCString() );

	return ;
}