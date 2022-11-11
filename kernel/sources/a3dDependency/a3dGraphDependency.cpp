#include "a3dGraphDependency.h"
#include "a3dDependencyProperty.h"

a3dGraphDependency::a3dGraphDependency() : a3dSimpleDependency() { 
	this->type = DEPENDENCY_GRAPH;
	this->graph = null;
	this->graph2 = null;
	this->src2 = null;
	this->loopGraph1 = false;
	this->loopGraph2 = false;
	this->operation = GO_SELECTOP1;
}

a3dGraphDependency::~a3dGraphDependency() { 
	
	if( this->graph != null )
		delete this->graph;
	if( this->graph2 != null )
		delete this->graph2;
}

void a3dGraphDependency::setProperty(a3dDependencyProperty *prop) { 
	
	a3dSimpleDependency::setProperty(prop);
	if( this->src2 != null ) {
		this->src2->addToNotifyList(this->dest);
		this->dest->updateProperty();
	}
}

void a3dGraphDependency::removeProperty() { 
	if(this->src2 != null)
		this->src2->removeFromNotifyList(this->dest);
	
	a3dSimpleDependency::removeProperty();
}

void a3dGraphDependency::setSource2Property(a3dDependencyProperty *prop) { 
	
	removeSource2Property();
	this->src2 = prop;
	if(this->dest != null) {
		this->src2->addToNotifyList(this->dest);
		this->dest->updateProperty();
	}
}

void a3dGraphDependency::removeSource2Property() { 
	if(this->src2 == null) return;
	if(this->dest != null)
		this->src2->removeFromNotifyList(this->dest);
}

void a3dGraphDependency::updateProperty() { 
	
	float res1 = 0.0f;
	float res2 = 0.0f;

	if(this->src != null && this->graph != null) { 
		float val = this->src->toFloat();
		if(this->loopGraph1) {
			float len = this->graph->getLength();
			val = val - floor(val / len) * len;
		}
		res1 = this->graph->calcFloatValue(val);
	}

	if(this->src2 != null && this->graph2 != null) { 
		float val = this->src2->toFloat();
		if(this->loopGraph1) {
			float len = this->graph2->getLength();
			val = val - floor(val / len) * len;
		}
		res2 = this->graph2->calcFloatValue(val);
	}

	res1 = (this->*graphOpFuncArray[this->operation])( res1, res2 );
	(this->dest->*simpleDepValueConversion[DPROP_FLOAT][this->dest->getType()])(&res1);
}