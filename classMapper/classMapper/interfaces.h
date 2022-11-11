// cameraMapper.h

#pragma once

#include "a3dDesc\\a3dDesc.h"
#include "a3dModel\\a3dModel.h"
#include "a3dModel\\a3dObject.h"
#include "a3dModel\\a3dDetail.h"
#include "a3dMaterial\\a3dMaterialBase.h"
#include "a3dMaterial\\a3dShaderMaterial.h"
#include "a3dMaterial\\a3dStandardMaterial.h"
#include "a3dMaterial\\a3dMultiMaterial.h"
#include "a3dShaderParam\\a3dShaderParams.h"
#include "a3dObjectInfo\\a3dObjectsInfo.h"
#include "a3dLight\\a3dLight.h"
#include "lightMapCreator.h"
#include "a3dParticleSystem\\particleSystemBase.h"
#include "a3dParticleSystem\\particles.h"
#include "landscape.h"
#include "railManager.h"
#include "lightMapInfo.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public interface class IObjectWithBasis { 
		property float ScaleX { float get(); void set(float f); }
		property float ScaleY { float get(); void set(float f); }
		property float ScaleZ { float get(); void set(float f); }

		property Matrix4_Mapper^ Basis { Matrix4_Mapper^ get(); void set(Matrix4_Mapper^ mat); }
	};

	ref class a3dLightMapInfo_Mapper;
	public interface class ILightMapInfo_MapperProvider { 
		property a3dLightMapInfo_Mapper^ LightMapInfo { a3dLightMapInfo_Mapper^ get(); }
	};

	public ref class a3dObjectWithEvent { 
		static Object^ objectChanged = gcnew Object();
	protected:
		EventHandlerList^ events;
		virtual void RaiseObjectChanged(Object ^obj, EventArgs ^e) { 
			if(events == nullptr) return;
			EventHandler ^handler = (EventHandler^)events[objectChanged];
			if( handler == nullptr ) return;
			handler(obj, e);
		}
	public:
		a3dObjectWithEvent() { 
			events = gcnew EventHandlerList();
		}

		event EventHandler^ ObjectChanged { 
			void add(EventHandler ^value) { this->events->AddHandler(objectChanged, value); }
			void remove(EventHandler ^value) { this->events->RemoveHandler(objectChanged, value); }  
			void raise(Object ^sender, EventArgs ^e) { RaiseObjectChanged(sender, e); }
		}
	};

	public ref class StringCollection : CollectionBase {
	public:
		int Add(String ^info) { return List->Add( info ); }
		void Insert(int index, String ^info) { List->Insert(index, info); }
		int IndexOf(String ^info) { return List->IndexOf(info); }

		String ^get_Item(int index) { return dynamic_cast<String ^>(List->default[index]); }
		void set_Item(int index, String ^info) { return List->default[index] = info; }

		property String^ default [int] { 
			String^ get(int index) { return get_Item(index); }
			void set(int index, String^ info) { set_Item(index, info); }
		} 	
	};
}