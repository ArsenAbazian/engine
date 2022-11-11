#ifndef A3DSCENEFILE_INC
#define A3DSCENEFILE_INC

#include "a3dSceneDefinitions.h"
#include "headerCodes.h"
#include "lightMapCreator.h"
#include "hstring.h"
#include "Ncamera.h"
#include "a3dTimeProvider.h"

class a3dTextSceneFile;
class Kernel;
class a3dTextureDesc;
class a3dEffectDesc;
class a3dModelDesc;
class a3dLandscapeDesc;
class a3dParticleSystemDesc;
class a3dRailSystemDesc;
class a3dFontDesc;
class a3dSoundDesc;
class a3dLightBase;
class a3dModelInfo;
class a3dRailSystemInfo;
class a3dModelRef;

class a3dOmniLightRender;
class a3dDirectionalLightRender;
class a3dSprayParticleRender;
class a3dShaderMaterial;

class particleSystemBase;
class a3dGraph;

class a3dScene : public a3dSerializableObject, public a3dLightMapCreator { 
	// current version is 1.1
	int		defaultVersion;

	bool	importModels( a3dTextSceneFile &file );
	bool	importLands( a3dTextSceneFile &file );
	bool	importParticles( a3dTextSceneFile &file );
	bool	importRails( a3dTextSceneFile &file );
	bool	importLights( a3dTextSceneFile &file );

public:
	bool								designMode;

	shString							fullPathName;
	shString							fileName;
	shString							dataDirectory;

	shString							textureDirectory;
	shString							modelDirectory;
	shString							effectDirectory;
	shString							landDirectory;
	shString							particleDirectory;
	shString							railSystemDirectory;
	shString							fontDirectory;
	shString							soundDirectory;

	shString							textureDirShort;
	shString							modelDirShort;
	shString							effectDirShort;
	shString							landDirShort;
	shString							particleDirShort;
	shString							railSystemDirShort;
	shString							fontDirShort;
	shString							soundDirShort;

	Kernel								*kernel;
	
	// вспомогательные объекты для рендеринга невизуальных объектов
	a3dOmniLightRender					*omniLightRender;
	a3dDirectionalLightRender			*dirLightRender;
	a3dSprayParticleRender				*sprayParticleRender;
	a3dTimeProviderBase					timeProvider;

	// не буду использовать инкакую хеш таблицу - заипали...
	// каждая инфо будет хранить у себя кроме имени и индекс
	// при добавлении будет инициализироваться индекс
	// тупо выделю много памяти и все тут.
	// самый эффективный метод - а память дешевая
	a3dModelDesc						*models[SCENE_MODEL_MAX_COUNT];
	a3dLandscapeDesc					*lands[SCENE_LAND_MAX_COUNT];
	a3dParticleSystemDesc				*particles[SCENE_PS_MAX_COUNT];
	a3dRailSystemDesc					*rails[SCENE_RS_MAX_COUNT];
	a3dEffectDesc						*effects[SCENE_EFFECT_MAX_COUNT];
	a3dTextureDesc						*textures[SCENE_TEXTURE_MAX_COUNT];
	a3dFontDesc							*fonts[SCENE_FONT_MAX_COUNT];
	a3dSoundDesc						*sounds[SCENE_SOUND_MAX_COUNT];
	a3dLightBase						*lights[ SCENE_LIGHT_MAX_COUNT ];
	
	int									modelDescCount;
	int									landDescCount;
	int									particleDescCount;
	int									railDescCount;
	int									effectDescCount;
	int									textureDescCount;
	int									fontDescCount;
	int									soundDescCount;
	int									lightsCount;

	a3dModelInfo						*modelInfo[SCENE_MODEL_REF_MAX_COUNT];
	int									modelInfoCount;
	a3dModelRef							*modelRef[SCENE_MODEL_REF_MAX_COUNT];
	int									modelRefCount;
	a3dGraph							*graphs[ SCENE_GRAPH_MAX_COUNT ];
	int									graphCount;
	
	a3dScene() { this->initializeDefault(); }
	~a3dScene() { this->clear(); }

	void	setKernel( Kernel *krn ) { this->kernel = krn; }
	void	initializeDefault( );
	void	clear();

	bool	import( a3dTextSceneFile &file );
		
	bool	saveToFile(shString &fileName);
	bool	loadFromFile(shString &fileName);

	int		getHeaderCode() { return a3dHeaders::a3dSceneHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);

	a3dModelDesc *getModelDesc( shString &fileName );
	bool	addModelDesc( a3dModelInfo *info );
	a3dLightBase* createLight( a3dMemoryFile &file );

	HRESULT			loadData( shString &dataDirectory );
	HRESULT			loadTextures();
	HRESULT			loadEffects();
	HRESULT			loadModels();
	HRESULT			loadLands();
	HRESULT			loadParticles();
	HRESULT			loadRails();

	void			updateTexturePath( shString &fileName );
	
	a3dTextureDesc*	getTexture( shString &fileName );
	a3dModelDesc*	getModel( shString &fileName );
	a3dEffectDesc*	getEffect( shString &fileName );

	HRESULT			loadEffect( shString &fileName );
	HRESULT			loadTexture( shString &fileName );
	HRESULT			initializeModelInfos();
	HRESULT			createModelRef();

	// lightmap creation
	int				getLightMapCount();
	a3dLightMap**	getLightMaps();

	void			makeLongPathNames();
	HRESULT			addModelToScene( shString &fileName );
	bool			addModelRef( a3dModelDesc *desc );
	bool			removeModelRef( a3dModelRef *mr );
	int				getModelRefIndex( a3dModelRef *mr );
	HRESULT			addLandscape( shString &name, shString &hmFile, shString &lmFile, shString &effectFileName, float pixelError, int patchPerSide, float heightScale, float sideScale, int lightMapWidth, int lightMapHeight );
	void			addRailSystem( a3dRailSystemInfo *info );
	HRESULT			addRailSystem( shString &name, shString &railFileName, shString &lineModelFileName, shString &roundModelFileName, shString &lineEffectFileName, shString &roundEffectFileName, shString &round2EffectFileName );
	bool			addOmniLight( shString &name, Vector3 color, float farAttenStart, float farAttenEnd, Vector3 position );
	bool			addDirectionalLight( shString &name, Vector3 color, Vector3 direction );
	bool			addParticleSystem( shString &name, particleSystemBase *psys );

	HRESULT			update( camera *cam );
	HRESULT			render( camera *cam );
	HRESULT			renderModelRef( camera *cam );
	HRESULT			renderLand( camera *cam );
	HRESULT			renderRail( camera *cam );
	HRESULT			renderLights( camera *cam );

	HRESULT			initializeDesignModeHelpers();

	void			clearModelsInfo();
	void			updateModelsInfo();
	
	void			changeEffect( a3dShaderMaterial *material, a3dEffectDesc *effect );

	// imports
	bool			importModel( shString &textModelFileName, shString &fileName );
};

#endif