#ifndef A3D_RADIOSITY
#define A3D_RADIOSITY

#include "kernel.h"
#include "vecmat.h"
#include "a3dTextModelFile.h"
#include "otree.h"

class a3dScene;
class a3dLightMapPlaneInfo { 
public:
	bool    processed;
	
	Vector3	a;
	Vector3 na;
	Vector3 b;
	Vector3	nb;
	Vector3 c;
	Vector3 nc;
	Vector3 normal;
	Vector2 ta;
	Vector2 tb;
	Vector2 tc;
	
	// specific for raytrace
	//Vector3 uKoeff;
	//Vector3 vKoeff;

	int		dominantAxisIndex;

	a3dLightMapPlaneInfo() { this->initializeDefault(); }
	~a3dLightMapPlaneInfo() { this->clear(); }

	void	initializeDefault();
	//void	initializeBarycentricKoeffs();
	void	initializeNormal();
	void	clear();
	void	calcDominantAxisIndex();

	void	translate( Matrix4 basis, bool recalcNormal );
	void	bindToTree( a3dOcTreeManager *manager );	
};

class a3dLightMapPixelInfo { 
public:
	int		intMapIndex;		// индекс в таблице пересекаемостей
	
	Vector3 point;
	Vector3	normal;	// interpolated normal in point
	Vector3 resultColor;
	Vector3 emmitColor;

	a3dLightMapPlaneInfo	*ownerPlane;

	a3dLightMapPixelInfo() { this->initializeDefault(); }
	~a3dLightMapPixelInfo() { this->clear(); }

	void	initializeDefault();
	void	initialize( a3dLightMapPlaneInfo *ownerPlane, float u, float v );
	void	clear();
	void	calcLightColor( Vector3 lightColor, float intensity );
	void	calcLightColor( Vector3 lightColor, float intensity, float reflectionKoeff );
	
	bool	isEnabled() { return this->emmitColor[0] + this->emmitColor[1] + this->emmitColor[2] > 0.01f; }
};

class a3dLightMapPlaneCollection { 
public:
	int						planeCount;
	a3dLightMapPlaneInfo	*plane;

	a3dLightMapPlaneCollection() { this->initializeDefault(); }
	a3dLightMapPlaneCollection(int planeCount) { this->initialize( planeCount ); }
	~a3dLightMapPlaneCollection() { this->clear(); }

	void	initializeDefault();
	void	initialize( int planeCount );
	void	clear();
	void	preCalc();
	void	bindToTree( a3dOcTreeManager *manager );
};

class a3dLightMap { 

	void		initializePixels();
	void		mapPlaneToLightMap(a3dLightMapPlaneInfo &plane);
	void		calcOctreeVolume( Vector3 a, Vector3 b );
public:
	shString	fileName;
	int			width;
	int			height;
	
	a3dLightMapPlaneCollection *planeColl;
	a3dLightMapPixelInfo	*pixels;
	a3dLightMapPixelInfo	**map;

	a3dLightMap( int width, int height, a3dLightMapPlaneCollection *planeColl, shString &fileName ) { 
		this->initializeDefault();
		this->initialize( width, height, planeColl, fileName ); 
	}
	a3dLightMap() { this->initializeDefault(); }
	~a3dLightMap() { this->clear(); }

	void		initializeDefault();
	void		initialize( int width, int height, a3dLightMapPlaneCollection *planeColl, shString &fileName );
	void		clear();
	void		translate( Matrix4 basis, bool recalcNormal );
	
	void		prepareRadiosity();

	bool		logUsedPixelsToImage( shString &fileName );
	bool		logPixelPosToImage( shString &fileName );
	bool		saveLightMap( shString &fileName );
	bool		logUsedPixelsToImage( TCHAR *fileName ) { return this->logUsedPixelsToImage( shString( fileName ) ); }
	bool		saveLightMap( TCHAR *fileName ) { return this->saveLightMap( shString( fileName ) ); }
	void		preCalcLightMap();
	void		bindPlanesToTree( a3dOcTreeManager *manager );
};

typedef void (*radiosityDelegate)( TCHAR *statusText );

class a3dRadiosityCalculator { 

	float	*acosTable;
	TCHAR	statusText[1024];
	int		statusTextSize;
	
	BYTE	**intTable;
	int		intTableSize;

	void	calcOmniLightRay( a3dLightMapPixelInfo *pixel, a3dOmniLight *light, Vector3 ray );
	void	calcDirectionalLightRay( a3dLightMapPixelInfo *pixel, a3dDirectionalLight *light, Vector3 ray );
	void	calcLightPixel( a3dLightMapPixelInfo *pixel, a3dOmniLight *light );
	void	calcLightPixel( a3dLightMapPixelInfo *pixel, a3dDirectionalLight *light );
	void	calcLightPixel( a3dLightMapPixelInfo *pixel );
	void	calcLightMap( a3dLightMap *lightMap, radiosityDelegate fp );
	void	calcMapRadiosity( a3dLightMap *lightMap, int lIndex, radiosityDelegate fp );
	void	calcPixelRadiosity( a3dLightMapPixelInfo *pixel );
	void	formFactor( a3dLightMapPixelInfo *pixel1, a3dLightMapPixelInfo *pixel2 );

	void	calcOcTreeVolume( Vector3 a, Vector3 b );
	bool	isIntersects( a3dLightMapPixelInfo *pixel, Vector3 point, Vector3 dir, a3dLightMapPlaneInfo **plane );
	bool	isIntersects( a3dLightMapPixelInfo *pixel, a3dLightMapPixelInfo *pixel2 );

	void	clearProcessedFlag();
	void	prepareRadiosity();
	void	initializeAcosTable();
	void	initializeIntTable();
	int		calcIntTableSize();
	void	calcPixelIntersection( a3dLightMapPixelInfo *pixel );
public:
	a3dScene					*scene;
	std::list<a3dLightMap*>		lightMapList;
	std::list<a3dLightBase*>	lightList;
	Vector3						ambientColor;
	float						radiosityKoeff;
	float						scaleCos;
	float						reflectionKoeff;
	int							radiosityPassCount;

	a3dOcTreeManager			ocTreeManager;

	a3dRadiosityCalculator() { this->initializeDefault(); }
	~a3dRadiosityCalculator() { this->clear(); }

	void initializeDefault();
	void clear();
	void initialize( a3dLightMap** lmList, int lightMapCount );
	void initialize( a3dScene *scene, int levelCount );
	void initializeLights( a3dScene *scene );
	void bindLightMapsToTree();

	void initializeOcTreeManager( int levelCount );
	void calculate( bool calcRadiosityAfterLightMap, radiosityDelegate fp );
	void calculate( a3dScene *scene, int levelCount, bool calcRadiosityAfterLightMap, radiosityDelegate fp );
	void calcLightMaps( radiosityDelegate fp );
	void calcRadiosity( radiosityDelegate fp );
	void saveLightMaps();
	void preCalcLightMaps();
};

#endif