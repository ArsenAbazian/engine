#include "stdafx.h"
#include "a3dSceneMapper.h"

namespace classMapper {

	void a3dSceneMapper::ClearCollection() { 
		
		Textures->Clear();
		Effects->Clear();
		Models->Clear();
		Lands->Clear();
		Particles->Clear();
		Rails->Clear();
		Fonts->Clear();
		Sounds->Clear();
		Lights->Clear();
		Graphs->Clear();
	}

	void a3dSceneMapper::InitializeCollection() { 

		ClearCollection();
		
		this->InitializeTextureCollection();
		this->InitializeEffectCollection();
		this->InitializeModelCollection();
		this->InitializeLandCollection();
		this->InitializeParticlesCollection();
		this->InitializeRailCollection();
		this->InitializeFontCollection();			
		this->InitializeSoundCollection();
		this->InitializeLightsCollection();
		this->InitializeModelRefCollection();
		this->InitializeGraphCollection();
	}

	void a3dSceneMapper::InitializeRailCollection() { 
		
		Rails->Clear();
		for( int index = 0; index < this->scene->railDescCount; index++ )
			Rails->Add( gcnew a3dRailSystemDesc_Mapper( this->scene->rails[index] ) );
	}

	void a3dSceneMapper::InitializeFontCollection() {

		Fonts->Clear();
		for( int index = 0; index < this->scene->fontDescCount; index++ )
			Fonts->Add( gcnew a3dFontDesc_Mapper( this->scene->fonts[index] ) );
	}

	void a3dSceneMapper::InitializeSoundCollection() { 
		
		Sounds->Clear();
		for( int index = 0; index < this->scene->soundDescCount; index++ )
			Sounds->Add( gcnew a3dSoundDesc_Mapper( this->scene->sounds[index] ) );
	}

	void a3dSceneMapper::InitializeLandCollection() { 

		Lands->Clear();
		for( int index = 0; index < this->scene->landDescCount; index++ )
			Lands->Add( gcnew a3dLandscapeDesc_Mapper( this->scene->lands[index] ) );
	}

	void a3dSceneMapper::InitializeModelCollection() { 
		
		Models->Clear();
		for( int index = 0; index < this->scene->modelDescCount; index++ )
			Models->Add( gcnew a3dModelDesc_Mapper( this->scene->models[index] ) );
	}

	void a3dSceneMapper::InitializeLightsCollection() { 
		
		Lights->Clear();
		for( int index = 0; index < this->scene->lightsCount; index++ ) { 
			if( this->scene->lights[index]->type == LIGHT_OMNI )
				Lights->Add( gcnew a3dOmniLight_Mapper( dynamic_cast<a3dOmniLight*>(this->scene->lights[index]) ) );
			else if( this->scene->lights[index]->type == LIGHT_DIRECTIONAL )
				Lights->Add( gcnew a3dDirectionalLight_Mapper( dynamic_cast<a3dDirectionalLight*>(this->scene->lights[index]) ) );
		}	
	}

	void a3dSceneMapper::InitializeParticlesCollection() { 

		Particles->Clear();
		for( int index = 0; index < this->scene->particleDescCount; index++ ) { 
			Particles->Add( gcnew a3dParticleSystemDesc_Mapper( this->scene->particles[index]) );
		}
	}

	void a3dSceneMapper::InitializeModelRefCollection() { 
		
		ModelRef->Clear();

		for( int index = 0; index < this->scene->modelRefCount; index++ ) { 
			ModelRef->Add( gcnew a3dModelRef_Mapper( this->scene->modelRef[index] ) );
		}
	}

	void a3dSceneMapper::InitializeTextureCollection() { 

		Textures->Clear();
		for( int index = 0; index < this->scene->textureDescCount; index++ ) { 
			Textures->Add( gcnew a3dTextureDesc_Mapper( this->scene->textures[index] ) );
		} 
	}

