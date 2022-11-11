// kernelMapper.h

#pragma once

#include "vecMatMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public enum class RenderFillType { Default, Point, Wireframe, Solid };
	
	public ref class Kernel_Mapper { 
	internal:		
		Kernel *krn;
		Color clearColor;
		RenderFillType fillType;
	public:
		Kernel_Mapper() { 
			krn = new Kernel();
			clearColor = Color::FromArgb( 0xff808080 );
			fillType = RenderFillType::Default;
		}
		
		int Init( long hwnd, TCHAR *cfgfile ) { 
			return krn->Init( (HWND)hwnd, cfgfile );
		}

		void BeginRender() { 
			krn->lpDevice->BeginScene();
			krn->lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor.ToArgb(), 1.0f, 0 );
			if( FillType == RenderFillType::Default )
				krn->lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			if( FillType == RenderFillType::Point )
				krn->lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			else if(FillType == RenderFillType::Wireframe )
				krn->lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			else if(FillType == RenderFillType::Solid )
				krn->lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		void EndRender() {
			krn->lpDevice->EndScene();
			krn->lpDevice->Present( NULL, NULL, NULL, NULL );
		}

		property Color ClearColor {
			Color get() { return clearColor; }
			void set(Color value) { 
				clearColor = value;
			}
		}

		property RenderFillType FillType { 
			RenderFillType get() { return fillType; }
			void set( RenderFillType ft ) { 
				fillType = ft;
			}
		}
		property bool IsReady { bool get() { return krn != null && krn->lpDevice != null; } }
	};


}