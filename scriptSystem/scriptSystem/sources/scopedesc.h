#ifndef __SCRIPT_SCOPE_INC
#define __SCRIPT_SCOPE_INC

#include "hstring.h"
#include "objDesc.h"
#include "scrcode.h"
#include <list>

class scriptScopeDesc
{
public:
	std::list< scriptObjectDesc* >		objectList;		// ������ ��������, ������� ����� ������� � ������ ������� ���������
	shString							name;			// ��� ������� ���������

	scriptScopeDesc();
	~scriptScopeDesc();

	scriptRetCode						addObject( scriptObjectDesc &oDesc );
};

#endif