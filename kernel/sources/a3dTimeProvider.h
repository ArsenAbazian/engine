#ifndef A3D_TIME_PROVIDER
#define A3D_TIME_PROVIDER

#include "a3dDependency\\a3dDependencyPropertyOwner.h"

class a3dTimeProviderBase : public a3dDependencyPropertyOwner { 
	static shString className;
public:
	float fTime;
	
	a3dDependencyProperty *timeProperty;

	a3dTimeProviderBase() { this->initializeDefault(); }
	
	void initializeDefault();
	virtual void registerDependencyProperties();
	virtual shString& getName() { return className; }
}; 

#endif