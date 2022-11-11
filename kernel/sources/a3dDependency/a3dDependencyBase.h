#ifndef A3D_DEPENDENCY_INC
#define A3D_DEPENDENCY_INC

#include "3dtypes.h"
#include "a3dDependencyType.h"

class a3dDependencyProperty;
class a3dDependencyBase { 
public:
	a3dDependencyType		type;
	a3dDependencyProperty	*dest;
	a3dDependencyBase() { 
		this->dest = null;
		this->type = DEPENDENCY_BASE;
	}	

	virtual void setProperty(a3dDependencyProperty *prop);
	virtual void removeProperty();
	virtual void updateProperty();
};

#endif