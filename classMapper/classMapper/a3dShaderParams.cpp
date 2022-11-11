// This is the main DLL file.

#include "stdafx.h"

#include "a3dShaderParamsMapper.h"
#include "DescMapper.h"
#include "CollectionEditorForm.h"

namespace classMapper { 
	
	a3dShaderParamTypeMapper a3dShaderParamBaseMapper::NativeTypeToManaged(a3dShaderParamType nativeType) { 
		switch(nativeType) { 
			case SPT_BASE: return a3dShaderParamTypeMapper::SptBase;
			case SPT_BOOL: return a3dShaderParamTypeMapper::SptBool;
			case SPT_BOOL_ARRAY: return a3dShaderParamTypeMapper::SptBoolArray;
			case SPT_FLOAT: return a3dShaderParamTypeMapper::SptFloat;
			case SPT_FLOAT_ARRAY: return a3dShaderParamTypeMapper::SptFloatArray;
			case SPT_COLOR: return a3dShaderParamTypeMapper::SptColor;
			case SPT_VECTOR: return a3dShaderParamTypeMapper::SptVector;
			case SPT_INT: return a3dShaderParamTypeMapper::SptInt;
			case SPT_INT_ARRAY: return a3dShaderParamTypeMapper::SptIntArray;
			case SPT_MATRIX: return a3dShaderParamTypeMapper::SptMatrix;
			case SPT_MATRIX_ARRAY: return a3dShaderParamTypeMapper::SptMatrixArray;
			case SPT_TEXTURE: return a3dShaderParamTypeMapper::SptTexture;
			case SPT_TECHNIQUE: return a3dShaderParamTypeMapper::SptTechnique;
		}
		return a3dShaderParamTypeMapper::SptBase;
	}
	
	a3dShaderParamType a3dShaderParamBaseMapper::ManagedTypeToNative(a3dShaderParamTypeMapper managedType) { 
		switch(managedType) { 
			case a3dShaderParamTypeMapper::SptBase: return SPT_BASE;
			case a3dShaderParamTypeMapper::SptBool: return SPT_BOOL;
			case a3dShaderParamTypeMapper::SptBoolArray: return SPT_BOOL_ARRAY;
			case a3dShaderParamTypeMapper::SptFloat: return SPT_FLOAT;
			case a3dShaderParamTypeMapper::SptFloatArray: return SPT_FLOAT_ARRAY;
			case a3dShaderParamTypeMapper::SptColor: return SPT_COLOR;
			case a3dShaderParamTypeMapper::SptVector: return SPT_VECTOR;
			case a3dShaderParamTypeMapper::SptInt: return SPT_INT;
			case a3dShaderParamTypeMapper::SptIntArray: return SPT_INT_ARRAY;
			case a3dShaderParamTypeMapper::SptMatrix: return SPT_MATRIX;
			case a3dShaderParamTypeMapper::SptMatrixArray: return SPT_MATRIX_ARRAY;
			case a3dShaderParamTypeMapper::SptTexture: return SPT_TEXTURE;
			case a3dShaderParamTypeMapper::SptTechnique: return SPT_TECHNIQUE;
		}
		return SPT_BASE;
	}

	a3dShaderParamWidgetTypeMapper a3dShaderParamBaseMapper::NativeWidgetTypeToManaged(a3dShaderParamWidgetType wtype) { 
		switch (wtype) {
			case SPWT_SLIDER: return a3dShaderParamWidgetTypeMapper::SpwtSlider;
			case SPWT_COLOR: return a3dShaderParamWidgetTypeMapper::SpwtColor;
		}
		return a3dShaderParamWidgetTypeMapper::SpwtNone;
	}

	a3dShaderParamWidgetType a3dShaderParamBaseMapper::ManagedWidgetTypeToNative(a3dShaderParamWidgetTypeMapper wtype) { 
		switch(wtype) { 
			case a3dShaderParamWidgetTypeMapper::SpwtSlider: return SPWT_SLIDER;
			case a3dShaderParamWidgetTypeMapper::SpwtColor: return SPWT_COLOR;
		}
		return SPWT_NONE;
	}

