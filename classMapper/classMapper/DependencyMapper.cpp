#include "stdafx.h"
#include "DependencyMapper.h"

namespace classMapper { 

	a3dDependencyPropertyMapper^ a3dDependencyPropertyMapper::GetPropertyMapper(a3dDependencyProperty *prop) { 
		IEnumerable ^enm = (IEnumerable ^)a3dDependencyPropertyMapper::PropertyList;
		IEnumerator ^en = enm->GetEnumerator();
		while(en->MoveNext()) { 
			if(((a3dDependencyPropertyMapper^)en->Current)->Property == prop) return (a3dDependencyPropertyMapper^)en->Current;
		}
		a3dDependencyPropertyMapper ^m = gcnew a3dDependencyPropertyMapper(prop);
		a3dDependencyPropertyMapper::PropertyList->Add(m);
		m->InitializeNotifyList();
		return m;
	}

	a3dDependencyPropertyMapper::a3dDependencyPropertyMapper(a3dDependencyProperty *prop) { 
	
		this->depProperty = prop;
		this->dependency = this->CreateDependency();
		if(this->dependency != nullptr)
			this->dependency->Initialize(this);
	}
	
	a3dDependencyPropertyTypeMapper a3dDependencyPropertyMapper::nativeToManaged( a3dDependencyPropertyType propType ) { 
		switch(propType) { 
			case DPROP_NONE: return a3dDependencyPropertyTypeMapper::None;
			case DPROP_BOOL: return a3dDependencyPropertyTypeMapper::Bool;
			case DPROP_FLOAT: return a3dDependencyPropertyTypeMapper::Float;
			case DPROP_INT: return a3dDependencyPropertyTypeMapper::Int;
		}
		return a3dDependencyPropertyTypeMapper::None;
	}

	a3dDependencyPropertyType a3dDependencyPropertyMapper::manageToNative( a3dDependencyPropertyTypeMapper propType ) { 
		switch(propType) { 
			case a3dDependencyPropertyTypeMapper::None: return DPROP_NONE;
			case a3dDependencyPropertyTypeMapper::Bool: return DPROP_BOOL;
			case a3dDependencyPropertyTypeMapper::Float: return DPROP_FLOAT;
			case a3dDependencyPropertyTypeMapper::Int: return DPROP_INT;
		}
		return DPROP_NONE;
	}

	a3dDependencyBaseMapper^ a3dDependencyPropertyMapper::CreateDependency() { 
		if(this->depProperty->getDependency() == null) return nullptr;
		switch(this->depProperty->getDependency()->type) { 
			case DEPENDENCY_BASE: return gcnew a3dDependencyBaseMapper(this->depProperty->getDependency());
			case DEPENDENCY_SIMPLE: return gcnew a3dSimpleDependencyMapper(dynamic_cast<a3dSimpleDependency*>(this->depProperty->getDependency()));
			case DEPENDENCY_GRAPH: return gcnew a3dGraphDependencyMapper(dynamic_cast<a3dGraphDependency*>(this->depProperty->getDependency()));
			case DEPENDENCY_SCRIPT: return gcnew a3dScriptDependencyMapper(dynamic_cast<a3dScriptDependency*>(this->depProperty->getDependency()));
		}
		return nullptr;
	}

	a3dDependencyBaseMapper^ a3dDependencyPropertyMapper::Dependency::get() { 
		return this->dependency;
	}

	void a3dDependencyPropertyMapper::Dependency::set(a3dDependencyBaseMapper^ value) { 
		if( Dependency != nullptr )
			Dependency->Clear();
		this->dependency = value;
		Dependency->Initialize(this);
		InitializeNotifyList();
	}

	void a3dDependencyPropertyMapper::InitializeNotifyList() { 
		this->notifyProps = gcnew DependencyPropertyCollection(nullptr);
		for( int i = 0; i < Property->getNotifyList().count; i++ ) { 
			this->notifyProps->Add(a3dDependencyPropertyMapper::GetPropertyMapper((a3dDependencyProperty*)Property->getNotifyList().lpArray[i]));
		}
	}

	DependencyPropertyCollection^ a3dDependencyPropertyMapper::NotifyProperties::get() { 
		return this->notifyProps;
	}

	void a3dDependencyBaseMapper::Clear() { 
		if(this->Dest != nullptr)
			this->Dest->Property->removeDependency();
	}	

