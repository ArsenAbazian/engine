#ifndef A3D_SCOPE_INFO_INC
#define A3D_SCOPE_INFO_INC

#include "objectInfo.h"

class scriptScopeInfo : public scriptObjectInfo { 
public:
	shString	name;
	shString	fullName;

	scriptScopeInfo() { }
	
};

#endif

