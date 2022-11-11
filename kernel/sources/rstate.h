#ifndef ARS_RENDER_STATE_INC
#define ARS_RENDER_STATE_INC

#include "kernel.h"
#include "stblock.h"
#include "hstring.h"


class RenderState{

	LPDIRECT3DSTATEBLOCK9		RSBlock;
	LPDIRECT3DSTATEBLOCK9		RSSaved;
public:

	Kernel			*RSKernel;

	RenderState(){ ZeroMemory(this, sizeof(RenderState)); }
	~RenderState(){ Clear(); }

	void		SetKernel(Kernel *krn) { RSKernel = krn; } 
	void		Clear() ;
	
	HRESULT     Create(D3DSTATEBLOCKTYPE Type) {
							if(RSKernel && RSKernel->lpDevice)
								return RSKernel->lpDevice->CreateStateBlock(Type, (IDirect3DStateBlock9**)&RSBlock);
						}
	HRESULT		Begin() {
							if(RSKernel && RSKernel->lpDevice)
								return RSKernel->lpDevice->BeginStateBlock();
							
							return D3DERR_INVALIDCALL;
						}
	HRESULT		End(DWORD wich) {
								if(RSKernel && RSKernel->lpDevice){
									if(wich == 1){
										return RSKernel->lpDevice->EndStateBlock((IDirect3DStateBlock9 **)&RSBlock);	
									}
									else {
										return RSKernel->lpDevice->EndStateBlock((IDirect3DStateBlock9 **)&RSSaved);
									}
								}
							return D3DERR_INVALIDCALL;
						}
	HRESULT		Apply() { 
							if(RSBlock)
								return RSBlock->Apply();

							return D3DERR_INVALIDCALL;
						}

	HRESULT		Restore() { 
							if(RSSaved)
								return RSBlock->Apply();

							return D3DERR_INVALIDCALL;
						}
	HRESULT		Capture() {
							
							if(RSSaved)
								return RSSaved->Capture();

							return D3DERR_INVALIDCALL;
						}
	HRESULT		GetDevice(LPDIRECT3DDEVICE9 &lpd3d9) {
							if(RSBlock)
								return RSBlock->GetDevice((IDirect3DDevice9**)&lpd3d9);

							return D3DERR_INVALIDCALL;
						}
	
	HRESULT		LoadFromFile(TCHAR *filename);
	HRESULT		MakeSpriteRenderState();

};

#endif