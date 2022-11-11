#ifndef A3D_GRAPH_DEPENDENCY
#define A3D_GRAPH_DEPENDENCY

#include "a3dSimpleDependency.h"
#include "..\\a3dGraph\\a3dGraph.h"

// менять значение нельзя!!!!!!!!!!
typedef enum { 
	GO_NONE = 0,
	GO_SELECTOP1 = 1,
	GO_SELECTOP2 = 2,
	GO_ADD = 3,
	GO_SUB = 4,
	GO_MUL = 5,
	GO_DIV = 6
} a3dGraphOperation;

class a3dGraphDependency : public a3dSimpleDependency { 

public:
	a3dGraph	*graph;
	a3dGraph	*graph2;
	a3dDependencyProperty *src2;
	bool		loopGraph1;
	bool		loopGraph2;
	a3dGraphOperation operation;

	a3dGraphDependency();
	~a3dGraphDependency();

	void setProperty(a3dDependencyProperty *prop);
	void removeProperty();
	void setSource2Property(a3dDependencyProperty *prop);
	void removeSource2Property();
	void updateProperty();

	float noneOp(float res1, float res2) { return 0.0f; }
	float selectOp1(float res1, float res2) { return res1; }
	float selectOp2(float res1, float res2) { return res2; }
	float addOp(float res1, float res2) { return res1 + res2; }
	float subOp(float res1, float res2) { return res1 - res2; }
	float mulOp(float res1, float res2) { return res1 * res2; }
	float divOp(float res1, float res2) { return res1 / res2; }
};

typedef float (a3dGraphDependency::*graphCalcFunc)(float re1, float re2);

static graphCalcFunc graphOpFuncArray[7] = { 

	&a3dGraphDependency::noneOp,
	&a3dGraphDependency::selectOp1,
	&a3dGraphDependency::selectOp2,
	&a3dGraphDependency::addOp,
	&a3dGraphDependency::subOp,
	&a3dGraphDependency::mulOp,
	&a3dGraphDependency::divOp
};

#endif