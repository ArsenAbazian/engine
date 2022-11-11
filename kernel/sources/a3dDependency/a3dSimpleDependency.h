#ifndef A3D_SIMPLE_DEPENDENCY_INC
#define A3D_SIMPLE_DEPENDENCY_INC

#include "a3dDependencyBase.h"

class a3dSimpleDependency : public a3dDependencyBase { 
public:
	a3dDependencyProperty	*src;
	
	a3dSimpleDependency();

	void setProperty(a3dDependencyProperty *prop);
	void removeProperty();
	void setSourceProperty(a3dDependencyProperty *prop);
	void removeSourceProperty();
	void updateProperty();
};

#endif