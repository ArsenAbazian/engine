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

	SSRV_SUCCESS											= 1000,	// все нормально
	SSRV_CANT_OPEN_FILE										= 1001,	// невозможно открыть файл
	SSRV_END_OF_FILE										= 1002,	// конец файла
	SSRV_FILE_NOT_OPENED									= 1003,	// файл не открыт
	SSRV_COMMENT_BLOCK_NOT_FINISHED							= 1004, // блок комментаря не был завершен
	SSRV_NEWLINE_IN_STRING_CONSTANT							= 1005,	// перевод строки в строке
	SSRV_EOF_IN_STRING_CONSTANT								= 1006,	// конец файла в строковой константе
	SSRV_NEWLINE_IN_CHAR_CONSTANT							= 1007,	// перевод строки в строке
	SSRV_EOF_IN_CHAR_CONSTANT								= 1008,	// конец файла в символьной константе
	SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT				= 1009,	// больше одного символа в символьной константе
	SSRV_EMPTY_CHAR_CONSTANT								= 1010,	// пустая символьная константа
	SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE				= 1011,	// недопустимый символ в escape последовательности
	SSRV_TOO_BIG_FOR_CHARACTER								= 1012,	// слишком большое число для символа
	SSRV_BAD_CHARACTER										= 1013,	// плохой символ
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_BINARY_FORMAT	= 1014, // либо плохой суффикс, либо неправильный бинарный формат
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_OCTAL_FORMAT		= 1015, // либо плохой суффикс, либо неправильный восьмеричный формат
	SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT		= 1016,	// либо плохой суффикс, либо неправильный шестнадцатеричный формат
	SSRV_UNRECOGNIZED_BINARY_FORMAT							= 1017,	// неправильный формат двоичного числа
	SSRV_UNRECOGNIZED_OCTAL_FORMAT							= 1018,	// неправильный формат восьмеричного числа
	SSRV_INVALID_SUFFIX_POSITION							= 1019,	// неправильное местоположение суффикса, суффикс должен идти после все цифр
	SSRV_INVALID_U_SUFFIX_POSITION							= 1020,	// перед суффиксом u нет цифры
	SSRV_INVALID_L_SUFFIX_POSITION							= 1021,	// перед суффиксом l нет цифры
	SSRV_UNRECOGNIZED_NUMBER_FORMAT							= 1022,	// неправильный формат числа
	SSRV_INVALID_E_SUFFIX_POSITION							= 1023,	// перед суффиксом e нет цифры
	SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX		= 1024,	// неправильный формат вещественного числа
	SSRV_EXPECTED_EXPONENT_VALUE							= 1025,	// необходимо значение экспоненты
	SSRV_END_OF_LINE										= 1026, // найден конец строки
	SSRV_APPLY_UNARY_MINUS_TO_UNSIGNED						= 1027, // применение унарного минуса к беззнаковой константе - результат знаковая константа

	SGRV_UNEXPECTED_END_OF_FILE								= 2000,	// неожиданный конец файла при анализе правила
	SGRV_EXPECTED_RULE_RIGHT_PART							= 2001,	// должна быть правая часть правила
	SGRV_EMPTY_RIGHT_PART									= 2002,	// пустая правая часть
	SGRV_END_OF_RULE										= 2003, // достигнут конец правила
	SGRV_OK													= 2004, // операция анализа завершена успешно
	SGRV_EXPECTED_COLON2									= 2005, // необходимо двоеточие а найден конец файла
	SGRV_EXPECTED_COLON2_2									= 2006, // необходимо двоеточие, а найдено что-то совсем другое
	SGRV_EXPECTED_ASSIGN									= 2007, // необходимо = а найден конец файла
	SGRV_EXPECTED_ASSIGN_2									= 2008, // необходимо = а найдено что то совсем другое
	SGRV_UNDEFINED_NTERMINAL								= 2009, // неопределена правая часть для нетерминала
	SGRV_LESS_EQUAL_CONFLICT								= 2010,	// конфликт <=
	SGRV_GREATER_EQUAL_CONFLICT								= 2011, // конфликт >=
	SGRV_LESS_GREATER_CONFLICT								= 2012, // конфликт <>
	SGRV_EXPECTED_NTERMINAL									= 2013, // требуется нетерминал в левой части
	SGRV_LESS_GREATER_EQUAL_CONFLICT						= 2014, // конфликт <>=
	SGRV_EQUAL_RIGHT_PART									= 2015, // 2 правила имеют одинаковые правые части
	SGRV_RIGHT_TRIADE_COLLISION								= 2016,	// коллизия правых троек
	SGRV_LEFT_TRIADE_COLLISION								= 2017,	// коллизия левых троек

	SPRV_UNRECOGNIZED_TOKEN									= 3000,	// нераспознанная лексема
	SPRV_NO_RELATION_FOUND									= 3001,	// нет отношения между лексемами
	SPRV_UNRESOLVED_RELATION								= 3002, // неразрешенное отношение
	
	SGRV_EXPECTED_ENTER_RULE								= 4001, // не задано главное правило
	SPRV_CAN_CONTINUE										= 4002, // правило не подошло но можно продолжать
	
	SPRV_NOT_THIS_RULE										= 4003, // это не то правило										
	SPRV_EOF												= 4004, // конец файла
	
	SRV_FORCE_DWORD											= 0xffffffff

}scriptRetCode;

#endif