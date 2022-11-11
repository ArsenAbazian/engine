// classMapper.h

#pragma once

#include "a3dDependency\\a3dDependencyProperty.h"
#include "a3dDependency\\a3dDependencyPropertyOwner.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "interfaces.h"
#include "a3dGraph_Mapper\\a3dGraph_Mapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public enum class a3dDependencyPropertyType_Mapper { 
		None,
		Bool,
		Float,
		Int
	};

	public ref class a3dDependencyProperty_Mapper : a3dObjectWithEvent { 
		a3dGraph_Mapper^ graph;
		a3dDependencyProperty *depProperty;
		a3dDependencyPropertyType_Mapper nativeToManaged( a3dDependencyPropertyType propType ) { 
			switch(propType) { 
				case DPROP_NONE: return a3dDependencyPropertyType_Mapper::None;
				case DPROP_BOOL: return a3dDependencyPropertyType_Mapper::Bool;
				case DPROP_FLOAT: return a3dDependencyPropertyType_Mapper::Float;
				case DPROP_INT: return a3dDependencyPropertyType_Mapper::Int;
			}
			return a3dDependencyPropertyType_Mapper::None;
		}
		a3dDependencyPropertyType manageToNative( a3dDependencyPropertyType_Mapper propType ) { 
			switch(propType) { 
				case a3dDependencyPropertyType_Mapper::None: return DPROP_NONE;
				case a3dDependencyPropertyType_Mapper::Bool: return DPROP_BOOL;
				case a3dDependencyPropertyType_Mapper::Float: return DPROP_FLOAT;
				case a3dDependencyPropertyType_Mapper::Int: return DPROP_INT;
			}
			return DPROP_NONE;
		}
		
	public:
		
		a3dDependencyProperty_Mapper( a3dDependencyProperty *prop ) { 
			this->depProperty = prop;
		}
	
		property String^ PropertyName { 
			String^ get() { return gcnew String(depProperty->getName().getCString()); }
		}

		property String^ NativeFieldName { 
			String^ get() { return gcnew String(depProperty->getNativeFieldName().getCString()); }
		}
	
		property a3dDependencyPropertyType_Mapper PropertyType { 
			a3dDependencyPropertyType_Mapper get() { return nativeToManaged(depProperty->getType()); }
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

		property a3dGraph_Mapper^ Graph { 
			a3dGraph_Mapper^ get() { return this->graph; }
			void set( a3dGraph_Mapper ^value ) { 
				this->graph = value;
				if(value == nullptr)
					this->depProperty->attachGraph(null);
				else
					this->depProperty->attachGraph(this->graph->NativeGraph);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	};

	public ref class DependencyPropertyCollection : CollectionBase {
	public:
		int Add(a3dDependencyProperty_Mapper ^prop) { return List->Add( prop ); }
		void Insert(int index, a3dDependencyProperty_Mapper ^prop) { List->Insert(index, prop); }
		int IndexOf(a3dDependencyProperty_Mapper ^prop) { return List->IndexOf(prop); }

		a3dDependencyProperty_Mapper ^get_Item(int index) { return dynamic_cast<a3dDependencyProperty_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dDependencyProperty_Mapper ^prop) { return List->default[index] = prop; }

		property a3dDependencyProperty_Mapper^ default [int] { 
			a3dDependencyProperty_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dDependencyProperty_Mapper^ prop) { set_Item(index, prop); }
		} 	
	};

	public ref class a3dDependencyPropertyOwner_Mapper : a3dObjectWithEvent {
		a3dDependencyPropertyOwner		*owner;
		DependencyPropertyCollection	^coll;
		
	protected:
		property a3dDependencyPropertyOwner* Owner { 
			a3dDependencyPropertyOwner* get() { return this->owner; }
			void set(a3dDependencyPropertyOwner* value) { 
				this->owner = value;
			}
		}
		void InitializeProperties() {  
			for(int index = 0; index < Owner->getPropertyCount(); index ++) {
				this->coll->Add(gcnew a3dDependencyProperty_Mapper( owner->getProperty(index)));
			}
		} 
	public:

		a3dDependencyPropertyOwner_Mapper() { 
			this->coll = gcnew DependencyPropertyCollection();
		}
		property DependencyPropertyCollection^ DependencyProperties { 
			DependencyPropertyCollection^ get() { return coll; }
		}

		void setFloat(String ^propName, float value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setFloat(name, value);
		}

		void setBool(String ^propName, bool value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setBool(name, value);
		}

		void setInt(String ^propName, int value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			Owner->setInt(name, value);
		}

		float getFloat(String ^propName, float value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getFloat(name);
		}

		bool getBool(String ^propName, bool value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getBool(name);
		}

		int getInt(String ^propName, int value) { 
			shString name;
			shStringHelper::Default.copyTo(name, propName);
			return Owner->getInt(name);
		}

		property String^ OwnerName { 
			String^ get() { return gcnew String( Owner->getName().getCString() ); }
		}
	};
}