	void a3dShaderParamBaseMapper::Name::set(String ^value) { 
		
		shStringHelper::Default.copyTo( Param->paramName, value );
		Param->paramName.copyToChar(Param->name);
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dShaderParamBaseMapper::Type::set(a3dShaderParamTypeMapper type) { 
	
		Param->type = ManagedTypeToNative(type);
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dShaderParamBaseMapper::Used::set(bool value) {
		
		Param->used = value;
		ObjectChanged(this, EventArgs::Empty);
	}

	int a3dShaderParamBaseMapper::ArrayDimension::get() { 
	
		if(ArraySize3 > 0) return 3;
		if(ArraySize2 > 0) return 2;
		if(ArraySize1 > 0) return 1;
		return 0;	
	}

	void a3dShaderParamBaseMapper::WidgetType::set(a3dShaderParamWidgetTypeMapper value) { 
		
		Param->uiWidget = ManagedWidgetTypeToNative(value);
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dShaderParamBaseMapper::SetFloat( String ^propName, float value ) { 

		this->Param->setFloat(shStringHelper::Default.tohString(propName), value);
	}

	void a3dShaderParamBaseMapper::SetInt( String ^propName, int value ) { 
		
		this->Param->setInt(shStringHelper::Default.tohString(propName), value);
	}

	void a3dShaderParamBaseMapper::SetBool( String ^propName, bool value ) { 
		
		this->Param->setBool(shStringHelper::Default.tohString(propName), value);
	}

	float a3dShaderParamBaseMapper::GetFloat( String ^propName ) { 
		
		return this->Param->getFloat(shStringHelper::Default.tohString(propName));
	}

	int a3dShaderParamBaseMapper::GetInt( String ^propName ) { 
		
		return this->Param->getInt(shStringHelper::Default.tohString(propName));
	}

	bool a3dShaderParamBaseMapper::GetBool( String ^propName ) { 
		
		return this->Param->getBool(shStringHelper::Default.tohString(propName));
	}

	String^ a3dShaderParamBaseMapper::DependencyOwnerName::get() { return Name; }
	void a3dShaderParamBaseMapper::InitializeDependencyProperties() { 
		this->properties = gcnew DependencyPropertyCollection(this);
		for(int i = 0; i < Param->getPropertyCount(); i++) { 
			this->properties->Add(a3dDependencyPropertyMapper::GetPropertyMapper(Param->getProperty(i)));
		}
	}
	DependencyPropertyCollection^ a3dShaderParamBaseMapper::DependencyProperties::get() { 
		return this->properties;
	}

	void a3dBoolShaderParamMapper::Value::set(BOOL value) { 
		
		BoolParam->boolValue = value;
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dIntShaderParamMapper::Value::set(int value) { 
		
		IntParam->intValue = value;
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dFloatShaderParamMapper::Value::set(float value) { 
		
		FloatParam->floatValue = value;
		ObjectChanged(this, EventArgs::Empty);
	}

	void a3dFloatArrayShaderParamMapper::Value::set(int index, float value) {
		
		if(index < 0 || index >= Count) return;
		FloatArrayParam->floatArray[index] = value;
	}

	void a3dIntArrayShaderParamMapper::Value::set(int index, int value) {
		
		if(index < 0 || index >= Count) return;
		IntArrayParam->intArray[index] = value;
	}

	void a3dBoolArrayShaderParamMapper::Value::set(int index, bool value) {
		
		if(index < 0 || index >= Count) return;
		BoolArrayParam->boolArray[index] = value;
	}

	a3dTextureDesc_Mapper^ a3dTextureShaderParam_Mapper::TextureDesc::get() { 
		return gcnew a3dTextureDesc_Mapper(TextureParam->textureDesc); 
	}
	
	void a3dTextureShaderParam_Mapper::TextureDesc::set(a3dTextureDesc_Mapper^ value) { 
		TextureParam->textureDesc = value->TexDesc;
		ObjectChanged(this, EventArgs::Empty);
	}
}