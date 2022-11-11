// gridMapper.h

#pragma once

#include "hstring.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class shStringHelper { 
		TCHAR *buf;
		shString *hstr;
	public:
		static shStringHelper Default;
		shStringHelper() { 
			buf = new TCHAR[1024];
			hstr = new shString();
		}
		shString* String2shString( String^ str) { 
			int i;
			for(i = 0; i < str->Length; i++ ) { 
				buf[i] = str[i];
			}
			buf[i] = 0;
			return new shString(buf);
		}
		shString& tohString(String ^str) { 
			copyTo(*hstr, str);
			return *(this->hstr);
		}
		void copyTo( shString &hstr, String ^str ) {
			int i;
			for(i = 0; i < str->Length; i++ ) { 
				buf[i] = str[i];
			}
			buf[i] = 0;
			hstr.initialize(buf);
		}
	};
}