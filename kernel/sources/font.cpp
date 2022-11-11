#include "font.h"
#include <math.h>
#include <dxerr9.h>

short   PosTable[] = {
//      00  01  02  03  04  05  06  07  08  09  0a  0b  0c  0d  0e  0f
/* 0 */ 126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,
/* 1 */ 116,117,118,119,120,121,122,123,124,125,142,143,144,145,146,147,
/* 2 */ 148, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
/* 3 */ 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,149,150,151,152,153,
/* 4 */154, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
/* 5 */ 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 155,156,157,158,0,
/* 6 */ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
/* 7 */ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
/* 8 */ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
/* 9 */ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
/* a */ 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
/* b */ 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
/* c */ 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
/* d */100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115
};

TCHAR	fbuf[256];

bool a3dFont::LoadPositions(TCHAR *FileName){
    
	DWORD    Count;
    char     buf[5];

    FILE	*fp;
	
	_tfopen_s(&fp, FileName,TEXT("rb"));
    if(fp == NULL)return false;
    
    fread(buf,1,5,fp);
    if( buf[0] != 'A' ||
        buf[1] != 'F' ||
        buf[2] != 'O' ||
        buf[3] != 'N' ||
		buf[4] != 'T'){
	
		fclose(fp);	
		return false;
	}
    
    fread(&dwItalic,4,1,fp);
    
    for(Count=0;Count<=158;Count++){
        fread(&Pos[Count].x0,4,1,fp);
        fread(&Pos[Count].y0,4,1,fp);
        fread(&Pos[Count].x1,4,1,fp);
        fread(&Pos[Count].x2,4,1,fp);
        fread(&Pos[Count].y1,4,1,fp);
        fread(&Pos[Count].x3,4,1,fp);
    }
    fclose(fp);
	
	return true;
}

HRESULT a3dFont::Create(){
    HRESULT hr;
       
    hr = Texture::kernel->lpDevice->CreateVertexBuffer(MAXLETTERS*4*sizeof(FontVertex),D3DUSAGE_DYNAMIC,0,D3DPOOL_DEFAULT,&lpVert, NULL);
    if(FAILED(hr)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't create vertex buffer (%s)"),DXGetErrorString9(hr));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return hr;
    }    
    
	hr = Texture::kernel->lpDevice->CreateIndexBuffer(MAXLETTERS*6*2,D3DUSAGE_DYNAMIC,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&lpIndex, NULL);
    if(FAILED(hr)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't create index buffer (%s)"),DXGetErrorString9(hr));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return hr;
	}
    	
	return hr;
}

HRESULT a3dFont::Clear(){
	if(lpVert)lpVert->Release();
    if(lpIndex)lpIndex->Release();
    	
	lpVert = NULL;
	lpIndex = NULL;
	
	if(bSaved){
		if(lpVBuffer)delete[] lpVBuffer;
		if(lpIBuffer)delete[] lpIBuffer;
		lpVBuffer = NULL;
		lpIBuffer = NULL;
	}
	
	bSaved = false;
	return D3D_OK;
}

