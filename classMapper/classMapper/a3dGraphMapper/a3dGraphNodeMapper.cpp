#include "stdafx.h"
#include "a3dGraphNodeMapper.h"

namespace classMapper { 
	
	void a3dGraphNodeMapper::UpdateNativeNode(a3dGraphNode *node) { 
		
		node->type = this->ManagedTypeToNative(NodeType);
		Vector2 v1, v2, v3, v4;

		v1[0] = P1.X;
		v1[1] = P1.Y;
		v2[0] = P2.X;
		v2[1] = P2.Y;
		v3[0] = P3.X;
		v3[1] = P3.Y;
		v4[0] = P4.X;
		v4[1] = P4.Y;

		node->setPoints(v1, v2, v3, v4);

		node->k = K;
		node->a = A;
		node->b = B;
		node->c = C;

		node->t = T;
		node->t2 = T2;
		node->t3 = T3;

		node->result = Result;
		node->bResult = BResult;
		node->splitNext = SplitNext;
	}
}