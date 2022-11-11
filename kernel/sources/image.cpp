#include "image.h"
//#include "Jpeg.h"
//#include "m_png.h"
#include <stdio.h>
#include "hstring.h"
#include <fstream>



// creates an 24  or 1 bit bitmap image in memory
bool CreateImage(HWND hWnd,HBITMAP* lphImage,void** lpPixel,DWORD x,DWORD y, DWORD bitcount){
	
	HDC hdc,				hcdc;
	A_BITMAPINFO1_24BIT		binf;
	DWORD					bitmap_type = DIB_RGB_COLORS;	
		
	// create an 24 bpp Bitmap
	binf.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	binf.bmiHeader.biWidth         = x;
	binf.bmiHeader.biHeight        = -1*y;
	binf.bmiHeader.biPlanes        = 1;
	binf.bmiHeader.biBitCount      = (WORD)bitcount;
	binf.bmiHeader.biCompression   = BI_RGB;
	binf.bmiHeader.biSizeImage     = 0;
	binf.bmiHeader.biXPelsPerMeter = 0;
	binf.bmiHeader.biYPelsPerMeter = 0;
	binf.bmiHeader.biClrImportant  = 0;
	if(bitcount == 1){
		binf.bmiHeader.biClrUsed		= 1<<bitcount;
				
		binf.bmiColors[0].rgbRed			= 0x00;
		binf.bmiColors[0].rgbGreen			= 0x00;
		binf.bmiColors[0].rgbBlue			= 0x00;
		binf.bmiColors[0].rgbReserved		= 0x00f;
	
		binf.bmiColors[1].rgbRed			= 0xff;
		binf.bmiColors[1].rgbGreen			= 0xff;
		binf.bmiColors[1].rgbBlue			= 0xff;
		binf.bmiColors[1].rgbReserved		= 0xff;	
		
		bitmap_type = DIB_PAL_COLORS;
	}

	
	hdc  = GetDC(hWnd);
	hcdc = CreateCompatibleDC(hdc);
	ReleaseDC(hWnd, hdc);

	// create DIB section in memory
	*lphImage = CreateDIBSection(hcdc,
		(BITMAPINFO*)&binf,
		DIB_RGB_COLORS,
		(void**)lpPixel,
		NULL,
		NULL);
			
	DeleteDC(hcdc);
	if(*lphImage==NULL)return false;
	return true;
}

Image::Image(HWND hwnd, DWORD width, DWORD height, A_IMAGE_TYPE im_type, bool is_transparent){

	// initialize params 
	ZeroMemory(this, sizeof(Image));
	
	Create(hwnd, width, height, im_type, is_transparent);

}

bool Image::Create(HWND hwnd, DWORD width, DWORD height, A_IMAGE_TYPE im_type, bool is_transparent){
	
	DWORD bytes;
	BYTE *tmp;

	Destroy();	
	
	// initialize params 
	ZeroMemory(this, sizeof(Image));
	
	hScreen		 = hwnd;
	dwWidth		 = width;
	dwHeight	 = height;
	ImageType    = im_type; 
	bTransparent = is_transparent;
	
	// create empty image
	if(im_type == A_IT_24BIT){
		CreateImage(hScreen,&hImage,(void**)&lpImage,width,height, 24);
	
		lpMap = (pixel**)new pixel*[height];

		// now we must initialize 2d array of pixels
		if(lpMap != NULL){
			// bitmap image is 4 byte align 
			// so we must calculate delta bytes for row initializing
			bytes = width*3;
			if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
			tmp = (BYTE*)lpImage;

			// save each bitmap row address in lpMap array
			for(DWORD i=0;i<height;i++){

				lpMap[i] = (pixel*)tmp;
				tmp+= bytes;

			}
		}
	
		if(is_transparent){
			ColorMask = new Image(hwnd, width, height, A_IT_1BIT, false);
		}
	}
	
	else if(im_type == A_IT_1BIT){
		CreateImage(hScreen,&hMask,(void**)&lpMask,width,height, 1);
		
		lpMaskMap = (BYTE**)new BYTE*[height];

		if(lpMaskMap != NULL){
			// bitmap image is 4 byte align 
			// so we must calculate delta bytes for row initializing
			bytes = width >> 3;
			if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
			tmp = (BYTE*)lpMask;

			// save each bitmap row address in lpMap array
			for(DWORD i=0;i<height;i++){

				lpMaskMap[i] = (BYTE*)tmp;
				tmp+= bytes;

			}
		}
	}
	else
		return false;

	return true;
}

