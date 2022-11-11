#include "StdAfx.h"
#include "VectorEditor.h"
#include "vecMatMapper.h"

namespace classMapper { 
	
	void VectorEditor::ParseVector( Vector3_Mapper^ vector ) { 
		this->textBox1->Text = vector->X.ToString();
		this->textBox2->Text = vector->Y.ToString();
		this->textBox3->Text = vector->Z.ToString();		
	}

	Vector3_Mapper^ VectorEditor::Vector::get() {  
		Vector3_Mapper ^ vector = gcnew Vector3_Mapper();
		vector->X = float::Parse( this->textBox1->Text );
		vector->Y = float::Parse( this->textBox2->Text );
		vector->Z = float::Parse( this->textBox3->Text );

		return vector;
	}
}