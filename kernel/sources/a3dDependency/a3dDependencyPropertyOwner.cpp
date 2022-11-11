#include "a3dDependencyPropertyOwner.h"
#include "a3dDependencyProperty.h"
#include "memoryFile.h"

a3dDependencyPropertyInfoStore* a3dDependencyPropertyOwner::propertyInfoStore = new a3dDependencyPropertyInfoStore();

void a3dDependencyPropertyOwner::initializeDefault() { 
	
	this->propertyList = null;
	this->propertyCount = 0;
}

void a3dDependencyPropertyOwner::clear() { 
	
	if(this->propertyList != null) { 
		for(int i = 0; i < this->propertyCount; i++) { 
			if(this->propertyList[i] != null) 
				delete this->propertyList[i];
		}
		delete[] this->propertyList;
	}
	
	this->propertyList = null;
	this->propertyCount = 0;
}

a3dDependencyProperty* a3dDependencyPropertyOwner::registerDependencyProperty(TCHAR *propName, TCHAR *fieldName, a3dDependencyPropertyType type, float *fieldPtr) { 
	return this->registerDependencyProperty(propName, fieldName, type, (BYTE*)fieldPtr);
}

a3dDependencyProperty* a3dDependencyPropertyOwner::registerDependencyProperty(TCHAR *propName, TCHAR *fieldName, a3dDependencyPropertyType type, BYTE *fieldPtr) { 
	
	if( this->propertyList == null ) {
		this->propertyList = new a3dDependencyProperty*[a3dDependencyPropertyOwner::propertyInfoStore->maxItemCount];
		ZeroMemory(this->propertyList, sizeof( a3dDependencyProperty* ) * a3dDependencyPropertyOwner::propertyInfoStore->maxItemCount);		
	}

	if(this->propertyCount == a3dDependencyPropertyOwner::propertyInfoStore->maxItemCount) return null;
	a3dDependencyProperty *prop = this->getProperty(propName);
	if( prop != null) return prop;
	
	if( !this->propertyInfoStore->registerPropertyInfo(propName, fieldName) ) return null;
	
	this->propertyList[this->propertyCount] = new a3dDependencyProperty( this->propertyInfoStore->propertyName[this->propertyInfoStore->itemCount-1], 
																			this->propertyInfoStore->fieldName[this->propertyInfoStore->itemCount-1],
																			type, this, fieldPtr);

	this->propertyCount ++;

	return this->propertyList[this->propertyCount - 1];
}

a3dDependencyProperty* a3dDependencyPropertyOwner::getProperty(TCHAR *propName ) { return this->getProperty(shString(propName)); }

a3dDependencyProperty* a3dDependencyPropertyOwner::getProperty(shString &propName ) { 
	
	for(int i = 0; i < this->propertyCount; i++) { 
		if(propName == this->getProperty(i)->getName())
			return this->getProperty(i);
	}

	return null;
}

void a3dDependencyPropertyOwner::setFloat( shString &propName, float value ) { 

	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return;

	prop->setFloatValue(value);
}

void a3dDependencyPropertyOwner::setBool( shString &propName, bool value ) { 

	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return;

	prop->setBoolValue(value);
}

void a3dDependencyPropertyOwner::setInt( shString &propName, int value ) { 
	
	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return;

	prop->setIntValue(value);
}

float a3dDependencyPropertyOwner::getFloat( shString &propName) { 

	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return 0.0f;

	return prop->getFloatValue();
}

bool a3dDependencyPropertyOwner::getBool( shString &propName ) { 

	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return false;

	return prop->getBoolValue();
}

int a3dDependencyPropertyOwner::getInt( shString &propName ) { 

	a3dDependencyProperty *prop = this->getProperty( propName );
	if(prop == null) return 0;

	return prop->getIntValue();
}

int a3dDependencyPropertyOwner::calcSizeCore() {  

	int size = sizeof( int );
	for( int i = 0; i < this->propertyCount; i++ ) { 
		size += this->propertyList[i]->calcSize();
	}
	return size;
}

bool a3dDependencyPropertyOwner::saveToMemoryCore( a3dMemoryFile &file ) { 
	
	if( !file.writeInt( this->propertyCount ) ) return false;
	for( int i = 0; i < this->propertyCount; i++ ) { 
		if( !this->propertyList[i]->saveToMemory( file ) ) return false;
	}

	return true;
} 

bool a3dDependencyPropertyOwner::loadFromMemoryCore( a3dMemoryFile &file ) { 
	
	if( !file.readInt( &this->propertyCount ) ) return false;
	for( int i = 0; i < this->propertyCount; i++ ) { 
		if( !this->propertyList[i]->loadFromMemory( file ) ) return false;
	}

	return true;
}