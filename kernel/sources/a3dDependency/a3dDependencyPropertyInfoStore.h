#ifndef A3D_DEPENDENCY_PROPERTY_INFO_STORE
#define A3D_DEPENDENCY_PROPERTY_INFO_STORE

#include "3dtypes.h"
#include "hstring.h"

class a3dDependencyPropertyInfoStore { 
public:
	int					itemCount;
	shString			**propertyName;
	shString			**fieldName;
	static const int	maxItemCount = 128;
	
	a3dDependencyPropertyInfoStore() { this->initializeDefault(); }
	~a3dDependencyPropertyInfoStore() { this->clear(); }

	void				initializeDefault();
	void				clear();

	bool				registerPropertyInfo( TCHAR *pName, TCHAR *fName );
};

#endif