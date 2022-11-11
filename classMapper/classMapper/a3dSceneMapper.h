// gridMapper.h

#pragma once

#include "stringHelper.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "DescMapper.h"
#include "a3dModelRefMapper.h"
#include "a3dLightMapper.h"
#include "a3dParticleSystemDescMapper.h"
#include "a3dGraphMapper\\a3dGraphMapper.h"
#include "a3dTimeProviderMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	ref class a3dSceneMapper;

	public ref class a3dSceneSystemValues : IDependencyTreeNode { 
		a3dSceneMapper ^scene;
		DependencyTreeNodeCollection^ children;
	public:
		a3dSceneSystemValues( a3dSceneMapper ^scene ) { 
			this->scene = scene;
		}
		
		property a3dSceneMapper^ Scene { a3dSceneMapper^ get() { return this->scene; } }
		
#pragma region IDependnecyTreeNode
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return (IDependencyTreeNode^)Scene; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return nullptr; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get(); }
		virtual property String^ NodeName { String^ get() { return gcnew String("SystemValues"); }	}
#pragma endregion
	};

	public ref class a3dSceneMapper : public IDependencyTreeNode { 
		static Object^ sceneChanged = gcnew Object();

		EventHandlerList^					events;
		
		a3dScene							*scene;

		a3dObjectDescBase_MapperCollection	^textures;
		a3dObjectDescBase_MapperCollection	^effects;
		a3dObjectDescBase_MapperCollection	^models;
		a3dObjectDescBase_MapperCollection	^lands;
		a3dObjectDescBase_MapperCollection	^particles;
		a3dObjectDescBase_MapperCollection	^rails;
		a3dObjectDescBase_MapperCollection	^fonts;
		a3dObjectDescBase_MapperCollection	^sounds;
		a3dLightBase_MapperCollection		^lights;
		a3dModelRef_MapperCollection		^modelRef;
		a3dGraphMapperCollection			^graphs;
		
		a3dTimeProviderBaseMapper			^timeProvider;
		DependencyTreeNodeCollection		^depTreeNodes;
		
		void ClearCollection();
		void InitializeCollection();
		void InitializeLandCollection();
		void InitializeModelCollection();
		void InitializeLightsCollection();
		void InitializeParticlesCollection();
		void InitializeModelRefCollection();
		void InitializeTextureCollection();
		void InitializeEffectCollection();
		void InitializeGraphCollection();
		void InitializeRailCollection();
		void InitializeFontCollection();
		void InitializeSoundCollection();
			
	protected:
		virtual a3dGraphMapper^ CreateGraphMapper();
		virtual void RaiseSceneChanged(Object ^obj, EventArgs ^e);
		void	InitializeDefault();
		DependencyTreeNodeCollection^ CreateDependencyTreeNodeCollection();

	public:
		a3dSceneMapper() { InitializeDefault(); }
	
		property bool DesignMode { bool get() { return this->scene->designMode; } void set(bool value) { this->scene->designMode = value; } }
		property System::String^ FileName { System::String^ get() { return gcnew System::String(scene->fileName.getCString()); } }
		property System::String^ FullPathName { System::String^ get() { return gcnew System::String(scene->fullPathName.getCString()); } }
		property String^ DataDirectory { String^ get() { return gcnew String(scene->dataDirectory.getCString()); } }
		property String^ TextureDirectory { String^ get() { return gcnew String(scene->textureDirShort.getCString()); } }
		property String^ EffectDirectory { String^ get() { return gcnew String(scene->effectDirShort.getCString()); } }
		property String^ ModelDirectory { String^ get() { return gcnew String(scene->modelDirShort.getCString()); } }
		property String^ LandDirectory { String^ get() { return gcnew String(scene->landDirShort.getCString()); } }
		property String^ ParticleSystemDirectory { String^ get() { return gcnew String(scene->particleDirShort.getCString()); } }
		property String^ RailSystemDirectory { String^ get() { return gcnew String(scene->railSystemDirShort.getCString()); } }
		property String^ FontDirectory { String^ get() { return gcnew String(scene->fontDirShort.getCString()); } }
		property String^ SoundDirectory { String^ get() { return gcnew String(scene->soundDirShort.getCString()); } }
		property String^ TextureDirectoryFull { String^ get() { return gcnew String(scene->textureDirectory.getCString()); } }
		property String^ EffectDirectoryFull { String^ get() { return gcnew String(scene->effectDirectory.getCString()); } }
		property String^ ModelDirectoryFull { String^ get() { return gcnew String(scene->modelDirectory.getCString()); } }
		property String^ LandDirectoryFull { String^ get() { return gcnew String(scene->landDirectory.getCString()); } }
		property String^ ParticleSystemDirectoryFull { String^ get() { return gcnew String(scene->particleDirectory.getCString()); } }
		property String^ RailSystemDirectoryFull { String^ get() { return gcnew String(scene->railSystemDirectory.getCString()); } }
		property String^ FontDirectoryFull { String^ get() { return gcnew String(scene->fontDirectory.getCString()); } }
		property String^ SoundDirectoryFull { String^ get() { return gcnew String(scene->soundDirectory.getCString()); } }
		property a3dObjectDescBase_MapperCollection^ Textures { a3dObjectDescBase_MapperCollection^ get() { return this->textures; } }
		property a3dObjectDescBase_MapperCollection^ Effects { a3dObjectDescBase_MapperCollection^ get() { return this->effects; } }
		property a3dObjectDescBase_MapperCollection^ Models { a3dObjectDescBase_MapperCollection^ get() { return this->models; } }
		property a3dObjectDescBase_MapperCollection^ Lands { a3dObjectDescBase_MapperCollection^ get() { return this->lands; } }
		property a3dObjectDescBase_MapperCollection^ Particles { a3dObjectDescBase_MapperCollection^ get() { return this->particles; } }
		property a3dObjectDescBase_MapperCollection^ Rails { a3dObjectDescBase_MapperCollection^ get() { return this->rails; } }
		property a3dObjectDescBase_MapperCollection^ Fonts { a3dObjectDescBase_MapperCollection^ get() { return this->fonts; } }
		property a3dObjectDescBase_MapperCollection^ Sounds { a3dObjectDescBase_MapperCollection^ get() { return this->sounds; } }
		property a3dLightBase_MapperCollection^ Lights { a3dLightBase_MapperCollection^ get() { return this->lights; } }
		property a3dModelRef_MapperCollection^ ModelRef { a3dModelRef_MapperCollection^ get() { return this->modelRef; } }
		property a3dGraphMapperCollection^ Graphs { a3dGraphMapperCollection^ get() { return this->graphs; } }
		property a3dTimeProviderBaseMapper^ TimeProvider { a3dTimeProviderBaseMapper^ get() { return this->timeProvider; } }

		event EventHandler^ SceneChanged { 
			void add(EventHandler ^val) { this->events->AddHandler(sceneChanged, val); }
			void remove(EventHandler ^val) { this->events->RemoveHandler(sceneChanged, val); }  
			void raise(Object ^obj, EventArgs ^e) { RaiseSceneChanged(obj, e); }
		}
		
		void SetKernel( Kernel_Mapper ^ kernel ) { this->scene->setKernel( kernel->krn ); }
		void Clear() { this->scene->clear(); }
		void MakeNewScene( String^ dataDir, String^ textureDir, String^ effectDir, String^ modelDir, String^ landDir, String^ partDir, String^ railDir, String^ fontDir, String^ soundDir);

		bool AddTexture( String ^fileName );
		bool AddModel( String ^fileName );
		bool AddModelRef( a3dModelDesc_Mapper ^desc );
		bool RemoveModelRef( a3dModelRef_Mapper ^modelRef );
		bool AddLandscape( String ^name, String ^hmFile, String ^lmFile, String ^effectFileName, float pixelError, int pps, float hscale, float sidescale, int lightMapWidth, int lightMapHeight ) ;
		bool AddRailSystem( String ^name, String ^railFileName, String ^lineModelFileName, String ^roundModelFileName, String ^lineEffectFileName, String ^roundEffectFileName, String ^round2EffectFileName );
		bool AddEffect( String^ fileName );
		void ChangeEffect( a3dShaderMaterialMapper ^material, a3dEffectDesc_Mapper ^effect );
		bool AddOmniLight( String^ lightName, System::Drawing::Color lightColor, float farAttenStart, float farAttenEnd, Vector3_Mapper ^position );
		bool AddDirectionalLight( String^ lightName, System::Drawing::Color lightColor, Vector3_Mapper ^direction );
		bool AddParticleSystem( String^ particleName, particleSystemBase_Mapper ^particle );
		bool SaveScene( String ^fileName );
		bool LoadScene( String ^sceneFileName, String ^dataDirName );
		bool ImportModel( String ^textModelFileName, String ^modelFileName );

		long Render( camera_Mapper^ cam ) { return this->scene->render( cam->cam ); }
		long Update( camera_Mapper^ cam ) { return this->scene->update( cam->cam );	}

#pragma region IDependnecyTreeNode
		virtual property IDependencyTreeNode^ Parent { IDependencyTreeNode^ get() { return nullptr; } }
		virtual property IDependencyPropertyOwner^ DependencyOwner { IDependencyPropertyOwner^ get() { return nullptr; } }
		virtual property DependencyTreeNodeCollection^ Children { DependencyTreeNodeCollection^ get(); }
		virtual property String^ NodeName { String^ get() { return gcnew String("Scene"); }	}
#pragma endregion
	};
}