Image::~Image(){
	
	Destroy();

}

void Image::Destroy(){
	
	// delete existing image
	if(hImage != NULL)DeleteObject(hImage);
	hImage = NULL;

	if(lpMap != NULL)delete[] lpMap;
	lpMap = NULL;

	if(ColorMask)delete ColorMask;

}

bool Image::Clear(DWORD color){
	
	DWORD i,j;
	if(lpMap==NULL)return false;
	
	pixel pix;
	
	if(ImageType == A_IT_24BIT){
	
		// convert dword color value to pixel
		pix.r = (char)(color>>16);
		pix.g = (char)(color>>8);
		pix.b = (char)color;

		// fill image with pixel colors
		for(i=0;i<dwHeight;i++){
			for(j=0;j<dwWidth;j++){
			
				lpMap[i][j].r=pix.r;
				lpMap[i][j].g=pix.g;
				lpMap[i][j].b=pix.b;

			}
		}
	}
	else if(ImageType == A_IT_1BIT){
		memset(lpMask, 0xff ,(dwWidth >> 3)* dwHeight); // set transparency to 1
	}	
	
	return true;
}

void Image::SetTransparentColor(DWORD color){
	
	// convert dword color value to pixel
	pixel	TransparentColor;

	TransparentColor.r = (char)(color>>16);
	TransparentColor.g = (char)(color>>8);
	TransparentColor.b = (char)color;

	// now if it transparent
	DWORD i,j;
	
	if(ImageType == A_IT_24BIT && ColorMask){
		for(i=0;i < dwHeight;i++){
			for(j=0;j<dwWidth;j++){
				if(IsTransparentColor(i,j,&TransparentColor)){
					A_clear_bit(ColorMask->lpMaskMap[i],j);
				}
				else {
					A_set_bit(ColorMask->lpMaskMap[i],j);
				}
			}
		}	
	}
}


void Image::AddTransparentColor(DWORD color){
	
	// convert dword color value to pixel
	pixel	TransparentColor;
	
	TransparentColor.r = (char)(color>>16);
	TransparentColor.g = (char)(color>>8);
	TransparentColor.b = (char)color;

	// now if it transparent
	DWORD i,j;
	
	if(ImageType == A_IT_24BIT && ColorMask){
		for(i=0;i<dwHeight;i++){
			for(j=0;j<dwWidth;j++){
				if(IsTransparentColor(i,j,&TransparentColor)){
					A_clear_bit(ColorMask->lpMaskMap[i],j);
				}
				else {
					// do nothing to save old transparent color
				}
			}
		}	
	}
}

void Image::SetTransparentColorRange(DWORD color_low, DWORD color_high){
	
	// convert dword color value to pixel
	pixel	TransparentColorLow;
	pixel	TransparentColorHigh;
	
	TransparentColorLow.r = (char)(color_low>>16);
	TransparentColorLow.g = (char)(color_low>>8);
	TransparentColorLow.b = (char)color_low;

	TransparentColorHigh.r = (char)(color_high>>16);
	TransparentColorHigh.g = (char)(color_high>>8);
	TransparentColorHigh.b = (char)color_high;

	// now if it transparent
	DWORD i,j;
	
	if(ImageType == A_IT_24BIT && ColorMask){
		for(i=0;i<dwHeight;i++){
			for(j=0;j<dwWidth;j++){
				if(IsInTransparentColorRange(i,j,&TransparentColorLow,&TransparentColorHigh)){
					A_clear_bit(ColorMask->lpMaskMap[i],j);
				}
				else {
					A_set_bit(ColorMask->lpMaskMap[i],j);				
				}
			}
		}	
	}
}

