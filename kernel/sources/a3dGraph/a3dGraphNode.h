#ifndef A3D_GRAPH_NODE_INC
#define A3D_GRAPH_NODE_INC

#include "3dtypes.h"
#include "..\\headerCodes.h"

typedef enum { GNT_NONE, GNT_BOOL, GNT_CONST, GNT_LINE, GNT_BEZIE } a3dGraphNodeType;

class a3dGraphNode : public a3dSerializableObject { 
	float			localTimeLength;
public:
	a3dGraphNodeType type;
	
	// for line
	float		k;
	
	// for bezie
	Vector2		p1;
	Vector2		p2;
	Vector2		p3;
	Vector2		p4;
	
	float		a;
	float		b;
	float		c;

	// for time
	float		t;
	float		t2;
	float		t3;

	float		result;
	bool		bResult;

	bool		splitNext;

	a3dGraphNode() { this->initializeDefault(); }
	~a3dGraphNode() { this->clear(); }
	
	void						initializeDefault();
	void						clear();
	
	void						setType( a3dGraphNodeType nodeType );
	
	void						setPoints(Vector2 pp1, Vector2 pp2);
	void						setPoints(Vector2 pp1, Vector2 pp2, Vector2 pp3, Vector2 pp4);
	
	void						calcParams();
	void						calcBezieParams();
	void						calcLineParams();

	void						calcTime() { t2 = t * t; t3 = t2 * t; }

								// used global time
	void						calcLine() { this->result = this->p1[1] + this->k * (this->t - this->p1[0]); }
								// do not forget to convert local time
	void						calcBezie() { this->result = this->t3 * this->a + this->t2 * this->b + this->t * this->c + this->p1[1]; }
	void						calcNode();

	void						setTime( float time ) { 
									this->t = this->type == GNT_BEZIE? (time - this->p1[0]) / this->localTimeLength: time; 
									if(this->type == GNT_BEZIE)
										this->calcTime();
								}
	void						updateLocalTime() { this->localTimeLength = p2[0] - p1[0]; }

	// serialization
	int							getHeaderCode() { return a3dHeaders::a3dGraphNodeHeader; }
	int							calcSizeCore();
	bool						saveToMemoryCore( a3dMemoryFile &file ); 
	bool						loadFromMemoryCore( a3dMemoryFile &file );
};

#endif