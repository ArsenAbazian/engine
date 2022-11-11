// classMapper.h

#pragma once

#include "psystem.h"
#include "particles.h"
#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "dependencyPropertyMapper.h"
#include "interfaces.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class particleSystemBase_Mapper : public a3dDependencyPropertyOwner_Mapper { 
		particleSystemBase			*pSystem;
		Kernel_Mapper				^pskernel;
		camera_Mapper				^currCamera;
	internal:
		property particleSystemBase* PSystem { particleSystemBase* get() { return pSystem; } }
	protected:
		virtual particleSystemBase* createPSystem() { 
			return new particleSystemBase();
		}
	public:
		particleSystemBase_Mapper() { 
			this->pSystem = this->createPSystem();
			this->Owner = PSystem;
			this->InitializeProperties();
		}

		particleSystemBase_Mapper(particleSystemBase *part) { 
			this->pSystem = part;
			this->Owner = PSystem;
			this->InitializeProperties();
		}
	
		property Kernel_Mapper^ PSysKernel { 
			Kernel_Mapper^ get() { return pskernel; } 
			void set(Kernel_Mapper ^value) { 
				pskernel = value; 
				if(PSysKernel == nullptr) return;
				PSystem->partKernel = PSysKernel->krn;
			} 
		}

		property camera_Mapper^ CurrCamera { 
			camera_Mapper^ get() { return currCamera; }
			void set(camera_Mapper^ value)  {  
				currCamera = value;
				if(CurrCamera == nullptr) return;
				PSystem->currCamera = CurrCamera->cam;
			}
		}

		property bool DoNotGenerate { 
			bool get() { return PSystem->doNotGenerateProperty->getBoolValue(); }
			void set(bool value) { 
				if(DoNotGenerate == value) return;
				PSystem->doNotGenerateProperty->setBoolValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}
	
		property bool UseLocalBasis { 
			bool get() { return PSystem->useLocalBasisProperty->getBoolValue(); }
			void set(bool value) { 
				if(UseLocalBasis == value) return;
				PSystem->useLocalBasisProperty->setBoolValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property int ParticleCount { 
			int get() { return PSystem->particleCountProperty->getIntValue(); }
			void set(int value) { 
				if(ParticleCount == value) return;
				PSystem->particleCountProperty->setIntValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		property float Time { 
			float get() { return PSystem->timeProperty->getFloatValue(); }
			void set(float value) { 
				if(Time == value) return;
				PSystem->timeProperty->setFloatValue(value);
				ObjectChanged(this, EventArgs::Empty);
			}
		}

		virtual property String^ Type { String^ get() { return gcnew String("base"); } };
	};
}