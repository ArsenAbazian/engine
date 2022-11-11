#include "kernel.h"
#include <dxerr9.h>

TCHAR* Kernel::tempBuffer = new TCHAR[Kernel::bufferSize];

Kernel::Kernel(){
	hWnd		= NULL;
	lpd3d9		= NULL;
	lpDevice    = NULL;
}


HRESULT Kernel::Init(HWND hwnd, TCHAR *cfgfile){
	
	HRESULT					rv;
	RECT					rc;
	DWORD                   behavior;
	shString				video_str(TEXT("VideoConfiguration"));

	hWnd = hwnd; // инициализируем дескриптор окна
	
	// загружаем конфигурацию
	Config	cfg;

	if(!cfg.Load(cfgfile))return D3DERR_INVALIDCALL;
				
	memcpy(&VParams, &cfg.VParams, sizeof(VideoParams));	
	
	//VParams.bFullscreen = true;
	
	// инициализируем объект LPDIRECT3D
	lpd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		
	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	
	if(VParams.bFullscreen)d3dpp.Windowed = false;	
	else d3dpp.Windowed = true;
	
	d3dpp.BackBufferCount          = 1;
	d3dpp.hDeviceWindow            = hWnd;
	d3dpp.MultiSampleType          = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect               = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil   = true;
	d3dpp.Flags					   = 0; // D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	

	if(d3dpp.Windowed){
		SetWindowPos( this->hWnd, NULL, 0, 0, this->VParams.VideoMode.Width, this->VParams.VideoMode.Height, SWP_NOZORDER );
		GetClientRect(hWnd,&rc);	
		
		rv = lpd3d9->GetAdapterDisplayMode(VParams.dwAdapter,&VParams.VideoMode);
		if(FAILED(rv)){ // если ошибка 
			lpd3d9->Release();
			lpd3d9 = NULL;
			_stprintf_s(this->tempBuffer, 256, TEXT("Error : can't get adapter display mode  (%s)"),DXGetErrorString9(rv));
			MessageBox( TEXT("Kernel"), MB_OK );
			return rv;
		}
	
		// инициализируем размер заднего буффера
		
		VParams.VideoMode.Width  = rc.right;
		VParams.VideoMode.Height = rc.bottom;
	
		d3dpp.BackBufferWidth  = VParams.VideoMode.Width;
		d3dpp.BackBufferHeight = VParams.VideoMode.Height;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.AutoDepthStencilFormat		= VParams.ZBufferFormat;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	
		rv = lpd3d9->CheckDeviceFormat(VParams.dwAdapter,
										VParams.DeviceType,
										VParams.VideoMode.Format,
										D3DUSAGE_DEPTHSTENCIL,
										D3DRTYPE_SURFACE,
										VParams.ZBufferFormat);
		if(FAILED(rv)){
			lpd3d9->Release();
			lpd3d9 = NULL;
		
			_stprintf_s( this->tempBuffer, 256, TEXT("Error : unsupported z buffer format (%s)"),DXGetErrorString9(rv));
			MessageBox( TEXT("Kernel"), MB_OK );
			return rv;
		}
		
		rv = lpd3d9->CheckDepthStencilMatch(VParams.dwAdapter,
											VParams.DeviceType,
										    VParams.VideoMode.Format,
										    VParams.VideoMode.Format,
										    d3dpp.AutoDepthStencilFormat);
		if(FAILED(rv)){
			lpd3d9->Release();
			lpd3d9 = NULL;
		
			_stprintf_s( this->tempBuffer, 256, TEXT("Error : z buffer not supported for this mode (%s)"),DXGetErrorString9(rv));
			MessageBox( TEXT("Kernel"), MB_OK );
			return rv;
		}
	
	}// если приложение оконное
	else { // если приложение полноэкранное
		d3dpp.BackBufferFormat				= VParams.VideoMode.Format;
		d3dpp.BackBufferHeight				= VParams.VideoMode.Height;
		d3dpp.BackBufferWidth				= VParams.VideoMode.Width;
		d3dpp.AutoDepthStencilFormat		= VParams.ZBufferFormat;
		d3dpp.FullScreen_RefreshRateInHz	= VParams.VideoMode.RefreshRate;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	}// если приложение полноэкранное
	
	rv = lpd3d9->GetDeviceCaps(VParams.dwAdapter,
							   VParams.DeviceType,
							   &Caps);
	
		
	if(FAILED(rv)){
		lpd3d9->Release();
		lpd3d9 = NULL;
		
		_stprintf_s( this->tempBuffer, 256, TEXT("Error : can't get device caps (%s)"),DXGetErrorString9(rv));
		MessageBox( TEXT("Kernel"), MB_OK );
		return rv;
	}
	
	if(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else 
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	rv = lpd3d9->CreateDevice(VParams.dwAdapter,
								VParams.DeviceType,
								hWnd,
								behavior,
								&d3dpp,
								&lpDevice);
	
	if(FAILED(rv)){
		lpd3d9->Release();
		lpd3d9 = NULL;
		
		_stprintf_s( this->tempBuffer, 256, TEXT("Error : can't create device (%s)"),DXGetErrorString9(rv));
		MessageBox( TEXT("Kernel"), MB_OK );
		return rv;
	}
	
	return rv;
}

Kernel::Kernel(HWND hwnd,TCHAR *cfgfile){
	Init(hwnd,cfgfile);
}

Kernel::~Kernel(){
	if(lpd3d9)lpd3d9->Release();
	if(lpDevice)lpDevice->Release();
}

void Kernel::Clear(){
	if(lpd3d9)lpd3d9->Release();
	if(lpDevice)lpDevice->Release();

	ZeroMemory(this,sizeof(Kernel));
}

HRESULT	Kernel::Restore(){
	HRESULT rv;
	
	//LPDIRECT3DSURFACE9	lpBack;
	
	rv = lpDevice->Reset(&d3dpp);	
	if(FAILED(rv))return rv;
	
	/*
	rv = lpDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&lpBack);
	if(FAILED(rv))return rv;
	
	lpBack->Release();
	*/
	
	return rv;
}

DWORD Kernel::DxErrorBox( TCHAR *caption, TCHAR *description, HRESULT rv ) {  
	
	_stprintf_s( this->tempBuffer, this->bufferSize, TEXT("%s rv = 0x%8.8x -> %s"), description, rv, DXGetErrorString9(rv) );
	return this->MessageBox( this->tempBuffer, caption, MB_ICONERROR );
}

DWORD Kernel::ErrorBox( TCHAR *caption, TCHAR *format, ... ) { 
	
	va_list args;

	va_start( args, format );
	_vstprintf_s( this->tempBuffer, this->bufferSize, format, args );
	
	return this->MessageBox( this->tempBuffer, caption, MB_ICONERROR );
}		