	void a3dSceneMapper::InitializeEffectCollection(){ 
				
		Effects->Clear();
		for( int index = 0; index < this->scene->effectDescCount; index++ ) { 
			Effects->Add( gcnew a3dEffectDesc_Mapper( this->scene->effects[index] ) );
		}
	}

	void a3dSceneMapper::InitializeGraphCollection() { 
		
		Graphs->Clear();
		for( int index = 0; index < this->scene->graphCount; index++ ) { 
			a3dGraphMapper ^graph = CreateGraphMapper();
			graph->NativeGraph = this->scene->graphs[index];
			Graphs->Add( graph );
		}
	}

	a3dGraphMapper^ a3dSceneMapper::CreateGraphMapper() {
		return gcnew a3dGraphMapper( false );
	}

	void a3dSceneMapper::RaiseSceneChanged(Object ^obj, EventArgs ^e) { 
		
		EventHandler ^handler = (EventHandler^)events[sceneChanged];
		if( handler != nullptr ) handler(obj, e);
	}

	void a3dSceneMapper::InitializeDefault() { 
		
		this->events = gcnew EventHandlerList();
		this->scene = new a3dScene();
		
		this->textures = gcnew a3dObjectDescBase_MapperCollection();
		this->effects = gcnew a3dObjectDescBase_MapperCollection();
		this->models = gcnew a3dObjectDescBase_MapperCollection();
		this->lands = gcnew a3dObjectDescBase_MapperCollection();
		this->particles = gcnew a3dObjectDescBase_MapperCollection();
		this->rails = gcnew a3dObjectDescBase_MapperCollection();
		this->fonts = gcnew a3dObjectDescBase_MapperCollection();
		this->sounds = gcnew a3dObjectDescBase_MapperCollection();
		this->lights = gcnew a3dLightBase_MapperCollection();
		this->modelRef = gcnew a3dModelRef_MapperCollection();
		this->graphs = gcnew a3dGraphMapperCollection();

		this->timeProvider = gcnew a3dTimeProviderBaseMapper(&this->scene->timeProvider);
	}

	void a3dSceneMapper::MakeNewScene(
								String^ dataDir, 
								String^ textureDir, 
								String^ effectDir, 
								String^ modelDir, 
								String^ landDir, 
								String^ partDir,
								String^ railDir,
								String^ fontDir,
								String^ soundDir) { 
		
		shStringHelper::Default.copyTo( this->scene->dataDirectory, dataDir );
		shStringHelper::Default.copyTo( this->scene->textureDirShort, textureDir );
		shStringHelper::Default.copyTo( this->scene->effectDirShort, effectDir );
		shStringHelper::Default.copyTo( this->scene->modelDirShort, modelDir );
		shStringHelper::Default.copyTo( this->scene->landDirShort, landDir );
		shStringHelper::Default.copyTo( this->scene->particleDirShort, partDir );
		shStringHelper::Default.copyTo( this->scene->railSystemDirShort, railDir );
		shStringHelper::Default.copyTo( this->scene->fontDirShort, fontDir );
		shStringHelper::Default.copyTo( this->scene->soundDirShort, soundDir );

		this->scene->makeLongPathNames();
		SceneChanged(this, EventArgs::Empty);
	}

	bool a3dSceneMapper::AddTexture( String ^fileName ) { 
		
		shString fn; 
		shStringHelper::Default.copyTo( fn, fileName );

		HRESULT rv = this->scene->loadTexture( fn );
		if(FAILED(rv)) 
			return false;
		this->InitializeTextureCollection();
		
		return true;
	}

	bool a3dSceneMapper::AddModel( String ^fileName ) { 
		
		shString fn; 
		shStringHelper::Default.copyTo( fn, fileName );
		
		HRESULT rv = this->scene->addModelToScene( fn );
		if( FAILED( rv ) ) 
			return false;
		
		this->InitializeTextureCollection();
		this->InitializeEffectCollection();
		this->InitializeModelCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return true;
	}

