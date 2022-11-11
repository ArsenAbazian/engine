#ifndef __SCRIPTDEF_INC
#define __SCRIPTDEF_INC

#include <windows.h>
#include <tchar.h>

typedef enum _scriptRetCode
{

	SRV_FAIL												= 0,
	SRV_SUCCESS												= 1,
	
	SPPRV_CANT_OPEN_READ_FILE								= 2,
	SPPRV_CANT_OPEN_WRITE_FILE								= 3,
	SPPRV_FILE_LIMIT_COUNT									= 4,
	SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR				= 5,
	SPPRV_FILE_RECURSION									= 6,
	SPPRV_NO_SUCH_FILE_IN_USER_INCLUDE_DIR					= 7,
	SPPRV_INVALID_FILE_NAME									= 8,
	SPPRV_OUTPUT_FILE_NOT_OPENED							= 9,
	SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST						= 10,
	SPPRV_DIRECTIVE_ERROR									= 11,
	SPPRV_INCLUDE_DIRECTIVE_ERROR01							= 12,
	SPPRV_INCLUDE_DIRECTIVE_ERROR02							= 13,
	SPPRV_INCLUDE_DIRECTIVE_ERROR03							= 14,
	SPPRV_INCLUDE_DIRECTIVE_ERROR04							= 15,
	SPPRV_INCLUDE_DIRECTIVE_ERROR05							= 16,
	SPPRV_INCLUDE_DIRECTIVE_ERROR06							= 17,
	SPPRV_INCLUDE_DIRECTIVE_ERROR07							= 18,
	SPPRV_INCLUDE_DIRECTIVE_ERROR08							= 19,
	SPPRV_CANT_EXECUTE_DIRECTIVE							= 20,
	SPPRV_DEFINE_DIRECTIVE_ERROR01							= 21,
	SPPRV_DEFINE_DIRECTIVE_ERROR02							= 22,
	SPPRV_MACRO_REDEFINITION								= 23,
	SPPRV_IFDEF_DIRECTIVE_ERROR01							= 24,
	SPPRV_IFDEF_DIRECTIVE_ERROR02							= 25,
	SPPRV_IFNDEF_DIRECTIVE_ERROR01							= 26,
	SPPRV_IFNDEF_DIRECTIVE_ERROR02							= 27,
	SPPRV_ENDIF_DIRECTIVE_ERROR01							= 28,
	SPPRV_ENDIF_DIRECTIVE_ERROR02							= 29,
	SPPRV_ELSE_DIRECTIVE_ERROR01							= 30,
	SPPRV_ELSE_DIRECTIVE_ERROR02							= 31,
	SPPRV_UNDEF_DIRECTIVE_ERROR01							= 32,
	SPPRV_UNDEF_DIRECTIVE_ERROR02							= 33,
	SPPRV_UNDEF_DIRECTIVE_ERROR03							= 34,
	SPPRV_UNDEFINED_MACRO									= 35,
	SPPRV_ERROR_DIRECTIVE									= 36,
	SPPRV_MACRO_PARAM_LIST_ERROR01							= 37,
	SPPRV_MACRO_PARAM_LIST_ERROR02							= 38,
	SPPRV_MACRO_PARAM_LIST_ERROR03							= 39,
	SPPRV_MACRO_RECURSION									= 40,
	SPPRV_EOF_IN_MACRO_PARAMS								= 41,
	SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO					= 42,
	SPPRV_EXPECTED_RIGHT_BRACKET_IN_MACRO					= 43,
	SPPRV_NOT_ENOUGH_ACTUAL_PARAMS_FOR_MACRO				= 44,
	SPPRV_IF_DIRECTIVE_ERROR01								= 45,
	SPPRV_IF_DIRECTIVE_ERROR02								= 46,
	SPPRV_IF_DIRECTIVE_ERROR03								= 47,
	SPPRV_IF_DIRECTIVE_ERROR04								= 48,
	SPPRV_IF_DIRECTIVE_ERROR05								= 49,
	SPPRV_IF_DIRECTIVE_ERROR06								= 50,
	SPPRV_IF_DIRECTIVE_ERROR07								= 51,
	SPPRV_IF_DIRECTIVE_WARNING01							= 52,
	SPPRV_IF_DIRECTIVE_ERROR08								= 53,
	SPPRV_IF_DIRECTIVE_ERROR09								= 54,
	SPPRV_IF_DIRECTIVE_ERROR10								= 55,
	SPPRV_IF_DIRECTIVE_ERROR11								= 56,
	SPPRV_IF_DIRECTIVE_ERROR12								= 57,
	SPPRV_IF_DIRECTIVE_ERROR13								= 58,
	SPPRV_IF_DIRECTIVE_ERROR14								= 59,
	SPPRV_IF_DIRECTIVE_WARNING02							= 60,
	SPPRV_IF_DIRECTIVE_WARNING03							= 61,
	SPPRV_IF_DIRECTIVE_ERROR15								= 62,
	SPPRV_IF_DIRECTIVE_ERROR16								= 63,
	SPPRV_IF_DIRECTIVE_ERROR17								= 64,
	SPPRV_IF_DIRECTIVE_ERROR18								= 65,
	SPPRV_QUICKIF_ERROR01									= 66,
	SPPRV_QUICKIF_ERROR02									= 67,
	SPPRV_QUICKIF_ERROR03									= 68,
	SPPRV_QUICKIF_ERROR04									= 69,
	SPPRV_QUICKIF_ERROR05									= 70,
	SPPRV_QUICKIF_ERROR06									= 71,
	SPPRV_QUICKIF_ERROR07									= 72,
	SPPRV_QUICKIF_ERROR08									= 73,

	SSRV_SUCCESS											= 1000,	// ��� ���������
	SSRV_CANT_OPEN_FILE										= 1001,	// ���������� ������� ����
	SSRV_END_OF_FILE										= 1002,	// ����� �����
	SSRV_FILE_NOT_OPENED									= 1003,	// ���� �� ������
	SSRV_COMMENT_BLOCK_NOT_FINISHED							= 1004, // ���� ���������� �� ��� ��������
	SSRV_NEWLINE_IN_STRING_CONSTANT							= 1005,	// ������� ������ � ������
	SSRV_EOF_IN_STRING_CONSTANT								= 1006,	// ����� ����� � ��������� ���������
	SSRV_NEWLINE_IN_CHAR_CONSTANT							= 1007,	// ������� ������ � ������
	SSRV_EOF_IN_CHAR_CONSTANT								= 1008,	// ����� ����� � ���������� ���������
	SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT				= 1009,	// ������ ������ ������� � ���������� ���������
	SSRV_EMPTY_CHAR_CONSTANT								= 1010,	// ������ ���������� ���������
	SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE				= 1011,	// ������������ ������ � escape ������������������
	SSRV_TOO_BIG_FOR_CHARACTER								= 1012,	// ������� ������� ����� ��� �������
	SSRV_BAD_CHARACTER										= 1013,	// ������ ������
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_BINARY_FORMAT	= 1014, // ���� ������ �������, ���� ������������ �������� ������
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_OCTAL_FORMAT		= 1015, // ���� ������ �������, ���� ������������ ������������ ������
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT		= 1016,	// ���� ������ �������, ���� ������������ ����������������� ������
	SSRV_UNRECOGNIZED_BINARY_FORMAT							= 1017,	// ������������ ������ ��������� �����
	SSRV_UNRECOGNIZED_OCTAL_FORMAT							= 1018,	// ������������ ������ ������������� �����
	SSRV_INVALID_SUFFIX_POSITION							= 1019,	// ������������ �������������� ��������, ������� ������ ���� ����� ��� ����
	SSRV_INVALID_U_SUFFIX_POSITION							= 1020,	// ����� ��������� u ��� �����
	SSRV_INVALID_L_SUFFIX_POSITION							= 1021,	// ����� ��������� l ��� �����
	SSRV_UNRECOGNIZED_NUMBER_FORMAT							= 1022,	// ������������ ������ �����
	SSRV_INVALID_E_SUFFIX_POSITION							= 1023,	// ����� ��������� e ��� �����
	SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX		= 1024,	// ������������ ������ ������������� �����
	SSRV_EXPECTED_EXPONENT_VALUE							= 1025,	// ���������� �������� ����������
	SSRV_END_OF_LINE										= 1026, // ������ ����� ������
	SSRV_APPLY_UNARY_MINUS_TO_UNSIGNED						= 1027, // ���������� �������� ������ � ����������� ��������� - ��������� �������� ���������

	SGRV_UNEXPECTED_END_OF_FILE								= 2000,	// ����������� ����� ����� ��� ������� �������
	SGRV_EXPECTED_RULE_RIGHT_PART							= 2001,	// ������ ���� ������ ����� �������
	SGRV_EMPTY_RIGHT_PART									= 2002,	// ������ ������ �����
	SGRV_END_OF_RULE										= 2003, // ��������� ����� �������
	SGRV_OK													= 2004, // �������� ������� ��������� �������
	SGRV_EXPECTED_COLON2									= 2005, // ���������� ��������� � ������ ����� �����
	SGRV_EXPECTED_COLON2_2									= 2006, // ���������� ���������, � ������� ���-�� ������ ������
	SGRV_EXPECTED_ASSIGN									= 2007, // ���������� = � ������ ����� �����
	SGRV_EXPECTED_ASSIGN_2									= 2008, // ���������� = � ������� ��� �� ������ ������
	SGRV_UNDEFINED_NTERMINAL								= 2009, // ������������ ������ ����� ��� �����������
	SGRV_LESS_EQUAL_CONFLICT								= 2010,	// �������� <=
	SGRV_GREATER_EQUAL_CONFLICT								= 2011, // �������� >=
	SGRV_LESS_GREATER_CONFLICT								= 2012, // �������� <>
	SGRV_EXPECTED_NTERMINAL									= 2013, // ��������� ���������� � ����� �����
	SGRV_LESS_GREATER_EQUAL_CONFLICT						= 2014, // �������� <>=
	SGRV_EQUAL_RIGHT_PART									= 2015, // 2 ������� ����� ���������� ������ �����
	SGRV_RIGHT_TRIADE_COLLISION								= 2016,	// �������� ������ �����
	SGRV_LEFT_TRIADE_COLLISION								= 2017,	// �������� ����� �����

	SPRV_UNRECOGNIZED_TOKEN									= 3000,	// �������������� �������
	SPRV_NO_RELATION_FOUND									= 3001,	// ��� ��������� ����� ���������
	SPRV_UNRESOLVED_RELATION								= 3002, // ������������� ���������
	
	SGRV_EXPECTED_ENTER_RULE								= 4001, // �� ������ ������� �������
	SPRV_CAN_CONTINUE										= 4002, // ������� �� ������� �� ����� ����������
	
	SPRV_NOT_THIS_RULE										= 4003, // ��� �� �� �������										
	SPRV_EOF												= 4004, // ����� �����
	
	SRV_FORCE_DWORD											= 0xffffffff

}scriptRetCode;

#endif