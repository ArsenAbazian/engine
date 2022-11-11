// cameraMapper.h

#pragma once

#include "a3dShaderParam\\a3dShaderParams.h"
#include "fxFileParser\\fxFileParser.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "interfaces.h"
#include "DependencyMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public enum class a3dShaderParamTypeMapper { 
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

	public enum class a3dShaderParamWidgetTypeMapper { 
		SpwtNone,
		SpwtSlider,
		SpwtColor
	};

	ref class a3dShaderMaterialMapper;
	public ref class a3dShaderParamBaseMapper : public a3dObjectWithEvent, IDependencyTreeNode, IDependencyPropertyOwner { 
		a3dShaderParamBase *param;
		a3dShaderMaterialMapper^ material;
		IDependencyTreeNode ^parent;
		DependencyPropertyCollection ^properties;
	internal:
		property a3dShaderParamBase* Param { a3dShaderParamBase* get() { return param; } }
		a3dShaderParamTypeMapper NativeTypeToManaged(a3dShaderParamType nativeType);
		a3dShaderParamType ManagedTypeToNative(a3dShaderParamTypeMapper managedType);

		a3dShaderParamWidgetTypeMapper NativeWidgetTypeToManaged(a3dShaderParamWidgetType wtype);
		a3dShaderParamWidgetType ManagedWidgetTypeToNative(a3dShaderParamWidgetTypeMapper wtype);
		void SetDependencyParent(IDependencyTreeNode ^parent) { this->parent = parent; }
	protected:
		void InitializeDependencyProperties();
	public:
		a3dShaderParamBaseMapper(a3dShaderParamBase *param) { 
			this->param = param;
			InitializeDependencyProperties();
		}

		property a3dShaderMaterialMapper^ Material { 
			a3dShaderMaterialMapper^ get() { return material; } 
			void set(a3dShaderMaterialMapper^ value) { this->material = value;	}
		}
		
		property String^ Name { 
			String^ get() { return gcnew String(Param->paramName.getCString()); }
			void set( String^ str );
		}

		property a3dShaderParamTypeMapper Type { 
			a3dShaderParamTypeMapper get() { return NativeTypeToManaged(Param->type); }
			void set(a3dShaderParamTypeMapper type);
		}

		virtual String^ ToString() override { return Name + " Base"; }
		property bool Used { 
			bool get() { return Param->used; }
			void set(bool value);
		}

		property int Row { int get() { return Param->row; } }
		property int Col { int get() { return Param->col; } }
		property int ArraySize1 { int get() { return Param->arraySize1; } }
		property int ArraySize2 { int get() { return Param->arraySize2; } }
		property int ArraySize3 { int get() { return Param->arraySize3; } }
		property int ArrayDimension { int get(); }
		property a3dShaderParamWidgetTypeMapper WidgetType { 
			a3dShaderParamWidgetTypeMapper get() { return NativeWidgetTypeToManaged(Param->uiWidget); }
			void set(a3dShaderParamWidgetTypeMapper type);
		}

#pragma region IDependnecyTreeNode
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return this->parent; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return this; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get() { return nullptr; } }
		virtual property String^ NodeName { String^ get() { return Name; }	}
#pragma endregion
#pragma region IDependencyPropertyOwner
		virtual void SetFloat( String ^propName, float value );
		virtual void SetBool( String ^propName, bool value );
		virtual void SetInt( String ^propName, int value );
		virtual float GetFloat(String ^propName);
		virtual bool GetBool(String ^propName);
		virtual int GetInt(String ^propName);
		virtual property String^ DependencyOwnerName { String^ get(); }
		virtual property DependencyPropertyCollection^ DependencyProperties { DependencyPropertyCollection^ get(); }