	bool a3dSceneMapper::AddModelRef( a3dModelDesc_Mapper ^desc ) { 
		
		bool res = this->scene->addModelRef( desc->ModelDesc );
		this->InitializeModelRefCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return res;
	}

	bool a3dSceneMapper::RemoveModelRef( a3dModelRef_Mapper ^modelRef ) { 
		
		bool res = this->scene->removeModelRef( modelRef->modelRef );
		this->InitializeModelRefCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return res;
	}

	bool a3dSceneMapper::AddLandscape( String ^name, String ^hmFile, String ^lmFile, String ^effectFileName, float pixelError, int pps, float hscale, float sidescale, int lightMapWidth, int lightMapHeight ) { 

		shString nm, hmf, lmf, ef;
				
		shStringHelper::Default.copyTo(nm, name);
		shStringHelper::Default.copyTo( hmf, hmFile );
		shStringHelper::Default.copyTo( lmf, lmFile );
		shStringHelper::Default.copyTo( ef, effectFileName );
		
		long rv = this->scene->addLandscape( nm, hmf, lmf, ef, pixelError, pps, hscale, sidescale, lightMapWidth, lightMapHeight );
		if( FAILED( rv ) )
			return false;
		this->InitializeLandCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return true;
	}

	bool a3dSceneMapper::AddRailSystem( String ^name, String ^railFileName, String ^lineModelFileName, String ^roundModelFileName, String ^lineEffectFileName, String ^roundEffectFileName, String ^round2EffectFileName ) { 

		shString nm, rfn, lmfn, rmfn, lefn, refn, r2efn;
				
		shStringHelper::Default.copyTo(nm, name);
		shStringHelper::Default.copyTo(rfn, railFileName);
		shStringHelper::Default.copyTo(lmfn, lineModelFileName);
		shStringHelper::Default.copyTo(rmfn, roundModelFileName);
		shStringHelper::Default.copyTo(lefn, lineEffectFileName);
		shStringHelper::Default.copyTo(refn, roundEffectFileName);
		shStringHelper::Default.copyTo(r2efn, round2EffectFileName);

		long rv = this->scene->addRailSystem( nm, rfn, lmfn, rmfn, lefn, refn, r2efn );
		if( FAILED( rv ) )
			return false;
		InitializeRailCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return true;
	}

	bool a3dSceneMapper::AddEffect( String^ fileName ) {

		shString fn;
		shStringHelper::Default.copyTo(fn, fileName);

		long rv = this->scene->loadEffect( fn );
		if( FAILED( rv ) )
			return false;
		
		InitializeEffectCollection();
		SceneChanged(this, EventArgs::Empty);
		
		return true;
	}

	void a3dSceneMapper::ChangeEffect( a3dShaderMaterialMapper ^material, a3dEffectDesc_Mapper ^effect ) { 

		if(material == nullptr || effect == nullptr ) return;
		this->scene->changeEffect(material->ShaderMaterial, effect->EffDesc);
		InitializeModelCollection();
		InitializeModelRefCollection();

		SceneChanged(this, EventArgs::Empty);
	}

	bool a3dSceneMapper::AddOmniLight( String^ lightName, System::Drawing::Color lightColor, float farAttenStart, float farAttenEnd, Vector3_Mapper ^position ) { 

		shString str;
		Vector3 pos, lc;
		shStringHelper::Default.copyTo(str, lightName);
		colorHelper::ColorToRGB(lc, lightColor);
		position->ToVector3(pos);
		
		bool res = this->scene->addOmniLight(str, lc, farAttenStart, farAttenEnd, pos);
		this->InitializeLightsCollection();

		SceneChanged(this, EventArgs::Empty);

		return res;
	}

