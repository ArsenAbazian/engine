#include "texture.h"
#include <dxerr9.h>

TCHAR    tbuf[200];

BYTE	checkByte15;

WORD	checkWord4;
#define CHECK_WORD4

BYTE	checkByte37;
BYTE	checkByte1_33;

WORD	checkWord4_1;
#define CHECK_WORD4_1

BYTE	checkByte4;
BYTE	checkByte24;

BYTE	checkByte1_4;
BYTE	checkByte1_10;

Texture::~Texture(){
	Clear();
}

void Texture::SetKernel(Kernel *krn){
	kernel = krn;
}

HRESULT Texture::ConvertTextureFormat(D3DXIMAGE_INFO *info, D3DFORMAT pFormat){

	LPDIRECT3DTEXTURE9	lpTexture1;
	HRESULT				hr;

	// creating texture
	hr = kernel->lpDevice->CreateTexture(info->Width,info->Height,info->MipLevels,D3DUSAGE_AUTOGENMIPMAP,pFormat,D3DPOOL_MANAGED,&lpTexture1, NULL);
    if(FAILED(hr)){
		_stprintf_s(tbuf, 200, TEXT("Error : can't create texture (%s)"),DXGetErrorString9(hr));
		MessageBox(kernel->hWnd,tbuf,TEXT("Texture"),MB_OK);
		return hr;
	}

	// perfom to copy
	D3DSURFACE_DESC   desc1;
    D3DLOCKED_RECT    lr1;

	D3DSURFACE_DESC   desc2;
    D3DLOCKED_RECT    lr2;
    
	Color888		  *col1;
	Color8888		  *col11;
		
	Color8888         *col2;
    Color1555         *col3;
    Color4444         *col4;
	    
	DWORD             str,col;
    
	// prepare texture1  
	hr = lpTexture->GetLevelDesc(0,&desc1);
    if(hr != D3D_OK)return hr;
        
    hr = lpTexture->LockRect(0,&lr1,NULL,NULL);
    if(hr != D3D_OK)return hr;

	// prepare texture2
	hr = lpTexture1->GetLevelDesc(0,&desc2);
	if(hr != D3D_OK){
		lpTexture->UnlockRect(0);
		return hr;
	}
        
    hr = lpTexture1->LockRect(0,&lr2,NULL,NULL);
	if(hr != D3D_OK){
		lpTexture->UnlockRect(0);
		return hr;
	}
	
	if(desc1.Format == D3DFMT_R8G8B8){
	
		col1 = (Color888*)lr1.pBits;

		if(desc2.Format == D3DFMT_A8R8G8B8){
			
			col2 = (Color8888*)lr2.pBits;

			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col2->r = col1->r;
					col2->g = col1->g;
					col2->b = col1->b;
					col2->a = 0xff;
				
					col1++;
					col2++;
				}
			}
		}
		
		else if(desc2.Format == D3DFMT_A1R5G5B5){
			
			col3 = (Color1555*)lr2.pBits;
			
			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col3->r = col1->r >> 3;
					col3->g = col1->g >> 3;
					col3->b = col1->b >> 3;
					col3->a = 0x1;
				
					col1++;
					col3++;
				}
			}
		}
	
		else if(desc2.Format == D3DFMT_A4R4G4B4){
			
			col4 = (Color4444*)lr2.pBits;
			
			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col4->r = col1->r >> 4;
					col4->g = col1->g >> 4;
					col4->b = col1->b >> 4;
					col4->a = 0x0f;
				
					col1++;
					col4++;
				}
			}
		}
	}

	if(desc1.Format == D3DFMT_X8R8G8B8){
	
		col11 = (Color8888*)lr1.pBits;

		if(desc2.Format == D3DFMT_A8R8G8B8){
			
			col2 = (Color8888*)lr2.pBits;

			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col2->r = col11->r;
					col2->g = col11->g;
					col2->b = col11->b;
					col2->a = 0xff;
				
					col11++;
					col2++;
				}
			}
		}
		
		else if(desc2.Format == D3DFMT_A1R5G5B5){
			
			col3 = (Color1555*)lr2.pBits;
			
			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col3->r = col11->r >> 3;
					col3->g = col11->g >> 3;
					col3->b = col11->b >> 3;
					col3->a = 0x1;
				
					col11++;
					col3++;
				}
			}
		}
	
		else if(desc2.Format == D3DFMT_A4R4G4B4){
			
			col4 = (Color4444*)lr2.pBits;
			
			for(str=0;str<desc2.Height;str++){
				for(col=0;col<desc2.Width; col++){
				
					col4->r = col11->r >> 4;
					col4->g = col11->g >> 4;
					col4->b = col11->b >> 4;
					col4->a = 0x0f;
				
					col11++;
					col4++;
				}
			}
		}
	}
	
	lpTexture->UnlockRect(0);
	lpTexture1->UnlockRect(0);

	lpTexture->Release();
	lpTexture = lpTexture1;

	return D3D_OK;
}

