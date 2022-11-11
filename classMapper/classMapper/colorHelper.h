#pragma once

#include "vecmat.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {

	class colorHelper {
	public:
		static Color VectorRGBToColor(Vector3 vector) { 
			return Color::FromArgb(255, (int)(vector[0] * 255), (int)(vector[1] * 255), (int)(vector[2] * 255));
		}

		static Color VectorARGBToColor(Vector4 vector) { 
			return Color::FromArgb((int)(vector[0] * 255), (int)(vector[1] * 255), (int)(vector[2] * 255), (int)(vector[3] * 255));
		}

		static Color VectorRGBAToColor(Vector4 vector) { 
			return Color::FromArgb((int)(vector[3] * 255), (int)(vector[0] * 255), (int)(vector[1] * 255), (int)(vector[2] * 255));
		}

		static void ColorToRGB(Vector3 vector, Color c) { 
			vector[0] = c.R / 255.0f;
			vector[1] = c.G / 255.0f;
			vector[2] = c.B / 255.0f;
		}

		static void ColorToARGB(Vector4 vector, Color c) { 
			vector[0] = c.A / 255.0f;
			vector[1] = c.R / 255.0f;
			vector[2] = c.G / 255.0f;
			vector[3] = c.B / 255.0f;
		}

		static void ColorToRGBA(Vector4 vector, Color c) { 
			vector[3] = c.A / 255.0f;
			vector[0] = c.R / 255.0f;
			vector[1] = c.G / 255.0f;
			vector[2] = c.B / 255.0f;
		}
	}; 	
}