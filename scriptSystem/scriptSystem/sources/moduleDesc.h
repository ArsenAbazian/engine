#ifndef __MODULE_DESC_INC
#define __MODULE_DESC_INC

#include "scaner.h"
#include "syntparser2.h"
#include "objDesc.h"
#include "scopeDesc.h"

// ��������� ����� ���������� ��������
class scriptProgramm;

class scriptModule
{
	
	// ��������� �� ���������, ������� ����������� ������ ������
	scriptProgramm			*programm;	
	
	// ������� ��������� ���������
	scriptScopeDesc			scope;
	
	// ������ ��� ������� ������
	scriptScaner			scaner;
	// �������������� ���������� ��� ������� ������
	scriptSyntaxParser2		parser;
	
	// ������ ������ � ������ �������
	int						index;	

public:
	// ��� ������, ������������ � �����������
	scriptModule( scriptProgramm *prg );
	~scriptModule(){ this->clear(); }

	// ������� ������
	void				clear();

	// ������������� ������
	scriptRetCode		initialize( int ind );
	
	// ����������� ������
	scriptRetCode		compile( shString );

};

void clearModuleList( std::list< scriptModule* > &ml );

#endif