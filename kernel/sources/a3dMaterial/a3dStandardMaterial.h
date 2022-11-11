#ifndef A3D_STANDARD_MATERIAL_INC
#define A3D_STANDARD_MATERIAL_INC

#include "a3dMaterialBase.h"

class a3dMapBase { 
public:

	virtual a3dMapBase* clone();
};

class a3dBitmapMap : public a3dMapBase { 
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

class a3dStandardMaterial : public a3dMaterialBase { 

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


#endif