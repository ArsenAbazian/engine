// classMapper.h

#pragma once

#include "a3dDependency\\a3dDependencyProperty.h"
#include "a3dDependency\\a3dDependencyPropertyOwner.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "interfaces.h"
#include "a3dGraphMapper\\a3dGraphMapper.h"
#include "a3dDependency\\a3dGraphDependency.h"
#include "a3dDependency\\a3dSimpleDependency.h"
#include "a3dDependency\\a3dScriptDependency.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public enum class a3dDependencyPropertyTypeMapper { 
		None,
		Bool,
		Float,
		Int
	};

	ref class a3dDependencyBaseMapper;
	ref class DependencyPropertyCollection;
	
	public interface class IDependencyPropertyOwner { 
		void SetFloat( String ^propName, float value );
		void SetBool( String ^propName, bool value );
		void SetInt( String ^propName, int value );
		float GetFloat(String ^propName);
		bool GetBool(String ^propName);
		int GetInt(String ^propName);
		property String^ DependencyOwnerName { String^ get(); }
		property DependencyPropertyCollection^ DependencyProperties { DependencyPropertyCollection^ get(); }
	};

	public ref class a3dDependencyPropertyMapper : a3dObjectWithEvent { 
		a3dDependencyProperty	*depProperty;
		a3dDependencyBaseMapper ^dependency;
		DependencyPropertyCollection ^notifyProps;
		IDependencyPropertyOwner^ depOwner;

		a3dDependencyPropertyTypeMapper nativeToManaged( a3dDependencyPropertyType propType );
		a3dDependencyPropertyType manageToNative( a3dDependencyPropertyTypeMapper propType );
		a3dDependencyBaseMapper^ CreateDependency();
		void InitializeNotifyList();
		a3dDependencyPropertyMapper( a3dDependencyProperty *prop );
	internal:
		static a3dDependencyPropertyMapper^ GetPropertyMapper(a3dDependencyProperty *prop);
		static ArrayList ^PropertyList = gcnew ArrayList();
		void SetDependencyOwner( IDependencyPropertyOwner^ owner) { this->depOwner = owner; }
	public:
		property String^ PropertyName { 
			String^ get() { return gcnew String(depProperty->getName().getCString()); }
		}

		property String^ NativeFieldName { 
			String^ get() { return gcnew String(depProperty->getNativeFieldName().getCString()); }
		}
	
		property a3dDependencyPropertyTypeMapper PropertyType { 
			a3dDependencyPropertyTypeMapper get() { return nativeToManaged(depProperty->getType()); }
		}
	
		
		property IDependencyPropertyOwner^ DependencyOwner {
			IDependencyPropertyOwner^ get() { return this->depOwner; }
		}

		property bool BoolValue { 
			bool get() { return depProperty->getBoolValue(); }
			void set(bool value) { 
				depProperty->setBoolValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int IntValue { 
			int get() { return depProperty->getIntValue(); }
			void set(int value) { 
				depProperty->setIntValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float FloatValue { 
			float get() { return depProperty->getFloatValue(); }
			void set(float value) { 
				depProperty->setFloatValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		
		property a3dDependencyBaseMapper^ Dependency { 	
			a3dDependencyBaseMapper^ get(); 
			void set(a3dDependencyBaseMapper ^value);
		}

		property DependencyPropertyCollection^ NotifyProperties { 
			DependencyPropertyCollection^ get();
		}

		bool IsNativeEquals(a3dDependencyPropertyMapper ^p) { return Property == p->Property; }
		
		virtual String^ ToString() override { 
			if(DependencyOwner == nullptr) return PropertyName;
			return String::Concat(DependencyOwner->DependencyOwnerName, gcnew String("->"), PropertyName); 
		}
	internal:
		property a3dDependencyProperty *Property { a3dDependencyProperty *get() { return this->depProperty; } }
	};

	
	public ref class DependencyPropertyCollection : CollectionBase {
		IDependencyPropertyOwner ^owner;
	public:
		DependencyPropertyCollection(IDependencyPropertyOwner ^owner) { 
			this->owner = owner;
		}

		int Add(a3dDependencyPropertyMapper ^prop) { return List->Add( prop ); }
		void Insert(int index, a3dDependencyPropertyMapper ^prop) { List->Insert(index, prop); }
		int IndexOf(a3dDependencyPropertyMapper ^prop) { return List->IndexOf(prop); }

		a3dDependencyPropertyMapper ^get_Item(int index) { return dynamic_cast<a3dDependencyPropertyMapper ^>(List->default[index]); }
		void set_Item(int index, a3dDependencyPropertyMapper ^prop) { return List->default[index] = prop; }

		property a3dDependencyPropertyMapper^ default [int] { 
			a3dDependencyPropertyMapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dDependencyPropertyMapper^ prop) { set_Item(index, prop); }
		} 	
		virtual void OnInsertComplete(int index, System::Object ^value) override { 
			a3dDependencyPropertyMapper ^prop = dynamic_cast<a3dDependencyPropertyMapper ^>(value);
			if(prop != nullptr) prop->SetDependencyOwner(owner);
		}
	};

	public ref class a3dDependencyPropertyOwner_Mapper : a3dObjectWithEvent, IDependencyPropertyOwner {
		a3dDependencyPropertyOwner		*owner;
		DependencyPropertyCollection	^coll;
		
	public:
		
		a3dDependencyPropertyOwner_Mapper() { 
			this->coll = gcnew DependencyPropertyCollection(this);
		}
		property a3dDependencyPropertyOwner* Owner { 
			a3dDependencyPropertyOwner* get() { return this->owner; }
			void set(a3dDependencyPropertyOwner* value) { 
				this->owner = value;
			}
		}
		void InitializeProperties() {  
			for(int index = 0; index < Owner->getPropertyCount(); index ++) {
				this->coll->Add(a3dDependencyPropertyMapper::GetPropertyMapper( owner->getProperty(index)));
			}
		} 
		virtual property DependencyPropertyCollection^ DependencyProperties { 
			DependencyPropertyCollection^ get() { return coll; }
		}

		virtual void SetFloat(String ^propName, float value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setFloat(name, value);
		}

		virtual void SetBool(String ^propName, bool value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setBool(name, value);
		}

		virtual void SetInt(String ^propName, int value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setInt(name, value);
		}

		virtual float GetFloat(String ^propName) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getFloat(name);
		}

		virtual bool GetBool(String ^propName) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getBool(name);
		}

		virtual int GetInt(String ^propName) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getInt(name);
		}

		virtual property String^ DependencyOwnerName { 
			String^ get() { return gcnew String( Owner->getName().getCString() ); }
		}
	};
	
	ref class DependencyTreeNodeCollection;

	public interface class IDependencyTreeNode { 
		property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get(); }
		property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get(); }
		property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get(); }
		property String^ NodeName { String^ get(); }
	}; 

	public ref class DependencyTreeNodeCollection : CollectionBase { 
	public:
		int Add(IDependencyTreeNode ^node) { return List->Add( node ); }
		void Insert(int index, IDependencyTreeNode ^node) { List->Insert(index, node ); }
		int IndexOf(IDependencyTreeNode ^node) { return List->IndexOf(node); }

		IDependencyTreeNode ^get_Item(int index) { return dynamic_cast<IDependencyTreeNode ^>(List->default[index]); }
		void set_Item(int index, IDependencyTreeNode ^node) { return List->default[index] = node; }

		property IDependencyTreeNode^ default [int] { 
			IDependencyTreeNode^ get(int index) { return get_Item(index); }
			void set(int index, IDependencyTreeNode^ node) { set_Item(index, node); }
		} 	
	};

	public ref class a3dDependencyBaseMapper { 
	protected:		
		a3dDependencyBase *dependency;
		a3dDependencyPropertyMapper^ dest;
	internal:
		a3dDependencyBaseMapper( a3dDependencyBase *dependency ) { 
			this->dependency = dependency;
		}
	public:
		a3dDependencyBaseMapper() { }

		property a3dDependencyPropertyMapper^ Dest { 
			a3dDependencyPropertyMapper^ get();
			void set(a3dDependencyPropertyMapper^ value);
		}
	internal:
		virtual void Initialize(a3dDependencyPropertyMapper ^prop);
		virtual void Clear();
	};

	public ref class a3dSimpleDependencyMapper : public a3dDependencyBaseMapper { 
		a3dDependencyPropertyMapper^ source;
	internal:
		a3dSimpleDependencyMapper( a3dSimpleDependency *dependency );
	public:
		a3dSimpleDependencyMapper() { 
			this->dependency = new a3dSimpleDependency();
		}
	
		property a3dDependencyPropertyMapper^ Source { 
			a3dDependencyPropertyMapper^ get();
			void set(a3dDependencyPropertyMapper^ value);
		}
	internal:
		virtual void Initialize(a3dDependencyPropertyMapper ^prop) override;
	};

	public enum class GraphOperationType { 
		None,
		SelectOp1,
		SelectOp2,
		Add,
		Sub,
		Mul,
		Div
	};

	public ref class a3dGraphDependencyMapper : public a3dSimpleDependencyMapper {
		a3dGraphMapper^ graph1;
		a3dGraphMapper^ graph2;
		a3dDependencyPropertyMapper ^src2;
	internal:		
		a3dGraphDependencyMapper( a3dGraphDependency *dependency );
		property a3dGraphDependency* GraphDependency { a3dGraphDependency* get() { return dynamic_cast<a3dGraphDependency*>(this->dependency); } }
		GraphOperationType Native2Managed( a3dGraphOperation op );
		a3dGraphOperation Managed2Native( GraphOperationType op );
	public:
		a3dGraphDependencyMapper() { 
			this->dependency = new a3dGraphDependency();
		}
		
		property a3dGraphMapper^ Graph { 
			a3dGraphMapper^ get() { return graph1; }
			void set( a3dGraphMapper^ value);
		} 

		property a3dGraphMapper^ Graph2 { 
			a3dGraphMapper^ get() { return graph2; }
			void set( a3dGraphMapper^ value);
		} 

		property a3dDependencyPropertyMapper^ Source2 { 
			a3dDependencyPropertyMapper^ get() { return this->src2; }
			void set(a3dDependencyPropertyMapper ^value);
		}

		property bool LoopGraph { 
			bool get() { return GraphDependency->loopGraph1; }
			void set(bool value) { GraphDependency->loopGraph1 = value; }
		}

		property bool LoopGraph2 { 
			bool get() { return GraphDependency->loopGraph2; }
			void set(bool value) { GraphDependency->loopGraph2 = value; }
		}

		property GraphOperationType Operation { 
			GraphOperationType get() { return Native2Managed( GraphDependency->operation ); }
			void set( GraphOperationType value ) { GraphDependency->operation = Managed2Native(value); }
		}
	};

	public ref class a3dScriptDependencyMapper : public a3dDependencyBaseMapper { 
	internal:
		a3dScriptDependencyMapper( a3dScriptDependency *dependency );
	public:
		a3dScriptDependencyMapper() { 
			this->dependency = new a3dScriptDependency();
		}
	};
}