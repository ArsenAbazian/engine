#ifndef A3D_SMOKE_PARTICLE_SYSTEM_MAPPER
#define A3D_SMOKE_PARTICLE_SYSTEM_MAPPER
#include "particleSystemBaseMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class a3dSmokeParticleSystem_Mapper : public particleSystemBase_Mapper { 
	protected:
		virtual particleSystemBase* createPSystem() override { return new a3dSmokeParticleSystem(); }
	public:
		a3dSmokeParticleSystem_Mapper(a3dSmokeParticleSystem *part) : particleSystemBase_Mapper(part) { }
		a3dSmokeParticleSystem_Mapper() : particleSystemBase_Mapper() { }
		virtual property String^ Type { String^ get() override { return gcnew String("smoke"); } };
	};
}

#endif