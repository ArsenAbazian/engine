#include "scopeDesc.h"

scriptScopeDesc::scriptScopeDesc()
{
	this->name = TEXT( "" );
}

scriptScopeDesc::~scriptScopeDesc()
{
	this->name.clear();
	clearObjList( this->objectList );
}