#ifndef __SCRIPT_SCOPE_INC
#define __SCRIPT_COPE_INC

#include "hstring.h"
#include "objDesc.h"
#include "scrcode.h"
#include <list>

class scriptScopeDesc
{

	std::list< scriptObjectDesc* >		objectList;		// список объектов, которые будут найдены в данной области видимости
	shString							name;			// имя области видимости

public:
	scriptScopeDesc();
	~scriptScopeDesc();

	scriptRetCode						addObject( scriptObjectDesc &oDesc );

};

#endif