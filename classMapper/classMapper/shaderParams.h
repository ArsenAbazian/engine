// cameraMapper.h

#pragma once

#include "a3dShaderParam\\a3dShaderParams.h"
#include "fxFileParser\\fxFileParser.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "interfaces.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public enum class a3dShaderParamType_Mapper { 
		SptBase, 
		SptBool, 
		SptBoolArray, 
		SptFloat, 
		SptFloatArray, 
		SptColor,
		SptVector,
		SptInt, 
		SptIntArray, 
		SptMatrix, 
		SptMatrixArray, 
		SptTexture, 
		SptTechnique
	};

	public enum class a3dShaderParamWidgetType_Mapper { 
		SpwtNone,
		SpwtSlider,
		SpwtColor
	};

	ref class a3dShaderMaterial_Mapper;
	public ref class a3dShaderParamBase_Mapper : public a3dObjectWithEvent { 
		a3dShaderParamBase *param;
		a3dShaderMaterial_Mapper^ material;
	internal:
		property a3dShaderParamBase* Param { a3dShaderParamBase* get() { return param; } }
		a3dShaderParamType_Mapper NativeTypeToManaged(a3dShaderParamType nativeType) { 
			switch(nativeType) { 
				case SPT_BASE: return a3dShaderParamType_Mapper::SptBase;
				case SPT_BOOL: return a3dShaderParamType_Mapper::SptBool;
				case SPT_BOOL_ARRAY: return a3dShaderParamType_Mapper::SptBoolArray;
				case SPT_FLOAT: return a3dShaderParamType_Mapper::SptFloat;
				case SPT_FLOAT_ARRAY: return a3dShaderParamType_Mapper::SptFloatArray;
				case SPT_COLOR: return a3dShaderParamType_Mapper::SptColor;
				case SPT_VECTOR: return a3dShaderParamType_Mapper::SptVector;
				case SPT_INT: return a3dShaderParamType_Mapper::SptInt;
				case SPT_INT_ARRAY: return a3dShaderParamType_Mapper::SptIntArray;
				case SPT_MATRIX: return a3dShaderParamType_Mapper::SptMatrix;
				case SPT_MATRIX_ARRAY: return a3dShaderParamType_Mapper::SptMatrixArray;
				case SPT_TEXTURE: return a3dShaderParamType_Mapper::SptTexture;
				case SPT_TECHNIQUE: return a3dShaderParamType_Mapper::SptTechnique;
			}
			return a3dShaderParamType_Mapper::SptBase;
		}
		a3dShaderParamType ManagedTypeToNative(a3dShaderParamType_Mapper managedType) { 
			switch(managedType) { 
			case a3dShaderParamType_Mapper::SptBase: return SPT_BASE;
			case a3dShaderParamType_Mapper::SptBool: return SPT_BOOL;
			case a3dShaderParamType_Mapper::SptBoolArray: return SPT_BOOL_ARRAY;
			case a3dShaderParamType_Mapper::SptFloat: return SPT_FLOAT;
			case a3dShaderParamType_Mapper::SptFloatArray: return SPT_FLOAT_ARRAY;
			case a3dShaderParamType_Mapper::SptColor: return SPT_COLOR;
			case a3dShaderParamType_Mapper::SptVector: return SPT_VECTOR;
			case a3dShaderParamType_Mapper::SptInt: return SPT_INT;
			case a3dShaderParamType_Mapper::SptIntArray: return SPT_INT_ARRAY;
			case a3dShaderParamType_Mapper::SptMatrix: return SPT_MATRIX;
			case a3dShaderParamType_Mapper::SptMatrixArray: return SPT_MATRIX_ARRAY;
			case a3dShaderParamType_Mapper::SptTexture: return SPT_TEXTURE;
			case a3dShaderParamType_Mapper::SptTechnique: return SPT_TECHNIQUE;
			}
			return SPT_BASE;
		}

		a3dShaderParamWidgetType_Mapper NativeWidgetTypeToManaged(a3dShaderParamWidgetType wtype) { 
			switch (wtype) {
				case SPWT_SLIDER: return a3dShaderParamWidgetType_Mapper::SpwtSlider;
				case SPWT_COLOR: return a3dShaderParamWidgetType_Mapper::SpwtColor;
			}
			return a3dShaderParamWidgetType_Mapper::SpwtNone;
		}
		a3dShaderParamWidgetType ManagedWidgetTypeToNative(a3dShaderParamWidgetType_Mapper wtype) { 
			switch(wtype) { 
				case a3dShaderParamWidgetType_Mapper::SpwtSlider: return SPWT_SLIDER;
				case a3dShaderParamWidgetType_Mapper::SpwtColor: return SPWT_COLOR;
			}
			return SPWT_NONE;
		}
		
	public:
		a3dShaderParamBase_Mapper(a3dShaderParamBase *param) { 
			this->param = param;
		}

		property a3dShaderMaterial_Mapper^ Material { 
			a3dShaderMaterial_Mapper^ get() { return material; } 
			void set(a3dShaderMaterial_Mapper^ value) { 
				this->material = value;
			}
		}
		
		property String^ Name { 
			String^ get() { return gcnew String(Param->paramName.getCString()); }
			void set( String^ str ) { 
				shStringHelper::Default.copyTo( Param->paramName, str );
				Param->paramName.copyToChar(Param->name);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property a3dShaderParamType_Mapper Type { 
			a3dShaderParamType_Mapper get() { return NativeTypeToManaged(Param->type); }
			void set(a3dShaderParamType_Mapper type) { 
				Param->type = ManagedTypeToNative(type);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		virtual String^ ToString() override { 
			return Name + " " + Type.ToString();
		};
		property bool Used { 
			bool get() { return Param->used; }
			void set(bool value) { 
				Param->used = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int Row { int get() { return Param->row; } }
		property int Col { int get() { return Param->col; } }
		property int ArraySize1 { int get() { return Param->arraySize1; } }
		property int ArraySize2 { int get() { return Param->arraySize2; } }
		property int ArraySize3 { int get() { return Param->arraySize3; } }
		property int ArrayDimension { 
			int get() { 
				if(ArraySize3 > 0) return 3;
				if(ArraySize2 > 0) return 2;
				if(ArraySize1 > 0) return 1;
				return 0;
			}  
		}
		property a3dShaderParamWidgetType_Mapper WidgetType { 
			a3dShaderParamWidgetType_Mapper get() { return NativeWidgetTypeToManaged(Param->uiWidget); }
			void set(a3dShaderParamWidgetType_Mapper type) { 
				Param->uiWidget = ManagedWidgetTypeToNative(type);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	
	};

	public ref class a3dBoolShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dBoolShaderParam* BoolParam { a3dBoolShaderParam* get() { return dynamic_cast<a3dBoolShaderParam*>(Param); } }
	public:
		a3dBoolShaderParam_Mapper(a3dBoolShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property BOOL Value { 
			BOOL get() { return BoolParam->boolValue; } 
			void set(BOOL value) { 
				BoolParam->boolValue = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	};

	public ref class a3dIntShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dIntShaderParam* IntParam { a3dIntShaderParam* get() { return dynamic_cast<a3dIntShaderParam*>(Param); } }
	public:
		a3dIntShaderParam_Mapper(a3dIntShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property int Value { 
			int get() { return IntParam->intValue; } 
			void set(int value) { 
				IntParam->intValue = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	};

	public ref class a3dFloatShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dFloatShaderParam* FloatParam { a3dFloatShaderParam* get() { return dynamic_cast<a3dFloatShaderParam*>(Param); } }
	public:
		a3dFloatShaderParam_Mapper(a3dFloatShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property float Value { 
			float get() { return FloatParam->floatValue; } 
			void set(float value) { 
				FloatParam->floatValue = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		property float MinValue  { float get() { return FloatParam->minValue; } void set(float value) { FloatParam->minValue = value; } }
		property float MaxValue  { float get() { return FloatParam->maxValue; } void set(float value) { FloatParam->maxValue = value; } }
		property float Step { float get() { return FloatParam->step; } void set(float value) { FloatParam->step = value; } }
	};

	public ref class a3dFloatArrayShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dFloatArrayShaderParam* FloatArrayParam { a3dFloatArrayShaderParam* get() { return dynamic_cast<a3dFloatArrayShaderParam*>(Param); } }
	public:
		a3dFloatArrayShaderParam_Mapper(a3dFloatArrayShaderParam *param) : a3dShaderParamBase_Mapper(param) { 
		}
	
		property int Count { int get() { return FloatArrayParam->count; } }
		property float Value[int] { 
			float get(int index) { return index < Count && index >=0 ? FloatArrayParam->floatArray[index]: 0.0f; }
			void set(int index, float value) { 
				if(index < 0 || index >= Count) return;
				FloatArrayParam->floatArray[index] = value;
			}
		}
	};

	public ref class a3dIntArrayShaderParam_Mapper : a3dShaderParamBase_Mapper {
		property a3dIntArrayShaderParam* IntArrayParam { a3dIntArrayShaderParam* get() { return dynamic_cast<a3dIntArrayShaderParam*>(Param); } }
	public:
		a3dIntArrayShaderParam_Mapper(a3dIntArrayShaderParam *param) : a3dShaderParamBase_Mapper(param) { 
		}

		property int Count { int get() { return IntArrayParam->count; } }
		property int Value[int] { 
			int get(int index) { return index < Count && index >=0 ? IntArrayParam->intArray[index]: 0; }
			void set(int index, int value) { 
				if(index < 0 || index >= Count) return;
				IntArrayParam->intArray[index] = value;
			}
		}
	};

	public ref class a3dBoolArrayShaderParam_Mapper : a3dShaderParamBase_Mapper {
		property a3dBoolArrayShaderParam* BoolArrayParam { a3dBoolArrayShaderParam* get() { return dynamic_cast<a3dBoolArrayShaderParam*>(Param); } }
	public:
		a3dBoolArrayShaderParam_Mapper(a3dBoolArrayShaderParam *param) : a3dShaderParamBase_Mapper(param) { 
		}
		
		property int Count { int get() { return BoolArrayParam->count; } }
		property bool Value[int] { 
			bool get(int index) { return index < Count && index >=0 ? BoolArrayParam->boolArray[index]: false; }
			void set(int index, bool value) { 
				if(index < 0 || index >= Count) return;
				BoolArrayParam->boolArray[index] = value;
			}
		}
	};

	public ref class a3dColorShaderParam_Mapper : a3dFloatArrayShaderParam_Mapper {
		property a3dColorShaderParam* ColorParam { a3dColorShaderParam* get() { return dynamic_cast<a3dColorShaderParam*>(Param); } }
	public:
		a3dColorShaderParam_Mapper(a3dColorShaderParam* param) : a3dFloatArrayShaderParam_Mapper(param) { 
		}

		property Color ColorValue {
			Color get() {  
				if( Count == 4 ) return Color::FromArgb(vector4ToDWORD(ColorParam->floatArray));
				return Color::FromArgb(vector3ToDWORD(ColorParam->floatArray));
			}
			void set(Color value) { 
				if( Count == 4 ) setVector4( ColorParam->floatArray, value.ToArgb() );
				else setVector3( ColorParam->floatArray, value.ToArgb() );
			}
		};  
	};

	public ref class a3dVectorShaderParam_Mapper : a3dFloatArrayShaderParam_Mapper {
		property a3dVectorShaderParam* ColorParam { a3dVectorShaderParam* get() { return dynamic_cast<a3dVectorShaderParam*>(Param); } }
	public:
		a3dVectorShaderParam_Mapper(a3dVectorShaderParam* param) : a3dFloatArrayShaderParam_Mapper(param) { 
		}
	};

	public ref class a3dMatrixShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dMatrixShaderParam* MatrixParam { a3dMatrixShaderParam* get() { return dynamic_cast<a3dMatrixShaderParam*>(Param); } }
	public:
		a3dMatrixShaderParam_Mapper(a3dMatrixShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property Matrix4_Mapper^ Value { 
			Matrix4_Mapper^ get() { 
				Matrix4_Mapper^ mat = gcnew Matrix4_Mapper();
				mat->FromMatrix4(MatrixParam->matrixValue);
				return mat; 
			} 
			void set(Matrix4_Mapper^ value) { 
				value->ToMatrix4(MatrixParam->matrixValue);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		
		void SetValue(int row, int col, float val) { 
			MatrixParam->matrixValue[row][col] = val;
		}
	};

	public ref class a3dMatrixArrayShaderParam_Mapper : a3dShaderParamBase_Mapper {
		property a3dMatrixArrayShaderParam* MatrixArrayParam { a3dMatrixArrayShaderParam* get() { return dynamic_cast<a3dMatrixArrayShaderParam*>(Param); } }
	public:
		a3dMatrixArrayShaderParam_Mapper( a3dMatrixArrayShaderParam *param ) : a3dShaderParamBase_Mapper(param) { }

		property int Count { int get() { return MatrixArrayParam->count; } }
		
		property Matrix4_Mapper^ Value[int] { 
			Matrix4_Mapper^ get(int index) {
				if( index < 0 || index >= Count ) return nullptr;
				Matrix4_Mapper^ map = gcnew Matrix4_Mapper();
				map->FromMatrix4(MatrixArrayParam->matrixArray[index]);
				return map;
			}
			void set(int index, Matrix4_Mapper^ value) { 
				if( index < 0 || index >= Count ) return;
				value->ToMatrix4(MatrixArrayParam->matrixArray[index]);
			}
		}
	};

	public ref class a3dTechniqueShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dTechniqueShaderParam* TechniqueParam { a3dTechniqueShaderParam* get() { return dynamic_cast<a3dTechniqueShaderParam*>(Param); } }
	public:
		a3dTechniqueShaderParam_Mapper(a3dTechniqueShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property String^ Technique { 
			String^ get() { return Name; } 
			void set(String^ val) { Name = val; }
		}
	};

	ref class a3dTextureDesc_Mapper;
	public ref class a3dTextureShaderParam_Mapper : a3dShaderParamBase_Mapper { 
		property a3dTextureShaderParam* TextureParam { a3dTextureShaderParam* get() { return dynamic_cast<a3dTextureShaderParam*>(Param); } }
	public:
		a3dTextureShaderParam_Mapper(a3dTextureShaderParam *param) : a3dShaderParamBase_Mapper(param) { }

		property String^ FileName { 
			String^ get() { return gcnew String(TextureParam->fileName.getCString()); } 
			void set(String^ val) { 
				shStringHelper::Default.copyTo(TextureParam->fileName, val);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property a3dTextureDesc_Mapper^ TextureDesc { 
			a3dTextureDesc_Mapper^ get();
			void set(a3dTextureDesc_Mapper^ value);
		}
	};

	public ref class a3dShaderParamBase_MapperCollection : Collections::CollectionBase { 

	public:
		int Add(a3dShaderParamBase_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dShaderParamBase_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dShaderParamBase_Mapper ^info) { return List->IndexOf(info); }

		a3dShaderParamBase_Mapper ^get_Item(int index) { return dynamic_cast<a3dShaderParamBase_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dShaderParamBase_Mapper ^info) { return List->default[index] = info; }

		property a3dShaderParamBase_Mapper^ default [int] { 
			a3dShaderParamBase_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dShaderParamBase_Mapper^ info) { set_Item(index, info); }
		} 
	};
}