#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace classMapper {

	/// <summary>
	/// Summary for CollectionEditorControl
	/// </summary>
	public ref class CollectionEditorControl : public System::Windows::Forms::UserControl
	{
	public:
		CollectionEditorControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->IsAdvancedViewer = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CollectionEditorControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ListBox^  itemsListBox;
	private: System::Windows::Forms::Button^  advViewver;

	private: System::Windows::Forms::Button^  downButton;
	private: System::Windows::Forms::Button^  upButton;




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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->itemsListBox = (gcnew System::Windows::Forms::ListBox());
			this->advViewver = (gcnew System::Windows::Forms::Button());
			this->downButton = (gcnew System::Windows::Forms::Button());
			this->upButton = (gcnew System::Windows::Forms::Button());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer2);
			this->splitContainer1->Panel1MinSize = 173;
			this->splitContainer1->Size = System::Drawing::Size(380, 410);
			this->splitContainer1->SplitterDistance = 173;
			this->splitContainer1->TabIndex = 0;
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->itemsListBox);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->advViewver);
			this->splitContainer2->Panel2->Controls->Add(this->downButton);
			this->splitContainer2->Panel2->Controls->Add(this->upButton);
			this->splitContainer2->Size = System::Drawing::Size(173, 410);
			this->splitContainer2->SplitterDistance = 370;
			this->splitContainer2->TabIndex = 0;
			// 
			// itemsListBox
			// 
			this->itemsListBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->itemsListBox->FormattingEnabled = true;
			this->itemsListBox->Location = System::Drawing::Point(0, 0);
			this->itemsListBox->Name = L"itemsListBox";
			this->itemsListBox->Size = System::Drawing::Size(173, 368);
			this->itemsListBox->TabIndex = 0;
			this->itemsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &CollectionEditorControl::itemsListBox_SelectedIndexChanged);
			// 
			// advViewver
			// 
			this->advViewver->Location = System::Drawing::Point(95, 3);
			this->advViewver->Name = L"advViewver";
			this->advViewver->Size = System::Drawing::Size(75, 23);
			this->advViewver->TabIndex = 2;
			this->advViewver->Text = L"adv viewer";
			this->advViewver->UseVisualStyleBackColor = true;
			this->advViewver->Click += gcnew System::EventHandler(this, &CollectionEditorControl::advViewver_Click);
			// 
			// downButton
			// 
			this->downButton->Location = System::Drawing::Point(44, 3);
			this->downButton->Name = L"downButton";
			this->downButton->Size = System::Drawing::Size(45, 23);
			this->downButton->TabIndex = 1;
			this->downButton->Text = L"down";
			this->downButton->UseVisualStyleBackColor = true;
			this->downButton->Click += gcnew System::EventHandler(this, &CollectionEditorControl::downButton_Click);
			// 
			// upButton
			// 
			this->upButton->Location = System::Drawing::Point(3, 3);
			this->upButton->Name = L"upButton";
			this->upButton->Size = System::Drawing::Size(35, 23);
			this->upButton->TabIndex = 0;
			this->upButton->Text = L"up";
			this->upButton->UseVisualStyleBackColor = true;
			this->upButton->Click += gcnew System::EventHandler(this, &CollectionEditorControl::upButton_Click);
			// 
			// CollectionEditorControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Name = L"CollectionEditorControl";
			this->Size = System::Drawing::Size(380, 410);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		public: property Panel^ ViewerPanel { Panel^ get() { return splitContainer1->Panel2; } }
		private: bool isAdvancedViewer;
		public: property bool IsAdvancedViewer { 
					bool get() { return isAdvancedViewer; } 
					void set(bool val) {  
						isAdvancedViewer = val;
						updatePropertyView();
					} 
				}	
		
		private: System::Void upButton_Click(System::Object^  sender, System::EventArgs^  e) { moveUp(); }
		private: System::Void downButton_Click(System::Object^  sender, System::EventArgs^  e) { moveDown(); }
		private: System::Void advViewver_Click(System::Object^  sender, System::EventArgs^  e) { IsAdvancedViewer = !IsAdvancedViewer; }

		protected: virtual void moveUp();
		protected: virtual void moveDown();
		protected: virtual void updatePropertyView();
		protected: virtual void showPropertyGrid();
		protected: virtual void showAdvancedPropertyView();
		protected: virtual void clearView();
		protected: virtual void updateSelectedObject();
		protected: virtual void updateAdvancedSelectedObject();

		private: CollectionBase^ coll;
		public: property CollectionBase^ SelectedCollection { 
					   CollectionBase^ get() { return coll; } 
					   void set(CollectionBase ^collection) { 
							coll = collection; 
							OnCollectionChanged();
					   } 
					}
		protected: virtual void OnCollectionChanged();
		private: System::Void itemsListBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			updateSelectedObject();
		}
	};
}