HRESULT a3dFont::Print(TCHAR *String,float x,float y,DWORD Color){
    
	FontVertex  *Vert;
    WORD        *Index;
    WORD         Offset = 0;
    float        Height,Width;
    float		 ph;
	HRESULT      hr;
    int          letter;
        
    dwFaceCount = 0;
	dwLength    = 0;
	
	hr = lpVert->Lock(0,0,(void**)&Vert,D3DLOCK_DISCARD);
    if(FAILED(hr))return hr;

    hr = lpIndex->Lock(0,MAXLETTERS*3*2,(void**)&Index,D3DLOCK_DISCARD);
    if(FAILED(hr)){
        lpVert->Unlock();
        return hr;
    }
    
    D3DVIEWPORT9    vp;
    
    Texture::kernel->lpDevice->GetViewport(&vp);
    
    x *= vp.Width;
    y *= vp.Height; 
    
	fPrintedWidth = 0.0f;
	fPrintedHeight = 0.0f;

    while(*String){
        
        letter = (int)(((BYTE)*String)-32);
        
        Height = (float)(Pos[PosTable[letter]].y1 - Pos[PosTable[letter]].y0)*Texture::dwHeight;
        Width = (float)(Pos[PosTable[letter]].x1 - Pos[PosTable[letter]].x0)*Texture::dwWidth;
                
		fPrintedWidth += Width / vp.Width;
		
		ph = Height / vp.Height;
		if(fPrintedHeight < ph)fPrintedHeight = ph;

		
		Vert[dwLength].SCoor[0] = x+Offset+dwItalic;
        Vert[dwLength].SCoor[1] = y;
        Vert[dwLength].SCoor[2] = 0.0f;
        Vert[dwLength].RHW      = 1.0f;
        Vert[dwLength].Color    = Color;
        Vert[dwLength].T1Coor[0] = Pos[PosTable[letter]].x0;
        Vert[dwLength].T1Coor[1] = Pos[PosTable[letter]].y0;
                
        Vert[dwLength+1].SCoor[0] = x+Offset+Width+dwItalic;
        Vert[dwLength+1].SCoor[1] = y;
        Vert[dwLength+1].SCoor[2] = 0.0f;
        Vert[dwLength+1].RHW      = 1.0f;
        Vert[dwLength+1].Color    = Color;
        Vert[dwLength+1].T1Coor[0] = Pos[PosTable[letter]].x1;
        Vert[dwLength+1].T1Coor[1] = Pos[PosTable[letter]].y0;
                
        Vert[dwLength+2].SCoor[0] = x+Offset;
        Vert[dwLength+2].SCoor[1] = y+Height;
        Vert[dwLength+2].SCoor[2] = 0.0f;
        Vert[dwLength+2].RHW      = 1.0f;
        Vert[dwLength+2].Color    = Color;
        Vert[dwLength+2].T1Coor[0] = Pos[PosTable[letter]].x2;
        Vert[dwLength+2].T1Coor[1] = Pos[PosTable[letter]].y1;
                
        Vert[dwLength+3].SCoor[0] = x+Offset+Width;
        Vert[dwLength+3].SCoor[1] = y+Height;
        Vert[dwLength+3].SCoor[2] = 0.0f;
        Vert[dwLength+3].RHW      = 1.0f;  
        Vert[dwLength+3].Color    = Color;
        Vert[dwLength+3].T1Coor[0] = Pos[PosTable[letter]].x3;
        Vert[dwLength+3].T1Coor[1] = Pos[PosTable[letter]].y1;
                
        Index[dwFaceCount]   = dwLength;
        Index[dwFaceCount+1] = dwLength+1;
        Index[dwFaceCount+2] = dwLength+2;
        Index[dwFaceCount+3] = dwLength+1;
        Index[dwFaceCount+4] = dwLength+3;
        Index[dwFaceCount+5] = dwLength+2;
        
        dwFaceCount+=6;
        Offset+= (WORD)Width;
        dwLength+= 4;
        String++;
        
    }
    
    hr = lpVert->Unlock();
    if(FAILED(hr))
		return hr;
	
	lpIndex->Unlock();
    if(FAILED(hr))
		return hr;
    
	return D3D_OK;

}

void a3dFont::Italic(int it){
	dwItalic = it;
}

