#ifndef __RULE_POST_PROCESSOR_INC
#define __RULE_POST_PROCESSOR_INC

#include < windows.h >
#include "lexeme.h"
#include "scrcode.h"
#include "screrror.h"
#include <list>
#include <stack>

// �������� ������, ������� ����� ���������� ������� ������������
// �������������� �������� �� ��������������� �����������
class rulePostProcessor
{
	
public:
	rulePostProcessor(){};
	~rulePostProcessor(){};

	// ���������� �������
	scriptRetCode	processRule( );

};

#endif