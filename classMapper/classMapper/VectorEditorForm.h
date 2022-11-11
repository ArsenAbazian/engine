#pragma once

#include "VectorEditor.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace classMapper {

	ref struct Vector3_Mapper;
	
	/// <summary>
	/// Summary for VectorEditorForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class VectorEditorForm : public System::Windows::Forms::Form
	{
	public:
		VectorEditorForm(void)
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
		~VectorEditorForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 





	private: System::Windows::Forms::Button^  button1;
	private: VectorEditor^  vectorEditor1;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->vectorEditor1 = (gcnew classMapper::VectorEditor());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->button1->Location = System::Drawing::Point(171, 3);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 6;
			this->button1->Text = L"OK";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// vectorEditor1
			// 
			this->vectorEditor1->Location = System::Drawing::Point(2, 3);
			this->vectorEditor1->Name = L"vectorEditor1";
			this->vectorEditor1->Size = System::Drawing::Size(163, 80);
			this->vectorEditor1->TabIndex = 7;
			// 
			// VectorEditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(254, 83);
			this->Controls->Add(this->vectorEditor1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"VectorEditorForm";
			this->Text = L"Vector Editor Form";
			this->TopMost = true;
			this->ResumeLayout(false);

		}
#pragma endregion

		public: void ParseVector( Vector3_Mapper^ vector );
		public: property Vector3_Mapper^ Vector { 
			Vector3_Mapper^ get();
		}
	};
}
