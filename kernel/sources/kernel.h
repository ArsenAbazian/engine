#ifndef G3DE_KERNEL_INC
#define G3DE_KERNEL_INC

#include "config.h"

#include "hstring.h"
#include "d3d9.h"

class Kernel{
	D3DPRESENT_PARAMETERS	d3dpp;
	
public:	
		
	HWND				hWnd;				// хендл окна
	LPDIRECT3D9			lpd3d9;				// указатель на интерфейс 
	LPDIRECT3DDEVICE9	lpDevice;			// указатель на интерфейс
	D3DCAPS9			Caps;
	static TCHAR		*tempBuffer;		// сюда прозиводим запись
	static const int	bufferSize = 1024;	// размер

	VideoParams			VParams;
		
	Kernel();
	Kernel(HWND hwnd,TCHAR *cfgfile);
	~Kernel();

	HRESULT Init(HWND hwnd,TCHAR *cfgfile);		// инициализируется из файла конфигурации
	
	void    Clear();							// очищает
	HRESULT Restore();
	DWORD MessageBox( TCHAR *desc, TCHAR *caption, DWORD flags ) { return ::MessageBox( hWnd, desc, caption, flags ); }
	DWORD MessageBox( TCHAR *caption, DWORD flags ) { return ::MessageBox( hWnd, this->tempBuffer, caption, flags ); }
	DWORD ErrorBox( TCHAR *caption ) { return ::MessageBox( hWnd, this->tempBuffer, caption, MB_ICONERROR ); }
	DWORD DxErrorBox( TCHAR *caption, TCHAR *description, HRESULT rv );
	DWORD ErrorBox( TCHAR *caption, TCHAR *format, ... );
};

#endif