void Image::AddTransparentColorRange(DWORD color_low, DWORD color_high){
	
	// convert dword color value to pixel
	pixel	TransparentColorLow;
	pixel	TransparentColorHigh;
	
	TransparentColorLow.r = (char)(color_low>>16);
	TransparentColorLow.g = (char)(color_low>>8);
	TransparentColorLow.b = (char)color_low;

	TransparentColorHigh.r = (char)(color_high>>16);
	TransparentColorHigh.g = (char)(color_high>>8);
	TransparentColorHigh.b = (char)color_high;

	// now if it transparent
	DWORD i,j;
	
	if(ImageType == A_IT_24BIT && ColorMask){
		for(i=0;i<dwHeight;i++){
			for(j=0;j<dwWidth;j++){
				if(IsInTransparentColorRange(i,j,&TransparentColorLow,&TransparentColorHigh)){
					A_clear_bit(ColorMask->lpMaskMap[i],j);
				}
				else {
					// do nothing to save old transparent color
				}
			}
		}	
	}
}

bool Image::IsTransparentColor(DWORD x, DWORD y, pixel *TransparentColor){

	// convert dword color value to pixel
	return ( TransparentColor->r == lpMap[x][y].r &&
				TransparentColor->g == lpMap[x][y].g &&
				TransparentColor->b == lpMap[x][y].b) ; 
}

bool Image::IsInTransparentColorRange(DWORD x, DWORD y, pixel *TransparentColorLow, pixel *TransparentColorHigh){

	// convert dword color value to pixel
	return (( TransparentColorLow->r <= lpMap[x][y].r &&
				TransparentColorLow->g <= lpMap[x][y].g &&
				TransparentColorLow->b <= lpMap[x][y].b)
				&&
			 ( TransparentColorHigh->r >= lpMap[x][y].r &&
				TransparentColorHigh->g >= lpMap[x][y].g &&
				TransparentColorHigh->b >= lpMap[x][y].b)
			) ; 
}


bool Image::LoadImage(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, bool is_transparent){

	
	
	FILE *fp;
	_tfopen_s(&fp, filename, TEXT("r"));
	if(fp == NULL)return false;

	BYTE buf[4];
	fread(buf, 1, 4, fp);
	fclose(fp);
		
	if(buf[0] == 'B' && buf[1] == 'M'){ // загрузка из BMP файла
		return LoadResBMP(hWnd, im_type, filename, is_transparent);
	}
	else if(buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G'){ // загрузка из PNG файла
		return false;
		//return LoadResPNG(hWnd, im_type, filename, is_transparent);
	}
	else if(buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff){ // загрузка из JPG файла
		return false;
		//return LoadResJPG(hWnd, im_type, filename, is_transparent);
	}
 
	return false;
}

bool Image::LoadImage(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, TCHAR *maskfile){

	FILE *fp;
	_tfopen_s(&fp, filename, TEXT("r"));
	if(fp == NULL)return false;

	char buf[4];
	fread(buf, 1, 4, fp);
	fclose(fp);
	
	if(buf[0] == 'B' && buf[1] == 'M'){ // загрузка из BMP файла
		return LoadResBMP(hWnd, im_type, filename, maskfile);
	}
	else if(buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G'){ // загрузка из PNG файла
		return false;
		//return LoadResPNG(hWnd, im_type, filename, maskfile);
	}
	else if(buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff){ // загрузка из JPG файла
		return false;
		//return LoadResJPG(hWnd, im_type, filename, maskfile);
	}

	return false;

}

