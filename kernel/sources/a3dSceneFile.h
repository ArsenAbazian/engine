#ifndef A3DSCENEFILE_INC
#define A3DSCENEFILE_INC

#include "kernel.h"
#include "hstring.h"
#include "vecmat.h"
#include "scaner\\scaner.h"
#include "shaderParam.h"
#include "memoryFile.h"

typedef struct { 
	int aIndex;
	int bIndex;
	int cIndex;
} a3dFace;

typedef enum { MT_NONE, MT_MULTI, MT_STANDARD, MT_SHADER } a3dMaterialType;
typedef enum { LGHT_NONE, LGHT_OMNI, LGHT_DIRECTIONAL } a3dLightType;

class a3dLightBase { 
public:
	a3dLightType	type;
	shString		name;
	Vector3			color;

	a3dLightBase() { this->initializeDefault(); }
	~a3dLightBase() { this->clear(); }

	virtual void initializeDefault();
	virtual void clear();
	virtual bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	virtual int calcSize();
	virtual bool saveToMemory( a3dMemoryFile &file );
	virtual bool loadFromMemory( a3dMemoryFile &file );
	virtual bool supportSave() { return true; }
};

class a3dOmniLight : public a3dLightBase { 
public:
	Vector3		pos;
	float		farAttenStart;
	float		farAttenEnd;

	a3dOmniLight() : a3dLightBase() { }
	~a3dOmniLight() { this->clear(); }

	void		initializeDefault();
	void		clear();
	bool		import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	int			calcSize();
	bool		saveToMemory( a3dMemoryFile &file );
	bool		loadFromMemory( a3dMemoryFile &file );
};

class a3dDirectionalLight : public a3dLightBase { 
public:
	Vector3		dir;
	
	a3dDirectionalLight() : a3dLightBase() { }
	~a3dDirectionalLight() { this->clear(); }

	void		initializeDefault();
	void		clear();
	bool		import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	int			calcSize();
	bool		saveToMemory( a3dMemoryFile &file );
	bool		loadFromMemory( a3dMemoryFile &file );
};

class a3dMaterialBase { 
public:
	a3dMaterialType	type;
	shString				name;
	
	a3dMaterialBase() { 
		this->type = MT_NONE;
	}
	~a3dMaterialBase() { this->clear(); }

	virtual bool supportSave() { return false; }
	virtual bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	virtual void clear();
	virtual a3dMaterialBase* clone();

	virtual int calcSize();
	virtual bool saveToMemory( a3dMemoryFile &file );
	virtual bool loadFromMemory( a3dMemoryFile &file );
	virtual bool initializeTextures( std::list<TextureDesc*> &textures );
	virtual bool initializeEffects( std::list<EffectDesc*> &effects );
};

class a3dMultiMaterial : a3dMaterialBase { 
	
	bool importSubMaterialEnablesList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importSubMaterialIdList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importSubMaterialNameList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
public:
	int						subMaterialCount;
	shString				*subMaterialNameList;
	bool					*subMaterialEnablesList;
	int						*subMaterialIdList;
	
	a3dMultiMaterial() { 
		this->type = MT_MULTI;
		this->subMaterialCount = 0;
		this->subMaterialNameList = null;
		this->subMaterialEnablesList = null;
		this->subMaterialIdList = null;
	}

	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void clear();
	a3dMaterialBase* clone();
};

class a3dMapBase { 
public:

	virtual a3dMapBase* clone();
};

class a3dBitmapMap : a3dMapBase { 
public:
	shString mapType;
	shString name;
	shString fileName;

	float	clipu;
	float	clipv;
	float	clipw;
	float	cliph;
	bool	umirror;
	bool	vmirror;
	bool	utile;
	bool	vtile;
	int		uvwType;
	int		mapChannel;
	float	uOffset;
	float	vOffset;
	float	uTiling;
	float	vTiling;

	a3dBitmapMap( shString mapType ) { 
		
		this->mapType = mapType;
		this->clipu = 0.0f;
		this->clipv = 0.0f;
		this->clipw = 0.0f;
		this->cliph = 0.0f;
		this->umirror = false;
		this->vmirror = false;
		this->utile = true;
		this->vtile = true;
		this->uvwType = 0;
		this->mapChannel = 0;
		this->uOffset = 0.0f;
		this->vOffset = 0.0f;
		this->uTiling = 0.0f;
		this->vTiling = 0.0f;
	}
	~a3dBitmapMap() { }

	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	a3dMapBase* clone();
};

class a3dStandardMaterial : a3dMaterialBase { 

