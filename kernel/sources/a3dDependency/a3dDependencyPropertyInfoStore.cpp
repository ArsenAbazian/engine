#include "a3dDependencyPropertyInfoStore.h"

void a3dDependencyPropertyInfoStore::initializeDefault() { 
	
	this->itemCount = 0;
	this->propertyName = new shString*[this->maxItemCount];
	this->fieldName = new shString*[this->maxItemCount];

	ZeroMemory(this->propertyName, sizeof(shString*) * this->maxItemCount);
	ZeroMemory(this->fieldName, sizeof(shString*) * this->maxItemCount);
}

void a3dDependencyPropertyInfoStore::clear() { 
	
	for(int i = 0; i < this->itemCount; i++) { 
		if( this->propertyName[i] != null )
			delete this->propertyName[i];

		if( this->fieldName[i] != null )
			delete this->fieldName[i];
	}

	delete[] this->propertyName;
	delete[] this->fieldName;
	
	this->itemCount = 0;
	this->propertyName = null;
	this->fieldName = null;
}

bool a3dDependencyPropertyInfoStore::registerPropertyInfo(TCHAR *pName, TCHAR *fName) { 

	if( this->itemCount == this->maxItemCount ) return false;
	
	this->propertyName[this->itemCount] = new shString(pName);
	this->fieldName[this->itemCount] = new shString(fName);
	
	this->itemCount++;

	return true;
}