/*
bool Image::LoadResJPG(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, bool is_transparent){

	JPEG	jpg;
	char	cf[1024];
	TCHAR	buf[1024];
	DWORD	i, j;

	WCSToStr(filename, cf);
	
	jpg.openRFile(cf);
	if(jpg.data->status){
		_stprintf(buf,TEXT("Error : unable to open file %s!"),filename);
		MessageBox(hWnd,buf,TEXT("Texture"),MB_OK);
		return false; 
	}
	jpg.load();
	
	// инициализация картинки
	im_type = A_IT_24BIT;

	Destroy();
	
	if(im_type == A_IT_24BIT){
	
		// create new image
		if(!CreateImage(hScreen,&hImage,(void**)&lpImage,jpg.data->width ,jpg.data->height , 24)){
			jpg.freeMem();
			jpg.closeRFile();
			return false;
		}
	}

	dwWidth = jpg.data->width;
	dwHeight = jpg.data->height;

	// create 2d array for fast image pixel access
 	lpMap = (pixel**)new pixel*[dwHeight];

	if(lpMap==NULL){
		jpg.freeMem();
		jpg.closeRFile();
		return false;
	}

	// image row is always align to 4 bytes	
	// so we must calculate actual row size in bytes
	
	pixel *tmp = lpImage;
	BYTE  *btmp = (BYTE*)lpImage;	

	// initialize each row
	for(i=0;i<dwHeight;i++){
		lpMap[i] = tmp;
		tmp+= dwWidth;
	}

	tmp = (pixel*)jpg.data->buf;
	
	if(jpg.data->bpp == 3){
		for(i=0;i<dwHeight;i++){
			memcpy(lpMap[i], tmp, dwWidth * sizeof(pixel));
			tmp += dwWidth;
		}
	}
	else if(jpg.data->bpp == 1){
		for(i=0;i<dwHeight;i++){
			for(j=0;j<dwWidth; j++){
				lpMap[i][j].r = btmp[j];
				lpMap[i][j].g = btmp[j];
				lpMap[i][j].b = btmp[j];
			}
			btmp += dwWidth;
		}
	}

	if(is_transparent){
		ColorMask = new Image(hWnd, dwWidth, dwHeight,A_IT_1BIT,false);
	}
	
	jpg.freeMem();
	jpg.closeRFile();
	return true;
}

bool Image::LoadResJPG(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, TCHAR *mask_file){
	
	if(!LoadResJPG(hWnd,im_type,filename,true))return false;
	
	return ColorMask->LoadResBMP(hWnd,A_IT_1BIT,mask_file,false);	
					   
}

bool Image::SaveResJPG(TCHAR *filename){

	JPEG	jpg;
	char	cf[1024];
	DWORD	i;
	pixel	*pix;
	
	if(this->ImageType != A_IT_24BIT)
		return false;
	
	WCSToStr(filename, cf);

	jpg.data->width  = dwWidth;
	jpg.data->height = dwHeight;
	jpg.data->bpp    = 3;
	jpg.allocateMem();
		
	pix = (pixel*)jpg.data->buf;
	for(i=0; i< dwHeight; i++){
		memcpy(pix, lpMap[i], sizeof(pixel) * dwWidth);
		pix += dwWidth; 
	}

	jpg.openWFile(cf);
			
	jpg.data->smoothingfactor = 0;
	jpg.data->quality         = 100;
	jpg.data->aritcoding	  = HUFFMAN_CODING;
	jpg.data->CCIR601sampling = false; 

	jpg.save();
	jpg.closeWFile();
	jpg.freeMem();

	return true;
}


bool Image::SaveResJPG(TCHAR *filename, TCHAR *maskfile){

	if(!SaveResJPG(filename))return false;
	if(ColorMask && !ColorMask->SaveResBMP(maskfile))return false;

	return true;
}

bool Image::LoadResPNG(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, bool is_transparent){

	PNG		*png = new PNG;
	char	cf[1024];
	TCHAR	buf[1024];
	DWORD	i, j;
		
	WCSToStr(filename, cf);
		
	png->openRFile(cf);
	if(png->data->status){
		_stprintf(buf,TEXT("Error : unable to open file %s!"),filename);
		MessageBox(hWnd,buf,TEXT("Image"),MB_OK);
		delete png;
		return false; 
	}
	png->load();
	
	// инициализация картинки
	im_type = A_IT_24BIT;

	Destroy();
	
	if(im_type == A_IT_24BIT){
	
		// create new image
		if(!CreateImage(hScreen,&hImage,(void**)&lpImage,png->data->width ,png->data->height , 24)){
			png->freeMem();
			png->closeRFile();
			delete png;
			return false;
		}
	}
	
	dwWidth = png->data->width;
	dwHeight = png->data->height;
	
	// create 2d array for fast image pixel access
 	lpMap = (pixel**)new pixel*[dwHeight];

	if(lpMap==NULL){
		png->freeMem();
		png->closeRFile();
		delete png;
		return false;
	}

	// image row is always align to 4 bytes	
	// so we must calculate actual row size in bytes
	
	pixel *tmp = lpImage;

	// initialize each row
	for(i=0;i<dwHeight;i++){
		lpMap[i] = tmp;
		tmp += dwWidth;
	}

	tmp			= (pixel*)png->data->buf;
	BYTE  *btmp = (BYTE*)png->data->buf;
    DWORD *dtmp	= (DWORD*)png->data->buf;	
	
	if(png->data->bpp == 24){
		for(i=0;i<dwHeight;i++){
			memcpy(lpMap[i], tmp, dwWidth * sizeof(pixel));
			tmp += dwWidth;
		}
	}
	else if(png->data->bpp == 8){
		for(i=0;i<dwHeight;i++){
			for(j=0; j< dwWidth; j++){
				lpMap[i][j].r = btmp[j];
				lpMap[i][j].g = btmp[j];
				lpMap[i][j].b = btmp[j];
			}
			btmp += dwWidth;
		}
	}
	else if(png->data->bpp == 32){
		for(i=0;i<dwHeight;i++){
			for(j=0; j< dwWidth; j++){
				lpMap[i][j].r = (BYTE)(dtmp[j] & 0x00ff0000) >> 16;
				lpMap[i][j].g = (BYTE)(dtmp[j] & 0x0000ff00) >> 8;
				lpMap[i][j].b = (BYTE)(dtmp[j] & 0x000000ff);
			}
			dtmp += dwWidth;
		}
	}
	
	if(is_transparent){
		ColorMask = new Image(hWnd, dwWidth, dwHeight,A_IT_1BIT,false);
	}
	
	png->freeMem();
	png->closeRFile();
	
	delete png;
	return true;
}

bool Image::LoadResPNG(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR *filename, TCHAR *mask_file){

	if(!LoadResPNG(hWnd,im_type,filename,true))return false;
	return ColorMask->LoadResBMP(hWnd,A_IT_1BIT,mask_file,false);	

}


bool Image::SaveResPNG(TCHAR *filename){
	
	PNG		png;
	char	cf[1024];
	TCHAR	buf[1024];
	DWORD	i;
	pixel	*pix;
	
	if(this->ImageType != A_IT_24BIT)
		return false;
	
	WCSToStr(filename, cf);

	png.data->width = dwWidth;
	png.data->height = dwHeight;
	png.allocateMem();
	
	if(png.data->status)
		return false;
	
	pix = (pixel*)png.data->buf;
	for(i=0; i< dwHeight; i++){
		memcpy(pix, lpMap[i], sizeof(pixel) * dwWidth);
		pix += dwWidth; 
	}

	png.data->aspect = 1.0f;
	png.data->bpp	 = 32;
	png.data->gamma  = 1.0f;
		
	png.openWFile(cf);
	
	
	png.closeWFile();
	png.freeMem();

	return true;
}


bool Image::SaveResPNG(TCHAR *filename, TCHAR *maskfile){

	if(!SaveResPNG(filename))return false;
	if(ColorMask && !ColorMask->SaveResBMP(maskfile))return false;

	return true;
}
*/