#pragma endregion
	};

	public ref class a3dBoolShaderParamMapper : a3dShaderParamBaseMapper { 
		property a3dBoolShaderParam* BoolParam { a3dBoolShaderParam* get() { return dynamic_cast<a3dBoolShaderParam*>(Param); } }
	public:
		a3dBoolShaderParamMapper(a3dBoolShaderParam *param) : a3dShaderParamBaseMapper(param) { }

		property BOOL Value { 
			BOOL get() { return BoolParam->boolValue; } 
			void set(BOOL value);
		}
		virtual String^ ToString() override { return Name + " Bool"; }
	};

	public ref class a3dIntShaderParamMapper : a3dShaderParamBaseMapper { 
		property a3dIntShaderParam* IntParam { a3dIntShaderParam* get() { return dynamic_cast<a3dIntShaderParam*>(Param); } }
	public:
		a3dIntShaderParamMapper(a3dIntShaderParam *param) : a3dShaderParamBaseMapper(param) { }

		property int Value { 
			int get() { return IntParam->intValue; } 
			void set(int value);
		}

		virtual String^ ToString() override { return Name + " Int"; }
	};

	public ref class a3dFloatShaderParamMapper : a3dShaderParamBaseMapper { 
		property a3dFloatShaderParam* FloatParam { a3dFloatShaderParam* get() { return dynamic_cast<a3dFloatShaderParam*>(Param); } }
	public:
		a3dFloatShaderParamMapper(a3dFloatShaderParam *param) : a3dShaderParamBaseMapper(param) { }

		property float Value { 
			float get() { return FloatParam->floatValue; } 
			void set(float value);
		}
		property float MinValue  { float get() { return FloatParam->minValue; } void set(float value) { FloatParam->minValue = value; } }
		property float MaxValue  { float get() { return FloatParam->maxValue; } void set(float value) { FloatParam->maxValue = value; } }
		property float Step { float get() { return FloatParam->step; } void set(float value) { FloatParam->step = value; } }
	
		virtual String^ ToString() override { return Name + " Float"; }
	};

	public ref class a3dFloatArrayShaderParamMapper : a3dShaderParamBaseMapper { 
		property a3dFloatArrayShaderParam* FloatArrayParam { a3dFloatArrayShaderParam* get() { return dynamic_cast<a3dFloatArrayShaderParam*>(Param); } }
	public:
		a3dFloatArrayShaderParamMapper(a3dFloatArrayShaderParam *param) : a3dShaderParamBaseMapper(param) { 
		}
	
		property int Count { int get() { return FloatArrayParam->count; } }
		property float Value[int] { 
			float get(int index) { return index < Count && index >=0 ? FloatArrayParam->floatArray[index]: 0.0f; }
			void set(int index, float value);
		}

		virtual String^ ToString() override { return Name + " FloatArray"; }
	};

	public ref class a3dIntArrayShaderParamMapper : a3dShaderParamBaseMapper {
		property a3dIntArrayShaderParam* IntArrayParam { a3dIntArrayShaderParam* get() { return dynamic_cast<a3dIntArrayShaderParam*>(Param); } }
	public:
		a3dIntArrayShaderParamMapper(a3dIntArrayShaderParam *param) : a3dShaderParamBaseMapper(param) { 
		}

		property int Count { int get() { return IntArrayParam->count; } }
		property int Value[int] { 
			int get(int index) { return index < Count && index >=0 ? IntArrayParam->intArray[index]: 0; }
			void set(int index, int value);
		}

		virtual String^ ToString() override { return Name + " IntArray"; }
	};

	public ref class a3dBoolArrayShaderParamMapper : a3dShaderParamBaseMapper {
		property a3dBoolArrayShaderParam* BoolArrayParam { a3dBoolArrayShaderParam* get() { return dynamic_cast<a3dBoolArrayShaderParam*>(Param); } }
	public:
		a3dBoolArrayShaderParamMapper(a3dBoolArrayShaderParam *param) : a3dShaderParamBaseMapper(param) { 
		}
		
		property int Count { int get() { return BoolArrayParam->count; } }
		property bool Value[int] { 
			bool get(int index) { return index < Count && index >=0 ? (bool)BoolArrayParam->boolArray[index]: false; }
			void set(int index, bool value);
		}

		virtual String^ ToString() override { return Name + " BoolArray"; }
	};

	public ref class a3dColorShaderParam_Mapper : a3dFloatArrayShaderParamMapper {
		property a3dColorShaderParam* ColorParam { a3dColorShaderParam* get() { return dynamic_cast<a3dColorShaderParam*>(Param); } }
	public:
		a3dColorShaderParam_Mapper(a3dColorShaderParam* param) : a3dFloatArrayShaderParamMapper(param) { 
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

	public ref class a3dVectorShaderParam_Mapper : a3dFloatArrayShaderParamMapper {
		property a3dVectorShaderParam* ColorParam { a3dVectorShaderParam* get() { return dynamic_cast<a3dVectorShaderParam*>(Param); } }
	public:
		a3dVectorShaderParam_Mapper(a3dVectorShaderParam* param) : a3dFloatArrayShaderParamMapper(param) { 
		}
	};

	public ref class a3dMatrixShaderParam_Mapper : a3dShaderParamBaseMapper { 
		property a3dMatrixShaderParam* MatrixParam { a3dMatrixShaderParam* get() { return dynamic_cast<a3dMatrixShaderParam*>(Param); } }
	public:
		a3dMatrixShaderParam_Mapper(a3dMatrixShaderParam *param) : a3dShaderParamBaseMapper(param) { }

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

	public ref class a3dMatrixArrayShaderParam_Mapper : a3dShaderParamBaseMapper {
		property a3dMatrixArrayShaderParam* MatrixArrayParam { a3dMatrixArrayShaderParam* get() { return dynamic_cast<a3dMatrixArrayShaderParam*>(Param); } }
	public:
		a3dMatrixArrayShaderParam_Mapper( a3dMatrixArrayShaderParam *param ) : a3dShaderParamBaseMapper(param) { }

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

	public ref class a3dTechniqueShaderParam_Mapper : a3dShaderParamBaseMapper { 
		property a3dTechniqueShaderParam* TechniqueParam { a3dTechniqueShaderParam* get() { return dynamic_cast<a3dTechniqueShaderParam*>(Param); } }
	public:
		a3dTechniqueShaderParam_Mapper(a3dTechniqueShaderParam *param) : a3dShaderParamBaseMapper(param) { }

		property String^ Technique { 
			String^ get() { return Name; } 
			void set(String^ val) { Name = val; }
		}
	};

	ref class a3dTextureDesc_Mapper;
	public ref class a3dTextureShaderParam_Mapper : a3dShaderParamBaseMapper { 
		property a3dTextureShaderParam* TextureParam { a3dTextureShaderParam* get() { return dynamic_cast<a3dTextureShaderParam*>(Param); } }
	public:
		a3dTextureShaderParam_Mapper(a3dTextureShaderParam *param) : a3dShaderParamBaseMapper(param) { }

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

	public ref class a3dShaderParamBaseMapperCollection : Collections::CollectionBase { 

	public:
		int Add(a3dShaderParamBaseMapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dShaderParamBaseMapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dShaderParamBaseMapper ^info) { return List->IndexOf(info); }

		a3dShaderParamBaseMapper ^get_Item(int index) { return dynamic_cast<a3dShaderParamBaseMapper ^>(List->default[index]); }
		void set_Item(int index, a3dShaderParamBaseMapper ^info) { return List->default[index] = info; }

		property a3dShaderParamBaseMapper^ default [int] { 
			a3dShaderParamBaseMapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dShaderParamBaseMapper^ info) { set_Item(index, info); }
		} 
	};
}