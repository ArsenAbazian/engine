#ifndef A3D_DEPENDENCY_PROPERTY_OWNER
#define A3D_DEPENDENCY_PROPERTY_OWNER

#include "3dtypes.h"
#include "hstring.h"
#include "a3dDependencyPropType.h"
#include "a3dDependencyPropertyInfoStore.h"
#include "..\\headerCodes.h"

class a3dDependencyProperty;

class a3dDependencyPropertyOwner : public a3dSerializableObject { 
	static a3dDependencyPropertyInfoStore	*propertyInfoStore;
	a3dDependencyProperty					**propertyList;
	int										propertyCount;
public:	
	a3dDependencyPropertyOwner() { this->initializeDefault(); }
	~a3dDependencyPropertyOwner() { this->clear(); }

	void					initializeDefault();
	void					clear();
	a3dDependencyProperty*	registerDependencyProperty(TCHAR *propName, TCHAR *fieldName, a3dDependencyPropertyType type, BYTE *fieldPtr);
	a3dDependencyProperty*	registerDependencyProperty(TCHAR *propName, TCHAR *fieldName, a3dDependencyPropertyType type, float *fieldPtr);

	a3dDependencyProperty*	getProperty( TCHAR *propName );
	a3dDependencyProperty*  getProperty( shString &propName );
	int						getPropertyCount() { return this->propertyCount; }
	a3dDependencyProperty*	getProperty( int index ) { return index < propertyCount? this->propertyList[index]: null; }

	void					setFloat( shString &propName, float value );
	void					setBool( shString &propName, bool value );
	void					setInt( shString &propName, int value );

	float					getFloat( shString &propName );
	bool					getBool( shString &propName );
	int						getInt( shString &propName );

	virtual shString&		getName() = 0;			

	int						getHeaderCode() { return a3dHeaders::a3dDependencyPropertyOwnerHeader; }
	int						calcSizeCore();
	bool					loadFromMemoryCore( a3dMemoryFile &file );
	bool					saveToMemoryCore( a3dMemoryFile &file );
};

#endif