#include "typeDesc.h"

scriptTypeDesc::scriptTypeDesc()
{
	this->refCount			= 0;
	this->flags				= STF_NONE;
	this->objectType		= ST_NONE;
}

scriptTypeDesc::~scriptTypeDesc()
{
	this->clear();
}

scriptTypeDesc& scriptTypeDesc::operator = ( scriptTypeDesc &dsk )
{
	this->refCount			= dsk.refCount;
	this->flags				= dsk.flags;
	this->objectType		= dsk.objectType;

	return *this;
}

void scriptTypeDesc::clear()
{
	this->refCount			= 0;
	this->flags				= STF_NONE;
	this->objectType		= ST_NONE;
}