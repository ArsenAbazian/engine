// cameraMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "lightMapCreator_Mapper.h"
#include "a3dModel\\a3dModel.h"
#include "a3dModel\\a3dObject.h"
#include "a3dModel\\a3dDetail.h"
#include "a3dModelRef\\a3dModelRef.h"
#include "a3dModelRef\\a3dObjectRef.h"
#include "a3dModelRef\\a3dDetailRef.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class a3dDetailRef_Mapper : a3dObjectWithEvent, ILightMapInfo_MapperProvider { 
		a3dDetailRef *detail;
		a3dLightMapInfo_Mapper ^lightMapInfo;
	internal:
		property a3dDetailRef* Detail { a3dDetailRef* get() { return detail; } }
	protected:
		void ObjectChanged() { 
			a3dObjectWithEvent::ObjectChanged(this, EventArgs::Empty);
		}
	public:
		property a3dShaderMaterial_Mapper^ Material {  
			a3dShaderMaterial_Mapper^ get() { return gcnew a3dShaderMaterial_Mapper(&Detail->material); }
		}
		
		a3dDetailRef_Mapper(a3dDetailRef *detail) {
			this->detail = detail;
			this->lightMapInfo = gcnew a3dLightMapInfo_Mapper(&Detail->lightMapInfo);
		}
		virtual property a3dLightMapInfo_Mapper^ LightMapInfo { 
			a3dLightMapInfo_Mapper^ get() { 
				UpdateLightMapInfo();
				return lightMapInfo;
			} 
		}
		virtual String^ ToString() override { return gcnew String(this->detail->material.name.getCString()); }
	protected:
		void InitializeTextureParamColl() { 
			lightMapInfo->TextureParamColl->Clear();
			for(int i = 0; i < this->detail->material.paramCount; i++) { 
				if(this->detail->material.paramList[i]->type == SPT_TEXTURE) { 
					a3dTextureShaderParam *param = dynamic_cast<a3dTextureShaderParam *>(this->detail->material.paramList[i]);
					if(param != null)
						lightMapInfo->TextureParamColl->Add(gcnew String(param->paramName.getCString()));
				}
			}
		}
		void UpdateLightMapInfo() {  
			String ^str = String::Concat(gcnew String(this->detail->objectRef->modelRef->name.getCString()), gcnew String("_"), gcnew String(this->detail->objectRef->object->name.getCString()), gcnew String("_"), gcnew String(this->detail->material.name.getCString()));
			lightMapInfo->setAutoFileName(str);
			InitializeTextureParamColl();
		}
	};
	
	public ref class a3dDetailRef_MapperCollection : Collections::CollectionBase { 
		
	public:
		int Add(a3dDetailRef_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dDetailRef_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dDetailRef_Mapper ^info) { return List->IndexOf(info); }

		a3dDetailRef_Mapper ^get_Item(int index) { return dynamic_cast<a3dDetailRef_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dDetailRef_Mapper ^info) { return List->default[index] = info; }
		
		property a3dDetailRef_Mapper^ default [int] { 
			a3dDetailRef_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dDetailRef_Mapper^ info) { set_Item(index, info); }
		} 
	};

	public ref class a3dObjectRef_Mapper : a3dObjectWithEvent { 
		a3dObjectRef *obj;
		a3dDetailRef_MapperCollection ^detColl;
	internal:
		property a3dObjectRef* ModelObject { a3dObjectRef* get() { return obj; } }
	protected:
		void ObjectChanged() { 
			a3dObjectWithEvent::ObjectChanged(this, EventArgs::Empty);
		}
		void InitializeCollection() { 
			this->detColl = gcnew a3dDetailRef_MapperCollection();
			for( int i = 0; i < ModelObject->detailCount; i++ ) { 
				detColl->Add(gcnew a3dDetailRef_Mapper(ModelObject->detailRef[i]));
			}
		}
	public:
		a3dObjectRef_Mapper(a3dObjectRef *obj) { 
			this->obj = obj;
			InitializeCollection();
		}

		property int DetailCount { int get() { return ModelObject->detailCount; } }
		property a3dDetailRef_MapperCollection^ Details { a3dDetailRef_MapperCollection^ get() { return detColl; } }
		virtual String^ ToString() override { return gcnew String( this->obj->object->name.getCString()); }
	};
	
	public ref class a3dObjectRef_MapperCollection : Collections::CollectionBase { 
		
	public:
		int Add(a3dObjectRef_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dObjectRef_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dObjectRef_Mapper ^info) { return List->IndexOf(info); }

		a3dObjectRef_Mapper ^get_Item(int index) { return dynamic_cast<a3dObjectRef_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dObjectRef_Mapper ^info) { return List->default[index] = info; }
		
		property a3dObjectRef_Mapper^ default [int] { 
			a3dObjectRef_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dObjectRef_Mapper^ info) { set_Item(index, info); }
		} 
	};

	public ref class a3dModelRef_Mapper : a3dObjectWithEvent, public ILightMapCreatorProvider, public IObjectWithBasis { 
	internal:
		a3dModelRef *modelRef;
		a3dObjectRef_MapperCollection ^objColl;
	protected:
		void InitializeCollection() { 
			this->objColl = gcnew a3dObjectRef_MapperCollection();
			for(int i = 0; i < modelRef->objCount; i++) { 
				this->objColl->Add( gcnew a3dObjectRef_Mapper( modelRef->objectRef[i] ) );
			}
		}
	public:
		a3dModelRef_Mapper(a3dModelRef *modelRef) { 
			this->modelRef = modelRef;
			InitializeCollection();
		}

		property int ObjectCount { int get() { return modelRef->objCount; } }
		property a3dObjectRef_MapperCollection^ Objects { a3dObjectRef_MapperCollection^ get() { return objColl; } }

		property String^ Name { 
			String^ get() { return modelRef == null? String::Empty: gcnew String(modelRef->name.getCString()); }
			void set(String^ str) { 
				if(modelRef == null) return;
				shString *hs = shStringHelper::Default.String2shString(str);
				modelRef->name = *hs;
				delete hs;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

#pragma region IObjectWithBasis
		virtual property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				Matrix4_Mapper^ mat = gcnew Matrix4_Mapper();
				if(modelRef == null) return mat;
				mat->FromMatrix4( modelRef->basis );
				return mat;
			}
			void set(Matrix4_Mapper ^mat) { 
				if(modelRef == null) return;
				mat->ToMatrix4( modelRef->basis );
				modelRef->correct();
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		
		virtual property float ScaleX { 
			float get() { return modelRef == null? 1.0f: modelRef->scaleX; }
			void set(float val) { 
				if(modelRef == null) return;
				modelRef->scale( val, modelRef->scaleY, modelRef->scaleZ );
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		virtual property float ScaleY { 
			float get() { return modelRef == null? 1.0f: modelRef->scaleY; }
			void set(float val) { 
				if(modelRef == null) return;
				modelRef->scale( modelRef->scaleX, val, modelRef->scaleZ );
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		virtual property float ScaleZ { 
			float get() { return modelRef == null? 1.0f: modelRef->scaleZ; }
			void set(float val) { 
				if(modelRef == null) return;
				modelRef->scale( modelRef->scaleX, modelRef->scaleY, val );
				ObjectChanged(this, EventArgs::Empty);
			}
		}
#pragma endregion
		property a3dModelDesc_Mapper^ ModelDesc { 
			a3dModelDesc_Mapper^ get() { return modelRef == null? nullptr: gcnew a3dModelDesc_Mapper(modelRef->desc); }
		}

#pragma region ILightMapCreatorProvider
		virtual property a3dLightMapCreator_Mapper^ LightMapCreator { a3dLightMapCreator_Mapper^ get() { return gcnew a3dLightMapCreator_Mapper(dynamic_cast<a3dLightMapCreator*>(this->modelRef)); } } 
#pragma endregion
	};

	[Editor(CollectionEditor::typeid, UITypeEditor::typeid )]
	public ref class a3dModelRef_MapperCollection : Collections::CollectionBase { 
	public:
		int Add(a3dModelRef_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dModelRef_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dModelRef_Mapper ^info) { return List->IndexOf(info); }
		void Remove(a3dModelRef_Mapper ^info) { List->Remove(info); }

		a3dModelRef_Mapper ^get_Item(int index) { return dynamic_cast<a3dModelRef_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dModelRef_Mapper ^info) { return List->default[index] = info; }

		property a3dModelRef_Mapper^ default [int] { 
			a3dModelRef_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dModelRef_Mapper^ info) { set_Item(index, info); }
		} 
		virtual void OnInsertComplete(int index, System::Object^ value) override { 
			a3dModelRef_Mapper^ modelRef = dynamic_cast<a3dModelRef_Mapper^>(value);
			if(modelRef == nullptr) {
				CollectionBase::OnInsertComplete(index, value);
				return;
			}
			if(modelRef->modelRef == null)
				Remove(modelRef);
		}
	};
}