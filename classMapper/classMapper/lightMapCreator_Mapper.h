#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "cameraMapper.h"
#include "DescMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class a3dLightMapCreator_Mapper {
		a3dLightMapCreator *lightMapCreator;
	public:
		a3dLightMapCreator_Mapper(a3dLightMapCreator *creator) { 
			this->lightMapCreator = creator;
		}

	internal:
		property a3dLightMapCreator* LightMapCreator { a3dLightMapCreator* get() { return this->lightMapCreator; } }
	};

	public interface class ILightMapCreatorProvider { 
		property a3dLightMapCreator_Mapper^ LightMapCreator { a3dLightMapCreator_Mapper^ get(); }
	};

}