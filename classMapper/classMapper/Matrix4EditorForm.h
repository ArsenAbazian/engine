#pragma once

#include "Matrix4Editor.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace classMapper {

	ref struct Matrix4_Mapper;
	
	/// <summary>
	/// Summary for Matrix4EditorForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Matrix4EditorForm : public System::Windows::Forms::Form
	{
	public:
		Matrix4EditorForm(void)
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
		~Matrix4EditorForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: classMapper::Matrix4Editor^  matrix4Editor1;
	protected: 
	private: System::Windows::Forms::Button^  OK;

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
			this->matrix4Editor1 = (gcnew classMapper::Matrix4Editor());
			this->OK = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// matrix4Editor1
			// 
			this->matrix4Editor1->Location = System::Drawing::Point(12, 12);
			this->matrix4Editor1->Name = L"matrix4Editor1";
			this->matrix4Editor1->Size = System::Drawing::Size(293, 105);
			this->matrix4Editor1->TabIndex = 0;
			// 
			// OK
			// 
			this->OK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->OK->Location = System::Drawing::Point(227, 123);
			this->OK->Name = L"OK";
			this->OK->Size = System::Drawing::Size(75, 23);
			this->OK->TabIndex = 1;
			this->OK->Text = L"button1";
			this->OK->UseVisualStyleBackColor = true;
			// 
			// Matrix4EditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(314, 153);
			this->Controls->Add(this->OK);
			this->Controls->Add(this->matrix4Editor1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"Matrix4EditorForm";
			this->Text = L"Matrix4 Editor Form";
			this->TopMost = true;
			this->ResumeLayout(false);

		}
#pragma endregion

	public: void ParseMatrix( Matrix4_Mapper ^ mat );
	public: property Matrix4_Mapper ^ Matrix { Matrix4_Mapper ^ get(); }
	};
}
