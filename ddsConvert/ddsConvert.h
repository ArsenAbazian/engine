// ddsConvert.h

#pragma once

#include <nvcore/StdStream.h>

#include <nvimage/Image.h>
#include <nvimage/DirectDrawSurface.h>

#include <nvimage/ImageIO.h>

using namespace System;

namespace ddsConvert {

	class PathContainer { 
	public:
		char inFile[1024];
		char outFile[1024];
	};

	public ref class ddsConverter {
		PathContainer *pc;
		//nvDecompressor	*dc;

		void createFileName(System::String ^src, char *dest) { 
			int i;
			for(i = 0; i < src->Length; i++) { 
				dest[i] = src[i];
			}
			dest[i] = 0;
			return;
		}
	public:
		static ddsConverter ^Default;
		
		static ddsConverter() { 
			Default = gcnew ddsConverter();
		}

		ddsConverter() { 
			this->pc = new PathContainer();
			//this->dc = new nvDecompressor();
		}
		~ddsConverter() { 
			delete this->pc;
		}
		bool convert(System::String ^inputFile, System::String ^outputFile) { 
			
			createFileName(inputFile, this->pc->inFile);
			createFileName(outputFile, this->pc->outFile);

			nv::DirectDrawSurface dds(this->pc->inFile);
			if (!dds.isValid())
				return false;
				
			nv::Image img;
			dds.mipmap(&img, 0, 0); // get first image
			nv::ImageIO::saveBMP(this->pc->outFile, &img);
			return true;
		}
	};
}
