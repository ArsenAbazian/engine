// boundBoxMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"
#include "gridMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class a3dBoundBox_Mapper { 
		a3dBoundBox *bBox;

		a3dBoundBox_Mapper() { 
			this->bBox = new a3dBoundBox();
		}

		void clear() { this->bBox->clear(); }
		
		void getVertex( Vector3_Mapper ^pos, Vector3_Mapper ^ v ) { 
			Vector3 v1, v2;
			pos->ToVector3( v1 );
			v->ToVector3( v2 );
			this->bBox->getVertex( v1, v2 );
		}
		void getVertex0( Vector3_Mapper ^ v ) { 
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex0( v1 );  
		}
		void getVertex1( Vector3_Mapper ^ v ) { 
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex1( v1 );  
		}
		void getVertex2( Vector3_Mapper ^ v ) {
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex2( v1 );  
		}
		void getVertex3( Vector3_Mapper ^ v ) {
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex3( v1 );  
		}
		void getVertex4( Vector3_Mapper ^ v ) {
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex4( v1 );  
		}
		void getVertex5( Vector3_Mapper ^ v ) {
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex5( v1 );  
		}
		void getVertex6( Vector3_Mapper ^ v ) { 
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex6( v1 );  
		}
		void getVertex7( Vector3_Mapper ^ v ) {
			Vector3 v1;
			v->ToVector3( v1 );
			this->bBox->getVertex7( v1 );  
		}

		
		property float Width { float get() {  return this->bBox->getWidth(); } }
		property float Height { float get() {  return this->bBox->getHeight(); } }
		property float Depth { float get() {  return this->bBox->getDepth(); } }
	};
}