	void a3dDependencyBaseMapper::Initialize(a3dDependencyPropertyMapper ^prop) { 
		this->dest = prop;
		if(this->dependency->dest == prop->Property) return;
		prop->Property->attachDependency(this->dependency);
	}

	a3dDependencyPropertyMapper^ a3dDependencyBaseMapper::Dest::get() { return this->dest; }
	void a3dDependencyBaseMapper::Dest::set(a3dDependencyPropertyMapper^ value) { 
		this->dest = value;
		this->dependency->setProperty(value->Property);
	}

	a3dSimpleDependencyMapper::a3dSimpleDependencyMapper(a3dSimpleDependency *dependency) : a3dDependencyBaseMapper(dependency) { }
	
	a3dDependencyPropertyMapper^ a3dSimpleDependencyMapper::Source::get() { return this->source; }
	void a3dSimpleDependencyMapper::Source::set(a3dDependencyPropertyMapper^ value) { 
		this->source = value;
		a3dSimpleDependency *sd = dynamic_cast<a3dSimpleDependency *>(this->dependency);
		if(Source == nullptr)
			sd->setSourceProperty(null);
		else if(sd != null && sd->src != Source->Property)
			sd->setSourceProperty(value->Property);
	}

	void a3dSimpleDependencyMapper::Initialize(a3dDependencyPropertyMapper ^prop) { 
		a3dDependencyBaseMapper::Initialize(prop);
		a3dSimpleDependency *sd = dynamic_cast<a3dSimpleDependency *>(this->dependency);
		if(sd == null || sd->src == null)
			this->source = nullptr;
		else 
			this->source = a3dDependencyPropertyMapper::GetPropertyMapper(sd->src);
	}

	a3dGraphDependencyMapper::a3dGraphDependencyMapper( a3dGraphDependency *dependency ) : a3dSimpleDependencyMapper(dependency) { }
	a3dScriptDependencyMapper::a3dScriptDependencyMapper( a3dScriptDependency *dependency ) : a3dDependencyBaseMapper(dependency) { }

	GraphOperationType a3dGraphDependencyMapper::Native2Managed( a3dGraphOperation op ) { 
		static GraphOperationType managed[] = { GraphOperationType::None, GraphOperationType::SelectOp1, GraphOperationType::SelectOp2, GraphOperationType::Add, GraphOperationType::Sub, GraphOperationType::Mul, GraphOperationType::Div };
		static a3dGraphOperation native[] = { GO_NONE, GO_SELECTOP1, GO_SELECTOP2, GO_ADD, GO_SUB, GO_MUL, GO_DIV };
		for(int i = 0; i < 7; i++) { 
			if( native[i] == op ) return managed[i];
		}
		return GraphOperationType::None;
	}
	a3dGraphOperation a3dGraphDependencyMapper::Managed2Native( GraphOperationType op ) { 
		static GraphOperationType managed[] = { GraphOperationType::None, GraphOperationType::SelectOp1, GraphOperationType::SelectOp2, GraphOperationType::Add, GraphOperationType::Sub, GraphOperationType::Mul, GraphOperationType::Div };
		static a3dGraphOperation native[] = { GO_NONE, GO_SELECTOP1, GO_SELECTOP2, GO_ADD, GO_SUB, GO_MUL, GO_DIV };
		for(int i = 0; i < 7; i++) { 
			if( managed[i] == op ) return native[i];
		}
		return GO_NONE;
	}

	void a3dGraphDependencyMapper::Source2::set(a3dDependencyPropertyMapper^ value) { 
		this->src2 = value;
		if(Source2 == nullptr)
			GraphDependency->setSource2Property(null);
		else if(GraphDependency->src2 != Source2->Property)
			GraphDependency->setSource2Property(value->Property);
	}

	void a3dGraphDependencyMapper::Graph::set(a3dGraphMapper^ value) { 
		this->graph1 = value;
		if(this->graph1 != nullptr) { 
			this->graph1->UpdateNativeGraph();
			GraphDependency->graph = this->graph1->NativeGraph;
		}
		else 
			GraphDependency->graph = null;
		if( this->dependency->dest != null )
			this->dependency->updateProperty();
	} 

	void a3dGraphDependencyMapper::Graph2::set(a3dGraphMapper^ value) { 
		this->graph2 = value;
		if(this->graph2 != nullptr) { 
			this->graph2->UpdateNativeGraph();
			GraphDependency->graph2 = this->graph2->NativeGraph;
		}
		else
			GraphDependency->graph2 = null;
		if( this->dependency->dest != null )
			this->dependency->updateProperty();
	}
}