#ifndef A3D_TEXTURE_SHADER_PARAM_INC
#define A3D_TEXTURE_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dTextureShaderParam : public a3dShaderParamBase { 
public:
	a3dTextureDesc *textureDesc;
	shString	fileName;

	a3dTextureShaderParam( shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_TEXTURE;
		this->textureDesc = null;
	}
	a3dTextureShaderParam( shString &paramName, a3dTextureDesc *textDesc ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_TEXTURE;
		this->textureDesc = textDesc;
	}
	a3dTextureShaderParam( shString &paramName, shString &fileName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_TEXTURE;
		this->fileName = fileName;
		this->textureDesc = null;
	}
	a3dTextureShaderParam(fxParamInfo *info);

	a3dShaderParamBase* clone();
	void assign(a3dShaderParamBase*);
	HRESULT setParam( a3dEffectDesc *effect ) { 
		if(this->textureDesc == null) return D3D_OK;
		return effect->lpEffect->SetTexture( name, this->textureDesc->texture->lpTexture ); 
	}
	HRESULT getParam( a3dEffectDesc *effect ) { 
		if(this->textureDesc == null) return D3D_OK;
		return effect->lpEffect->GetTexture( name, (LPDIRECT3DBASETEXTURE9*)&this->textureDesc->texture->lpTexture ); 
	}

	int		getHeaderCode() { return a3dHeaders::a3dTextureShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif