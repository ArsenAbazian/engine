#include "sprite.h"
#include <dxerr9.h>

TCHAR	spbuf[200];

HRESULT Sprite::Clear(){
	if(lpVert)lpVert->Release();
	if(lpBuffer)delete[] lpBuffer;

	lpVert = NULL;
	lpBuffer = NULL;
 
	bSaved  = false;

	return D3D_OK;
} 


HRESULT Sprite::Init(){
	
	HRESULT rv;
	SpriteVertex	*buf;
	
	if(SpriteKernel == NULL){
		return D3DERR_INVALIDCALL;
	}

	rv = SpriteKernel->lpDevice->CreateVertexBuffer(6*sizeof(T1Vertex),
											D3DUSAGE_DYNAMIC,
											T1VERTEX,
											D3DPOOL_DEFAULT,
											&lpVert, 
											NULL);

	if(FAILED(rv)){
		_stprintf_s(spbuf, 200, TEXT("Error : can't create vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(SpriteKernel->hWnd,spbuf,TEXT("Sprite"),MB_OK);
		return rv;
	}

	rv = lpVert->Lock(0,0,(void**)&buf,0);
	if(FAILED(rv)){
		_stprintf_s(spbuf, 200,TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(SpriteKernel->hWnd,spbuf,TEXT("Sprite"),MB_OK);
		lpBuffer = NULL;
		lpVert->Release();
		lpVert = NULL;
		return rv;
	}

	buf[0].Color     = 0xffffffff;
	buf[0].RHW	     = 1.0f;
	buf[0].SCoor[0]  = 0.0f;
	buf[0].SCoor[1]  = 0.0f;		
	buf[0].SCoor[2]  = 0.0f;
	buf[0].T1Coor[0] = 0.0f;
	buf[0].T1Coor[1] = 0.0f;
	
	buf[1].Color     = 0xffffffff;
	buf[1].RHW	     = 1.0f;
	buf[1].SCoor[0]  = 0.0f;
	buf[1].SCoor[1]  = 0.0f;		
	buf[1].SCoor[2]  = 0.0f;
	buf[1].T1Coor[0] = 1.0f;
	buf[1].T1Coor[1] = 0.0f;
	
	buf[2].Color     = 0xffffffff;
	buf[2].RHW	     = 1.0f;
	buf[2].SCoor[0]  = 0.0f;
	buf[2].SCoor[1]  = 0.0f;		
	buf[2].SCoor[2]  = 0.0f;
	buf[2].T1Coor[0] = 0.0f;
	buf[2].T1Coor[1] = 1.0f;
	
	buf[3].Color     = 0xffffffff;
	buf[3].RHW	     = 1.0f;
	buf[3].SCoor[0]  = 0.0f;
	buf[3].SCoor[1]  = 0.0f;		
	buf[3].SCoor[2]  = 0.0f;
	buf[3].T1Coor[0] = 1.0f;
	buf[3].T1Coor[1] = 0.0f;
	
	buf[4].Color     = 0xffffffff;
	buf[4].RHW	     = 1.0f;
	buf[4].SCoor[0]  = 0.0f;
	buf[4].SCoor[1]  = 0.0f;		
	buf[4].SCoor[2]  = 0.0f;
	buf[4].T1Coor[0] = 1.0f;
	buf[4].T1Coor[1] = 1.0f;
	
	buf[5].Color     = 0xffffffff;
	buf[5].RHW	     = 1.0f;
	buf[5].SCoor[0]  = 0.0f;
	buf[5].SCoor[1]  = 0.0f;		
	buf[5].SCoor[2]  = 0.0f;
	buf[5].T1Coor[0] = 0.0f;
	buf[5].T1Coor[1] = 1.0f;

	lpVert->Unlock();

	return D3D_OK;
}

HRESULT Sprite::Save(){
	
	HRESULT			rv;
	SpriteVertex	*tmp;
		
	rv = lpVert->Lock(0,0,(void**)&tmp,0);
	if(FAILED(rv)){
		_stprintf_s(spbuf, 200, TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(SpriteKernel->hWnd,spbuf,TEXT("Sprite"),MB_OK);
		lpBuffer = NULL;
	}	
	
	if(lpBuffer == NULL)lpBuffer = new SpriteVertex[6];	
	memcpy(lpBuffer,tmp,sizeof(SpriteVertex)*6);
		
	lpVert->Unlock();
	lpVert->Release();	
	lpVert = NULL;
	
	bSaved = true;
	
	return D3D_OK;
}

HRESULT Sprite::Restore(){
	
	HRESULT rv;
	SpriteVertex	*tmp;
	
	if(!bSaved){
		MessageBox(SpriteKernel->hWnd,TEXT("Error : sprite not saved!"),TEXT("Sprite"),MB_OK);
		return D3DERR_INVALIDCALL;
	}

	if(FAILED(rv = Init())){
		return rv;
	}
	
	rv = lpVert->Lock(0,0,(void**)&tmp,0);
	if(FAILED(rv)){
		_stprintf_s(spbuf, 200, TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(SpriteKernel->hWnd,spbuf,TEXT("Sprite"),MB_OK);
		return rv;
	}
	
	memcpy(tmp,lpBuffer,6*(sizeof(SpriteVertex)));	
		
	lpVert->Unlock();
	delete[] lpBuffer;
	lpBuffer = NULL;
	bSaved   = false;
	
	return D3D_OK;
}

HRESULT Sprite::Update(){
	
	SpriteVertex	*tmp;
	HRESULT			rv;

	rv = lpVert->Lock(0,0,(void**)&tmp,0);
	if(FAILED(rv)){
		_stprintf_s(spbuf, 200, TEXT("Error : can't lock vertex buffer (%s)"),DXGetErrorString9(rv));
		MessageBox(SpriteKernel->hWnd,spbuf,TEXT("Sprite"),MB_OK);
		return rv;
	}		

	
	DWORD i;
	float center_x,center_y;
	float dx,dy,ndx,ndy;
	float ca,sa;
	float sw,sh;

	Vector3 vct1;
	
	if(SpriteTexture){
		sw = (u1-u0) * fabs(fScaleX) * SpriteTexture->dwWidth;
		sh = (v1-v0) * fabs(fScaleY) * SpriteTexture->dwHeight;
	}
	else {
		sw = fabs(fScaleX) * fWidth * SpriteKernel->VParams.VideoMode.Width;
		sh = fabs(fScaleY) * fHeight * SpriteKernel->VParams.VideoMode.Height;
	}
	
	center_x = fPosX0 * SpriteKernel->VParams.VideoMode.Width  + 0.0f;
	center_y = fPosY0 * SpriteKernel->VParams.VideoMode.Height + 0.0f;

	tmp[0].SCoor[1] = center_y - fImagePosY0* sh; 
	tmp[1].SCoor[1] = tmp[0].SCoor[1]; 
	tmp[3].SCoor[1] = tmp[0].SCoor[1];
		
	tmp[2].SCoor[1] = center_y + (1-fImagePosY0) * sh; 
	tmp[4].SCoor[1] = tmp[2].SCoor[1];
	tmp[5].SCoor[1] = tmp[2].SCoor[1];

	tmp[0].SCoor[0] = center_x - fImagePosX0 * sw; 
	tmp[2].SCoor[0] = tmp[0].SCoor[0];
	tmp[5].SCoor[0] = tmp[0].SCoor[0];

	tmp[1].SCoor[0] = center_x + (1.0f - fImagePosX0) * sw; 
	tmp[3].SCoor[0] = tmp[1].SCoor[0];
	tmp[4].SCoor[0] = tmp[1].SCoor[0];

	tmp[0].T1Coor[0] = tmp[2].T1Coor[0] = tmp[5].T1Coor[0] = u0; 
	tmp[3].T1Coor[0] = tmp[4].T1Coor[0] = tmp[1].T1Coor[0] = u1; 
		
	tmp[0].T1Coor[1] = tmp[1].T1Coor[1] = tmp[3].T1Coor[1] = v0; 
	tmp[5].T1Coor[1] = tmp[4].T1Coor[1] = tmp[2].T1Coor[1] = v1; 
	
	for(i=0;i<6;i++){
		tmp[i].Color    = dwColor;
		tmp[i].SCoor[2] = fPosZ0;
	}
	
	
	if(fScaleX < 0.0f){
		
		// необходимо поменять местами некоторые вершины
		// 0   13
		// 25   4
		
		copyVector3(tmp[0].SCoor, vct1);
				
		// копируем в 0 то что было в вершине 3
		copyVector3(tmp[3].SCoor, tmp[0].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[3].SCoor);
		
		// копируем в 1 то что было в 0
		copyVector3(vct1, tmp[1].SCoor);
		
		copyVector3(tmp[4].SCoor, vct1);
		
		// копируем в 5 то что было в 2
		copyVector3(tmp[2].SCoor, tmp[4].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[2].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[5].SCoor);
		
	}

	if(fScaleY < 0.0f){
		
		// меняем все по у
		copyVector3(tmp[0].SCoor, vct1);
		
		// копируем в 0 то что было в вершине 3
		copyVector3(tmp[2].SCoor, tmp[0].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[2].SCoor);
		
		// копируем в 1 то что было в 0
		copyVector3(vct1, tmp[5].SCoor);
		
		copyVector3(tmp[4].SCoor, vct1);
		
		// копируем в 5 то что было в 2
		copyVector3(tmp[1].SCoor, tmp[4].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[1].SCoor);
		
		// копируем в 3 то что было в 0
		copyVector3(vct1, tmp[3].SCoor);
	
	}
	
	if(fRotationAngle != 0.0f){
			
		ca = (float)cos(fRotationAngle);
		sa = (float)sin(fRotationAngle);
		
		for(i=0;i<6;i++){
		
			dx = tmp[i].SCoor[0] - center_x;
			dy = tmp[i].SCoor[1] - center_y;

			ndx = dx * ca - dy * sa;
			ndy = dx * sa + dy * ca;
		
			tmp[i].SCoor[0] = center_x + ndx;
			tmp[i].SCoor[1] = center_y + ndy;
		}
	}

	
	lpVert->Unlock();

	return D3D_OK;
}

void Sprite::Copy(const Sprite &sp){
	
	this->SpriteKernel   = sp.SpriteKernel;
	this->SpriteTexture  = sp.SpriteTexture;
	this->dwColor		 = sp.dwColor;
	this->fPosX0		 = sp.fPosX0;
	this->fPosY0		 = sp.fPosY0;
	this->fPosZ0		 = sp.fPosZ0;
	this->fImagePosX0	 = sp.fImagePosX0;
	this->fImagePosY0	 = sp.fImagePosY0;
	this->fRotationAngle = sp.fRotationAngle;
	this->fScaleX		 = sp.fScaleX;
	this->fScaleY		 = sp.fScaleY;
	this->u0			 = sp.u0;
	this->v0			 = sp.v0;
	this->u1			 = sp.u1;
	this->v1             = sp.v1;

}

void Sprite::CopyWithoutPos(const Sprite &sp){
	
	this->SpriteKernel   = sp.SpriteKernel;
	this->SpriteTexture  = sp.SpriteTexture;
	this->dwColor		 = sp.dwColor;
	this->fImagePosX0	 = sp.fImagePosX0;
	this->fImagePosY0	 = sp.fImagePosY0;
	this->fRotationAngle = sp.fRotationAngle;
	this->fScaleX		 = sp.fScaleX;
	this->fScaleY		 = sp.fScaleY;
	this->u0			 = sp.u0;
	this->v0			 = sp.v0;
	this->u1			 = sp.u1;
	this->v1             = sp.v1;

}

HRESULT Sprite::Render(){
	
	HRESULT rv;
	LPDIRECT3DTEXTURE9	lpText;
	//D3DMATERIAL9		mat;

	rv = SpriteKernel->lpDevice->GetTexture(0, (IDirect3DBaseTexture9**)&lpText);
	if(FAILED(rv))
		return rv;
	
	if(this->SpriteTexture == NULL){
		rv = SpriteKernel->lpDevice->SetTexture(0, NULL);
		if(FAILED(rv))
			return rv;		
	
		/*
		ZeroMemory(&mat, sizeof(D3DMATERIAL9));
		
		mat.Diffuse.a = (dwColor >> 24) / 255.0f;
		mat.Diffuse.r = ((dwColor >> 16) & 0xff) / 255.0f;
		mat.Diffuse.g = ((dwColor >> 8) & 0xff) / 255.0f;
		mat.Diffuse.b = ((dwColor) & 0xff) / 255.0f;
	
		SpriteKernel->lpDevice->SetMaterial(&mat);
		*/
	}
		
	else if(lpText != this->SpriteTexture->lpTexture){
		rv = SpriteKernel->lpDevice->SetTexture(0, SpriteTexture->lpTexture);
		if(FAILED(rv))
			return rv;
	}
		
	rv = SpriteKernel->lpDevice->SetStreamSource(0,lpVert,0 ,sizeof(SpriteVertex));
	if(FAILED(rv))
		return rv;
	
	rv = SpriteKernel->lpDevice->SetFVF(SPRITEVERTEX);
	if(FAILED(rv))
		return rv;
	
	rv = SpriteKernel->lpDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	if(FAILED(rv))
		return rv;

	return D3D_OK;
}

bool Sprite::HitTestEx(float x, float y){
	
	if(!HitTest(x, y))return false;
	
	// hit test texture
	HRESULT rv;
	float localx, localy;
	D3DSURFACE_DESC	sDesc;
	D3DLOCKED_RECT	lRect;
	BYTE			*surfBytes;
	bool			bResult = false;

	ScreenToLocal(x,y,&localx, &localy);
		
	if(SpriteTexture && SpriteTexture->lpTexture){ // process texture
	
		rv = SpriteTexture->lpTexture->GetLevelDesc(0,&sDesc);
		if(FAILED(rv))
			return false;
		
		rv = SpriteTexture->lpTexture->LockRect(0,&lRect,NULL,NULL);
		if(FAILED(rv))
			return false;
	
		surfBytes = (BYTE*)lRect.pBits;

		if(sDesc.Format == D3DFMT_A8R8G8B8){
			surfBytes += (DWORD)((localy * sDesc.Width + localx) * 4);
			if(*surfBytes != 0x00)
				bResult = true;
		}
		
		else if(sDesc.Format == D3DFMT_A1R5G5B5 || sDesc.Format == D3DFMT_A4R4G4B4){
			surfBytes += (DWORD)((localy * sDesc.Width + localx) * 2);
			
			if(sDesc.Format == D3DFMT_A1R5G5B5){
				if((*surfBytes & 0x80) != 0x00)
					bResult = true;
			}

			else if(sDesc.Format == D3DFMT_A4R4G4B4){
				if((*surfBytes & 0xf0) != 0x00)
					bResult = true;
			}
		}
		
		SpriteTexture->lpTexture->UnlockRect(0);
		return bResult;
	}

	return false;
}

void Sprite::ScreenToLocal(float x, float y, float *loc_x, float *loc_y){
	
	float center_x,center_y; 
	float dx, dy;

	float ca, sa;
	
	center_x = fPosX0 * SpriteKernel->VParams.VideoMode.Width  + 0.0f;
	center_y = fPosY0 * SpriteKernel->VParams.VideoMode.Height + 0.0f;
	
	dx = x - center_x;
	dy = y - center_y;

	ca = cos(fRotationAngle);
	sa = sin(fRotationAngle);

	x  = dx * ca + dy * sa;
	y  = dx * sa + dy * ca;

	x /= fScaleX;
	y /= fScaleY;

	x -= fImagePosX0 * SpriteKernel->VParams.VideoMode.Width;
	y -= fImagePosY0 * SpriteKernel->VParams.VideoMode.Height;

	*loc_x = x;
	*loc_y = y;

	
	return ;
}

bool Sprite::HitTest(float x,float y){
	
	float center_x,center_y;
	float dx,dy;
	float ca,sa;
	float sw,sh;

	sw = (u1-u0) * fScaleX * SpriteTexture->dwWidth;
	sh = (v1-v0) * fScaleY * SpriteTexture->dwHeight;
	
	center_x = fPosX0 * SpriteKernel->VParams.VideoMode.Width  + 0.0f;
	center_y = fPosY0 * SpriteKernel->VParams.VideoMode.Height + 0.0f;

	FRECT zone; 
	
	zone.v0 = center_y - fImagePosY0 * sh; 
	zone.v1 = center_y + (1-fImagePosY0) * sh; 		
	
	zone.u0 = center_x - fImagePosX0 * sw; 
	zone.u1 = center_x + (1 - fImagePosX0 ) * sw; 

	if(fRotationAngle){
		ca = cos(-fRotationAngle);
		sa = sin(-fRotationAngle);
		
		dx = x - center_x;
		dy = y - center_y;
	
		x = dx * ca - dy * sa;
		y = dx * sa + dy * ca;
	
		x +=center_x;
		y +=center_y;
	}

	if(x >= zone.u0 && x<= zone.u1 &&
		y >= zone.v0 && y<= zone.v1)return true;

	return false;
}

/*
rv = SpriteKernel->lpDevice->BeginStateBlock();
	if(FAILED(rv))return rv;
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_USEW);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);
    SpriteKernel->lpDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_AMBIENT,0xff404040);	
	SpriteKernel->lpDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ALPHAREF,0x01);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_CLIPPING,TRUE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_EDGEANTIALIAS,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_CLIPPLANEENABLE,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_VERTEXBLEND,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
	SpriteKernel->lpDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);	
	SpriteKernel->lpDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,SpriteKernel->VParams.dwFiltrationMetod);	
*/


void Sprite::ApplyAnimWithoutPos(SpriteAnimationParams &spAnimPars){

	this->fPosZ0			= spAnimPars.fPosZ0;
	this->fImagePosX0		= spAnimPars.fImagePosX0;
	this->fImagePosY0       = spAnimPars.fImagePosY0;
	this->fRotationAngle    = spAnimPars.fRotationAngle;
	this->fScaleX			= spAnimPars.fScaleX;
	this->fScaleY			= spAnimPars.fScaleY;
	this->dwColor			= spAnimPars.dwColor;
	this->u0				= spAnimPars.u0;
	this->u1				= spAnimPars.u1;
	this->v0				= spAnimPars.v0;
	this->v1				= spAnimPars.v1;	
	
	this->fWidth			= spAnimPars.fWidth;
	this->fHeight			= spAnimPars.fHeight;

	return ;
}
