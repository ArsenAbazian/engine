#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace classMapper {

	ref struct Matrix4_Mapper;

	/// <summary>
	/// Summary for Matrix4Editor
	/// </summary>
	public ref class Matrix4Editor : public System::Windows::Forms::UserControl
	{
	public:
		Matrix4Editor(void)
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
		~Matrix4Editor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  a00;
	protected: 
	private: System::Windows::Forms::TextBox^  a01;
	private: System::Windows::Forms::TextBox^  a02;
	private: System::Windows::Forms::TextBox^  a03;
	private: System::Windows::Forms::TextBox^  a13;

	private: System::Windows::Forms::TextBox^  a12;

	private: System::Windows::Forms::TextBox^  a11;

	private: System::Windows::Forms::TextBox^  a10;
	private: System::Windows::Forms::TextBox^  a23;


	private: System::Windows::Forms::TextBox^  a22;

	private: System::Windows::Forms::TextBox^  a21;

	private: System::Windows::Forms::TextBox^  a20;
	private: System::Windows::Forms::TextBox^  a33;


	private: System::Windows::Forms::TextBox^  a32;

	private: System::Windows::Forms::TextBox^  a31;

	private: System::Windows::Forms::TextBox^  a30;


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
			this->a00 = (gcnew System::Windows::Forms::TextBox());
			this->a01 = (gcnew System::Windows::Forms::TextBox());
			this->a02 = (gcnew System::Windows::Forms::TextBox());
			this->a03 = (gcnew System::Windows::Forms::TextBox());
			this->a13 = (gcnew System::Windows::Forms::TextBox());
			this->a12 = (gcnew System::Windows::Forms::TextBox());
			this->a11 = (gcnew System::Windows::Forms::TextBox());
			this->a10 = (gcnew System::Windows::Forms::TextBox());
			this->a23 = (gcnew System::Windows::Forms::TextBox());
			this->a22 = (gcnew System::Windows::Forms::TextBox());
			this->a21 = (gcnew System::Windows::Forms::TextBox());
			this->a20 = (gcnew System::Windows::Forms::TextBox());
			this->a33 = (gcnew System::Windows::Forms::TextBox());
			this->a32 = (gcnew System::Windows::Forms::TextBox());
			this->a31 = (gcnew System::Windows::Forms::TextBox());
			this->a30 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// a00
			// 
			this->a00->Location = System::Drawing::Point(3, 3);
			this->a00->Name = L"a00";
			this->a00->Size = System::Drawing::Size(67, 20);
			this->a00->TabIndex = 0;
			// 
			// a01
			// 
			this->a01->Location = System::Drawing::Point(76, 3);
			this->a01->Name = L"a01";
			this->a01->Size = System::Drawing::Size(67, 20);
			this->a01->TabIndex = 1;
			// 
			// a02
			// 
			this->a02->Location = System::Drawing::Point(149, 3);
			this->a02->Name = L"a02";
			this->a02->Size = System::Drawing::Size(67, 20);
			this->a02->TabIndex = 2;
			// 
			// a03
			// 
			this->a03->Location = System::Drawing::Point(222, 3);
			this->a03->Name = L"a03";
			this->a03->Size = System::Drawing::Size(67, 20);
			this->a03->TabIndex = 3;
			// 
			// a13
			// 
			this->a13->Location = System::Drawing::Point(222, 29);
			this->a13->Name = L"a13";
			this->a13->Size = System::Drawing::Size(67, 20);
			this->a13->TabIndex = 7;
			// 
			// a12
			// 
			this->a12->Location = System::Drawing::Point(149, 29);
			this->a12->Name = L"a12";
			this->a12->Size = System::Drawing::Size(67, 20);
			this->a12->TabIndex = 6;
			// 
			// a11
			// 
			this->a11->Location = System::Drawing::Point(76, 29);
			this->a11->Name = L"a11";
			this->a11->Size = System::Drawing::Size(67, 20);
			this->a11->TabIndex = 5;
			// 
			// a10
			// 
			this->a10->Location = System::Drawing::Point(3, 29);
			this->a10->Name = L"a10";
			this->a10->Size = System::Drawing::Size(67, 20);
			this->a10->TabIndex = 4;
			// 
			// a23
			// 
			this->a23->Location = System::Drawing::Point(222, 55);
			this->a23->Name = L"a23";
			this->a23->Size = System::Drawing::Size(67, 20);
			this->a23->TabIndex = 11;
			// 
			// a22
			// 
			this->a22->Location = System::Drawing::Point(149, 55);
			this->a22->Name = L"a22";
			this->a22->Size = System::Drawing::Size(67, 20);
			this->a22->TabIndex = 10;
			// 
			// a21
			// 
			this->a21->Location = System::Drawing::Point(76, 55);
			this->a21->Name = L"a21";
			this->a21->Size = System::Drawing::Size(67, 20);
			this->a21->TabIndex = 9;
			// 
			// a20
			// 
			this->a20->Location = System::Drawing::Point(3, 55);
			this->a20->Name = L"a20";
			this->a20->Size = System::Drawing::Size(67, 20);
			this->a20->TabIndex = 8;
			// 
			// a33
			// 
			this->a33->Location = System::Drawing::Point(222, 81);
			this->a33->Name = L"a33";
			this->a33->Size = System::Drawing::Size(67, 20);
			this->a33->TabIndex = 15;
			// 
			// a32
			// 
			this->a32->Location = System::Drawing::Point(149, 81);
			this->a32->Name = L"a32";
			this->a32->Size = System::Drawing::Size(67, 20);
			this->a32->TabIndex = 14;
			// 
			// a31
			// 
			this->a31->Location = System::Drawing::Point(76, 81);
			this->a31->Name = L"a31";
			this->a31->Size = System::Drawing::Size(67, 20);
			this->a31->TabIndex = 13;
			// 
			// a30
			// 
			this->a30->Location = System::Drawing::Point(3, 81);
			this->a30->Name = L"a30";
			this->a30->Size = System::Drawing::Size(67, 20);
			this->a30->TabIndex = 12;
			// 
			// Matrix4Editor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->a33);
			this->Controls->Add(this->a32);
			this->Controls->Add(this->a31);
			this->Controls->Add(this->a30);
			this->Controls->Add(this->a23);
			this->Controls->Add(this->a22);
			this->Controls->Add(this->a21);
			this->Controls->Add(this->a20);
			this->Controls->Add(this->a13);
			this->Controls->Add(this->a12);
			this->Controls->Add(this->a11);
			this->Controls->Add(this->a10);
			this->Controls->Add(this->a03);
			this->Controls->Add(this->a02);
			this->Controls->Add(this->a01);
			this->Controls->Add(this->a00);
			this->Name = L"Matrix4Editor";
			this->Size = System::Drawing::Size(293, 105);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		public: property Matrix4_Mapper ^ Matrix { Matrix4_Mapper ^ get(); }
		public: void ParseMatrix( Matrix4_Mapper ^ mat );
	};
}