HRESULT a3dFont::Print(TCHAR *String,float x,float y,float scalex,float scaley,DWORD Color){
    
	FontVertex  *Vert;
    WORD        *Index;
    WORD         Offset = 0;
    float        Height,Width;
    float        allWidth;
	float        allHeight;
	float        minWidth;
	float        minHeight;
	HRESULT      hr;
    int          letter;
    float		 ph;
	TCHAR		 *strnone = TEXT("NULL");

    DWORD		 i;
	
	dwFaceCount = 0;
	dwLength    = 0;

	if(String == NULL)
		String = strnone;

	hr = lpVert->Lock(0,0,(void**)&Vert,D3DLOCK_DISCARD);
    if(FAILED(hr))return hr;

    hr = lpIndex->Lock(0,MAXLETTERS*3*2,(void**)&Index,D3DLOCK_DISCARD);
    if(FAILED(hr)){
        lpVert->Unlock();
        return hr;
    }
    
    D3DVIEWPORT9    vp;
    
    Texture::kernel->lpDevice->GetViewport(&vp);
    
    x *= vp.Width;
    y *= vp.Height; 
    		
	letter     = (int)(((BYTE)*String)-32);
	if(letter < 0)letter = 0;
	allHeight  = (float)(Pos[PosTable[letter]].y1 - Pos[PosTable[letter]].y0)*Texture::dwHeight;
	allHeight *= scaley;
	allWidth   = 0;
	
	fPrintedWidth	= 0.0f;
	fPrintedHeight	= 0.0f;
	
	while(*String){
        letter = (int)(((BYTE)*String)-32);
                        
        Height = (float)(Pos[PosTable[letter]].y1 - Pos[PosTable[letter]].y0)*Texture::dwHeight;
        Width = (float)(Pos[PosTable[letter]].x1 - Pos[PosTable[letter]].x0)*Texture::dwWidth;
        
        Height*=scaley;
        Width*=scalex;
        
		fPrintedWidth += Width / vp.Width;
		
		ph = Height / vp.Height;
		if(fPrintedHeight < ph)fPrintedHeight = ph;

		allWidth+=Width;
		
		Vert[dwLength].SCoor[0] = x+Offset+dwItalic;
        Vert[dwLength].SCoor[1] = y;
        Vert[dwLength].SCoor[2] = 0.0f;
        Vert[dwLength].RHW      = 1.0f;
        Vert[dwLength].Color    = Color;
        Vert[dwLength].T1Coor[0] = Pos[PosTable[letter]].x0;
        Vert[dwLength].T1Coor[1] = Pos[PosTable[letter]].y0;
                
        Vert[dwLength+1].SCoor[0] = x+Offset+Width+dwItalic;
        Vert[dwLength+1].SCoor[1] = y;
        Vert[dwLength+1].SCoor[2] = 0.0f;
        Vert[dwLength+1].RHW      = 1.0f;
        Vert[dwLength+1].Color    = Color;
        Vert[dwLength+1].T1Coor[0] = Pos[PosTable[letter]].x1;
        Vert[dwLength+1].T1Coor[1] = Pos[PosTable[letter]].y0;
                
        Vert[dwLength+2].SCoor[0] = x+Offset;
        Vert[dwLength+2].SCoor[1] = y+Height;
        Vert[dwLength+2].SCoor[2] = 0.0f;
        Vert[dwLength+2].RHW      = 1.0f;
        Vert[dwLength+2].Color    = Color;
        Vert[dwLength+2].T1Coor[0] = Pos[PosTable[letter]].x2;
        Vert[dwLength+2].T1Coor[1] = Pos[PosTable[letter]].y1;
                
        Vert[dwLength+3].SCoor[0] = x+Offset+Width;
        Vert[dwLength+3].SCoor[1] = y+Height;
        Vert[dwLength+3].SCoor[2] = 0.0f;
        Vert[dwLength+3].RHW      = 1.0f;  
        Vert[dwLength+3].Color    = Color;
        Vert[dwLength+3].T1Coor[0] = Pos[PosTable[letter]].x3;
        Vert[dwLength+3].T1Coor[1] = Pos[PosTable[letter]].y1;
                
        Index[dwFaceCount]   = dwLength;
        Index[dwFaceCount+1] = dwLength+1;
        Index[dwFaceCount+2] = dwLength+2;
        Index[dwFaceCount+3] = dwLength+1;
        Index[dwFaceCount+4] = dwLength+3;
        Index[dwFaceCount+5] = dwLength+2;
        
        dwFaceCount+=6;
        Offset+= (WORD)Width;
        dwLength+= 4;
        String++;
        
    }
    	
	minWidth	= allWidth  * fFontX; 
	minHeight	= allHeight * fFontY;
	
	for(i=0;i<dwLength;i++){
		Vert[i].SCoor[0] -= minWidth;		
		Vert[i].SCoor[1] -= minHeight; 
	}
		
	
	hr = lpVert->Unlock();
    if(FAILED(hr))
		return hr;
	
	lpIndex->Unlock();
    if(FAILED(hr))
		return hr;
           
    return D3D_OK;   
}

HRESULT a3dFont::Render(){
	
	HRESULT rv;
	LPDIRECT3DTEXTURE9	lpText;
	
	rv = Texture::kernel->lpDevice->GetTexture(0, (IDirect3DBaseTexture9**)&lpText);
	if(FAILED (rv))
		return rv;

	if(lpText != Texture::lpTexture){
		rv = Texture::kernel->lpDevice->SetTexture(0, Texture::lpTexture);
		if(FAILED (rv))
			return rv;
	}
	
	rv = Texture::kernel->lpDevice->SetStreamSource(0,lpVert,0,sizeof(FontVertex));
    if(FAILED(rv))
		return rv;
	
	rv = Texture::kernel->lpDevice->SetFVF(FONT2DVERTEX);
    if(FAILED(rv))
		return rv;

	rv = Texture::kernel->lpDevice->SetIndices(lpIndex);
	if(FAILED(rv))
		return rv;
	
	rv = Texture::kernel->lpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,dwLength,0,dwLength>>1);        
	if(FAILED(rv))
		return rv;

	return D3D_OK;
}

