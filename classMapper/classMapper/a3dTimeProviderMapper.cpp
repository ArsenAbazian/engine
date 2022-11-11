#include "stdafx.h"
#include "a3dTimeProviderMapper.h"
#include "stringHelper.h"

namespace classMapper {
	
	void a3dTimeProviderBaseMapper::SetFloat(String ^propName, float value) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		this->native->setFloat(str, value);
	}

	void a3dTimeProviderBaseMapper::SetInt(String ^propName, int value) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		this->native->setInt(str, value);
	}

	void a3dTimeProviderBaseMapper::SetBool(String ^propName, bool value) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		this->native->setBool(str, value);
	}

	float a3dTimeProviderBaseMapper::GetFloat(String ^propName) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		return this->native->getFloat(str);
	}

	int a3dTimeProviderBaseMapper::GetInt(String ^propName) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		return this->native->getInt(str);
	}

	bool a3dTimeProviderBaseMapper::GetBool(String ^propName) { 
		
		shString str;
		shStringHelper::Default.copyTo(str, propName);
		return this->native->getBool(str);
	}

	String^ a3dTimeProviderBaseMapper::DependencyOwnerName::get() { return gcnew String("SceneTime"); }

	DependencyPropertyCollection^ a3dTimeProviderBaseMapper::DependencyProperties::get() { 
		if( this->properties != nullptr )
			return this->properties;
		this->properties = gcnew DependencyPropertyCollection(this);
		for(int i = 0; i < this->native->getPropertyCount(); i++) { 
			this->properties->Add(a3dDependencyPropertyMapper::GetPropertyMapper(this->native->getProperty(i)));
		}
		return this->properties;
	}
}