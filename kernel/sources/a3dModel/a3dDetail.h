#ifndef A3D_DETAIL_INC
#define A3D_DETAIL_INC

#include "3dtypes.h"
#include "..\\kernel.h"
#include "..\\headerCodes.h"
#include "..\\lightMapInfo.h"
#include "..\\a3dShaderParam\\a3dShaderParamType.h"
#include "..\\delegate.h"

class a3dTextModelFile;
class a3dMesh;
class a3dObject;
class a3dShaderMaterial;
class a3dDetailRef;
class a3dLightMap;
class a3dLightMapPlaneCollection;
class a3dObjectRef;
class camera;

class a3dDetail : public a3dSerializableObject {
	bool	importFaces( a3dTextModelFile &scFile, a3dMesh &mesh, int index );
	bool	importMaterial( a3dTextModelFile &scFile, a3dMesh &mesh, int index );
public:
	a3dObject	*object;
	a3dLightMapInfo	lightMapInfo;
	Kernel		*kernel;
	shString	materialName;

	a3dFace		*faces;
	int			faceCount;

	a3dShaderMaterial	*material;

	LPDIRECT3DINDEXBUFFER9		indexBuffer;

	a3dDetail( ) { this->initializeDefault(); }
	~a3dDetail() { this->clear(); }

	bool			import( a3dTextModelFile &scFile, a3dMesh &mesh, int index );
	void			setObject( a3dObject *obj );
	void			setKernel( Kernel *krn ) { this->kernel = krn; }
	void			initializeDefault();
	void			clear();
	bool			supportSave();

	int				getHeaderCode() { return a3dHeaders::a3dDetailHeader; }
	int				calcSizeCore();
	bool			saveToMemoryCore( a3dMemoryFile &file );
	bool			loadFromMemoryCore( a3dMemoryFile &file );

	HRESULT 		createRenderBuffer();
	HRESULT 		fillRenderBuffer();
	bool			readDataFromBuffer();
	HRESULT 		clearRenderBuffer();

	HRESULT			render( camera *cam, Matrix4 WorldXf, Matrix4 WvpXf, Matrix4 WorldITXf );
	HRESULT			render( camera *cam, Matrix4 WorldXf, Matrix4 WvpXf, Matrix4 WorldITXf, a3dDetailRef *detailRef );
	HRESULT			save();
	HRESULT			restore();
	HRESULT			initialize() { return this->restore(); }

	// for lightmap generation
	a3dDetailRef*	createReference( a3dObjectRef *objectRef );
	a3dLightMap*	getLightMap( processLightMapPlaneDeletate fp, void *lpData );
	a3dLightMap*	getLightMap( processLightMapPlaneDeletate fp, void *lpData, a3dLightMapInfo *lmInfo );
	a3dLightMap*	getLightMap( processLightMapPlaneDeletate fp, void *lpData, a3dLightMapInfo *lmInfo, Matrix4 basis );
	a3dLightMapPlaneCollection* getLightMapPlaneCollection( int textureIndex, processLightMapPlaneDeletate fp, void *lpData );
	a3dLightMapPlaneCollection* getLightMapPlaneCollection( int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis );
	bool			hasLightMap();
	void			fillLightMapPlaneCollection( a3dLightMapPlaneCollection *coll, int textureIndex, processLightMapPlaneDeletate fp, void *lpData );
	void			fillLightMapPlaneCollection( a3dLightMapPlaneCollection *coll, int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis );
	void			initializeLightMapPlane( a3dLightMapPlaneInfo *info, int faceIndex, int textureIndex, processLightMapPlaneDeletate fp, void *lpData );
	void			initializeLightMapPlane( a3dLightMapPlaneInfo *info, int faceIndex, int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis );
};

#endif