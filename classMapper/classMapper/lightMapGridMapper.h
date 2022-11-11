// cameraMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "stringHelper.h"
#include "cameraMapper.h"
#include "DescMapper.h"
#include "lightMapGrid.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Drawing;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	public ref class a3dLightMapGrid_Mapper : public a3dObjectWithEvent { 
		a3dLightMapGrid *grid;
		Color pointColor;
	public:
		a3dLightMapGrid_Mapper() { 
			this->grid = new a3dLightMapGrid();
		}
	
		void setKernel(Kernel_Mapper^ kernel) { this->grid->setKernel(kernel->krn); }
		void setCamera(camera_Mapper^ cam) { this->grid->setCamera(cam->cam); }
		long setLightMapCreator( a3dLightMapCreator_Mapper^ ligthMapCreator ) { 
			return this->grid->setLightMapCreator( ligthMapCreator->LightMapCreator );
		}
		property float PointSize { 
			float get() { return grid->pointSize; } 
			void set(float val) { 
				grid->pointSize = val;
				ObjectChanged(this, EventArgs::Empty);
			} 
		}

		property Color PointColor { 
			Color get() { return pointColor; }
			void set(Color c) { 
				pointColor = c;
				setVector3(grid->color, pointColor.ToArgb());
			}
		}

		long Render() { return this->grid->render(); }
	};
}