	bool importMapEnablesList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importMapAmountsList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importMap( shString &mapName, a3dBitmapMap **map, std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importMaps( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
public:
	int shaderType;
	shString shaderName;
	bool wire;
	bool twoSided;
	bool faceted;
	float opacity;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 selfIllum;
	float specularLevel;
	float glossiness;
	float soften;
	bool mapEnablesList[12];
	float mapAmountsList[12];
	a3dBitmapMap *maps[12];

	a3dStandardMaterial() { 
		this->type = MT_STANDARD;
		this->shaderType = 0;
		this->wire = false;
		this->twoSided = false;
		this->faceted = false;
		this->opacity = 0.0f;
		clearVector3( this->ambient );
		clearVector3( this->diffuse );
		clearVector3( this->specular );
		clearVector3( this->selfIllum );
		this->specularLevel = 0.0f;
		this->glossiness = 0.0f;
		this->soften = 0.0f;
	
		int i;
		for( i = 0; i < 12; i ++ ) { 
			this->mapEnablesList[ i ] = false;
			this->mapAmountsList[ i ] = 0.0f;
			this->maps[ i ] = null;
		}
	}

	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void clear();
	a3dMaterialBase* clone();
};

class a3dShaderMaterial : a3dMaterialBase { 

	bool importParams( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importShaderParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importBooleanParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importIntegerParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importBitMapParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importStandardMaterialParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );

public:
	shString fileName;
	a3dShaderParamBase** paramList;
	int	paramCount;
	bool  renderEnabled;
	LPD3DXEFFECT	lpEffect;	

	a3dShaderMaterial() { 
		this->type = MT_SHADER;
		this->paramList = null;
		this->paramCount = 0;
		this->renderEnabled = true;
		this->lpEffect = null;
	}

	bool supportSave() { return true; }
	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void clear();
	a3dMaterialBase* clone();

	int calcSize();
	bool saveToMemory( a3dMemoryFile &file );
	bool loadFromMemory( a3dMemoryFile &file );
	a3dShaderParamBase* createParam( a3dShaderParamType type );
	bool initializeTextures( std::list<TextureDesc*> &textures );
	bool initializeEffects( std::list<EffectDesc*> &effects );
};

class a3dMesh { 
	
	bool		importVertices( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool		importFaces( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	
	bool		importMapsChannelCount( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool		importFaceMatIdList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	bool		importMapChannelCoors( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName, int vCount, Vector3 *mapCoors );
	bool		importMapChannelsCoors( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	bool		importMapChannelFaceList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName, int fCount, a3dFace *mapFaces );
	bool		importMapChannelsFaceList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
public:
	shString	name;
	shString	vertexFVF;
	
	int			vertexCount;
	Vector3		*vertexList;

	int			faceCount;
	a3dFace		*faceList;
	int			*faceMatIdList;

	int			mapChannelCount;
	
	Matrix4		transform;
	Vector3		pivot;
	Vector3		objectOffsetPos;
	Vector4		objectOffsetRot;
	Vector3		objectOffsetScale;
	Matrix4		objectTransform;

	shString	materialName;

	int			mapChannelVertCount[8];
	Vector3		*mapChannelCoors[8];
	a3dFace		*mapChannelFaceList[8];
	
	a3dMesh();
	~a3dMesh() { this->clear(); }

	int		getMapFaceCount(int index) { return this->mapChannelFaceList[ index ] == null? 0: this->faceCount; }
	int		getSubMaterialFaceCount(int id);
	void	fillSubMaterialFaceList(a3dFace *face, int id);
	bool	import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void	initializeData( );
	void	clear();

	void getVertexTextureCoor( int vIndex, int tIndex, Vector3 v );
};

class a3dSceneFile {

	void initializeRwords();

	int		getTokenCount( std::list< scriptLexeme* > &lexList, shString &tokenName );
	int		getMeshCount( std::list< scriptLexeme* > &lexList );
	bool	initializeMeshes( std::list< scriptLexeme* > &lexList );
	void	findNextToken( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName );
	void	findNextMesh( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool	initializeMesh( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dMesh &mesh );

	bool	initializeMaterials( std::list< scriptLexeme* > &lexList );
	bool	initializeMaterial( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dMaterialBase **material );
	int		getMaterialCount( std::list< scriptLexeme* > &lexList );
	void	findNextMaterial( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	bool	initializeLights( std::list< scriptLexeme* > &lexList );
	bool	initializeLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dLightBase **light );
	int		getLightCount( std::list< scriptLexeme* > &lexList );
	void	findNextLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
public:
	a3dMesh			*meshList;
	int				meshCount;

	a3dMaterialBase **materialList;
	int				materialCount;
	
	a3dLightBase	**lightList;
	int				lightCount;

	a3dSceneFile() { 
		this->initializeRwords(); 
		this->meshList = null;
		this->meshCount = 0;
		this->materialList = null;
		this->materialCount = 0;
		this->lightList = null;
		this->lightCount = 0;
	}
	~a3dSceneFile() { this->clear(); }
	
	bool importFromFile( shString &fileName );
	void clear();

	a3dMaterialBase* getMaterial( shString &name );
};

bool	safeNextLex( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
bool	isToken( std::list< scriptLexeme* >::iterator &li, shString &tokenName );
bool	isTokenEnd( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString tokenName );
bool	readIntValue( std::list< scriptLexeme* >::iterator &li, int *intVal );
bool	readBoolValue( std::list< scriptLexeme* >::iterator &li, bool *boolVal );
bool	readFloatValue( std::list< scriptLexeme* >::iterator &li, float *floatVal );
bool	readParser( std::list< scriptLexeme* >::iterator &li, lexParserType pars );
bool	readParamAssignment( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName );
bool	readIntParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, int *intVal );
bool	readFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, float *floatVal );
bool	readBoolParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, bool *boolVal );
bool	readMatrixParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Matrix4 mat );
bool	readVertexParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector3 v );
bool	readQuaternionParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector4 v );
bool	readColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector3 v );
bool	readStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, shString &stringVal );
bool	importVertex(  std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, Vector3 v );
bool	importFace(  std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dFace &face );
bool	importStringList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, shString *strings );
bool	importBooleanList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, bool *values );
bool	importFloatList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, float *values );
bool	importIntList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, int *values  );
bool	isString( std::list< scriptLexeme* >::iterator &li );
bool	isBooleanClass( std::list< scriptLexeme* >::iterator &li );
bool    isFloatClass( std::list< scriptLexeme* >::iterator &li );
bool	isIntegerClass( std::list< scriptLexeme* >::iterator &li );
bool	isStringClass( std::list< scriptLexeme* >::iterator &li );
bool    isColorClass( std::list< scriptLexeme* >::iterator &li );
bool	isBitMapClass( std::list< scriptLexeme* >::iterator &li );
bool	isStandardMaterialClass( std::list< scriptLexeme* >::iterator &li );
bool	isTokenClass( std::list< scriptLexeme* >::iterator &li );

#endif