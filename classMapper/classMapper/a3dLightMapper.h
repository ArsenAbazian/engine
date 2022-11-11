// gridMapper.h

#pragma once

#include "stringHelper.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "interfaces.h"
#include "DescMapper.h"
#include "a3dModelRefMapper.h"
#include "colorHelper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class a3dLightBase_Mapper : a3dObjectWithEvent { 

		LightType NativeToLightType() {  
			if( light == null ) return LightType::LightNone;
			if(light->type == LIGHT_NONE) return LightType::LightNone;
			if(light->type == LIGHT_OMNI) return LightType::OmniLight;
			if(light->type == LIGHT_DIRECTIONAL) return LightType::DirectionLight;
			return LightType::LightNone;
		}
	internal:		
		a3dLightBase *light;
	public:
		a3dLightBase_Mapper() {
			this->light = null;
		}

		a3dLightBase_Mapper( a3dLightBase *light ) { 
			this->light = light;
		}

		property String^ Name { 
			String^ get() { return light == null? String::Empty: gcnew String(light->name.getCString()); } 
			void set(String ^value) { 
				if(light == null) return;
				shStringHelper::Default.copyTo(light->name, value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		property System::Drawing::Color Color { 
			System::Drawing::Color get() { return light == null? System::Drawing::Color::Empty: colorHelper::VectorRGBToColor(light->color); } 
			void set(System::Drawing::Color value) { 
				if(light == null) return;
				colorHelper::ColorToRGB(light->color, value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		property LightType Type { LightType get() { return NativeToLightType(); }  }
	};

	public ref class a3dOmniLight_Mapper : a3dLightBase_Mapper {
		property a3dOmniLight* Light { a3dOmniLight* get() { return dynamic_cast<a3dOmniLight*>(light); } }

	public:
		a3dOmniLight_Mapper( a3dOmniLight *light ) : a3dLightBase_Mapper( light ) { }

		property Vector3_Mapper^ Position { Vector3_Mapper^ get() { return Light == null? Vector3_Mapper::Empty: gcnew Vector3_Mapper(Light->pos[0], Light->pos[1], Light->pos[2]); } }
		property float FarAttenStart { float get() { return Light == null? 0.0f: Light->farAttenStart; } }
		property float FarAttenEnd { float get() { return Light == null? 0.0f: Light->farAttenEnd; } }
	};

	public ref class a3dDirectionalLight_Mapper : a3dLightBase_Mapper { 
		property a3dDirectionalLight* Light { a3dDirectionalLight* get() { return dynamic_cast<a3dDirectionalLight*>(this->light); } }
	public:
		a3dDirectionalLight_Mapper( a3dDirectionalLight* light ) : a3dLightBase_Mapper( light ) { }

		property Vector3_Mapper^ Direction { Vector3_Mapper^ get() { return Light == null? Vector3_Mapper::Empty: gcnew Vector3_Mapper( Light->dir[0], Light->dir[1], Light->dir[2] ); } }
	};


	[Editor(CollectionEditor::typeid, UITypeEditor::typeid )]
	public ref class a3dLightBase_MapperCollection : Collections::CollectionBase { 
	public:
		int Add(a3dLightBase_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dLightBase_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dLightBase_Mapper ^info) { return List->IndexOf(info); }
		void Remove(a3dLightBase_Mapper ^info) { List->Remove(info); }

		a3dLightBase_Mapper ^get_Item(int index) { return dynamic_cast<a3dLightBase_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dLightBase_Mapper ^info) { return List->default[index] = info; }

		property a3dLightBase_Mapper^ default [int] { 
			a3dLightBase_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dLightBase_Mapper^ info) { set_Item(index, info); }
		} 
		virtual void OnInsertComplete(int index, System::Object^ value) override { 
			a3dLightBase_Mapper^ light = dynamic_cast<a3dLightBase_Mapper^>(value);
			if(light == nullptr) {
				CollectionBase::OnInsertComplete(index, value);
				return;
			}
			if(light->light == null)
				Remove(light);
		}
	};

		
}