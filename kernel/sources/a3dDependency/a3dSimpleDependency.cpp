#include "a3dSimpleDependency.h"
#include "a3dDependencyProperty.h"

a3dSimpleDependency::a3dSimpleDependency() : a3dDependencyBase() { 
	this->src = null;
	this->type = DEPENDENCY_SIMPLE;
}

void a3dSimpleDependency::setProperty(a3dDependencyProperty *prop) { 
	
	a3dDependencyBase::setProperty(prop);
	if( this->src != null ) {
		this->src->addToNotifyList(this->dest);
		this->dest->updateProperty();
	}
}

void a3dSimpleDependency::removeProperty() { 
	
	if(this->src != null)
		this->src->removeFromNotifyList(this->dest);
	
	a3dDependencyBase::removeProperty();
}

void a3dSimpleDependency::setSourceProperty(a3dDependencyProperty *prop) { 
	
	if( this->src != null )
		this->src->removeFromNotifyList(this->dest);
	this->src = prop;
	if(this->dest != null) {
		this->src->addToNotifyList(this->dest);
		this->dest->updateProperty();
	}
}

void a3dSimpleDependency::removeSourceProperty() { 
	if(this->src == null) return;
	if(this->dest != null)
		this->src->removeFromNotifyList(this->dest);
}

void a3dSimpleDependency::updateProperty() { 

	(this->dest->*simpleDepConversion[src->getType()][dest->getType()])(this->src);
}