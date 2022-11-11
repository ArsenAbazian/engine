#include "a3dDependencyProperty.h"
#include "..\\a3dGraph\\a3dGraph.h"
#include "memoryFile.h"
#include "a3dDependencyBase.h"
#include "a3dSimpleDependency.h"

void a3dDependencyProperty::initializeDefault() {
	
	this->type = DPROP_NONE;
	this->owner = null;
	this->name = null;
	this->nativeFieldName = null;
	this->boolValuePtr = null;
	this->dependency = null;
}

void a3dDependencyProperty::clear() { 
	this->initializeDefault();
}

a3dDependencyProperty::a3dDependencyProperty(shString *propName, shString *fieldName, a3dDependencyPropertyType type, a3dDependencyPropertyOwner *owner, BYTE *fieldPtr) { 
	
	this->initializeDefault();

	this->name = propName;
	this->nativeFieldName = fieldName;
	this->type = type;
	this->owner = owner;
	this->ptr = fieldPtr;
}

void a3dDependencyProperty::onPropertyChanged() { 

}

int	a3dDependencyProperty::calcSizeCore() { 

	int size = 0;
	
	if( this->type == DPROP_BOOL )
		size += sizeof( bool );
	else if( this->type == DPROP_INT )
		size += sizeof( int );
	else if( this->type == DPROP_FLOAT )
		size += sizeof( float );

	return size;
}

bool a3dDependencyProperty::loadFromMemoryCore(a3dMemoryFile &file) { 

	int tp;

	if( !file.readInt( &tp ) ) return false;
	this->type = (a3dDependencyPropertyType) tp;
	if( this->type == DPROP_BOOL && !file.readBool( this->boolValuePtr ) ) return false;
	if( this->type == DPROP_INT && !file.readInt( this->intValuePtr ) ) return false;
	if( this->type == DPROP_FLOAT && !file.readFloat( this->floatValuePtr ) ) return false;

	return true;
}

bool a3dDependencyProperty::saveToMemoryCore(a3dMemoryFile &file) { 
	
	if( !file.writeInt( (int)this->type ) ) return false;
	if( this->type == DPROP_BOOL && !file.writeBool( *(this->boolValuePtr) ) ) return false;
	if( this->type == DPROP_INT && !file.writeInt( *(this->intValuePtr) ) ) return false;
	if( this->type == DPROP_FLOAT && !file.writeFloat( *(this->floatValuePtr) ) ) return false;

	return true;
}

int a3dDependencyProperty::getPropertyIndexInNotifyList(a3dDependencyProperty *prop) { 
	
	for(int i = 0; i < this->notifyList.count; i++) { 
		if(this->notifyList.lpArray[i] == (int)prop) return i;
	}

	return -1;
}

void a3dDependencyProperty::addToNotifyList(a3dDependencyProperty *prop) { 
	
	if(this->getPropertyIndexInNotifyList(prop) >= 0) 
		return;
	
	this->notifyList.addValue((int)prop);
	prop->updateProperty();
}

void a3dDependencyProperty::removeFromNotifyList(a3dDependencyProperty *prop) { 
	
	this->notifyList.removeValueByIndex(this->getPropertyIndexInNotifyList(prop));
}

void a3dDependencyProperty::none2None(a3dDependencyProperty *prop) { }
void a3dDependencyProperty::none2Bool(a3dDependencyProperty *prop) { this->setBoolValue( false ); }
void a3dDependencyProperty::none2Float(a3dDependencyProperty *prop) { this->setFloatValue( 0.0f ); }
void a3dDependencyProperty::none2Int(a3dDependencyProperty *prop) { this->setIntValue( 0 ); }

void a3dDependencyProperty::bool2None(a3dDependencyProperty *prop) { }
void a3dDependencyProperty::bool2Bool(a3dDependencyProperty *prop) { this->setBoolValue( prop->getBoolValue() ); }
void a3dDependencyProperty::bool2Float(a3dDependencyProperty *prop) { this->setFloatValue( prop->getBoolValue()? 1.0f: 0.0f ); }
void a3dDependencyProperty::bool2Int(a3dDependencyProperty *prop) { this->setIntValue( (int)(prop->getBoolValue()) ); }

