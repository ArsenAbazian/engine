#ifndef A3D_SHADER_PARAM_BASE_INC
#define A3D_SHADER_PARAM_BASE_INC

#include "serialization.h"
#include "a3dShaderParamType.h"
#include "..\\texture.h"
#include "..\\a3dDesc\\a3dEffectDesc.h"
#include "..\\a3dDesc\\a3dTextureDesc.h"
#include "..\\fxFileParser\\fxFileParser.h"
#include <d3dx9effect.h>
#include "..\\a3dDependency\\a3dDependencyPropertyOwner.h"
#include "..\\a3dDependency\\a3dDependencyProperty.h"

class a3dShaderMaterial;
class a3dShaderParamBase : public a3dDependencyPropertyOwner { 
	// current version is 1.3
public:
	a3dShaderMaterial			*material;
	a3dShaderParamType			type;
	shString					paramName;
	char						name[128];
	bool						used;
	a3dShaderParamWidgetType	uiWidget;		// since 1.1
	
	shString					uiName;			// since 1.1
	shString					resourceType;	// since 1.1
	shString					object;			// since 1.1
	shString					space;			// since 1.1

	int							row;			// since 1.1
	int							col;			// since 1.1
	int							arraySize1;		// since 1.1
	int							arraySize2;		// since 1.1
	int							arraySize3;		// since 1.1

	shString					token;			// since 1.2

	a3dShaderParamBase( shString &paramName ) { 
		this->material = null;
		this->uiWidget = SPWT_NONE;
		this->used = true;
		this->type = SPT_BASE;
		this->row = 1; this->col = 1;
		this->arraySize1 = this->arraySize2 = this->arraySize3 = 1;
		this->paramName = paramName;
		paramName.copyToChar( name );
		this->version = a3dVersion(1,3);
	}
	a3dShaderParamBase( fxParamInfo *info );
	~a3dShaderParamBase() { }

	void    setMaterial(a3dShaderMaterial *material) { this->material = material; }
	
	virtual a3dShaderParamBase* clone();
	virtual void				assign(a3dShaderParamBase*);
	virtual HRESULT setParam( a3dEffectDesc *effect ) { return D3D_OK; }
	virtual HRESULT getParam( a3dEffectDesc *effect ) { return D3D_OK; }

	int		getHeaderCode() { return a3dHeaders::a3dShaderParamBaseHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file ); 
	bool	loadFromMemoryCore( a3dMemoryFile &file );

	void	initializeUIInfo( fxParamInfo *info );

	virtual void registerDependencyProperties();
	virtual shString& getName() { return this->paramName; }
};

#endif
