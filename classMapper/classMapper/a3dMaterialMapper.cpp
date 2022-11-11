#include "stdafx.h"
#include "a3dMaterialMapper.h"

namespace classMapper { 
	
	void a3dShaderMaterialMapper::InitializeParamCollection() { 
		
		for( int i = 0; i < ShaderMaterial->paramCount; i++ ) { 
			switch(ShaderMaterial->paramList[i]->type) { 
				case SPT_BOOL:
					Params->Add( gcnew a3dBoolShaderParamMapper( (a3dBoolShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_INT:
					Params->Add( gcnew a3dIntShaderParamMapper( (a3dIntShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_FLOAT:
					Params->Add( gcnew a3dFloatShaderParamMapper( (a3dFloatShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_TEXTURE:
					Params->Add( gcnew a3dTextureShaderParam_Mapper( (a3dTextureShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_TECHNIQUE:
					Params->Add( gcnew a3dTechniqueShaderParam_Mapper( (a3dTechniqueShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_FLOAT_ARRAY:
					Params->Add( gcnew a3dFloatArrayShaderParamMapper( (a3dFloatArrayShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_INT_ARRAY:
					Params->Add( gcnew a3dIntArrayShaderParamMapper( (a3dIntArrayShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_BOOL_ARRAY:
					Params->Add( gcnew a3dBoolArrayShaderParamMapper( (a3dBoolArrayShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_COLOR:
					Params->Add( gcnew a3dColorShaderParam_Mapper( (a3dColorShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_VECTOR:
					Params->Add( gcnew a3dVectorShaderParam_Mapper( (a3dVectorShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				case SPT_MATRIX:
					Params->Add( gcnew a3dMatrixShaderParam_Mapper( (a3dMatrixShaderParam*)ShaderMaterial->paramList[i] ) );
					break;
				default:
					Params->Add( gcnew a3dShaderParamBaseMapper( ShaderMaterial->paramList[i] ) );
			}
		}  
		for(int i = 0; i < Params->Count; i++) { 
			Params[i]->Material = this;
		}
	}

	a3dShaderMaterialMapper::a3dShaderMaterialMapper( a3dShaderMaterial *mat ) : a3dMaterialBaseMapper(mat) { 
	
		paramColl = gcnew a3dShaderParamBaseMapperCollection();
		techList = gcnew StringCollection();
		InitializeParamCollection();
		InitializeTechniqueList();
	}
	
	void a3dShaderMaterialMapper::InitializeTechniqueList() { 
	
		for(int i = 0; i < ShaderMaterial->techniqueCount; i++) { 
			techList->Add(gcnew String(ShaderMaterial->techniqueList[i].getCString()));
		}
	}

	DependencyTreeNodeCollection^ a3dShaderMaterialMapper::Children::get() { 
		
		if( this->children != nullptr )
			return this->children;
		
		this->children = gcnew DependencyTreeNodeCollection();
		for( int i = 0; i < this->Params->Count; i++ ) { 
			this->children->Add(this->Params[i]);
		}
	
		return this->children;
	}
}