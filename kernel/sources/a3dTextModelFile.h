#ifndef A3D_TEXTMODELFILE_INC
#define A3D_TEXTMODELFILE_INC

#include "3dtypes.h"
#include "a3dLight\\a3dLight.h"
#include "a3dMaterial\\a3dMaterialBase.h"

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

class a3dTextModelFile {

	int		getTokenCount( std::list< scriptLexeme* > &lexList, shString &tokenName );
	int		getMeshCount( std::list< scriptLexeme* > &lexList );
	bool	initializeMeshes( std::list< scriptLexeme* > &lexList );
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

	a3dTextModelFile() { 
		this->meshList = null;
		this->meshCount = 0;
		this->materialList = null;
		this->materialCount = 0;
		this->lightList = null;
		this->lightCount = 0;
	}
	~a3dTextModelFile() { this->clear(); }
	
	bool loadFromFile( shString &fileName );
	void clear();

	a3dMaterialBase* getMaterial( shString &name );
};

#endif