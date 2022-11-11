#ifndef A3D_OBJECT_INC
#define A3D_OBJECT_INC

#include "a3dMathObject.h"
#include "..\\Ncamera.h"
#include "..\\delegate.h"

class a3dModel;
class a3dObjectRef;
class a3dDetailRef;
class a3dDetail;
class a3dModelRef;
class a3dTextModelFile;
class a3dMesh;
class a3dLightMap;

class a3dObject : public a3dMathObject { 

	int					getDetailCount( a3dTextModelFile &scFile, a3dMesh &mesh );
	bool				importDetails( a3dTextModelFile &scFile, a3dMesh &mesh );
	bool				initializeVertexInfo( a3dTextModelFile &scFile, a3dMesh &mesh );	
	void				swapVerticesYZ();
	void				swapBasisYZ();
	void				correctBasisMirrors();
	void				localizeVertices();
	bool				processVertices( a3dMesh &mesh );
	bool				initializeVertices( a3dMesh &mesh );
	void				initializeBasis( a3dMesh &mesh );
	bool				calcNormals( a3dMesh &mesh ); 

	bool				initializeVertexData( a3dMesh &mesh );
	bool				fillVertexData( a3dMesh &mesh );
	void				clearVertexData(); 
public:
	a3dModel					*model;
	Kernel						*kernel;

	shString					name;

	a3dVertexInfo				vertexInfo;
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;
	BYTE						*vertexData;

	Vector3						*vertices;
	Vector3						*normals;
	int							vertexCount;

	a3dDetail					*detail;
	int							detailCount;

	a3dObject() { this->initializeDefault(); }
	~a3dObject() { this->clear(); }

	void			setModel( a3dModel *mdl ) { this->model = mdl; }
	void			setKernel( Kernel *krn );
	void			initializeDefault();
	void			clear();

	bool			import( a3dTextModelFile &scFile, a3dMesh &mesh );
	void			updateBasis();

	HRESULT 		createRenderBuffer();
	HRESULT 		fillRenderBuffer();
	void			clearRenderBuffer();
	bool			readDataFromBuffer();

	bool			supportSave();

	int				calcSizeCore();
	int				getHeaderCode() { return a3dHeaders::a3dObjectHeader; }
	bool			saveToMemoryCore( a3dMemoryFile &file );
	bool			loadFromMemoryCore( a3dMemoryFile &file );
	int				calcSaveDetailCount();
	bool			calcNormals();
	bool			extractVertices();

	a3dObjectRef*	createReference( a3dModelRef *modelRef );

	HRESULT			save();
	HRESULT     	restore();
	HRESULT			render( camera *cam, a3dObjectRef *objRef);
	HRESULT			render( camera *cam );

	// virtual
	void			correct( Matrix4 basis, float invScX, float invScY, float invScZ );
	void			calcBVolume();

	int				calcFaceCount();
	void			getLightMaps( a3dLightMap** llList );
	void			getLightMaps( a3dLightMap** llList, processLightMapPlaneDeletate fp, void *lpData );
	BYTE*			getVertexData( int vertexIndex ) { return this->vertexData + this->vertexInfo.getVertexSize() * vertexIndex; }
	int				getLightMapCount();
};

#endif