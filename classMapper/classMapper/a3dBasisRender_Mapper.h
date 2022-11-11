// cameraMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "cameraMapper.h"
#include "DescMapper.h"
#include "lightMapGrid.h"
#include "renderHelper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class a3dBasisRender_Mapper { 
		a3dBasisRender *render;
		Matrix4_Mapper ^basis;
	internal:
		property a3dBasisRender* BasisRender { a3dBasisRender* get() { return this->render; } };
	public:
		a3dBasisRender_Mapper() { 
			this->render = new a3dBasisRender();
		} 

		void SetKernel( Kernel_Mapper ^krn ) { BasisRender->setKernel( krn->krn ); }
		void SetCamera( camera_Mapper ^cam ) { BasisRender->setCamera( cam->cam ); }
		long Initialize() { return BasisRender->initialize(); }
		long Render() { return BasisRender->render(); }

		property Matrix4_Mapper^ Basis { 
			Matrix4_Mapper^ get() { 
				if(BasisRender->model == null ) return basis;
				if(basis == nullptr) return nullptr;
				basis->FromMatrix4(BasisRender->model->basis);
				return basis;
			} 
			void set(Matrix4_Mapper^ bas) { 
				basis = bas;
				if(BasisRender->model == null ) return;
				if(basis == nullptr) return;
				Matrix4 mat;
				basis->ToMatrix4(mat);
				BasisRender->update(mat);
			}
		}
	};
}