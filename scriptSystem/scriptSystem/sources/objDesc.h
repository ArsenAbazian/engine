#ifndef __OBJDESC_INC
#define __OBJDESC_INC

#include <list>
#include <windows.h>
#include "typeDesc.h"
#include "lexeme.h"

typedef enum
{
	OBJ_TYPE_NONE			= 0,	// ������
	OBJ_TYPE_VARIALE		= 1,	// ������� ���������� 
	
	OBJ_TYPE_FORCE_DWORD	= 0xffffffff
}objectType;

// ���� ����� �������� ������� ������� ��� 
// �������� ����������� �������, ���� �������� 
// ���������, ������������, �����, ��������� � �. �.
class scriptObjectDesc
{
public:
	// ���������� � ���������
	scriptLexeme		lexeme;			// ������� ����������� ������

	objectType			objClass;		// ����� ������� ...
	scriptTypeDesc		typeDesc;		// �������� ���� ������� (��� ����������, ������������ �������� ������� � �.�.)
	
	scriptObjectDesc();
	~scriptObjectDesc();
};

// �������� ������ �������
void clearObjList( std::list< scriptObjectDesc* > &sodl );

#endif