bool Image::LoadResBMP(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR* file, bool is_transparent){
	
	BITMAPFILEHEADER	bfh;
	BITMAPINFOHEADER	bih;
	FILE				*fp;
	DWORD				bytes,b_read,i;
	BYTE				*tmp;

	// initializing parameters and open file
 	hScreen		= hWnd;
	ImageType	= im_type;	
	bTransparent = is_transparent;
	
	 _tfopen_s(&fp, file,TEXT("rb"));

	if(fp == NULL)return false;

	// read bitmap header
	b_read = (DWORD)fread(&bfh,1,sizeof(BITMAPFILEHEADER),fp);
	if(b_read != sizeof(BITMAPFILEHEADER)){
		fclose(fp);	
		return false;
	}
	
	// check image type
	if(bfh.bfType!=0x4d42){
		fclose(fp);
		return false;
	}
		
	// read image info
	b_read = (DWORD)fread(&bih,1,sizeof(BITMAPINFOHEADER),fp);
	// if can't read
	if(b_read != sizeof(BITMAPINFOHEADER)){
		fclose(fp);
		return false;
	}

	// check bitmap image pixel depth
	if((im_type == A_IT_24BIT && bih.biBitCount!=24) &&
		(im_type == A_IT_1BIT && bih.biBitCount!=1)){
		fclose(fp);
		return false;
	}
		
	Destroy();
	
	if(im_type == A_IT_24BIT){
	
		// create new image
		if(!CreateImage(hScreen,&hImage,(void**)&lpImage,bih.biWidth,bih.biHeight, 24)){
			fclose(fp);
			return false;
		}
	}
	else if(im_type == A_IT_1BIT){
		// create new image
		if(!CreateImage(hScreen,&hMask,(void**)&lpMask,bih.biWidth,bih.biHeight, 1)){
			fclose(fp);
			return false;
		}
	}
	
	dwWidth  = bih.biWidth;
	dwHeight = bih.biHeight;

	if(im_type == A_IT_24BIT){
		// create 2d array for fast image pixel access
 		lpMap = (pixel**)new pixel*[dwHeight];

		if(lpMap==NULL){
			fclose(fp);
			return false;
		}

		// image row is always align to 4 bytes	
		// so we must calculate actual row size in bytes

		bytes = dwWidth*3;
		if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
		tmp = (BYTE*)lpImage;

		// initialize each row
		for(i=0;i<dwHeight;i++){
			lpMap[i] = (pixel*)tmp;
			tmp+= bytes;
		}

		fseek(fp, bfh.bfOffBits, SEEK_SET);

		for(i=0;i<dwHeight;i++){
			b_read = (DWORD)fread(lpMap[dwHeight-i-1],1,bytes,fp);
			if(b_read != bytes){
				fclose(fp);
				return false;
			}
		}
	
		if(is_transparent){
			ColorMask = new Image(hWnd, dwWidth, dwHeight,A_IT_1BIT,false);
		}
	}
	else if(im_type == A_IT_1BIT){
		
		// create 2d array for fast mask bit access
		lpMaskMap = (BYTE**)new BYTE*[dwHeight];

		if(lpMaskMap == NULL){
			fclose(fp);
			return false;
		}	

		bytes = dwWidth >> 3;
		if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
		tmp = (BYTE*)lpMask;

		// initialize each row
		for(i=0;i<dwHeight;i++){
			lpMaskMap[i] = (BYTE*)tmp;
			tmp+= bytes;
		}

		// load image data
		fseek(fp, bfh.bfOffBits, SEEK_SET);

		for(i=0;i<dwHeight;i++){
			b_read = (DWORD)fread((void*)(lpMaskMap[dwHeight - 1 - i]),1,bytes,fp);
			if(b_read != bytes){
				fclose(fp);
				return false;
			}
		}
	}
		
	fclose(fp);
	return true;
}

