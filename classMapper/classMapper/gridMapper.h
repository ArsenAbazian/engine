// gridMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"
#include "cameraMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class a3dGrid_Mapper { 
	internal:
		a3dGrid	*grid;
		Color axisColor;
		Color lineColor;
		Color boldLineColor;

	public:
		a3dGrid_Mapper() { 
			this->grid = new a3dGrid();
		}
	
		void setKernel( Kernel_Mapper^ kernel ) { 
			this->grid->setKernel( kernel->krn );
		}

		void setCamera( camera_Mapper^ camera ) {
			this->grid->setCamera( camera->cam );
		}

		property float SideSize { 
			float get() { return this->grid->sideSize; }
			void set(float value) { 
				this->grid->sideSize = value; 
				this->initialize( this->grid->sideSize, this->grid->smallStep, this->grid->boldLineSteps );
			}
		}
		
		property float SmallStep { 
			float get() { return this->grid->smallStep; }
			void set(float value) { 
				this->grid->smallStep = value; 
				this->initialize( this->grid->sideSize, this->grid->smallStep, this->grid->boldLineSteps );
			}
		}

		property int BoldLineSteps { 
			int get() { return this->grid->boldLineSteps; }
			void set(int value) { 
				this->grid->boldLineSteps = value; 
				this->initialize( this->grid->sideSize, this->grid->smallStep, this->grid->boldLineSteps );
			}
		}

		property Color AxisColor { 
			Color get() { return axisColor; }
			void set( Color color ) { 
				axisColor = color;
				setVector3( this->grid->axisColor, axisColor.R / 255.0f, axisColor.G / 255.0f, axisColor.B / 255.0f );
			}
		}

		property Color LineColor { 
			Color get() { return lineColor; }
			void set( Color color ) { 
				lineColor = color;
				setVector3(this->grid->lineColor, lineColor.R / 255.0f, lineColor.G / 255.0f, lineColor.B / 255.0f );
			}
		}

		property Color BoldLineColor { 
			Color get() { return boldLineColor; }
			void set( Color color ) { 
				boldLineColor = color;
				setVector3(this->grid->boldLineColor, boldLineColor.R / 255.0f, boldLineColor.G / 255.0f, boldLineColor.B / 255.0f );
			}
		}
		
		long initialize( float sideSize, float smallStep, int boldLineStep ) { 
			return this->grid->initialize( sideSize, smallStep, boldLineStep );
		}
	
		long render() { 
			return this->grid->render();
		}

	};

}