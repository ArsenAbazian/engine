// This is the main DLL file.

#include "stdafx.h"

#include "shaderParams.h"
#include "DescMapper.h"
#include "CollectionEditorForm.h"

namespace classMapper { 
	
	a3dTextureDesc_Mapper^ a3dTextureShaderParam_Mapper::TextureDesc::get() { 
		return gcnew a3dTextureDesc_Mapper(TextureParam->textureDesc); 
	}
	void a3dTextureShaderParam_Mapper::TextureDesc::set(a3dTextureDesc_Mapper^ value) { 
		TextureParam->textureDesc = value->TexDesc;
		ObjectChanged(this, EventArgs::Empty);
	}
}