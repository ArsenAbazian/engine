/*********************************************************************
*	
*		class AImage	- simple image (bmp 24 bpp) class
*		created by Abazyan Arsen
*		
*		Tula 17.03.2004		22:11
*		e-mail: ars_s@uic.tula.ru
*		
*		information:
*			- support only 24 bit BMP images
*			- support transparent color
**********************************************************************/

#ifndef A_IMAGE_INC
#define A_IMAGE_INC

#include <windows.h>

#define A_get_bit(value,l) (value[l>>3] & (1 << (7-(l&7))))
#define A_set_bit(value,l) (value[l>>3] |= (1 << (7-(l&7))))
#define A_clear_bit(value,l) (value[l>>3] &= ~(1 << (7-(l&7))))

typedef struct{
	BYTE		r;	// red value
	BYTE		g;	// green value
	BYTE		b;	// blue value
}pixel;

typedef struct{
	BITMAPINFOHEADER	bmiHeader;
	RGBQUAD				bmiColors[2];
}A_BITMAPINFO1_24BIT;

typedef enum{
	A_IT_24BIT	= 0,
	A_IT_1BIT	= 1
}A_IMAGE_TYPE;

class Image{

	A_IMAGE_TYPE	ImageType;
	
	union{
		HBITMAP		hImage;		
		HBITMAP		hMask;
	};
	DWORD			dwWidth;
	DWORD			dwHeight;

public:

	union {
		pixel*		lpImage;			// pointer to DIB section
		BYTE*		lpMask;
	};
	union{
		pixel**		lpMap;				// for fast access to image pixels 		
		BYTE**		lpMaskMap;
	};
	
	Image			*ColorMask;			// mask image
	HWND			hScreen;			// screen window
	
	HDC				hRenderDevice;	 
	HDC				hMemoryDevice;

	HDC				hTmpMaskPositive;
	HDC				hTmpMaskNegative;
	HDC				hTmpImage;
	HDC				hTmpOriginalCopy;
		
	DWORD			dwPosX0;
	DWORD			dwPosY0;
	DWORD			dwPosZ0;
	
	DWORD			dwImagePosX0;
	DWORD			dwImagePosY0;

	DWORD			dwRenderWidth;
	DWORD			dwRenderHeight;
	
	float			fScaleX;
	float			fScaleY;
	
	bool			bTransparent;
	
	Image() { ZeroMemory(this, sizeof(Image)); }
	Image(HWND hwnd, DWORD width, DWORD height, A_IMAGE_TYPE im_type, bool is_transparent);
	~Image();
	
	bool			Create(HWND hwnd, DWORD width, DWORD height, A_IMAGE_TYPE im_type, bool is_transparent);
	bool			Clear(DWORD color);						// clear an image
	void			Destroy();								// destroy an image
	void			SetTransparentColor(DWORD color);		// set transparent color
	void			AddTransparentColor(DWORD color);		// add new transparent color
	void			AddTransparentColorRange(DWORD color_low, DWORD color_high);
	void			SetTransparentColorRange(DWORD color_low, DWORD color_high);
	bool			IsTransparentColor(DWORD x, DWORD y, pixel *TransColor);	// check, is color in pixel x,y  transparent ?
	bool			IsInTransparentColorRange(DWORD x, DWORD y, pixel *TransColorLow, pixel *TransColorHigh);	// check, is color in pixel x,y  transparent ?
	
	bool			LoadResBMP(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, bool is_transparent);
	bool			LoadResBMP(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, TCHAR *mask_file);
	bool			SaveResBMP(TCHAR *filename);
	bool			SaveResBMP(TCHAR *filename, TCHAR *maskname);
	
	/*
	bool			LoadResJPG(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, bool is_transparent);
	bool			LoadResJPG(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, TCHAR *mask_file);
	bool			SaveResJPG(TCHAR *filename);
	bool			SaveResJPG(TCHAR *filename, TCHAR *maskname);

	bool			LoadResPNG(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, bool is_transparent);
	bool			LoadResPNG(HWND hWnd, A_IMAGE_TYPE im_type, 
								TCHAR *filename, TCHAR *mask_file);
	bool			SaveResPNG(TCHAR *filename);
	bool			SaveResPNG(TCHAR *filename, TCHAR *maskname);
	*/
	
	bool			LoadImage(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, bool is_transparent);
	bool			LoadImage(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, TCHAR *maskfile);

	void			Render();								// render image :)))
	HBITMAP			SetToDC(HDC hdc){ return (HBITMAP)SelectObject(hdc, hImage); };
	DWORD			GetWidth() { return dwWidth; }
	DWORD			GetHeight() { return dwHeight; }
	A_IMAGE_TYPE	GetType(){ return ImageType; }

	bool			HitTest(int x, int y);
	bool			IsTransparentPos(int x, int y);
	BYTE*			GetData(){ return (BYTE*)lpImage; }

};

#endif