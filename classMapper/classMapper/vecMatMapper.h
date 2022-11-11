// vecMatMapper.h

#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {	
	
	ref class Vector3_MapperConverter;
	ref class Vector3_MapperEditor;
	
	[TypeConverter( Vector3_MapperConverter::typeid ), Editor(Vector3_MapperEditor::typeid, UITypeEditor::typeid ) ]
	public ref struct Vector3_Mapper { 
		float x;
		float y;
		float z;
	
	public:
		static Vector3_Mapper^ Empty;
		static Vector3_Mapper() { Vector3_Mapper::Empty = gcnew Vector3_Mapper(); }
		
		Vector3_Mapper() { 
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		Vector3_Mapper( float x, float y, float z ) { 
			this->x = x;
			this->y = y;
			this->z = z;
		}
		
		property float X { float get() { return this->x; } void set( float value ) { this->x = value; } }

		property float Y { float get() { return this->y; } void set( float value ) { this->y = value; } }

		property float Z { float get() { return this->z; } void set( float value ) { this->z = value; } }

		virtual System::String^ ToString() override { 
			
			cli::array<System::String^> ^arr = gcnew cli::array<System::String^> { 
				gcnew System::String("{X="), this->X.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",Y="), this->Y.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",Z="), this->Z.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String("}") }; 
			return String::Concat( arr );
		}

		static Vector3_Mapper ^ Parse( String ^ source) {
			CultureInfo ^culture = CultureInfo::CurrentCulture;
			char ch = (char)culture->TextInfo->ListSeparator->default[0];
			cli::array<System::String^> ^sArray = source->Split( ch );
			TypeConverter ^ converter = TypeDescriptor::GetConverter(float::typeid);
			
			Vector3_Mapper ^ vector = gcnew Vector3_Mapper();
			vector->X = (float)converter->ConvertFrom( sArray[0] );
			vector->Y = (float)converter->ConvertFrom( sArray[1] );
			vector->Z = (float)converter->ConvertFrom( sArray[2] );
			return vector;
		}
		void ToVector3( Vector3 v ) { v[0] = this->X; v[1] = this->Y; v[2] = this->Z; }
 	};

	public ref class Vector3_MapperConverter : public TypeConverter { 
	public:
		virtual bool CanConvertFrom(ITypeDescriptorContext ^ context, Type ^ sourceType) override { 
			return ((sourceType == (System::String::typeid)) || TypeConverter::CanConvertFrom(context, sourceType));
		}
		
		virtual bool CanConvertTo(ITypeDescriptorContext ^ context, Type ^ destinationType) override { 
			return ((destinationType == (System::String::typeid)) || TypeConverter::CanConvertTo(context, destinationType));
		}
		
		virtual Object ^ ConvertFrom(ITypeDescriptorContext ^ context, CultureInfo ^ culture, Object ^ value) override { 
			if (value == nullptr) {
				 throw TypeConverter::GetConvertFromException(value);
			}
			System::String ^ source = dynamic_cast<String^>(value);
			if (source != nullptr) {
				return Vector3_Mapper::Parse(source);
			}
			return TypeConverter::ConvertFrom(context, culture, value);
		}
		
		virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context, CultureInfo ^ culture, Object ^ value, Type ^ destinationType) override { 
			Vector3_Mapper ^vector = dynamic_cast<Vector3_Mapper^>(value);
			if( vector == nullptr )
				return TypeConverter::ConvertTo( context, culture, value, destinationType );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String ^ sep = String::Concat( culture->TextInfo->ListSeparator->default[0], gcnew System::String( " " ) );
			cli::array<String^> ^arr = gcnew cli::array<String^> { vector->X.ToString(), sep, vector->Y.ToString(), sep, vector->Z.ToString() };
			return String::Concat( arr );
		}
	};

	public ref class Vector3_MapperEditor : public UITypeEditor { 
	public:
		Vector3_MapperEditor() {}
		virtual UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext ^ context) override { 
			return UITypeEditorEditStyle::Modal;
		}
		virtual Object^ EditValue( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) override { 
			return GetEditValueCore( context, provider, value );
		}
		Object ^ GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value );
	};

	
	ref class Matrix4_MapperConverter;
	ref class Matrix4_MapperEditor;
	
	[TypeConverter( Matrix4_MapperConverter::typeid ), Editor(Matrix4_MapperEditor::typeid, UITypeEditor::typeid ) ]
	public ref struct Matrix4_Mapper { 
		float a00, a01, a02, a03;
		float a10, a11, a12, a13;
		float a20, a21, a22, a23;
		float a30, a31, a32, a33;
	public:
		static Matrix4_Mapper^ Origin;
		static Matrix4_Mapper() { Matrix4_Mapper::Origin = gcnew Matrix4_Mapper(); }		

		Matrix4_Mapper() { 
			this->originMatrix();
		}

		void originMatrix() { 
			Matrix4 mat;
			originMatrix4( mat );
			this->FromMatrix4( mat );
		}
	
		void ToMatrix4( Matrix4 mat ) { 
			mat[0][0] = a00; mat[0][1] = a01; mat[0][2] = a02; mat[0][3] = a03;
			mat[1][0] = a10; mat[1][1] = a11; mat[1][2] = a12; mat[1][3] = a13;
			mat[2][0] = a20; mat[2][1] = a21; mat[2][2] = a22; mat[2][3] = a23;
			mat[3][0] = a30; mat[3][1] = a31; mat[3][2] = a32; mat[3][3] = a33;
		}
		void FromMatrix4( Matrix4 mat ) {
			a00 = mat[0][0]; a01 = mat[0][1]; a02 = mat[0][2]; a03 = mat[0][3];
			a10 = mat[1][0]; a11 = mat[1][1]; a12 = mat[1][2]; a13 = mat[1][3];
			a20 = mat[2][0]; a21 = mat[2][1]; a22 = mat[2][2]; a23 = mat[2][3];
			a30 = mat[3][0]; a31 = mat[3][1]; a32 = mat[3][2]; a33 = mat[3][3];
		}

		property float A00 { float get() { return a00; } void set( float value ) { a00 = value; } } 
		property float A01 { float get() { return a01; } void set( float value ) { a01 = value; } } 
		property float A02 { float get() { return a02; } void set( float value ) { a02 = value; } } 
		property float A03 { float get() { return a03; } void set( float value ) { a03 = value; } } 
		property float A10 { float get() { return a10; } void set( float value ) { a10 = value; } } 
		property float A11 { float get() { return a11; } void set( float value ) { a11 = value; } } 
		property float A12 { float get() { return a12; } void set( float value ) { a12 = value; } } 
		property float A13 { float get() { return a13; } void set( float value ) { a13 = value; } } 
		property float A20 { float get() { return a20; } void set( float value ) { a20 = value; } } 
		property float A21 { float get() { return a21; } void set( float value ) { a21 = value; } } 
		property float A22 { float get() { return a22; } void set( float value ) { a22 = value; } } 
		property float A23 { float get() { return a23; } void set( float value ) { a23 = value; } } 
		property float A30 { float get() { return a30; } void set( float value ) { a30 = value; } } 
		property float A31 { float get() { return a31; } void set( float value ) { a31 = value; } } 
		property float A32 { float get() { return a32; } void set( float value ) { a32 = value; } } 
		property float A33 { float get() { return a33; } void set( float value ) { a33 = value; } } 

		virtual System::String^ ToString() override { 
			
			cli::array<System::String^> ^arr = gcnew cli::array<System::String^> { 
				gcnew System::String("{a00="), this->A00.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a01="), this->A01.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a02="), this->A02.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a03="), this->A03.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a10="), this->A10.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a11="), this->A11.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a12="), this->A12.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a13="), this->A13.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a20="), this->A20.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a21="), this->A21.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a22="), this->A22.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a23="), this->A23.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a30="), this->A30.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a31="), this->A31.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a32="), this->A32.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String(",a33="), this->A33.ToString(CultureInfo::CurrentCulture), 
				gcnew System::String("}") }; 
			return String::Concat( arr );
		}

		static Matrix4_Mapper ^ Parse( String ^ source) {
			CultureInfo ^culture = CultureInfo::CurrentCulture;
			char ch = (char)culture->TextInfo->ListSeparator->default[0];
			cli::array<System::String^> ^sArray = source->Split( ch );
			TypeConverter ^ converter = TypeDescriptor::GetConverter(float::typeid);
			
			Matrix4_Mapper ^ matrix = gcnew Matrix4_Mapper();
			matrix->A00 = (float)converter->ConvertFrom( sArray[0] );
			matrix->A01 = (float)converter->ConvertFrom( sArray[1] );
			matrix->A02 = (float)converter->ConvertFrom( sArray[2] );
			matrix->A03 = (float)converter->ConvertFrom( sArray[3] );
			matrix->A10 = (float)converter->ConvertFrom( sArray[4] );
			matrix->A11 = (float)converter->ConvertFrom( sArray[5] );
			matrix->A12 = (float)converter->ConvertFrom( sArray[6] );
			matrix->A13 = (float)converter->ConvertFrom( sArray[7] );
			matrix->A20 = (float)converter->ConvertFrom( sArray[8] );
			matrix->A21 = (float)converter->ConvertFrom( sArray[9] );
			matrix->A22 = (float)converter->ConvertFrom( sArray[10] );
			matrix->A23 = (float)converter->ConvertFrom( sArray[11] );
			matrix->A30 = (float)converter->ConvertFrom( sArray[12] );
			matrix->A31 = (float)converter->ConvertFrom( sArray[13] );
			matrix->A32 = (float)converter->ConvertFrom( sArray[14] );
			matrix->A33 = (float)converter->ConvertFrom( sArray[15] );
			
			return matrix;
		}
		float getValue(int row, int col) { 
			if(row == 0 && col == 0) return A00;
			if(row == 0 && col == 1) return A01;
			if(row == 0 && col == 2) return A02;
			if(row == 0 && col == 3) return A03;

			if(row == 1 && col == 0) return A10;
			if(row == 1 && col == 1) return A11;
			if(row == 1 && col == 2) return A12;
			if(row == 1 && col == 3) return A13;

			if(row == 2 && col == 0) return A20;
			if(row == 2 && col == 1) return A21;
			if(row == 2 && col == 2) return A22;
			if(row == 2 && col == 3) return A23;

			if(row == 3 && col == 0) return A30;
			if(row == 3 && col == 1) return A31;
			if(row == 3 && col == 2) return A32;
			if(row == 3 && col == 3) return A33;
			
			return 0.0f;
		}

		void setValue(int row, int col, float val) { 
			if(row == 0 && col == 0) A00 = val;
			if(row == 0 && col == 1) A01 = val;
			if(row == 0 && col == 2) A02 = val;
			if(row == 0 && col == 3) A03 = val;

			if(row == 1 && col == 0) A10 = val;
			if(row == 1 && col == 1) A11 = val;
			if(row == 1 && col == 2) A12 = val;
			if(row == 1 && col == 3) A13 = val;

			if(row == 2 && col == 0) A20 = val;
			if(row == 2 && col == 1) A21 = val;
			if(row == 2 && col == 2) A22 = val;
			if(row == 2 && col == 3) A23 = val;

			if(row == 3 && col == 0) A30 = val;
			if(row == 3 && col == 1) A31 = val;
			if(row == 3 && col == 2) A32 = val;
			if(row == 3 && col == 3) A33 = val;
		}
	};

	public ref class Matrix4_MapperConverter : public TypeConverter { 
	public:
		virtual bool CanConvertFrom(ITypeDescriptorContext ^ context, Type ^ sourceType) override { 
			return ((sourceType == (System::String::typeid)) || TypeConverter::CanConvertFrom(context, sourceType));
		}
		
		virtual bool CanConvertTo(ITypeDescriptorContext ^ context, Type ^ destinationType) override { 
			return ((destinationType == (System::String::typeid)) || TypeConverter::CanConvertTo(context, destinationType));
		}
		
		virtual Object ^ ConvertFrom(ITypeDescriptorContext ^ context, CultureInfo ^ culture, Object ^ value) override { 
			if (value == nullptr) {
				 throw TypeConverter::GetConvertFromException(value);
			}
			System::String ^ source = dynamic_cast<String^>(value);
			if (source != nullptr) {
				return Matrix4_Mapper::Parse(source);
			}
			return TypeConverter::ConvertFrom(context, culture, value);
		}
		
		virtual Object ^ ConvertTo(ITypeDescriptorContext ^ context, CultureInfo ^ culture, Object ^ value, Type ^ destinationType) override { 
			Matrix4_Mapper ^matrix = dynamic_cast<Matrix4_Mapper^>(value);
			if( matrix == nullptr )
				return TypeConverter::ConvertTo( context, culture, value, destinationType );

			if( culture == nullptr )
				culture = CultureInfo::CurrentCulture;

			String ^ sep = String::Concat( culture->TextInfo->ListSeparator->default[0], gcnew System::String( " " ) );
			cli::array<String^> ^arr = gcnew cli::array<String^> { 
				matrix->A00.ToString(), sep, matrix->A01.ToString(), sep, matrix->A02.ToString(), sep, matrix->A03.ToString(),
				matrix->A10.ToString(), sep, matrix->A11.ToString(), sep, matrix->A12.ToString(), sep, matrix->A13.ToString(),
				matrix->A20.ToString(), sep, matrix->A21.ToString(), sep, matrix->A22.ToString(), sep, matrix->A23.ToString(),
				matrix->A30.ToString(), sep, matrix->A31.ToString(), sep, matrix->A32.ToString(), sep, matrix->A33.ToString()
			};
			return String::Concat( arr );
		}
	};

	public ref class Matrix4_MapperEditor : public UITypeEditor { 
	public:
		Matrix4_MapperEditor() {}
		virtual UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext ^ context) override { 
			return UITypeEditorEditStyle::Modal;
		}
		virtual Object^ EditValue( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value ) override { 
			return GetEditValueCore( context, provider, value );
		}
		Object ^ GetEditValueCore( ITypeDescriptorContext ^ context, System::IServiceProvider ^ provider, Object ^ value );
	}; 
}