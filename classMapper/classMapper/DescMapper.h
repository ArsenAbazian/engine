// cameraMapper.h

#pragma once

#include "a3dDesc\\a3dDesc.h"
#include "a3dModel\\a3dModel.h"
#include "a3dModel\\a3dObject.h"
#include "a3dModel\\a3dDetail.h"
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
#include "a3dShaderParamsMapper.h"
#include "interfaces.h"
#include "DependencyMapper.h"
#include "a3dMaterialMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class a3dLightMapInfo_Mapper : public a3dObjectWithEvent { 
		a3dLightMapInfo *lightMapInfo;		
		StringCollection ^coll;
		property a3dLightMapInfo* LightMapInfo { a3dLightMapInfo* get() { return lightMapInfo; } }
	internal:
		void setAutoFileName(String^ str) { this->autoFileName = str; }
	public:
		a3dLightMapInfo_Mapper( a3dLightMapInfo *lightMapInfo ) { 
			this->lightMapInfo = lightMapInfo;
			this->autoFileName = String::Empty;
			this->coll = gcnew StringCollection();
		}

		property bool Enabled { 
			bool get() { return LightMapInfo->enabled; }
			void set(bool value) { 
				LightMapInfo->enabled = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		
		property String^ LightMapFileName {  
			String^ get() { return gcnew String(LightMapInfo->lightMapFileName.getCString()); }
			void	set( String^ value ) { 
				shStringHelper::Default.copyTo(LightMapInfo->lightMapFileName, value);			
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	
		property String^ LightMapTextureParamName { 
			String^ get() { return gcnew String(LightMapInfo->lightMapTextureParamName.getCString()); }
			void set(String^ value) {
				shStringHelper::Default.copyTo(LightMapInfo->lightMapTextureParamName, value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int LightMapWidth { 
			int get() { return LightMapInfo->lightMapWidth; }
			void set(int value) { 
				LightMapInfo->lightMapWidth = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int LightMapHeight { 
			int get() { return LightMapInfo->lightMapHeight; }
			void set(int value) { 
				LightMapInfo->lightMapHeight = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int TextureCoorIndex { 
			int get() { return LightMapInfo->textureCoorIndex; }
			void set( int value ) { 
				LightMapInfo->textureCoorIndex = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		
		property bool IsAtlasTextureCoords { 
			bool get() { return LightMapInfo->isAtlasTextureCoords; }
			void set( bool value ) { 
				LightMapInfo->isAtlasTextureCoords = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float U0 { 
			float get() { return LightMapInfo->u0; }
			void set(float value) { 
				LightMapInfo->u0 = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float V0 { 
			float get() { return LightMapInfo->v0; }
			void set(float value) { 
				LightMapInfo->v0 = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float USize { 
			float get() { return LightMapInfo->uSize; }
			void set(float value) { 
				LightMapInfo->uSize = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float VSize { 
			float get() { return LightMapInfo->vSize; }
			void set(float value) { 
				LightMapInfo->vSize = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		String ^autoFileName;
		property String^ AutoFileName { String^ get() { return autoFileName; } }
		property StringCollection^ TextureParamColl { StringCollection^ get() { return coll; } }
	};

	public ref class a3dObjectDescBase_Mapper : a3dObjectWithEvent, IDependencyTreeNode {
	internal:
		a3dObjectDescBase *desc;
	
	public:
		a3dObjectDescBase_Mapper() : a3dObjectWithEvent() { 
			desc = null;
		}
		a3dObjectDescBase_Mapper( a3dObjectDescBase *desc ) : a3dObjectWithEvent() { 
			this->desc = desc;
		}

		property String^ Name { 
			String^ get() { return this->desc == null? String::Empty: gcnew String(this->desc->name.getCString()); } 
			void set( String^ val ) { 
				shStringHelper::Default.copyTo(this->desc->name, val);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		property bool Used { 
			bool get() { return this->desc == null? false: this->desc->used; }
			void set(bool val) { 
				if(this->desc != null) 
				this->desc->used = val; 
				ObjectChanged(this, EventArgs::Empty);
			}
		}

#pragma region IDependnecyTreeNode
	protected: IDependencyTreeNode^ parent;
	protected: String^ nodeName;
	protected: DependencyTreeNodeCollection ^depTreeNodes;
	internal: 
		void SetDependencyParent(IDependencyTreeNode^ parent) { 
			this->parent = parent;
		}
		void SetNodeName( String^ str ) { 
			this->nodeName = str;
		}
	protected:
		DependencyTreeNodeCollection^ CreateDependencyTreeNodeCollection() { 
			return gcnew DependencyTreeNodeCollection();
		}
	public:		
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return nullptr; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return nullptr; } }
		virtual property DependencyTreeNodeCollection^ Children { 
			DependencyTreeNodeCollection^ get() {  
				if( this->depTreeNodes == nullptr ) { 
					this->depTreeNodes = CreateDependencyTreeNodeCollection();
				}
				return this->depTreeNodes;
			} 
		}
		virtual property String^ NodeName {
			String^ get() { return nullptr; }
		}
#pragma endregion
	};
	
	public ref class a3dTextureDesc_Mapper : a3dObjectDescBase_Mapper { 
	internal:
		
		property a3dTextureDesc* TexDesc { a3dTextureDesc* get() { return dynamic_cast<a3dTextureDesc*>(desc); } }
	public:
		a3dTextureDesc_Mapper( a3dTextureDesc *desc ) : a3dObjectDescBase_Mapper(desc) { }

		property String^ FullPathName { String^ get() { return TexDesc == null? String::Empty: gcnew String(TexDesc->fullPathName.getCString()); } }			
		property String^ FileName { String^ get() { return TexDesc == null? String::Empty: gcnew String(TexDesc->fileName.getCString()); } }			
	};

	public ref class a3dEffectDesc_Mapper : a3dObjectDescBase_Mapper { 
	internal:

		property a3dEffectDesc* EffDesc { a3dEffectDesc* get() { return dynamic_cast<a3dEffectDesc*>(desc); } }
	public:
		a3dEffectDesc_Mapper( a3dEffectDesc *desc ) : a3dObjectDescBase_Mapper(desc) { }

		property String^ FullPathName { String^ get() { return EffDesc == null? String::Empty: gcnew String(EffDesc->fullPathName.getCString()); } }			
		property String^ FileName { String^ get() { return EffDesc == null? String::Empty: gcnew String(EffDesc->fileName.getCString()); } }			
		virtual String^ ToString() override { return FileName; }
	};

	public ref class a3dDetail_Mapper : a3dObjectWithEvent, public ILightMapInfo_MapperProvider { 
		a3dDetail *detail;
		a3dLightMapInfo_Mapper ^lightMapInfo;
	internal:
		property a3dDetail* Detail { a3dDetail* get() { return detail; } }
	protected:
		void ObjectChanged() { 
			a3dObjectWithEvent::ObjectChanged(this, EventArgs::Empty);
		}
	public:
		a3dDetail_Mapper(a3dDetail *detail) {
			this->detail = detail;
			this->lightMapInfo = gcnew a3dLightMapInfo_Mapper(&detail->lightMapInfo);
		}
		property String^ MaterialName { String^ get() { return gcnew String(Detail->material->name.getCString()); } }
		virtual String^ ToString() override { return MaterialName; }
		virtual property a3dLightMapInfo_Mapper^ LightMapInfo { a3dLightMapInfo_Mapper^ get() { return lightMapInfo; } }
	};

	public ref class a3dDetail_MapperCollection : Collections::CollectionBase { 
		
	public:
		int Add(a3dDetail_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dDetail_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dDetail_Mapper ^info) { return List->IndexOf(info); }

		a3dDetail_Mapper ^get_Item(int index) { return dynamic_cast<a3dDetail_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dDetail_Mapper ^info) { return List->default[index] = info; }
		
		property a3dDetail_Mapper^ default [int] { 
			a3dDetail_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dDetail_Mapper^ info) { set_Item(index, info); }
		} 
	};

	public ref class a3dObject_Mapper : a3dObjectWithEvent { 
		a3dObject *obj;
		a3dDetail_MapperCollection ^detColl;
	internal:
		property a3dObject* ModelObject { a3dObject* get() { return obj; } }
	protected:
		void ObjectChanged() { 
			a3dObjectWithEvent::ObjectChanged(this, EventArgs::Empty);
		}
		void InitializeCollection() { 
			this->detColl = gcnew a3dDetail_MapperCollection();
			for( int i = 0; i < ModelObject->detailCount; i++ ) { 
				detColl->Add(gcnew a3dDetail_Mapper(&ModelObject->detail[i]));
			}
		}
	public:
		a3dObject_Mapper(a3dObject *obj) { 
			this->obj = obj;
			InitializeCollection();
		}

		property String^ Name { String^ get() { return gcnew String(ModelObject->name.getCString()); } }
		property int DetailCount { int get() { return ModelObject->detailCount; } }
		property a3dDetail_MapperCollection^ Details { a3dDetail_MapperCollection^ get() { return detColl; } }
		virtual String^ ToString() override { return Name; }
	};

	public ref class a3dObject_MapperCollection : Collections::CollectionBase { 
		
	public:
		int Add(a3dObject_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dObject_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dObject_Mapper ^info) { return List->IndexOf(info); }

		a3dObject_Mapper ^get_Item(int index) { return dynamic_cast<a3dObject_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dObject_Mapper ^info) { return List->default[index] = info; }
		
		property a3dObject_Mapper^ default [int] { 
			a3dObject_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dObject_Mapper^ info) { set_Item(index, info); }
		} 
	};
	
	public ref class a3dModelDesc_Mapper : a3dObjectDescBase_Mapper { 
		a3dObject_MapperCollection ^objColl;
		a3dMaterialBaseMapperCollection ^materialColl;
		bool						modelChanged;
	protected:
		void InitializeObjectCollection() { 
			this->objColl = gcnew a3dObject_MapperCollection();
			for(int i = 0; i < ModelDesc->model->objCount; i++) { 
				this->objColl->Add( gcnew a3dObject_Mapper( &ModelDesc->model->object[i] ) );
			}
		}
		void InitializeMaterialCollection() {
			this->materialColl = gcnew a3dMaterialBaseMapperCollection();
			for(int i = 0; i < ModelDesc->model->materialCount; i++) { 
				if(ModelDesc->model->material[i]->type == MT_SHADER)
					this->materialColl->Add(gcnew a3dShaderMaterialMapper((a3dShaderMaterial*)ModelDesc->model->material[i]));
			}
		}
	internal:

		property a3dModelDesc* ModelDesc { a3dModelDesc* get() { return dynamic_cast<a3dModelDesc*>(desc); } }
	public:
		a3dModelDesc_Mapper( a3dModelDesc *desc ) : a3dObjectDescBase_Mapper(desc) { 
			this->modelChanged = false;
			InitializeObjectCollection();
			InitializeMaterialCollection();
		}

		property String^ Name { 
			String^ get() { 
				return ModelDesc == null? String::Empty: gcnew String(ModelDesc->name.getCString()); 
			}
			void set(String^ str) { 
				if(ModelDesc == null) return;
				shStringHelper::Default.copyTo(ModelDesc->name, str);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
		property String^ FullPathName { String^ get() { return ModelDesc == null? String::Empty: gcnew String(ModelDesc->fullPathName.getCString()); } }			
		property String^ FileName { String^ get() { return ModelDesc == null? String::Empty: gcnew String(ModelDesc->fileName.getCString()); } }			
	
		property int ObjectCount { int get() { return ModelDesc->model->objCount; } }
		property a3dObject_MapperCollection^ Objects { a3dObject_MapperCollection^ get() { return objColl; } }
	
		property int MaterialCount { int get() { return ModelDesc->model->materialCount; } }
		property a3dMaterialBaseMapperCollection^ Materials { a3dMaterialBaseMapperCollection^ get() { return this->materialColl; } }

		virtual String^ ToString() override { 
			if(ModelDesc->name.getCString() == null)
				return FileName; 
			return Name; 
		}

		property bool ModelChanged { 
			bool get() { return this->modelChanged; }
			void set(bool value) { this->modelChanged = value; }
		}

		bool SaveModel() { 
			return ModelDesc->model->saveToFile(ModelDesc->fullPathName);
		}
	};
	
	public ref class a3dLandPassInfo_Mapper { 
	internal:
		a3dLandPassInfo *info;

		virtual void UpdateLandMapPassInfoSimpleProperties() { 
			if(info == null) return;
			landMapPassInfo *passInfo = this->getPass();
			if(passInfo == null) { 
				this->info->landInfo->land->lkrn->ErrorBox( TEXT("a3dSceneMapper"), TEXT("Error: can't get pass by index! Values not updated") );
				return ;
			}
			passInfo->enabled = info->enabled;
			passInfo->animated = info->animated;
			passInfo->mapUSize = info->mapUSize;
			passInfo->mapVSize = info->mapVSize;
			passInfo->detUSize = info->detUSize;
			passInfo->detVSize = info->detVSize;
			passInfo->minDetailDistance = info->minDetailDist;
			passInfo->maxDetailDistance = info->maxDetailDist;
			passInfo->maxDetailAlphaValue = info->maxDetailAlphaValue;
		}

		virtual bool CheckPassAndTexture(shString &fileName, String ^val, landMapPassInfo **passInfo, a3dTextureDesc **texDesc) { 
			if(info == null) return false;
			shString tmp;
			shStringHelper::Default.copyTo( tmp, val );
			*passInfo = getPass();
			if(*passInfo == null) {
				this->info->landInfo->land->lkrn->ErrorBox( TEXT("a3dSceneMapper"), TEXT("Error: can't get pass by index") );
				return false;
			}
			*texDesc = this->getTexture(tmp);
			if(texDesc == null) { 
				this->info->landInfo->land->lkrn->ErrorBox( TEXT("a3dSceneMapper"), TEXT("Error: can't get texture") );
				return false;
			}
			fileName = tmp;
			return true;
		}
	public:
		a3dLandPassInfo_Mapper(a3dLandPassInfo *info) { this->info = info; }

		property bool Enabled { 
			bool get() { return info->enabled; } 
			void set(bool val) { 
				info->enabled = val; 
				this->UpdateLandMapPassInfoSimpleProperties();
			} 
		}
		property bool Animated { 
			bool get() { return info->animated; } 
			void set(bool val) { 
				info->animated = val; 
				this->UpdateLandMapPassInfoSimpleProperties();
				info->landInfo->land->calcPassEnabled(info->passIndex);
			} 
		}
		
		landMapPassInfo *getPass() { 
			if(this->info == null) return null;
			return this->info->landInfo->land->passInfo[info->passIndex];
		}

		a3dTextureDesc *getTexture( shString &fileName ) { 
			return this->info->landInfo->land->scene->getTexture(fileName);
		}
		
		property String^ Name { 
			String^ get() { return gcnew String(info->name.getCString()); }
			void set(String ^val) {  
				shStringHelper::Default.copyTo( info->name, val );
			}
		}

		property String^ MapFileName { 
			String^ get() { return info == null? String::Empty: gcnew String(info->mapFileName.getCString()); } 
			void set(String ^val) { 
				shString tmp;
				landMapPassInfo *passInfo;
				a3dTextureDesc *texDesc;
				if( !CheckPassAndTexture(tmp, val, &passInfo, &texDesc) ) return;
				passInfo->mapTextureDesc = texDesc;
				info->mapFileName = tmp;
			}
		}			
		property String^ MaskFileName { 
			String^ get() { return info == null? String::Empty: gcnew String(info->maskFileName.getCString()); } 
			void set(String ^val) { 
				shString tmp;
				landMapPassInfo *passInfo;
				a3dTextureDesc *texDesc;
				if( !CheckPassAndTexture(tmp, val, &passInfo, &texDesc) ) return;
				passInfo->setMaskTexture( texDesc );
				info->maskFileName = tmp;
				info->landInfo->land->calcPassEnabled(info->passIndex);
			}
		}			
		property String^ DetailFileName { 
			String^ get() { return info == null? String::Empty: gcnew String(info->detailFileName.getCString()); } 
			void set( String ^val ) {
				shString tmp;
				landMapPassInfo *passInfo;
				a3dTextureDesc *texDesc;
				if( !CheckPassAndTexture(tmp, val, &passInfo, &texDesc) ) return;
				passInfo->detailTextureDesc = texDesc;
				info->detailFileName = tmp;
			}
		}			

		property float MapUSize { 
			float get() { return info == null? 0.0f: info->mapUSize; } 
			void set(float val) { 
				if(info == null) return;
				info->mapUSize = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}
		property float MapVSize { 
			float get() { return info == null? 0.0f: info->mapVSize; } 
			void set(float val) { 
				if(info == null) return;
				info->mapVSize = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}

		property float DetailUSize { 
			float get() { return info == null? 0.0f: info->detUSize; } 
			void set(float val) {  
				if( info == null ) return;
				info->detUSize = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}
		property float DetailVSize { 
			float get() { return info == null? 0.0f: info->detVSize; } 
			void set(float val) { 
				if( info == null ) return;
				info->detVSize = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}

		property float MinDetailDistance { 
			float get() { return info == null? 0.0f: info->minDetailDist; } 
			void set(float val) {
				if( info == null ) return;
				info->minDetailDist = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}
		property float MaxDetailDistance { 
			float get() { return info == null? 0.0f: info->maxDetailDist; } 
			void set(float val) { 
				if( info == null ) return;
				info->maxDetailDist = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}
		property float MaxDetailAlphaValue { 
			float get() { return info == null? 0.0f: info->maxDetailAlphaValue; } 
			void set( float val ) { 
				if( info == null ) return;
				info->maxDetailAlphaValue = val;
				this->UpdateLandMapPassInfoSimpleProperties();
			}
		}

		property int PassIndex { int get() { return info->passIndex; } }
	};

	public ref class a3dLandPassInfo_MapperCollection : Collections::CollectionBase { 
		
	public:
		int Add(a3dLandPassInfo_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dLandPassInfo_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dLandPassInfo_Mapper ^info) { return List->IndexOf(info); }

		a3dLandPassInfo_Mapper ^get_Item(int index) { return dynamic_cast<a3dLandPassInfo_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dLandPassInfo_Mapper ^info) { return List->default[index] = info; }
		
		property a3dLandPassInfo_Mapper^ default [int] { 
			a3dLandPassInfo_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dLandPassInfo_Mapper^ info) { set_Item(index, info); }
		} 
	};
	
	public ref class a3dLandscapeDesc_Mapper : a3dObjectDescBase_Mapper, public IObjectWithBasis { 
	internal:
		void LandChanged() { ObjectChanged(this, EventArgs::Empty); }
		property a3dLandscapeDesc* LandDesc { a3dLandscapeDesc* get() { return dynamic_cast<a3dLandscapeDesc*>(desc); } }
		a3dLandPassInfo_MapperCollection ^coll;

		bool reinitializeLand() { 
			if(LandDesc == null) return true;
			HRESULT rv = LandDesc->land->loadHeightMap( LandDesc->landInfo->heightMapFileName, Width, Height, PatchPerSide );
			if( FAILED( rv ) )
				return false;
			return true;
		}

	public:
		a3dLandscapeDesc_Mapper( a3dLandscapeDesc *desc ) : a3dObjectDescBase_Mapper(desc) { 
			
			this->coll = gcnew a3dLandPassInfo_MapperCollection();
			for(int i = 0; i < 16; i++) { 
				Pass->Add(gcnew a3dLandPassInfo_Mapper( &LandDesc->landInfo->pass[i] ));
			}
		}

		property String^ HeightMapFileName { 
			String^ get() { return LandDesc == null? String::Empty: gcnew String(LandDesc->landInfo->heightMapFileName.getCString()); } 
			void set(String^ val) { 
				if( LandDesc == null ) return;
				shStringHelper::Default.copyTo(LandDesc->landInfo->heightMapFileName, val);
				LandDesc->land->heightMapFileName = LandDesc->landInfo->heightMapFileName;
				if( !reinitializeLand() ) { 
					LandDesc->land->lkrn->ErrorBox( TEXT("a3dLandscapeDesc_Mapper"), TEXT("Error: can't load heightMap! '%s'"), LandDesc->landInfo->heightMapFileName.getCString() );
					return;
				}
				LandChanged();
			}
		}
		property String^ EffectFileName { 
			String^ get() { return LandDesc == null? String::Empty: gcnew String(LandDesc->landInfo->effectFileName.getCString()); } 
			void set( String^ val ) { 
				if( LandDesc == null ) return;
				shStringHelper::Default.copyTo(LandDesc->landInfo->effectFileName, val);
				LandDesc->land->effectFileName = LandDesc->landInfo->effectFileName;
				HRESULT rv = LandDesc->land->createEffect();
				if( FAILED( rv ) ) {
					LandDesc->land->lkrn->ErrorBox( TEXT("a3dLandscapeDesc_Mapper"), TEXT("Error: can't create effect! '%s'"), LandDesc->landInfo->effectFileName.getCString() );
					return;
				}
				LandChanged();
			}
		}
		property String^ LightMapFileName { 
			String^ get() { return LandDesc == null? String::Empty: gcnew String(LandDesc->landInfo->lightMapFileName.getCString()); } 
			void set( String^ val ) { 
				if( LandDesc == null ) return;
				shStringHelper::Default.copyTo(LandDesc->landInfo->lightMapFileName, val);
				LandDesc->land->lightMapTextureDesc = LandDesc->land->scene->getTexture( LandDesc->landInfo->lightMapFileName );
				LandChanged();
			}
		}
		property float Width { 
			float get() { return LandDesc == null? 0.0f: LandDesc->landInfo->width; } 
			void set(float w) {
				if( LandDesc == null ) return;
				LandDesc->landInfo->width = w;
				if( !reinitializeLand() ) { 
					LandDesc->land->lkrn->ErrorBox( TEXT("a3dLandscapeDesc_Mapper"), TEXT("Error: can't load heightMap. invalid sideScale -> %f!"), w );
					return;
				}
				LandChanged();
			}
		}
		property float Height { 
			float get() { return LandDesc == null? 0.0f: LandDesc->landInfo->height; } 
			void set(float h) {
				if( LandDesc == null ) return;
				LandDesc->landInfo->height = h;
				if( !reinitializeLand() ) { 
					LandDesc->land->lkrn->ErrorBox( TEXT("a3dLandscapeDesc_Mapper"), TEXT("Error: can't load heightMap. invalid heightScale -> %f!"), h );
					return;
				}
				LandChanged();
			}
		}
		property float PixelError { 
			float get() { return LandDesc == null? 0.0f: LandDesc->landInfo->pixelError; } 
			void set(float pe) { 
				if(LandDesc == null) return;
				LandDesc->land->setPixelError(pe);
				LandDesc->landInfo->pixelError = pe;
				LandChanged();
			}
		}
		property int PatchPerSide { 
			int get() { return LandDesc == null? 0: LandDesc->landInfo->patchPerSide; } 
			void set(int pps) {
				if( LandDesc == null ) return;
				LandDesc->landInfo->patchPerSide = pps;
				if( !reinitializeLand() ) { 
					LandDesc->land->lkrn->ErrorBox( TEXT("a3dLandscapeDesc_Mapper"), TEXT("Error: can't load heightMap. invalid patchPerSide -> %d!"), pps );
					return;
				}
				LandChanged();
			}
		}
		property int LightMapWidth { 
			int get() { return LandDesc == null? 0: LandDesc->landInfo->lightMapWidth; } 
			void set(int lmw) { 
				if(LandDesc == null) return;
				LandDesc->land->lightMapWidth = lmw;
				LandDesc->landInfo->lightMapWidth = lmw;
				LandChanged();
			}
		}
		property int LightMapHeight { 
			int get() { return LandDesc == null? 0: LandDesc->landInfo->lightMapHeight; } 
			void set(int lmh) {
				if(LandDesc == null) return;
				LandDesc->land->lightMapWidth = lmh;
				LandDesc->landInfo->lightMapWidth = lmh;
				LandChanged();
			}
		}
		property int PassCount { int get() { return this->LandDesc->land->passCount; } }
		property a3dLandPassInfo_MapperCollection^ Pass { a3dLandPassInfo_MapperCollection^ get() { return coll; } }

		void insertPass(int passIndex) { 
			LandDesc->land->insertPass(passIndex, new landMapPassInfo(LandDesc->land, null, null, null, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f, 1.0f, 0.0f));
			LandChanged();
		}

		void removePass(int passIndex) { 
			LandDesc->land->removePass(passIndex);
			LandChanged();
		}

#pragma region IObjectWithBasis
		virtual property float ScaleX { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleY { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleZ { float get() { return 1.0f; } void set( float f ) { } }
		virtual property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				if(LandDesc == null) return nullptr;
				Matrix4_Mapper ^mat = gcnew Matrix4_Mapper();
				mat->FromMatrix4( LandDesc->land->basis );
				return mat;
			} 
			void set(Matrix4_Mapper ^mat) { 
				if(LandDesc == null) return;
				mat->ToMatrix4(LandDesc->land->basis);
				copyMatrix4(LandDesc->land->basis, LandDesc->landInfo->basis);
				LandChanged();
			} 
		}
#pragma endregion IObjectWithBasis
	};

	public ref class a3dParticleSystemInfo_Mapper : public a3dObjectWithEvent, public IObjectWithBasis { 
		a3dParticleSystemInfo *info;
	internal:
		property a3dParticleSystemInfo* Info { a3dParticleSystemInfo* get() { return info; } }
	public:
		a3dParticleSystemInfo_Mapper() { 
			this->info = new a3dParticleSystemInfo();
		}

		property String^ ParticleName { 
			String^ get() { return gcnew String(Info->name.getCString()); }  
			void set(String ^value) { 
				shStringHelper::Default.copyTo(Info->name, value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property String^ EffectFileName { 
			String^ get() { return gcnew String(Info->effectFileName.getCString()); }
			void set(String ^value) { 
				shStringHelper::Default.copyTo(Info->effectFileName, value);
			}
		}
#pragma region IObjectWithBasis
		virtual property float ScaleX { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleY { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleZ { float get() { return 1.0f; } void set( float f ) { } }
		virtual property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				Matrix4_Mapper ^mat = gcnew Matrix4_Mapper();
				mat->FromMatrix4( Info->basis );
				return mat;
			} 
			void set(Matrix4_Mapper ^mat) { 
				mat->ToMatrix4(Info->basis);
				ObjectChanged(this, EventArgs::Empty);
			} 
		}
#pragma endregion IObjectWithBasis
	};

	public ref class a3dRailSystemDesc_Mapper : a3dObjectDescBase_Mapper, public IObjectWithBasis { 
	internal:

		property a3dRailSystemDesc* RailDesc { a3dRailSystemDesc* get() { return dynamic_cast<a3dRailSystemDesc*>(desc); } }
	public:
		a3dRailSystemDesc_Mapper( a3dRailSystemDesc *desc ) : a3dObjectDescBase_Mapper(desc) { }

		property String^ RailFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->railFileName.getCString()); } }
		property String^ LineModelFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->lineModelFileName.getCString()); } }
		property String^ RoundModelFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->roundModelFileName.getCString()); } }
		property String^ LineEffectFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->lineEffectFileName.getCString()); } }
		property String^ RoundEffectFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->roundEffectFileName.getCString()); } }
		property String^ Round2EffectFileName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->round2EffectFileName.getCString()); } }
		property String^ LandName { String^ get() { return RailDesc == null? String::Empty: gcnew String(RailDesc->rsInfo->landName.getCString()); } }
		property int LandHeightPixelX { int get() { return RailDesc == null? 0: RailDesc->rsInfo->landHeightPixelX; } }
		property int LandHeightPixelY { int get() { return RailDesc == null? 0: RailDesc->rsInfo->landHeightPixelY; } }
	
#pragma region IObjectWithBasis
		virtual property float ScaleX { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleY { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleZ { float get() { return 1.0f; } void set( float f ) { } }
		virtual property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				if(RailDesc == null) return nullptr;
				Matrix4_Mapper ^mat = gcnew Matrix4_Mapper();
				mat->FromMatrix4( RailDesc->rsInfo->basis);
				return mat;
			} 
			void set(Matrix4_Mapper ^mat) { 
				if(RailDesc == null) return;
				mat->ToMatrix4(RailDesc->rsInfo->basis);
				RailDesc->rSystem->setOrigin(RailDesc->rsInfo->basis[3][0], RailDesc->rsInfo->basis[3][1], RailDesc->rsInfo->basis[3][2]);
				ObjectChanged(this, EventArgs::Empty);
			} 
		}
#pragma endregion IObjectWithBasis
	
	};

	public ref class a3dFontDesc_Mapper : a3dObjectDescBase_Mapper { 
	internal:

		property a3dFontDesc* FontDesc { a3dFontDesc* get() { return dynamic_cast<a3dFontDesc*>(desc); } }
	public:
		a3dFontDesc_Mapper( a3dFontDesc *desc ) : a3dObjectDescBase_Mapper(desc) { }

		property String^ FullPathName { String^ get() { return FontDesc == null? String::Empty: gcnew String(FontDesc->fullPathName.getCString()); } }
		property String^ FileName { String^ get() { return FontDesc == null? String::Empty: gcnew String(FontDesc->fileName.getCString()); } }
		property String^ PosFullPathName { String^ get() { return FontDesc == null? String::Empty: gcnew String(FontDesc->posFullPathName.getCString()); } }
		property String^ PosFileName { String^ get() { return FontDesc == null? String::Empty: gcnew String(FontDesc->posFileName.getCString()); } }
	};
	
	public ref class a3dSoundDesc_Mapper : a3dObjectDescBase_Mapper { 
	internal:

		property a3dSoundDesc* SoundDesc { a3dSoundDesc* get() { return dynamic_cast<a3dSoundDesc*>(desc); } }
	public:
		a3dSoundDesc_Mapper( a3dSoundDesc *desc ) : a3dObjectDescBase_Mapper(desc) { }

		property String^ FullPathName { String^ get() { return SoundDesc == null? String::Empty: gcnew String(SoundDesc->fullPathName.getCString()); } }
		property String^ FileName { String^ get() { return SoundDesc == null? String::Empty: gcnew String(SoundDesc->fileName.getCString()); } }
	};

	public enum class LightType { LightNone, OmniLight, DirectionLight };

	ref class CollectionEditor;
	
	[Editor(CollectionEditor::typeid, UITypeEditor::typeid )]
	public ref class a3dObjectDescBase_MapperCollection : Collections::CollectionBase, IDependencyTreeNode { 

	public:
		int Add(a3dObjectDescBase_Mapper ^info) { return List->Add( info ); }
		void Insert(int index, a3dObjectDescBase_Mapper ^info) { List->Insert(index, info); }
		int IndexOf(a3dObjectDescBase_Mapper ^info) { return List->IndexOf(info); }
		void Remove(a3dObjectDescBase_Mapper ^info) { List->Remove(info); }

		a3dObjectDescBase_Mapper ^get_Item(int index) { return dynamic_cast<a3dObjectDescBase_Mapper ^>(List->default[index]); }
		void set_Item(int index, a3dObjectDescBase_Mapper ^info) { return List->default[index] = info; }

		property a3dObjectDescBase_Mapper^ default [int] { 
			a3dObjectDescBase_Mapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dObjectDescBase_Mapper^ info) { set_Item(index, info); }
		} 

		virtual void OnInsertComplete(int index, System::Object^ value) override { 
			a3dObjectDescBase_Mapper^ info = dynamic_cast<a3dObjectDescBase_Mapper^>(value);
			if(info == nullptr) {
				CollectionBase::OnInsertComplete(index, value);
				return;
			}
			if(info->desc == null)
				Remove(info);
		}

	protected:
		IDependencyTreeNode^ parent;
		String^ nodeName;
	internal: 
		void SetDependencyParent(IDependencyTreeNode^ parent) { 
			this->parent = parent;
		}
		void SetNodeName( String^ str ) { 
			this->nodeName = str;
		}
	public:			
#pragma region IDependencyTreeNode
	protected:
		DependencyTreeNodeCollection^ depChildren;
		DependencyTreeNodeCollection^ CreateDependencyTreeNodeCollection() { 
			DependencyTreeNodeCollection^ coll = gcnew DependencyTreeNodeCollection();
			for(int i = 0; i < this->Count; i++) { 
				a3dObjectDescBase_Mapper^ node = this->get_Item(i);
				node->SetDependencyParent(this);
				coll->Add(node);
			}
			return coll;
		}
	public:
		virtual property IDependencyTreeNode^ Parent { 
			IDependencyTreeNode^ get() { return this->parent; }
		}
		virtual property IDependencyPropertyOwner^ DependencyOwner { 
			IDependencyPropertyOwner^ get() { return nullptr; }
		}
		virtual property DependencyTreeNodeCollection^ Children { 
			DependencyTreeNodeCollection^ get() {  
				if(this->depChildren == nullptr)
					this->depChildren = CreateDependencyTreeNodeCollection();
				return this->depChildren;
			}
		} 
		virtual property String^ NodeName { 
			String^ get() { return this->nodeName; }
		}
#pragma endregion
	};

	public ref class CollectionEditor : public UITypeEditor { 
	public:
		CollectionEditor() {}
		virtual UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext ^ context) override { 
			return UITypeEditorEditStyle::Modal;
		}
		virtual Object^ EditValue( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) override { 
			return GetEditValueCore( context, provider, value );
		}
		Object ^ GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value );
	};
}