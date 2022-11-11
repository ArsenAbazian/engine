#pragma once

#include "a3dTimeProvider.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper { 
	
	public ref class a3dTimeProviderBase_Mapper { 
		a3dTimeProviderBase *native;
	internal:
		a3dTimeProviderBase_Mapper(a3dTimeProviderBase *timeProv) { 
			this->native = timeProv; 
		}
	public:
		a3dTimeProviderBase_Mapper() { 
			this->native = nullptr; 
		}
		
		property float Time {
			float get() { 
				return this->native == NULL? 0.0f: this->native->fTime;
			}
			void set(float value) { 
				if(this->native == NULL) return;
				this->native->setFloat(this->native->timeProperty->getName(), value);
			}
		}	
	};
}