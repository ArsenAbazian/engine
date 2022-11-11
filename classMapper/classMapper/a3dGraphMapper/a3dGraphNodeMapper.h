// cameraMapper.h

#pragma once

#include "..\\vecMatMapper.h"
#include "..\\stringHelper.h"
#include "..\\interfaces.h"
#include "a3dGraph\\a3dGraphNode.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper { 

	public enum class a3dGraphNodeType_Mapper { 
		None,
		Bool,
		Const, 
		Line,
		BezieSpline
	};

	ref class a3dGraphMapper;
	public ref class a3dGraphNodeMapper : a3dObjectWithEvent { 
				
		a3dGraphNode *nativeNode;
		a3dGraphMapper ^ownerGraph;

		a3dGraphNodeType_Mapper NativeTypeToManaged() { 
			switch(NativeNode->type) { 
				case GNT_NONE: return a3dGraphNodeType_Mapper::None;
				case GNT_BOOL: return a3dGraphNodeType_Mapper::Bool;
				case GNT_CONST: return a3dGraphNodeType_Mapper::Const;
				case GNT_LINE: return a3dGraphNodeType_Mapper::Line;
				case GNT_BEZIE: return a3dGraphNodeType_Mapper::BezieSpline;
			}
		
			return a3dGraphNodeType_Mapper::None;
		}

		a3dGraphNodeType ManagedTypeToNative(a3dGraphNodeType_Mapper tp) { 
			switch(tp) { 
				case a3dGraphNodeType_Mapper::None: return GNT_NONE;
				case a3dGraphNodeType_Mapper::Bool: return GNT_BOOL;
				case a3dGraphNodeType_Mapper::Const: return GNT_CONST;
				case a3dGraphNodeType_Mapper::Line: return GNT_LINE;
				case a3dGraphNodeType_Mapper::BezieSpline: return GNT_BEZIE;
			}
		
			return GNT_NONE;
		}
	protected:
		virtual void OnNodeTypeChanged() { }

	internal:
		property a3dGraphNode* NativeNode { 
			a3dGraphNode* get() { return this->nativeNode; } 
			void set(a3dGraphNode* value) { 
				this->nativeNode = value;
			}
		}
		void UpdateNativeNode(a3dGraphNode *node);
	public:
		a3dGraphNodeMapper(bool createNativeNode) { 
			if(createNativeNode)
				this->nativeNode = new a3dGraphNode();
			P3 = InvalidPoint;
			P4 = InvalidPoint;
		} 
		a3dGraphNodeMapper() {  
			this->nativeNode = new a3dGraphNode();
			P3 = InvalidPoint;
			P4 = InvalidPoint;
		}
		a3dGraphNodeMapper( a3dGraphNode *node ) { 
			this->nativeNode = node;
			P3 = InvalidPoint;
			P4 = InvalidPoint;
		}
	
		property a3dGraphNodeType_Mapper NodeType { 
			a3dGraphNodeType_Mapper get() { return NativeTypeToManaged(); } 
			void set(a3dGraphNodeType_Mapper value) { 
				this->NativeNode->setType(this->ManagedTypeToNative(value));
				this->NativeNode->updateLocalTime();
				OnNodeTypeChanged();
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float K {  
			float get() { return this->NativeNode->k; }
			void set( float value ) { 
				this->NativeNode->k = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float A {  
			float get() { return this->NativeNode->a; }
			void set( float value ) { 
				this->NativeNode->a = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float B {  
			float get() { return this->NativeNode->b; }
			void set( float value ) { 
				this->NativeNode->b = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float C {  
			float get() { return this->NativeNode->c; }
			void set( float value ) { 
				this->NativeNode->c = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float T {  
			float get() { return this->NativeNode->t; }
			void set( float value ) { 
				this->NativeNode->t = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float T2 {  
			float get() { return this->NativeNode->t2; }
			void set( float value ) { 
				this->NativeNode->t3 = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float T3 {  
			float get() { return this->NativeNode->t3; }
			void set( float value ) { 
				this->NativeNode->t3 = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float Result {  
			float get() { return this->NativeNode->result; }
			void set( float value ) { 
				this->NativeNode->result = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property bool BResult { 
			bool get() { return this->NativeNode->bResult; }
			void set(bool value) { 
				this->NativeNode->bResult = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property bool SplitNext {  
			bool get() { return this->NativeNode->splitNext; }
			void set( bool value ) { 
				this->NativeNode->splitNext = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property PointF P1 { 
			PointF get() { return PointF(this->NativeNode->p1[0], this->NativeNode->p1[1]); }
			void set(PointF value) { 
				this->NativeNode->p1[0] = value.X;
				this->NativeNode->p1[1] = value.Y;
				this->NativeNode->calcParams();
				this->NativeNode->updateLocalTime();
				ObjectChanged(this, EventArgs::Empty);
			}	
		}

		property PointF P2 { 
			PointF get() { return PointF(this->NativeNode->p2[0], this->NativeNode->p2[1]); }
			void set(PointF value) { 
				this->NativeNode->p2[0] = value.X;
				this->NativeNode->p2[1] = value.Y;
				this->NativeNode->calcParams();
				this->NativeNode->updateLocalTime();
				ObjectChanged(this, EventArgs::Empty);
			}	
		}

		property PointF P3 { 
			PointF get() { return PointF(this->NativeNode->p3[0], this->NativeNode->p3[1]); }
			void set(PointF value) { 
				this->NativeNode->p3[0] = value.X;
				this->NativeNode->p3[1] = value.Y;
				this->NativeNode->calcParams();
				ObjectChanged(this, EventArgs::Empty);
			}	
		}

		property PointF P4 { 
			PointF get() { return PointF(this->NativeNode->p4[0], this->NativeNode->p4[1]); }
			void set(PointF value) { 
				this->NativeNode->p4[0] = value.X;
				this->NativeNode->p4[1] = value.Y;
				this->NativeNode->calcParams();				
				ObjectChanged(this, EventArgs::Empty);
			}	
		}

		property a3dGraphMapper^ OwnerGraph { 
			a3dGraphMapper^ get() { return this->ownerGraph; }
			void set(a3dGraphMapper ^value) { 
				this->ownerGraph = value;
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		float CalcNode(float val ) { 
			NativeNode->setTime(val);
			NativeNode->calcTime();
			NativeNode->calcNode();
			return NativeNode->result;
		}

		property PointF InvalidPoint { 
			PointF get() { return PointF(-100000, 100000); }
		}
	};
}