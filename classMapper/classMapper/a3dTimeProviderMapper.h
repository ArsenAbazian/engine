#pragma once

#include "a3dTimeProvider.h"
#include "DependencyMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper { 
	
	public ref class a3dTimeProviderBaseMapper : IDependencyTreeNode, IDependencyPropertyOwner { 
		a3dTimeProviderBase *native;
		IDependencyTreeNode ^parent;
		DependencyPropertyCollection ^properties;
	internal:
		a3dTimeProviderBaseMapper(a3dTimeProviderBase *timeProv) { 
			this->native = timeProv; 
		}
		void SetDependencyParent( IDependencyTreeNode ^parent ) { this->parent = parent; }
	public:
		
		property float Time {
			float get() { return this->native->fTime; }
			void set(float value) { this->native->setFloat(this->native->timeProperty->getName(), value); }
		}	

#pragma region IDependnecyTreeNode
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return parent; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return this; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get() { return nullptr; } }
		virtual property String^ NodeName { String^ get() { return gcnew String("SceneTime"); }	}
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
}