/*
HRESULT Texture::CopyImageToSurface(Image *img, bool inv){

	DWORD			   i, j;
	UINT               cw,ch;
	D3DSURFACE_DESC    desc;
    D3DLOCKED_RECT     lr;
	Color8888         *col1;
    Color1555         *col2;
    Color4444         *col3;
    Color565          *col4;
	HRESULT            hr;

	hr = lpTexture->GetLevelDesc(0,&desc);
    if(hr != D3D_OK)return hr;
    
    hr = lpTexture->LockRect(0,&lr,NULL,0);
    if(hr != D3D_OK)return hr;

	cw = img->GetWidth() < desc.Width? img->GetWidth(): desc.Width;
    ch = img->GetHeight() < desc.Height? img->GetHeight(): desc.Height;

	if(desc.Format == D3DFMT_A8R8G8B8){
		col1 = (Color8888*)lr.pBits;
		for(i = 0;i < cw;i++){
			for(j=0 ; j < ch; j++){
				col1[j].a = 0xff;
				col1[j].g = img->lpMap[i][j].g;
				
				if(inv){
					col1[j].r = img->lpMap[i][j].b;
                    col1[j].b = img->lpMap[i][j].r;
				}
				else{
					col1[j].b = img->lpMap[i][j].b;
                    col1[j].r = img->lpMap[i][j].r;
				}
			}
			col1 += desc.Width;
		}	
	}
	else if(desc.Format == D3DFMT_A1R5G5B5){
		col2 = (Color1555*)lr.pBits;
		for(i = 0;i < cw;i++){
			for(j=0 ; j < ch; j++){
				col2[j].a = 1;
				col2[j].g = img->lpMap[i][j].g >> 3;
				
				if(inv){
					col2[j].r = img->lpMap[i][j].b >> 3;
                    col2[j].b = img->lpMap[i][j].r >> 3;
				}
				else{
					col2[j].b = img->lpMap[i][j].b >> 3;
                    col2[j].r = img->lpMap[i][j].r >> 3;
				}
			}
			col2 += desc.Width;
		}
	}
	else if(desc.Format == D3DFMT_A4R4G4B4){
		col3 = (Color4444*)lr.pBits;
		for(i = 0;i < cw;i++){
			for(j=0 ; j < ch; j++){
				col3[j].a = 0xf;
				col3[j].g = img->lpMap[i][j].g >> 4;
				
				if(inv){
					col3[j].r = img->lpMap[i][j].b >> 4;
                    col3[j].b = img->lpMap[i][j].r >> 4;
				}
				else{
					col3[j].b = img->lpMap[i][j].b >> 4;
                    col3[j].r = img->lpMap[i][j].r >> 4;
				}
			}
			col3 += desc.Width;
		}
	}
	else if(desc.Format == D3DFMT_R5G6B5){
		col4 = (Color565*)lr.pBits;
		for(i = 0;i < cw;i++){
			for(j=0 ; j < ch; j++){
				col4[j].g = img->lpMap[i][j].g >> 2;
				
				if(inv){
					col4[j].r = img->lpMap[i][j].b >> 3;
                    col4[j].b = img->lpMap[i][j].r >> 3;
				}
				else{
					col4[j].b = img->lpMap[i][j].b >> 3;
                    col4[j].r = img->lpMap[i][j].r >> 3;
				}
			}
			col4 += desc.Width;
		}
	}

	hr = lpTexture->UnlockRect(0);
	return hr;
}
*/
HRESULT Texture::Create(DWORD pWidth,DWORD pHeight,D3DFORMAT pFormat,DWORD Mperc){
    
	DWORD         Lenght;
    HRESULT       hr;
    	
	dwWidth      = pWidth;
    dwHeight     = pHeight;
    Format		 = pFormat;
    dwMipMap     = 0;
        
    if(dwWidth > kernel->Caps.MaxTextureWidth)dwWidth = kernel->Caps.MaxTextureWidth;
	if(dwHeight > kernel->Caps.MaxTextureHeight)dwHeight = kernel->Caps.MaxTextureHeight;
		
	for(Lenght=0;dwWidth>0;dwWidth>>=1)Lenght++;
	for(dwWidth=1;Lenght>1;Lenght--)dwWidth<<=1;
		
	for(Lenght=0;dwHeight>0;dwHeight>>=1)Lenght++;
	for(dwHeight=1;Lenght>1;Lenght--)dwHeight<<=1;
		
			
	if(Mperc == 100){
		dwMipMap = 0;
	}
	else {
		Lenght      = dwWidth>dwHeight? dwWidth: dwHeight;
		for(;Lenght>0;Lenght>>=1)dwMipMap++;
       
		dwMipMap = dwMipMap * Mperc/100;
		if(dwMipMap == 0)dwMipMap++;
	}
        
    hr = kernel->lpDevice->CreateTexture(dwWidth,dwHeight,dwMipMap,D3DUSAGE_AUTOGENMIPMAP,Format,D3DPOOL_MANAGED,&lpTexture, NULL);
    if(FAILED(hr)){
		_stprintf_s(tbuf, 200, TEXT("Error : can't create texture (%s) %d %d %d %d"),DXGetErrorString9(hr), dwWidth,dwHeight,dwMipMap, Format);
		MessageBox(kernel->hWnd,tbuf,TEXT("Texture"),MB_OK);
		return hr;
	}
    	
	return hr;
}

