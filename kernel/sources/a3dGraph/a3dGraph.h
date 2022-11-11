#ifndef A3D_GRAPH_INC
#define A3D_GRAPH_INC 

#include "a3dGraphType.h"
#include "a3dGraphNode.h"
#include "..\\headerCodes.h"
#include "vecmat.h"
#include "hstring.h"

class a3dGraph : public a3dSerializableObject {
	static const int maxNodeCount = 128;
public:	
	a3dGraphType	type;
	shString		name;
	int				nodeCount;
	a3dGraphNode	*nodeList[maxNodeCount];

	// visualization 
	Vector3			color;
	int				lineType;
	float			width;
	
	a3dGraph() { this->initializeDefault(); }
	~a3dGraph() { this->clear(); }

	void			initializeDefault();
	void			clear();
	
	void			addNode();
	void			insertNode(int index);
	void			removeNode(int index);

	// serialization
	int				getHeaderCode() { return a3dHeaders::a3dGraphHeader; }
	int				calcSizeCore();
	bool			saveToMemoryCore( a3dMemoryFile &file ); 
	bool			loadFromMemoryCore( a3dMemoryFile &file );

	float			calcFloatValue(float t);
	bool			calcBoolValue(float t);
	int				calcIntValue(float t);

	float			getLength() { return this->nodeCount > 0? this->nodeList[this->nodeCount - 1]->p2[0]: 0.0f; }
};

#endif