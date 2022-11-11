// cameraMapper.h

#pragma once

#include "..\\vecMatMapper.h"
#include "..\\stringHelper.h"
#include "..\\interfaces.h"
#include "a3dGraphNodeMapper.h"
#include "a3dGraph\\a3dGraph.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper { 

	ref class a3dGraphMapper;

	public enum class GraphType { Bool, Float };

	public ref class a3dGraphNodeCollection : CollectionBase { 
		a3dGraphMapper			^holder;
	public:
		a3dGraphNodeCollection(a3dGraphMapper ^graph) {  
			this->holder = graph;
		}

		property a3dGraphMapper^ Holder { a3dGraphMapper^ get() { return holder; } }
		
		int Add(a3dGraphNodeMapper ^node) { return this->List->Add(node); }
		void Insert(int index, a3dGraphNodeMapper ^node) { this->List->Insert(index, node); }
		void Remove(a3dGraphNodeMapper ^node) { this->List->Remove(node); }
		
		a3dGraphNodeMapper^ get_Item(int index) { return dynamic_cast<a3dGraphNodeMapper ^>(List->default[index]); }
		void set_Item(int index, a3dGraphNodeMapper ^node) { return List->default[index] = node; }
		property a3dGraphNodeMapper^ default [int] { 
			a3dGraphNodeMapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dGraphNodeMapper^ node) { set_Item(index, node); }
		} 
		int IndexOf(a3dGraphNodeMapper^ node) { return this->List->IndexOf(node); }

	protected:
		virtual void OnInsertComplete(int index, System::Object ^value) override { 
			CollectionBase::OnInsertComplete(index, value);
			((a3dGraphNodeMapper^)value)->OwnerGraph = Holder;
		}
		virtual void OnRemoveComplete(int index, System::Object ^value) override { 
			CollectionBase::OnRemoveComplete(index, value);
			((a3dGraphNodeMapper^)value)->OwnerGraph = nullptr;
		}
	};
	
	public ref class a3dGraphMapper : a3dObjectWithEvent { 
		a3dGraph				*nativeGraph;
		a3dGraphNodeCollection	^nodes;
	internal:
		property a3dGraph* NativeGraph { 
			a3dGraph* get() { return this->nativeGraph; } 
			void set( a3dGraph* value) { 
				this->nativeGraph = value;
			}
		}
		
		void InitializeNodes() { 
			for(int i = 0; i < this->NativeGraph->nodeCount; i++) { 
				a3dGraphNodeMapper^ node = CreateNode();
				node->NativeNode = this->NativeGraph->nodeList[i];
				this->Nodes->Add(node);
			}
		}
		void UpdateNativeGraph();
	protected:
		virtual a3dGraphNodeMapper^ CreateNode() { 
			return gcnew a3dGraphNodeMapper( false );
		}

	public:
		a3dGraphMapper(bool createNativeGraph) { 
			if(createNativeGraph) 
				this->nativeGraph = new a3dGraph();
			this->nodes = gcnew a3dGraphNodeCollection(this);
		}
		a3dGraphMapper() {
			this->nativeGraph = new a3dGraph();
			this->nodes = gcnew a3dGraphNodeCollection(this);
		}
		a3dGraphMapper( a3dGraph *nGraph ) { 
			this->nativeGraph = nGraph;
			this->nodes = gcnew a3dGraphNodeCollection(this);
			this->InitializeNodes();
		}
	
		property String^ GraphName {  
			String^ get() { return gcnew String(this->NativeGraph->name.getCString()); }
			void set( String^ value ) { 
				shStringHelper::Default.copyTo( this->NativeGraph->name, value );
			}  
		}

		property GraphType Type { 
			GraphType get() { 
				if( this->NativeGraph->type == GT_BOOL )
					return GraphType::Bool;
				return GraphType::Float;
			}
			void set(GraphType value) { 
				if(value == GraphType::Bool)
					this->NativeGraph->type = GT_BOOL;
				else 
					this->NativeGraph->type = GT_FLOAT;
			}
		}

		property a3dGraphNodeCollection^ Nodes { 
			a3dGraphNodeCollection^ get() { return this->nodes; }
		}; 
		
		virtual void OnPropertiesChanged() { } 
	};

	
	public ref class a3dGraphMapperCollection : CollectionBase { 
	public:
		int Add(a3dGraphMapper ^graph) { return this->InnerList->Add(graph); }
		void Insert(int index, a3dGraphMapper ^graph) { this->InnerList->Insert(index, graph); }
		void Remove(a3dGraphMapper ^graph) { this->InnerList->Remove(graph); }

		a3dGraphMapper^ get_Item(int index) { return dynamic_cast<a3dGraphMapper ^>(List->default[index]); }
		void set_Item(int index, a3dGraphMapper ^graph) { return List->default[index] = graph; }
		property a3dGraphMapper^ default [int] { 
			a3dGraphMapper^ get(int index) { return get_Item(index); }
			void set(int index, a3dGraphMapper^ graph) { set_Item(index, graph); }
		} 
		int IndexOf(a3dGraphMapper^ graph) { return this->InnerList->IndexOf(graph); }
	};
}