HRESULT a3dFont::Save(){
	
	HRESULT rv;
	FontVertex	*vtmp;
	WORD		*itmp;
	DWORD		i;
	
	if(bSaved)return D3D_OK;
	
	if(lpVert == NULL ||
		lpIndex == NULL){
		_stprintf_s(fbuf, 256, TEXT("Error : can't save font : nothing to save!"));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}
		
	lpVBuffer = new FontVertex[MAXLETTERS*4];
	lpIBuffer = new WORD[MAXLETTERS*6];
	
	rv = lpVert->Lock(0,0,(void**)&vtmp,0);
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}

	for(i=0;i<MAXLETTERS*4;i++){
		lpVBuffer[i].Color	= vtmp[i].Color;
		lpVBuffer[i].RHW		= vtmp[i].RHW;
		copyVector3(vtmp[i].SCoor,lpVBuffer[i].SCoor);
		
		lpVBuffer[i].T1Coor[0] = vtmp[i].T1Coor[0];
		lpVBuffer[i].T1Coor[1] = vtmp[i].T1Coor[1];
	}

	rv = lpVert->Unlock();
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't unlock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}

	lpVert->Release();
	lpVert = NULL;

	rv = lpIndex->Lock(0,0,(void**)&itmp,0);
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't lock index buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return rv;
	}

	for(i=0;i<MAXLETTERS*6;i++){
		lpIBuffer[i] = itmp[i];
	}

	rv = lpIndex->Unlock();
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't unlock index buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return rv;
	}

	lpIndex->Release();
	lpIndex = NULL;
	
	bSaved = true;
	return D3D_OK;
}


HRESULT a3dFont::Restore(){
	
	HRESULT rv;
	FontVertex	*vtmp;
	WORD		*itmp;
	DWORD		i;
	
	if(!bSaved){
		MessageBox(Texture::kernel->hWnd,TEXT("Error : can't restore object : nothing to restore"),TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}
				
	rv = Create();
	if(FAILED(rv))return rv;
	
	rv = lpVert->Lock(0,0,(void**)&vtmp,0);
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}

	for(i=0;i<MAXLETTERS*4;i++){
		vtmp[i].Color	= lpVBuffer[i].Color;
		vtmp[i].RHW		= lpVBuffer[i].RHW;
		copyVector3(lpVBuffer[i].SCoor,vtmp[i].SCoor);
		
		vtmp[i].T1Coor[0] = lpVBuffer[i].T1Coor[0];
		vtmp[i].T1Coor[1] = lpVBuffer[i].T1Coor[1];
	}

	rv = lpVert->Unlock();
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't unlock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return D3DERR_INVALIDCALL;
	}

	rv = lpIndex->Lock(0,0,(void**)&itmp,0);
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't lock index buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return rv;
	}

	for(i=0;i<MAXLETTERS*6;i++){
		itmp[i] = lpIBuffer[i];
	}

	rv = lpIndex->Unlock();
	if(FAILED(rv)){
		_stprintf_s(fbuf, 256, TEXT("Error : can't unlock index buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(Texture::kernel->hWnd,fbuf,TEXT("a3dFont"),MB_OK);
		return rv;
	}

	delete[] lpVBuffer;
	delete[] lpIBuffer;
	
	lpVBuffer = NULL;
	lpIBuffer = NULL;
	
	bSaved = false;
	return D3D_OK;
}

/*

	Texture::kernel->lpDevice->SetTexture(0,Texture::lpTexture);
        
    Texture::kernel->lpDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_ALPHAREF,0x1);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_CLIPPING,TRUE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_EDGEANTIALIAS,FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_CLIPPLANEENABLE,FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_VERTEXBLEND,FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);
    Texture::kernel->lpDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
       
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, Texture::kernel->Vparams.CFilterMetod );
	Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, Texture::kernel->Vparams.CFilterMetod );	
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, Texture::kernel->Vparams.CFilterMetod );
        
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    Texture::kernel->lpDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    Texture::kernel->lpDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    Texture::kernel->lpDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );    
        
*/

