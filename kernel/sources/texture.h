/*********************************************************************
*	
*		class Texture	- texture representation
*		created by Abazyan Arsen
*		
*		Tula 02.11.2004		12:29
*		e-mail: ars_s@uic.tula.ru
*		
*		information:
*			- supported formats: BMP TGA JPG PNG DDS
*			- supports AlphaChannels addition
**********************************************************************/
#ifndef TEXTURE_INC
#define TEXTURE_INC

#include <windows.h>
#include <stdio.h>
#include "kernel.h"
#include "3dtypes.h"
#include <d3dx9tex.h>

class Texture{
	void					*lpBuffer;
	bool					bSaved;
	
	HRESULT					FillAlpha(DWORD Level,DWORD minColor,DWORD maxColor,float Alpha);
	HRESULT					FillAlpha(DWORD minColor,DWORD maxColor,float Alpha);
	
public:
	Kernel					*kernel;
	LPDIRECT3DTEXTURE9		lpTexture;
	D3DFORMAT				Format;

	DWORD					dwWidth;
	DWORD					dwHeight;
	DWORD					dwMipMap;
	DWORD					dwBpp;
	
	Texture(){
		lpBuffer    = NULL;
		bSaved      = false;
		lpTexture	= NULL;
	}
	~Texture();
	
	HRESULT	Clear();
	void    SetKernel(Kernel *krn);
	
	HRESULT	Create(DWORD pWidth, DWORD pHeight,D3DFORMAT pFormat,DWORD pMMPercent);
	//HRESULT CopyImageToSurface(Image *img, bool inv);
	HRESULT ConvertTextureFormat(D3DXIMAGE_INFO *info, D3DFORMAT pFormat);

	HRESULT LoadFromFile(TCHAR *file,D3DFORMAT pFormat,DWORD pMMPercent);
	HRESULT loadFromFile( shString &fileName );
	HRESULT	loadFromFile( shString &fileName, D3DFORMAT format ); 
	HRESULT loadFromFile( shString &fileName, bool useFileFormat );

	HRESULT AlphaChannel(DWORD minColor,DWORD maxColor,float Alpha);
	void	GenerateMipSubLevels(D3DTEXTUREFILTERTYPE fType);
		
};

#endif