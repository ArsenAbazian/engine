// This is the main DLL file.

#include "stdafx.h"

#include "vecMatMapper.h"
#include "VectorEditorForm.h"
#include "Matrix4EditorForm.h"

namespace classMapper { 
	
	Object^ Vector3_MapperEditor::GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) {
		if( provider == nullptr ) return value;
		Vector3_Mapper ^vector = dynamic_cast<Vector3_Mapper ^>(value);

		IWindowsFormsEditorService ^ edSvc = dynamic_cast<IWindowsFormsEditorService^> (provider->GetService(IWindowsFormsEditorService::typeid) );
		if( edSvc == nullptr ) return value;
		
		if(vector == nullptr) vector = gcnew Vector3_Mapper();	
		
		VectorEditorForm ^ form = gcnew VectorEditorForm();
		form->ParseVector( vector );
		if( form->ShowDialog() != DialogResult::OK ) return value;
		return form->Vector;		
	}

	Object^ Matrix4_MapperEditor::GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) {
		if( provider == nullptr ) return value;
		Matrix4_Mapper ^mat = dynamic_cast<Matrix4_Mapper ^>(value);

		IWindowsFormsEditorService ^ edSvc = dynamic_cast<IWindowsFormsEditorService^> (provider->GetService(IWindowsFormsEditorService::typeid) );
		if( edSvc == nullptr ) return value;
		
		if(mat == nullptr) mat = gcnew Matrix4_Mapper();	
		
		Matrix4EditorForm ^ form = gcnew Matrix4EditorForm();
		form->ParseMatrix( mat );
		if( form->ShowDialog() != DialogResult::OK ) return value;
		return form->Matrix;		
	}
}