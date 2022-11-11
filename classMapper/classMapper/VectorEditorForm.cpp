#include "StdAfx.h"
#include "VectorEditorForm.h"
#include "classMapper.h"

namespace classMapper { 
	
	void VectorEditorForm::ParseVector( Vector3_Mapper^ vector ) { 
		this->vectorEditor1->ParseVector( vector );
	}

	Vector3_Mapper^ VectorEditorForm::Vector::get() {  
		return this->vectorEditor1->Vector;
	}
}