bool Image::LoadResBMP(HWND hWnd, A_IMAGE_TYPE im_type, TCHAR* file, TCHAR *mask_file){
	
	if(!LoadResBMP(hWnd,im_type,file,true))return false;
	
	return ColorMask->LoadResBMP(hWnd,A_IT_1BIT,mask_file,false);
}


bool Image::SaveResBMP(TCHAR *filename){
	
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBQUAD			 pal;
	FILE			*fp;
	DWORD            bytes;
	long             i;

	// if nothing to save 
	if(hImage == NULL)return false;

	if(ImageType == A_IT_24BIT){
		
		bytes = dwWidth*3;
		if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
	
	}
	else if(ImageType ==A_IT_1BIT){
	
		bytes = dwWidth >> 3;
		if(bytes & 0x03)bytes = (bytes & 0xfffffffc)+4;
	
	}
	
	ZeroMemory(&bfh, sizeof(BITMAPFILEHEADER));
	bfh.bfType			= 0x4d42;
	bfh.bfSize			= sizeof(BITMAPFILEHEADER) + 
						  sizeof(BITMAPINFOHEADER) + 
						  bytes * dwHeight;
	if(ImageType == A_IT_1BIT)
		bfh.bfSize += 8; // 2 RGBQUAD palette items
	
	bfh.bfReserved1		= 0;
	bfh.bfReserved2		= 0;
	bfh.bfOffBits		= sizeof(BITMAPFILEHEADER) + 
						  sizeof(BITMAPINFOHEADER); 
	if(ImageType == A_IT_1BIT)
		bfh.bfOffBits += 8; // 2 RGBQUAD palette items
	
	
	
	ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
	bih.biSize          = sizeof(BITMAPINFOHEADER);
	bih.biWidth         = dwWidth;
	bih.biHeight        = dwHeight;
	bih.biPlanes        = 1;
	if(ImageType ==A_IT_24BIT)
		bih.biBitCount  = 24;
	else if(ImageType ==A_IT_1BIT)
		bih.biBitCount  = 1;
	
	bih.biCompression   = BI_RGB;
	bih.biSizeImage     = bytes*dwHeight;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	if(ImageType ==A_IT_1BIT)
		bih.biClrUsed       = 2;
	else 
		bih.biClrUsed       = 0;
	
	bih.biClrImportant  = 0;

	_tfopen_s(&fp, filename,TEXT("wb"));
	if(fp == NULL)
		return false;

	// writung file header
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fp);

	// writing bitmap info header
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fp);

	if(ImageType == A_IT_1BIT){	
		// writing palette black color
		pal.rgbRed		= 0x00;
		pal.rgbGreen	= 0x00;
		pal.rgbBlue		= 0x00;
		pal.rgbReserved = 0x00;

		fwrite(&pal,sizeof(RGBQUAD),1,fp);
	
		// writing palette white color
		pal.rgbRed		= 0xff;
		pal.rgbGreen	= 0xff;
		pal.rgbBlue		= 0xff;
		pal.rgbReserved = 0x00;

		fwrite(&pal,sizeof(RGBQUAD),1,fp);
	}

	if(ImageType == A_IT_24BIT){
		for(i = dwHeight-1; i >= 0; i--){
			fwrite((BYTE*)(lpMap[i]),1,bytes,fp);
		}
	}
	else if(ImageType == A_IT_1BIT){
		for(i = dwHeight-1; i >= 0; i--){
			fwrite((lpMaskMap[i]),1,bytes,fp);	
		}
	}
	
	fclose(fp);
	return true;
}