	bool a3dSceneMapper::AddDirectionalLight( String^ lightName, System::Drawing::Color lightColor, Vector3_Mapper ^direction ) { 
		
		shString str;
		Vector3 dir, lc;
		shStringHelper::Default.copyTo(str, lightName);
		colorHelper::ColorToRGB(lc, lightColor);
		direction->ToVector3(dir);
		
		bool res = this->scene->addDirectionalLight(str, lc, dir);
		this->InitializeLightsCollection();

		SceneChanged(this, EventArgs::Empty);

		return res;
	}

	bool a3dSceneMapper::AddParticleSystem( String^ particleName, particleSystemBase_Mapper ^particle ) {

		shString name;

		shStringHelper::Default.copyTo(name, particleName);
		bool res = this->scene->addParticleSystem(name, particle->PSystem);
		this->InitializeParticlesCollection();
		
		return res;
	}

	bool a3dSceneMapper::SaveScene( String ^fileName ) { 

		shString fn; 
		shStringHelper::Default.copyTo( fn, fileName );
				
		return this->scene->saveToFile( fn );
	}

	bool a3dSceneMapper::LoadScene( String ^sceneFileName, String ^dataDirName ) { 
				
		shString sfn, ddn;
				
		shStringHelper::Default.copyTo( sfn, sceneFileName);
		shStringHelper::Default.copyTo( ddn, dataDirName);

		bool res = this->scene->loadFromFile( sfn );
		if( !res ) return false;
		res = FAILED(this->scene->loadData( ddn ));
		if( res ) return false;
		
		// синхронизируем коллекции
		this->InitializeCollection();
		
		SceneChanged(this, EventArgs::Empty);
		return true;
	}

	bool a3dSceneMapper::ImportModel( String ^textModelFileName, String ^modelFileName ) { 
		
		shString tmfn, mfn;
				
		shStringHelper::Default.copyTo( mfn, modelFileName);
		shStringHelper::Default.copyTo( tmfn, textModelFileName);

		return this->scene->importModel(tmfn, mfn);
	}

	DependencyTreeNodeCollection^ a3dSceneMapper::CreateDependencyTreeNodeCollection() { 
					
		DependencyTreeNodeCollection^ coll = gcnew DependencyTreeNodeCollection();
		
		Textures->SetDependencyParent(this);
		Textures->SetNodeName( gcnew String("Textures") );
		Effects->SetDependencyParent(this);
		Effects->SetNodeName( gcnew String("Effects") );
		Models->SetDependencyParent(this);
		Models->SetNodeName( gcnew String("Models") );
		Lands->SetDependencyParent(this);
		Lands->SetNodeName( gcnew String("Lands") );
		Particles->SetDependencyParent(this);
		Particles->SetNodeName( gcnew String("Particles") );
		Rails->SetDependencyParent(this);
		Rails->SetNodeName( gcnew String("Rails") );
		Fonts->SetDependencyParent(this);
		Fonts->SetNodeName( gcnew String("Fonts") );
		Sounds->SetDependencyParent(this);
		Sounds->SetNodeName( gcnew String("Sounds") );
		ModelRef->SetDependencyParent( this );
		ModelRef->SetNodeName( gcnew String("ModelRefs") );

		a3dSceneSystemValues ^sys = gcnew a3dSceneSystemValues(this);
		
		coll->Add(Textures);
		coll->Add(Effects);
		coll->Add(Models);
		coll->Add(Lands);
		coll->Add(Particles);
		coll->Add(Rails);
		coll->Add(ModelRef);
		coll->Add(Fonts);
		coll->Add(Sounds);
		coll->Add(sys);

		return coll;
	}

	DependencyTreeNodeCollection^ a3dSceneMapper::Children::get() { 
		
		if( this->depTreeNodes == nullptr ) { 
			this->depTreeNodes = CreateDependencyTreeNodeCollection();
		}
		return this->depTreeNodes;
	}

	DependencyTreeNodeCollection^ a3dSceneSystemValues::Children::get() { 
	
		if(this->children != nullptr) 
			return this->children;
		
		this->children = gcnew DependencyTreeNodeCollection();
		this->children->Add( Scene->TimeProvider );
		return this->children;
	}
}

