#ifndef __SCRIPT_MACRO_INC
#define __SCRIPT_MACRO_INC

#include <windows.h>
#include <tchar.h>
#include <list>
#include "fileinfo.h"
#include "hstring.h"
#include "lexeme.h"
#include "scrcode.h"

// �������� ������������������ ������ �� ������� ��������� �������
typedef struct
{
	std::list< scriptLexeme* >::iterator	begin;
	std::list< scriptLexeme* >::iterator	end;
	int										size;
}macrosInputParam;

class scriptMacros	// ����� ��������� �������
{
public:

	shString									name;			// ��� �������
	long										fileIndex;		// ������ ����� � ������� ��� ��������� ������
	int											line;			// ����� �� ������� ����� ������
	int											col;			// ������� �� ������� ����� ������
	std::list< scriptLexeme* >					params;			// ��������� �������
	std::list< scriptLexeme* >					body;			// ���� �������
	macrosInputParam							inputList;		// ������� ������ �������
	std::list< macrosInputParam >				input;			// ������� ��������� � ������
	bool										called;			// ������ ���� �� ���������� ������ ������
	bool										isSystem;		// ������ ������� ��� �������� � ���������� ������ 

	scriptMacros();
	~scriptMacros();

	void						clear();						// ������� ������� 
	scriptMacros&				copy( scriptMacros &sm );		// ����������� �������
	bool						isParam( scriptLexeme &lex, std::list< macrosInputParam >::iterator &mipi );	// ���������� ������, ���� ��������� ������� - �������� ������� 
	
};

#endif