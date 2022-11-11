#ifndef PREPROCSTATE
#define PREPROCSTATE

typedef enum _preProcessorState		// состояние препроцессора
{
	PPS_NONE					= 0,
	PPS_GET_SYMBOL				= 1,		// взять символ из файла
	PPS_GET_STRING				= 2,		// чтение строки
	PPS_DIRECTIVE				= 3,		// в режиме выполнения директивы препроцессора 
	PPS_FORCE_DWORD				= 0xffffffff
}preProcessorState;

#endif