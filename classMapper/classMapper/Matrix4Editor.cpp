#include "StdAfx.h"
#include "Matrix4Editor.h"
#include "vecMatMapper.h"

namespace classMapper { 
	
	void Matrix4Editor::ParseMatrix( Matrix4_Mapper^ mat ) { 
		
		this->a00->Text = mat->A00.ToString();
		this->a01->Text = mat->A01.ToString();
		this->a02->Text = mat->A02.ToString();		
		this->a03->Text = mat->A03.ToString();		

		this->a10->Text = mat->A10.ToString();
		this->a11->Text = mat->A11.ToString();
		this->a12->Text = mat->A12.ToString();		
		this->a13->Text = mat->A13.ToString();		
		
		this->a20->Text = mat->A20.ToString();
		this->a21->Text = mat->A21.ToString();
		this->a22->Text = mat->A22.ToString();		
		this->a23->Text = mat->A23.ToString();		

		this->a30->Text = mat->A30.ToString();
		this->a31->Text = mat->A31.ToString();
		this->a32->Text = mat->A32.ToString();		
		this->a33->Text = mat->A33.ToString();		
	}

	Matrix4_Mapper^ Matrix4Editor::Matrix::get() {  
		
		Matrix4_Mapper ^ mat = gcnew Matrix4_Mapper();
		
		mat->A00 = float::Parse( this->a00->Text );
		mat->A01 = float::Parse( this->a01->Text );
		mat->A02 = float::Parse( this->a02->Text );
		mat->A03 = float::Parse( this->a03->Text );

		mat->A10 = float::Parse( this->a10->Text );
		mat->A11 = float::Parse( this->a11->Text );
		mat->A12 = float::Parse( this->a12->Text );
		mat->A13 = float::Parse( this->a13->Text );

		mat->A20 = float::Parse( this->a20->Text );
		mat->A21 = float::Parse( this->a21->Text );
		mat->A22 = float::Parse( this->a22->Text );
		mat->A23 = float::Parse( this->a23->Text );

		mat->A30 = float::Parse( this->a30->Text );
		mat->A31 = float::Parse( this->a31->Text );
		mat->A32 = float::Parse( this->a32->Text );
		mat->A33 = float::Parse( this->a33->Text );

		return mat;
	}
}