void a3dDependencyProperty::float2None(a3dDependencyProperty *prop) { }
void a3dDependencyProperty::float2Bool(a3dDependencyProperty *prop) { this->setBoolValue( prop->getFloatValue() == 0.0f ); }
void a3dDependencyProperty::float2Float(a3dDependencyProperty *prop) { this->setFloatValue( prop->getFloatValue() ); }
void a3dDependencyProperty::float2Int(a3dDependencyProperty *prop) { this->setIntValue( (int)(prop->getFloatValue()) ); }

void a3dDependencyProperty::int2None(a3dDependencyProperty *prop) { }
void a3dDependencyProperty::int2Bool(a3dDependencyProperty *prop) { this->setBoolValue( prop->getIntValue() == 0 ); }
void a3dDependencyProperty::int2Float(a3dDependencyProperty *prop) { this->setFloatValue( prop->getIntValue() ); }
void a3dDependencyProperty::int2Int(a3dDependencyProperty *prop) { this->setIntValue( prop->getIntValue() ); }


void a3dDependencyProperty::noneValue2None(void *val) { }
void a3dDependencyProperty::noneValue2Bool(void *val) { this->setBoolValue( false ); }
void a3dDependencyProperty::noneValue2Float(void *val) { this->setFloatValue( 0.0f ); }
void a3dDependencyProperty::noneValue2Int(void *val) { this->setIntValue( 0 ); }

void a3dDependencyProperty::boolValue2None(void *val) { }
void a3dDependencyProperty::boolValue2Bool(void *val) { this->setBoolValue( *((bool*)val) ); }
void a3dDependencyProperty::boolValue2Float(void *val) { this->setFloatValue( (*((bool*)val))? 1.0f: 0.0f ); }
void a3dDependencyProperty::boolValue2Int(void *val) { this->setIntValue( (int)(*((bool*)val)) ); }

void a3dDependencyProperty::floatValue2None(void *val) { }
void a3dDependencyProperty::floatValue2Bool(void *val) { this->setBoolValue( *((float*)val) == 0.0f ); }
void a3dDependencyProperty::floatValue2Float(void *val) { this->setFloatValue( *((float*)val) ); }
void a3dDependencyProperty::floatValue2Int(void *val) { this->setIntValue( (int)(*((float*)val)) ); }

void a3dDependencyProperty::intValue2None(void *val) { }
void a3dDependencyProperty::intValue2Bool(void *val) { this->setBoolValue( *((int*)val) == 0 ); }
void a3dDependencyProperty::intValue2Float(void *val) { this->setFloatValue( *((int*)val) ); }
void a3dDependencyProperty::intValue2Int(void *val) { this->setIntValue( *((int*)val) ); }

void a3dDependencyProperty::attachDependency( a3dDependencyBase *dep ) { 
	
	if(this->dependency != null)
		this->removeDependency();

	this->dependency = dep;
	this->dependency->setProperty(this);
}

void a3dDependencyProperty::removeDependency() { 
	
	if(this->dependency != null)
		this->dependency->removeProperty();
}

void a3dDependencyProperty::updateProperty() { 
	if(this->dependency != null)
		this->dependency->updateProperty();
}

void a3dDependencyProperty::notifyProperties() { 
	
	for(int i = 0; i < this->notifyList.count; i++) { 
		a3dDependencyProperty *prop = (a3dDependencyProperty *)this->notifyList.lpArray[i];
		prop->updateProperty();
	}
}

void a3dDependencyProperty::simpleBind(a3dDependencyProperty *prop) { 
	
	this->removeDependency();	
	a3dSimpleDependency *dep = new a3dSimpleDependency();
	dep->setProperty( this );
	dep->setSourceProperty( prop );
	
	this->attachDependency( dep );
}

float a3dDependencyProperty::toFloat() { 
	
	switch( this->type ) { 
		case DPROP_BOOL: return *(this->boolValuePtr)? 1.0f: 0.0f;
		case DPROP_INT: return *(this->intValuePtr);
		case DPROP_FLOAT: return *(this->floatValuePtr);
	}
	return 0.0f;
}