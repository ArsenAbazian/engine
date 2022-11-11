#include "particleSystemBaseMapper.h"
#include "smokeParticleSystemMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class a3dParticleSystemDesc_Mapper : a3dObjectDescBase_Mapper, public IObjectWithBasis { 
		particleSystemBase_Mapper^ particle;
	internal:

		property a3dParticleSystemDesc* PartDesc { a3dParticleSystemDesc* get() { return dynamic_cast<a3dParticleSystemDesc*>(desc); } }
		particleSystemBase_Mapper^ createParticle() { 
			a3dSmokeParticleSystem* psSmoke = dynamic_cast<a3dSmokeParticleSystem*>(PartDesc->pSystem);
			if( psSmoke != null) return gcnew a3dSmokeParticleSystem_Mapper(psSmoke);
			return nullptr;
		}
	public:
		a3dParticleSystemDesc_Mapper( a3dParticleSystemDesc *desc ) : a3dObjectDescBase_Mapper(desc) { 
			particle = this->createParticle(); 
		}

		property String^ EffectFileName { String^ get() { return PartDesc == null? String::Empty: gcnew String(PartDesc->pSystem->effectFileName.getCString()); } }

#pragma region IObjectWithBasis
		virtual property float ScaleX { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleY { float get() { return 1.0f; } void set( float f ) { } }
		virtual property float ScaleZ { float get() { return 1.0f; } void set( float f ) { } }
		virtual property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				if(PartDesc == null) return nullptr;
				Matrix4_Mapper ^mat = gcnew Matrix4_Mapper();
				mat->FromMatrix4( PartDesc->pSystem->localBasis );
				return mat;
			} 
			void set(Matrix4_Mapper ^mat) { 
				if(PartDesc == null) return;
				mat->ToMatrix4(PartDesc->pSystem->localBasis);
				ObjectChanged(this, EventArgs::Empty);
			} 
		}
#pragma endregion IObjectWithBasis
		property particleSystemBase_Mapper^ Particle { particleSystemBase_Mapper^ get() { return particle; } }
	};
}