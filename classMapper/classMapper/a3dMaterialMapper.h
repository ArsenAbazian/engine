#pragma once

#include "a3dMaterial\\a3dMaterialBase.h"
#include "a3dMaterial\\a3dShaderMaterial.h"
#include "a3dMaterial\\a3dStandardMaterial.h"
#include "a3dShaderParam\\a3dShaderParams.h"
#include "interfaces.h"
#include "a3dShaderParamsMapper.h"
#include "DependencyMapper.h"

using namespace System;
using namespace System::Collections;

namespace classMapper { 
	
	public ref class a3dMaterialBaseMapper : a3dObjectWithEvent, IDependencyTreeNode {
		a3dMaterialBase *mat;
		IDependencyTreeNode ^parent;
	protected:
		DependencyTreeNodeCollection ^children;
	internal:
		void SetDependencyParent(IDependencyTreeNode ^parent) { this->parent = parent; }
	public:
		a3dMaterialBaseMapper( a3dMaterialBase *mat ) { this->mat = mat; }

		property a3dMaterialBase* Material { a3dMaterialBase* get() { return mat; } }
		property String^ Name { String^ get() { return gcnew String(Material->name.getCString()); } }
		
		virtual String^ ToString() override { return Name; }
	
#pragma region IDependnecyTreeNode
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return parent; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return nullptr; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get() { return nullptr; } }
		virtual property String^ NodeName { String^ get() { return String::Concat("Material ", Name); }	}
#pragma endregion
	};
	
	public ref class a3dMaterialBaseMapperCollection : CollectionBase { 
	public:
		int Add(a3dMaterialBaseMapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dMaterialBaseMapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dMaterialBaseMapper ^info) { return List->IndexOf(info); }

		a3dMaterialBaseMapper ^get_Item(int index) { return dynamic_cast<a3dMaterialBaseMapper ^>(List->default[index]); }
		void set_Item(int index, a3dMaterialBaseMapper ^info) { return List->default[index] = info; }

		property a3dMaterialBaseMapper^ default [int] { 
			a3dMaterialBaseMapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dMaterialBaseMapper^ info) { set_Item(index, info); }
		} 
	};

	public ref class a3dShaderMaterialMapper : a3dMaterialBaseMapper {
		a3dShaderParamBaseMapperCollection ^paramColl;
		StringCollection ^techList;
		
		void InitializeParamCollection();
		void InitializeTechniqueList();
	internal:
		property a3dShaderMaterial* ShaderMaterial { a3dShaderMaterial* get() { return dynamic_cast<a3dShaderMaterial*>(Material); } }
	public: 
		a3dShaderMaterialMapper( a3dShaderMaterial *mat );

		property a3dShaderParamBaseMapperCollection^ Params { a3dShaderParamBaseMapperCollection^ get() { return this->paramColl; } }
		property StringCollection^ Techniques { StringCollection^ get() { return techList; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get() override; }
	};
}