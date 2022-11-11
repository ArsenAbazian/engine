#include "stdafx.h"
#include "a3dModelRefMapper.h"

namespace classMapper { 

	a3dDetailRefMapper::a3dDetailRefMapper(a3dDetailRef *detail) { 
		
		this->detail = detail;
		this->lightMapInfo = gcnew a3dLightMapInfo_Mapper(&Detail->lightMapInfo);
	}

	a3dLightMapInfo_Mapper^ a3dDetailRefMapper::LightMapInfo::get() { 
		
		UpdateLightMapInfo();
		return lightMapInfo;
	}

	void a3dDetailRefMapper::InitializeTextureParamColl() { 
		
		lightMapInfo->TextureParamColl->Clear();
		for(int i = 0; i < this->detail->material.paramCount; i++) { 
			if(this->detail->material.paramList[i]->type == SPT_TEXTURE) { 
				a3dTextureShaderParam *param = dynamic_cast<a3dTextureShaderParam *>(this->detail->material.paramList[i]);
				if(param != null)
					lightMapInfo->TextureParamColl->Add(gcnew String(param->paramName.getCString()));
			}
		}
	}

	void a3dDetailRefMapper::UpdateLightMapInfo() { 
		
		String ^str = String::Concat(gcnew String(this->detail->objectRef->modelRef->name.getCString()), gcnew String("_"), gcnew String(this->detail->objectRef->object->name.getCString()), gcnew String("_"), gcnew String(this->detail->material.name.getCString()));
		lightMapInfo->setAutoFileName(str);
		InitializeTextureParamColl();
	}

	DependencyTreeNodeCollection^ a3dDetailRefMapper::CreateDependencyTreeNodeCollection() { 
		
		DependencyTreeNodeCollection^ coll = gcnew DependencyTreeNodeCollection();
		coll->Add(Material);
		return coll;
	}

	DependencyTreeNodeCollection^ a3dDetailRefMapper::Children::get() { 
	
		if(this->depChildren == nullptr)
			this->depChildren = CreateDependencyTreeNodeCollection();
		return this->depChildren;
	}
}

