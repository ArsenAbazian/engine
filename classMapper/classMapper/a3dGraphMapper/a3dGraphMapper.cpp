#include "stdafx.h"
#include "a3dGraphMapper.h"

namespace classMapper { 

	void a3dGraphMapper::UpdateNativeGraph() { 
		if(NativeGraph->nodeCount < Nodes->Count) { 
			while(NativeGraph->nodeCount < Nodes->Count)
				NativeGraph->addNode();
		}
		else if(NativeGraph->nodeCount > Nodes->Count) { 
			while(NativeGraph->nodeCount > Nodes->Count)
				NativeGraph->removeNode(NativeGraph->nodeCount - 1);
		}
		for(int i = 0; i < Nodes->Count; i++) { 
			Nodes[i]->UpdateNativeNode(NativeGraph->nodeList[i]);
		}
		NativeGraph->nodeCount = Nodes->Count;
	}
}