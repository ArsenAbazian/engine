#include "mathUtils.h"

void calcPolygonNormals( T1Vertex *vertices, Face *fcList, int faceCount, int vertCount ) { 

	Vector3 *norm = new Vector3[faceCount];
	Vector3 v1, v2;
	int i;
	
	for( i = 0; i < faceCount; i++ ) {	
		
		subVector3( vertices[ fcList[ i ].Vb_indx ].SCoor, vertices[ fcList[ i ].Va_indx ].SCoor,v1 );
		subVector3( vertices[ fcList[ i ].Vc_indx ].SCoor, vertices[ fcList[ i ].Va_indx ].SCoor,v2 );
	
		vectorMulOfVector( v2, v1, norm[ i ] );
		normalizeVector( norm[ i ] );

	}

	for( i = 0; i < faceCount; i++ ) {
		addVector3( vertices[ fcList[ i ].Va_indx ].Normal, norm[ i ], vertices[ fcList[ i ].Va_indx ].Normal );
		addVector3( vertices[ fcList[ i ].Vb_indx ].Normal, norm[ i ], vertices[ fcList[ i ].Vb_indx ].Normal );
		addVector3( vertices[ fcList[ i ].Vc_indx ].Normal, norm[ i ], vertices[ fcList[ i ].Vc_indx ].Normal );
	}
	
	for( i = 0; i < vertCount; i++ )
		normalizeVector( vertices[ i ].Normal );

	delete[] norm;
}