bool Image::SaveResBMP(TCHAR *filename,TCHAR *maskfile){
	
	if(!SaveResBMP(filename))return false;
	if(ColorMask && !ColorMask->SaveResBMP(maskfile))return false;

	return true;
}

void Image::Render(){
	
	/*
	if(bTransparent){
		SetToDC(hTmpImage);
		TransparentBlt(hRenderDevice,dwPosX0,dwPosY0,(DWORD)(dwRenderWidth * fScale), (DWORD)(dwRenderHeight * fScale), hTmpImage, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, 0x000000);
	}
	*/		
	
	HBITMAP	hOldImage, hOldMask;
	DWORD op_res;
	if(bTransparent){
		if(fScaleX == 1.0f && fScaleY == 1.0f){ // some optimization
		
			hOldImage = this->SetToDC(hTmpImage);
						
			// сохраняем наш битмап
			op_res = BitBlt(hTmpOriginalCopy, 0, 0, dwWidth, dwHeight, hTmpImage, 0, 0, SRCCOPY);
						
			// загружаем позитив
			hOldMask = ColorMask->SetToDC(hTmpMaskPositive);
						
			// необходим еще негатив нашей маски
			op_res = BitBlt(hTmpMaskNegative, 0, 0, dwRenderWidth, dwRenderHeight, hTmpMaskPositive, dwImagePosX0, dwImagePosY0, NOTSRCCOPY);			
		    		
			// копируем задний фон во временную картинку
			op_res = BitBlt(hMemoryDevice, 0, 0, dwRenderWidth, dwRenderHeight, hRenderDevice, dwPosX0, dwPosY0, SRCCOPY);
						
			// накладываем маску на фон
			op_res = BitBlt(hMemoryDevice, 0, 0, dwRenderWidth, dwRenderHeight, hTmpMaskNegative, 0, 0, SRCAND);
			
			// накладываем маску на рисунок 
			op_res = BitBlt(hTmpImage, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, hTmpMaskPositive, dwImagePosX0, dwImagePosY0, SRCAND);			
			
			op_res = BitBlt(hMemoryDevice, 0, 0, dwRenderWidth, dwRenderHeight, hTmpImage, dwImagePosX0, dwImagePosY0, SRCPAINT);
			
			// копируем все обратно			
			op_res = BitBlt(hRenderDevice, dwPosX0, dwPosY0, dwRenderWidth, dwRenderHeight, hMemoryDevice, 0, 0, SRCCOPY);
						
			// восстанавливаем рисунок
			op_res = BitBlt(hTmpImage, 0, 0, dwWidth, dwHeight, hTmpOriginalCopy, 0, 0, SRCCOPY);

			SelectObject(hTmpImage, hOldImage);
			SelectObject(hTmpMaskPositive, hOldMask);
		
		}
		else { // more slowly than bitblt
			
			hOldImage = this->SetToDC(hTmpImage);
						
			// сохраняем наш битмап
			op_res = BitBlt(hTmpOriginalCopy, 0, 0, dwWidth, dwHeight, hTmpImage, 0, 0, SRCCOPY);
						
			// загружаем позитив
			hOldMask = ColorMask->SetToDC(hTmpMaskPositive);
						
			// необходим еще негатив нашей маски
			op_res = StretchBlt(hTmpMaskNegative, 0, 0, (int)(dwRenderWidth * fScaleX), (int)(dwRenderHeight * fScaleY), hTmpMaskPositive, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, NOTSRCCOPY);			
		    		
			// копируем задний фон во временную картинку
			op_res = BitBlt(hMemoryDevice, 0, 0, (int)(dwRenderWidth * fScaleX), (int)(dwRenderHeight * fScaleY), hRenderDevice, dwPosX0, dwPosY0, SRCCOPY);
						
			// накладываем маску на фон
			op_res = BitBlt(hMemoryDevice, 0, 0, (int)(dwRenderWidth * fScaleX),(int)(dwRenderHeight * fScaleY), hTmpMaskNegative, 0, 0, SRCAND);
			
			// накладываем маску на рисунок 
			op_res = BitBlt(hTmpImage, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, hTmpMaskPositive, dwImagePosX0, dwImagePosY0, SRCAND);			
			
			op_res = StretchBlt(hMemoryDevice, 0, 0, (int)(dwRenderWidth * fScaleX),(int)(dwRenderHeight * fScaleY), hTmpImage, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, SRCPAINT);
			
			// копируем все обратно			
			op_res = BitBlt(hRenderDevice, dwPosX0, dwPosY0, (int)(dwRenderWidth * fScaleX), (int)(dwRenderHeight * fScaleY), hMemoryDevice, 0, 0, SRCCOPY);
						
			// восстанавливаем рисунок
			op_res = BitBlt(hTmpImage, 0, 0, dwWidth, dwHeight, hTmpOriginalCopy, 0, 0, SRCCOPY);
								
			SelectObject(hTmpImage, hOldImage);
			SelectObject(hTmpMaskPositive, hOldMask);
		}
	}
	else {
		if(fScaleX == 1.0f && fScaleY == 1.0f){ // some optimization
			hOldImage = this->SetToDC(hTmpImage);
			StretchBlt(hRenderDevice, dwPosX0, dwPosY0, (int)(fScaleX * dwRenderWidth), (int)(fScaleY * dwRenderHeight), hTmpImage, dwImagePosX0, dwImagePosY0, dwRenderWidth, dwRenderHeight, SRCCOPY);
			SelectObject(hTmpImage, hOldImage);
		}
		else {
			hOldImage = this->SetToDC(hTmpImage);
			BitBlt(hRenderDevice, dwPosX0, dwPosY0, dwRenderWidth, dwRenderHeight, hTmpImage, dwImagePosX0, dwImagePosY0, SRCCOPY);
			SelectObject(hTmpImage, hOldImage);
		}
	}	
	
	return ;					
}

bool Image::IsTransparentPos(int x, int y){
	
	if(x < 0 || y < 0)return false;
	if(ColorMask == NULL)return false;
	
	return (A_get_bit(ColorMask->lpMaskMap[y],x)) == 0;
}

bool Image::HitTest(int x, int y){

	int local_x, local_y;
	int scaled_x, scaled_y;
	
	local_x = x - dwPosX0;
	local_y = y - dwPosY0;

	if(x < 0 || y < 0) return false;

	scaled_x = (int)(fScaleX * local_x + 0.5f);
	scaled_y = (int)(fScaleY * local_y + 0.5f); 

	return !IsTransparentPos(scaled_x, scaled_y);
}