#pragma once

#include "CollectionEditorControl.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace classMapper {

	/// <summary>
	/// Summary for CollectionEditorForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CollectionEditorForm : public System::Windows::Forms::Form
	{
	public:
		CollectionEditorForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CollectionEditorForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CollectionEditorControl^  collectionEditorControl1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->collectionEditorControl1 = (gcnew classMapper::CollectionEditorControl());
			this->SuspendLayout();
			// 
			// collectionEditorControl1
			// 
			this->collectionEditorControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->collectionEditorControl1->IsAdvancedViewer = false;
			this->collectionEditorControl1->Location = System::Drawing::Point(0, 0);
			this->collectionEditorControl1->Name = L"collectionEditorControl1";
			this->collectionEditorControl1->SelectedCollection = nullptr;
			this->collectionEditorControl1->Size = System::Drawing::Size(507, 467);
			this->collectionEditorControl1->TabIndex = 0;
			// 
			// CollectionEditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(507, 467);
			this->Controls->Add(this->collectionEditorControl1);
			this->Name = L"CollectionEditorForm";
			this->Text = L"CollectionEditorForm";
			this->TopMost = true;
			this->ResumeLayout(false);

		}
#pragma endregion

	public: property CollectionBase^ SelectedCollection { CollectionBase^ get() { return collectionEditorControl1->SelectedCollection; } void set(CollectionBase^ val) { this->collectionEditorControl1->SelectedCollection = val; }  }
	};
}
