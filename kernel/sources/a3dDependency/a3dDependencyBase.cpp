#include "a3dDependencyBase.h"

void a3dDependencyBase::setProperty(a3dDependencyProperty *prop) { 
	if(this->dest != null)
		this->removeProperty();
	this->dest = prop;
}

void a3dDependencyBase::removeProperty() { 
	this->dest = null;
}

void a3dDependencyBase::updateProperty() { }