HRESULT Texture::loadFromFile( shString &fileName ) { 
	
	return this->loadFromFile( fileName, kernel->VParams.TextureFormat );
}

HRESULT Texture::loadFromFile( shString &fileName, bool useFileFormat ) { 
	
	if( useFileFormat ) return this->loadFromFile( fileName, D3DFMT_UNKNOWN );
	return this->loadFromFile( fileName );
}

HRESULT Texture::loadFromFile( shString &fileName, D3DFORMAT format ) { 
	
	HRESULT rv;

	D3DXIMAGE_INFO	info;
	rv = D3DXCreateTextureFromFileEx(	kernel->lpDevice, 
										fileName.getCString(), 
										D3DX_DEFAULT, 
										D3DX_DEFAULT, 
										D3DX_DEFAULT, 
										0, 
										format, 
										D3DPOOL_MANAGED, 
										D3DX_DEFAULT, 
										D3DX_DEFAULT, 
										0, 
										&info, 
										NULL, 
										(IDirect3DTexture9 **) &lpTexture );

	if( FAILED(rv) ) {
		_stprintf_s( kernel->tempBuffer, kernel->bufferSize, TEXT("Error : can't create texture from file '%s' -> (%s)" ), fileName.getCString(), DXGetErrorString9( rv ) );
		kernel->ErrorBox( TEXT( "Texture" ) );
		return rv;
	}

	return D3D_OK;
}

