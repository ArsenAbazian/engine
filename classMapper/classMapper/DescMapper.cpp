// This is the main DLL file.

#include "stdafx.h"

#include "DescMapper.h"
#include "CollectionEditorForm.h"

namespace classMapper { 

	Object^ CollectionEditor::GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) {
		if( provider == nullptr ) return value;
		CollectionBase ^coll = dynamic_cast<CollectionBase ^>(value);

		IWindowsFormsEditorService ^ edSvc = dynamic_cast<IWindowsFormsEditorService^> (provider->GetService(IWindowsFormsEditorService::typeid) );
		if( edSvc == nullptr ) return value;

		if(coll == nullptr) return nullptr;	

		CollectionEditorForm ^ form = gcnew CollectionEditorForm();
		form->SelectedCollection = coll;
		if( form->ShowDialog() != DialogResult::OK ) return value;
		return form->SelectedCollection;		
	}
}