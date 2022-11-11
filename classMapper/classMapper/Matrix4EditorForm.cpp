#include "StdAfx.h"
#include "Matrix4EditorForm.h"

#include "classMapper.h"

namespace classMapper { 
	
	void Matrix4EditorForm::ParseMatrix( Matrix4_Mapper^ mat ) { 
		this->matrix4Editor1->ParseMatrix( mat );
	}

	Matrix4_Mapper^ Matrix4EditorForm::Matrix::get() {  
		return this->matrix4Editor1->Matrix;
	}
}