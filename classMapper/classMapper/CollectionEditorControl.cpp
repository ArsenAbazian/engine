#include "StdAfx.h"
#include "CollectionEditorControl.h"

namespace classMapper { 
	
	void CollectionEditorControl::moveDown() { 
		

	}
	
	void CollectionEditorControl::moveUp() { 
		

	}

	void CollectionEditorControl::updatePropertyView() { 
		this->clearView();
		if( !this->IsAdvancedViewer ) showPropertyGrid();
		else this->showAdvancedPropertyView();
	}

	void CollectionEditorControl::showPropertyGrid() { 
		
		PropertyGrid ^grid = gcnew PropertyGrid();
		ViewerPanel->Controls->Add(grid);
		
		grid->Dock = System::Windows::Forms::DockStyle::Fill;
		this->updateSelectedObject();
		
		return ;
	}

	void CollectionEditorControl::showAdvancedPropertyView() { 
		
		return ;
	}

	void CollectionEditorControl::updateSelectedObject() { 
		if( !IsAdvancedViewer ) { 
			PropertyGrid ^grid = dynamic_cast<PropertyGrid^> (ViewerPanel->Controls[0]);
			if( grid == nullptr )
				return;
			grid->SelectedObject = itemsListBox->SelectedItem;
		}
		else { 
			this->updateAdvancedSelectedObject();
		}
	}

	void CollectionEditorControl::clearView() { 
		if(ViewerPanel->Controls->Count > 0) {
			Control ^ctrl = ViewerPanel->Controls[0];
			ViewerPanel->Controls->Clear();
		}
	}
	
	void CollectionEditorControl::updateAdvancedSelectedObject() { 
		
	}

	void CollectionEditorControl::OnCollectionChanged() { 
		itemsListBox->Items->Clear();
		if(SelectedCollection == nullptr) return ;
		IList ^list = SelectedCollection;
		for( int i = 0; i < SelectedCollection->Count; i++ ) { 
			itemsListBox->Items->Add( list[i] );
		}
		if(itemsListBox->Items->Count == 0) return;
		itemsListBox->SelectedIndex = 0;
	}
}
