#ifndef A3D_DEPENDENCY_PROPERTY
#define A3D_DEPENDENCY_PROPERTY

#include "a3dDependencyPropType.h"
#include "a3dDependencyPropertyOwner.h"
#include "3dtypes.h"
#include "hstring.h"
#include "serialization.h"
#include "..\\headerCodes.h"
#include "..\\a3dUtils\\resizeableArray.h"

class a3dGraph;
class a3dDependencyBase;
class a3dDependencyProperty : public a3dSerializableObject { 
	a3dDependencyPropertyType		type;
	shString						*name;
	a3dDependencyPropertyOwner		*owner;
	shString						*nativeFieldName;
	
	resizeableArray					notifyList;

	union {
		BYTE						*ptr;
		bool						*boolValuePtr;
		int							*intValuePtr;
		float						*floatValuePtr;
	};

	a3dDependencyBase				*dependency;

	void							addToNotifyList(a3dDependencyProperty *prop);
	void							removeFromNotifyList(a3dDependencyProperty *prop);
	int								getPropertyIndexInNotifyList(a3dDependencyProperty *prop);
	void							notifyProperties();

	friend class					a3dDependencyBase;
	friend class					a3dSimpleDependency;
	friend class					a3dGraphDependency;

public:
	a3dDependencyProperty( shString *propName, shString *fieldName, a3dDependencyPropertyType type, a3dDependencyPropertyOwner *owner, BYTE *fieldPtr );
	a3dDependencyProperty() { this->initializeDefault(); }
	~a3dDependencyProperty() { this->clear(); }

	resizeableArray&				getNotifyList() { return this->notifyList; }
	void							initializeDefault();
	void							clear();

	shString&						getName() { return *(this->name); }
	shString&						getNativeFieldName() { return *(this->nativeFieldName); }
	a3dDependencyPropertyType		getType() { return this->type; }
	
	void							setFloatValue( float value ) { 
										if(*this->floatValuePtr == value) return;
										*this->floatValuePtr = value; this->notifyProperties(); 
									}
	void							setBoolValue( bool value ) { 
										if(*this->boolValuePtr == value) return;
										*this->boolValuePtr = value; this->notifyProperties(); 
									}
	void							setIntValue( int value ) { 
										if(*this->intValuePtr == value)	return;
										*this->intValuePtr = value; this->notifyProperties(); 
									}

	void							onPropertyChanged();

	float							getFloatValue() { return *(this->floatValuePtr); }
	bool							getBoolValue() { return *(this->boolValuePtr); }
	int								getIntValue() { return *(this->intValuePtr); }
	a3dDependencyBase				*getDependency() { return this->dependency; }

	//serialization					
	int								getHeaderCode() { return a3dHeaders::a3dDependencyPropertyHeader; }
	int								calcSizeCore();
	bool							loadFromMemoryCore(a3dMemoryFile &file);
	bool							saveToMemoryCore(a3dMemoryFile &file);
	
	void							attachDependency( a3dDependencyBase *dep );
	void							removeDependency();
	void							simpleBind( a3dDependencyProperty *prop );

	void							updateProperty();

	void							none2None(a3dDependencyProperty *prop);
	void 							none2Bool(a3dDependencyProperty *prop);
	void 							none2Float(a3dDependencyProperty *prop);
	void 							none2Int(a3dDependencyProperty *prop);

	void 							bool2None(a3dDependencyProperty *prop);
	void 							bool2Bool(a3dDependencyProperty *prop);
	void 							bool2Float(a3dDependencyProperty *prop);
	void 							bool2Int(a3dDependencyProperty *prop);

	void 							float2None(a3dDependencyProperty *prop);
	void 							float2Bool(a3dDependencyProperty *prop);
	void 							float2Float(a3dDependencyProperty *prop);
	void 							float2Int(a3dDependencyProperty *prop);
	
	void 							int2None(a3dDependencyProperty *prop);
	void 							int2Bool(a3dDependencyProperty *prop);
	void 							int2Float(a3dDependencyProperty *prop);
	void 							int2Int(a3dDependencyProperty *prop);

	void							noneValue2None(void *val);
	void 							noneValue2Bool(void *val);
	void 							noneValue2Float(void *val);
	void 							noneValue2Int(void *val);

	void 							boolValue2None(void *val);
	void 							boolValue2Bool(void *val);
	void 							boolValue2Float(void *val);
	void 							boolValue2Int(void *val);

	void 							floatValue2None(void *val);
	void 							floatValue2Bool(void *val);
	void 							floatValue2Float(void *val);
	void 							floatValue2Int(void *val);
	
	void 							intValue2None(void *val);
	void 							intValue2Bool(void *val);
	void 							intValue2Float(void *val);
	void 							intValue2Int(void *val);

	float							toFloat();
};

typedef void (a3dDependencyProperty::*depPropConvFunc)(a3dDependencyProperty *prop);
typedef void (a3dDependencyProperty::*depValueConvFunc)(void*);

static depValueConvFunc simpleDepValueConversion[4][4] = { 
	{
		&a3dDependencyProperty::noneValue2None,
		&a3dDependencyProperty::noneValue2Bool,
		&a3dDependencyProperty::noneValue2Float,
		&a3dDependencyProperty::noneValue2Int,
	},
	{
		&a3dDependencyProperty::boolValue2None,
		&a3dDependencyProperty::boolValue2Bool,
		&a3dDependencyProperty::boolValue2Float,
		&a3dDependencyProperty::boolValue2Int,
	},
	{
		&a3dDependencyProperty::floatValue2None,
		&a3dDependencyProperty::floatValue2Bool,
		&a3dDependencyProperty::floatValue2Float,
		&a3dDependencyProperty::floatValue2Int,
	},
	{
		&a3dDependencyProperty::intValue2None,
		&a3dDependencyProperty::intValue2Bool,
		&a3dDependencyProperty::intValue2Float,
		&a3dDependencyProperty::intValue2Int,
	}
};

static depPropConvFunc simpleDepConversion[4][4] = { 

	{
		&a3dDependencyProperty::none2None,
		&a3dDependencyProperty::none2Bool,
		&a3dDependencyProperty::none2Float,
		&a3dDependencyProperty::none2Int,
	},
	{
		&a3dDependencyProperty::bool2None,
		&a3dDependencyProperty::bool2Bool,
		&a3dDependencyProperty::bool2Float,
		&a3dDependencyProperty::bool2Int,
	},
	{
		&a3dDependencyProperty::float2None,
		&a3dDependencyProperty::float2Bool,
		&a3dDependencyProperty::float2Float,
		&a3dDependencyProperty::float2Int,
	},
	{
		&a3dDependencyProperty::int2None,
		&a3dDependencyProperty::int2Bool,
		&a3dDependencyProperty::int2Float,
		&a3dDependencyProperty::int2Int,
	}
};

#endif