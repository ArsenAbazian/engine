#ifndef PREPROCSTATE
#define PREPROCSTATE

typedef enum _preProcessorState		// ��������� �������������
{
	PPS_NONE					= 0,
	PPS_GET_SYMBOL				= 1,		// ����� ������ �� �����
	PPS_GET_STRING				= 2,		// ������ ������
	PPS_DIRECTIVE				= 3,		// � ������ ���������� ��������� ������������� 
	PPS_FORCE_DWORD				= 0xffffffff
}preProcessorState;

#endif