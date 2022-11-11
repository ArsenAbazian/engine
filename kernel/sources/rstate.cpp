#include "rstate.h"
#include <dxerr9.h>

BYTE	checkByte18;

WORD	checkWord15;
#define CHECK_WORD15

WORD	checkWord15_1;
#define CHECK_WORD15_1

BYTE	checkByte1_9;

void RenderState::Clear(){

	if(RSBlock){
		RSBlock->Release();
		RSBlock = NULL;
	}
	if(RSSaved){
		RSSaved->Release();
		RSSaved = NULL;
	}
}


HRESULT RenderState::LoadFromFile(TCHAR *filename){

	char		cf[1024];
	TCHAR		rsbuf[1024];
	HRESULT		rv;
	
	wcsToStr(filename, cf);
	StateBlockInfo	SBInfo;
	if( !SBInfo.load(cf) )
		return false;

	
	if(RSKernel == NULL)return D3DERR_INVALIDCALL;

	int		i, j, k;
	
	Clear();
	
	DWORD	*values;
	char    **dbg_values;
	DWORD	count;
	
	for(k=0;k<2;k++){
		
		rv = this->Begin();
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: can't begin state block (%s)"),DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::LoadFromFile"), MB_ICONERROR);
            return rv;
		}
		// начинаем заполнение renderstate 
		for(i=0; i<103;i++){
			if(SBInfo.rsenabled[i]){ // если надо записать 
				
				SBInfo.get_rs_values(i, &values,&dbg_values,&count);
				if(RenderStateFunc[i] == SB_DWORD || RenderStateFunc[i] == SB_FLOAT || RenderStateFunc[i] == SB_INT || RenderStateFunc[i] == SB_COLOR || 
						(i >=31 && i<=38) ||
						(i == 51) ||
						(i >=92 && i<=99) ||
						(i == 65) ||
						(i >=85 && i<=87)){
					
					rv = RSKernel->lpDevice->SetRenderState(RenderStateType[i], SBInfo.rstate_value[i].dvalue );
				}
				else {
					rv = RSKernel->lpDevice->SetRenderState(RenderStateType[i], values[SBInfo.rstate_value[i].dvalue]);				
					
				}
				
				if(FAILED(rv)){
					_stprintf_s(rsbuf, 1024, TEXT("Error: can't set render state %s (%s)"),DBGRenderStateType[i], DXGetErrorString9(rv));
					MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::LoadFromFile"), MB_ICONERROR);
					return rv;
				}
			}
		}

		// начинаем заполнение texturestagestate
		for(i=0; i<8;i++){ // по числу всевозможных текстур
			if(SBInfo.tsenabled[i]){ // если для конкретного уровня текстуры
				for(j=0; j< 18; j++){
					if(SBInfo.tssenabled[i][j]){ // если надо записать 
						
						SBInfo.get_tss_values(j, &values, &dbg_values, &count);

						if(TextureStageStateFunc[j] == SB_TEXDWORD || TextureStageStateFunc[j] == SB_TEXFLOAT || TextureStageStateFunc[j] == SB_TEXBOOL || TextureStageStateFunc[j] == SB_TEXCOLOR || TextureStageStateFunc[j] == SB_TEXCOORINDEX){
							rv = RSKernel->lpDevice->SetTextureStageState(i, TextureStageStateType[j], SBInfo.tsstate_value[i][j].dvalue);
						}
						else {
							rv = RSKernel->lpDevice->SetTextureStageState(i, TextureStageStateType[j], values[SBInfo.tsstate_value[i][j].dvalue]);	
						}
						
						if(FAILED(rv)){
							_stprintf_s(rsbuf, 1024, TEXT("Error: can't set texture stage state %s (%s)"),DBGTextureStageStateType[i], DXGetErrorString9(rv));
							MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::LoadFromFile"), MB_ICONERROR);
							return rv;
						}
					}
				}
			
				for(j=0; j<13;j++){
					if(SBInfo.ssenabled[j]){ // если надо записать 
						
						SBInfo.get_ss_values(j, &values, &dbg_values, &count);
						
						if(SamplerStateFunc[j] == SB_SMRFLOAT || SamplerStateFunc[j] == SB_SMRDWORD || SamplerStateFunc[j] == SB_SMRBOOL || SamplerStateFunc[j] == SB_SMRCOLOR){
							rv = RSKernel->lpDevice->SetSamplerState(i, SamplerStateType[j], SBInfo.sstate_value[j].dvalue);
						}
						else {
							rv = RSKernel->lpDevice->SetSamplerState(i, SamplerStateType[j], values[SBInfo.sstate_value[j].dvalue]);
						}
						if(FAILED(rv)){
							_stprintf_s(rsbuf, 1024, TEXT("Error: can't set sampler state %s = %s (%s) for %s"),DBGSamplerStateType[j], dbg_values[ SBInfo.sstate_value[j].dvalue ], DXGetErrorString9(rv), SBInfo.name );
							MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::LoadFromFile"), MB_ICONERROR);
							return rv;
						}
					}
				}
			}
		}
		rv = End(k);
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: can't end state block (%s)"),DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::LoadFromFile"), MB_ICONERROR);
            return rv;
		}
	}
		
	return rv;
}

HRESULT RenderState::MakeSpriteRenderState(){

	TCHAR		rsbuf[1024];
	HRESULT		rv;
	int			i;

	Clear();

	if(RSKernel == NULL)return D3DERR_INVALIDCALL;
	
	for(i=0;i<2;i++){

		rv = this->Begin();
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: can't begin state block (%s)"),DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		
	
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_USEW );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_ZENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}

		rv = RSKernel->lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_ALPHABLENDENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_SRCBLEND"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_DESTBLEND"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		
		
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_ALPHATESTENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_ALPHAREF"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_ALPHAFUNC"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}

		rv = RSKernel->lpDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_FILLMODE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_CULLMODE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_STENCILENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_CLIPPING"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_CLIPPLANEENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_VERTEXBLEND"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_INDEXEDVERTEXBLENDENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_FOGENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetRenderState( D3DRS_COLORWRITEENABLE,
											D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN |
											D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set render state %s (%s)"),TEXT("D3DRS_COLORWRITEENABLE"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_COLOROP"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_COLORARG1"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_COLORARG2"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}

		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_ALPHAOP"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_ALPHAARG1"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    	if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_ALPHAARG2"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}	
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_TEXCOORDINDEX"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("0, D3DTSS_TEXTURETRANSFORMFLAGS"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("1, D3DTSS_COLOROP"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set texture stage state %s (%s)"),TEXT("1, D3DTSS_ALPHAOP"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}

		rv = RSKernel->lpDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set sampler stage state %s (%s)"),TEXT("0, D3DSAMP_MINFILTER"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set sampler stage state %s (%s)"),TEXT("0, D3DSAMP_MAGFILTER"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
		rv = RSKernel->lpDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: set sampler stage state %s (%s)"),TEXT("0, D3DSAMP_MIPFILTER"), DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}

		rv = this->End(i);
		if(FAILED(rv)){
			_stprintf_s(rsbuf, 1024, TEXT("Error: can't end state block (%s)"),DXGetErrorString9(rv));
			MessageBox(RSKernel->hWnd, rsbuf, TEXT("RenderState::MakeSpriteRenderState"), MB_ICONERROR);
            return rv;
		}
	}

	return D3D_OK;
}