/*
HRESULT Texture::LoadFromFile(TCHAR *file,D3DFORMAT pFormat,DWORD Mperc){
	
	HRESULT		hr; 
		
	dwMipMap = (DWORD)(11.0f * Mperc / 100.0f);
	if (dwMipMap == 0)dwMipMap = 1;

	D3DXIMAGE_INFO	info;
	hr = D3DXCreateTextureFromFileEx( kernel->lpDevice, 
									  file, 
									  D3DX_DEFAULT, 
									  D3DX_DEFAULT, 
									  D3DX_DEFAULT, 
									  0, 
									  pFormat, 
									  D3DPOOL_MANAGED, 
									  D3DX_DEFAULT, 
									  D3DX_DEFAULT, 
									  0, 
									  &info, 
									  NULL, 
									  (IDirect3DTexture9 **)&lpTexture);

	if(FAILED(hr)){
		_stprintf_s(tbuf, 200, TEXT("Error : can't create texture (%s)"),DXGetErrorString9(hr));
		MessageBox(kernel->hWnd,tbuf,TEXT("Texture"),MB_OK);
		return hr;
	}
	
	D3DSURFACE_DESC   desc1;
	
	hr = lpTexture->GetLevelDesc(0, &desc1);
	
	if(pFormat != desc1.Format){
		hr = ConvertTextureFormat(&info, pFormat);
		if(FAILED(hr)){
			_stprintf_s(tbuf, 200, TEXT("Error : can't convert texture (%s)"),DXGetErrorString9(hr));
			MessageBox(kernel->hWnd,tbuf,TEXT("Texture"),MB_OK);
			return hr;
		}	
	}
	
	dwWidth		= info.Width;
	dwHeight	= info.Height;
	dwMipMap	= lpTexture->GetLevelCount();
	dwBpp		= 0;
	Format		= pFormat;

	GenerateMipSubLevels(D3DTEXF_ANISOTROPIC);
	
	return D3D_OK;	
}

*/

void Texture::GenerateMipSubLevels(D3DTEXTUREFILTERTYPE fType){
	
	if(lpTexture){
		lpTexture->SetAutoGenFilterType(fType);
		lpTexture->GenerateMipSubLevels();
	}

}

HRESULT Texture::AlphaChannel(DWORD minColor,DWORD maxColor,float Alpha){
			
	return FillAlpha(minColor,maxColor,Alpha);
}

HRESULT Texture::Clear(){
	if(lpTexture)lpTexture->Release();
	lpTexture = NULL;
	if(lpBuffer)delete[] lpBuffer;
	
	lpTexture   = NULL;
	lpBuffer  	= NULL;	
	
	return D3D_OK;
}

HRESULT Texture::FillAlpha(DWORD Level,DWORD dwMinColor,DWORD dwMaxColor,float Alpha){
    
	D3DSURFACE_DESC   desc;
    D3DLOCKED_RECT    lr;
    Color8888		  MinColor1, MaxColor1;
	Color1555         MinColor2, MaxColor2;
	Color4444         MinColor3, MaxColor3;
	Color8888         *col1;
    Color1555         *col2;
    Color4444         *col3;
    DWORD             str,col,dwAlpha;
    HRESULT           hr;
    
    hr = lpTexture->GetLevelDesc(Level,&desc);
    if(hr != D3D_OK)return hr;
        
    hr = lpTexture->LockRect(Level,&lr,NULL,NULL);
    if(hr != D3D_OK)return hr;
    
    
	
	if(desc.Format == D3DFMT_A8R8G8B8){
        col1    = (Color8888*)lr.pBits;    
        dwAlpha = (DWORD)(Alpha*255.0f);
        
		MinColor1.r = (dwMinColor & 0x00ff0000) >> 16;
		MinColor1.a = (dwMinColor & 0xff000000) >> 24;
		MinColor1.g = (dwMinColor & 0x0000ff00) >> 8;
		MinColor1.b = (dwMinColor & 0x000000ff);

		MaxColor1.r = (dwMaxColor & 0x00ff0000) >> 16;
		MaxColor1.a = (dwMaxColor & 0xff000000) >> 24;
		MaxColor1.g = (dwMaxColor & 0x0000ff00) >> 8;
		MaxColor1.b = (dwMaxColor & 0x000000ff);

		for(str=0;str<desc.Height;str++){
            for(col=0;col<desc.Width;col++){
				if(col1[col].r >= MinColor1.r && col1[col].r <= MaxColor1.r &&
					col1[col].g >= MinColor1.g && col1[col].g <= MaxColor1.g &&
					col1[col].b >= MinColor1.b && col1[col].b <= MaxColor1.b &&
					col1[col].a >= MinColor1.a && col1[col].a <= MaxColor1.a){
                    col1[col].a = (BYTE)dwAlpha;
                }
            }
            col1+=desc.Width;
        }    
    }
    else if(desc.Format == D3DFMT_A1R5G5B5){
        col2 = (Color1555*)lr.pBits;
        dwAlpha = (DWORD)(Alpha+0.5f);
        		
		MinColor2.r = ((dwMinColor & 0x00ff0000) >> 18) & 0x1f;
		MinColor2.a = ((dwMinColor & 0xff000000) >> 27) & 0x1;
		MinColor2.g = ((dwMinColor & 0x0000ff00) >> 11) & 0x1f;
		MinColor2.b = ((dwMinColor & 0x000000ff) >> 3)  & 0x1f;

		MaxColor2.r = ((dwMaxColor & 0x00ff0000) >> 18) & 0x1f;
		MaxColor2.a = ((dwMaxColor & 0xff000000) >> 27) & 0x1;
		MaxColor2.g = ((dwMaxColor & 0x0000ff00) >> 11) & 0x1f;
		MaxColor2.b = ((dwMaxColor & 0x000000ff) >>  3) & 0x1f;

        for(str=0;str<desc.Height;str++){
            for(col=0;col<desc.Width;col++){
                
				if(col2[col].r >= MinColor2.r && col2[col].r <= MaxColor2.r &&
					col2[col].g >= MinColor2.g && col2[col].g <= MaxColor2.g &&
					col2[col].b >= MinColor2.b && col2[col].b <= MaxColor2.b &&
					col2[col].a >= MinColor2.a && col2[col].a <= MaxColor2.a){
                    col2[col].a = (BYTE)dwAlpha;
                }
            }
            col2+=desc.Width;
        }    
    }
    else if(desc.Format == D3DFMT_A4R4G4B4){
        col3 = (Color4444*)lr.pBits;
        dwAlpha = (DWORD)(Alpha*31.0f);
        
		MinColor3.r = ((dwMinColor & 0x00ff0000) >> 20) & 0x0f;
		MinColor3.a = ((dwMinColor & 0xff000000) >> 28) & 0x0f;
		MinColor3.g = ((dwMinColor & 0x0000ff00) >> 12) & 0x0f;
		MinColor3.b = ((dwMinColor & 0x000000ff) >> 4)  & 0x0f;

		MaxColor3.r = ((dwMaxColor & 0x00ff0000) >> 20) & 0x0f;
		MaxColor3.a = ((dwMaxColor & 0xff000000) >> 28) & 0x0f;
		MaxColor3.g = ((dwMaxColor & 0x0000ff00) >> 12) & 0x0f;
		MaxColor3.b = ((dwMaxColor & 0x000000ff) >> 4)  & 0x0f;

        for(str=0;str<desc.Height;str++){
            for(col=0;col<desc.Width;col++){
                
				if(col3[col].r >= MinColor3.r && col3[col].r <= MaxColor3.r &&
					col3[col].g >= MinColor3.g && col3[col].g <= MaxColor3.g &&
					col3[col].b >= MinColor3.b && col3[col].b <= MaxColor3.b &&
					col3[col].a >= MinColor3.a && col3[col].a <= MaxColor3.a){
                    col3[col].a = (BYTE)dwAlpha;
                }
				
            }
            col3+=desc.Width;
        }
    }
    
    return lpTexture->UnlockRect(Level);
}

HRESULT Texture::FillAlpha(DWORD minColor,DWORD maxColor,float Alpha){
    
	DWORD LCount = lpTexture->GetLevelCount();
    HRESULT hr;
    
    for(DWORD i=0;i<LCount;i++){
        hr = FillAlpha(i,minColor,maxColor,Alpha);
        if(FAILED(hr))return hr;
    }
    return hr;
}
