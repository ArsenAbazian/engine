#ifndef VIDEOPAR_INC
#define VIDEOPAR_INC

#include <windows.h>
#include <d3d9.h>

typedef struct{
	int						index;	     // индекс видеоадаптера
	D3DADAPTER_IDENTIFIER9	identifier;  // описание видеоадаптера
	D3DDISPLAYMODE          curr_mode;   // текущий режим
}VAdapter; // видеоадаптер

typedef struct{
	D3DCAPS9        caps;
	char           *desc;
	bool            present;
}VDevice; // устройство рендеринга

typedef struct{
	D3DDISPLAYMODE desc;
	int            bpp;
	char           name[64];
}VMode;

typedef struct{
	D3DFORMAT		fmt;	
	char			name[64];
}VFormat;

class VAdapters{ // список видеоадаптеров
public:
	LPDIRECT3D9	lpd3d9;	
	VAdapter   *adapters;
	DWORD       count;
	DWORD       current;

	VAdapters();
   ~VAdapters();
	
    HRESULT		init(LPDIRECT3D9 plpd3d9);
	VAdapter*   operator[](DWORD index){
					if(index < count)return &(adapters[index]);
					else return NULL;
				}
	VAdapter*   curr(){
					if(current < count)return &(adapters[current]);
					else return NULL;
				}
	void clear();
};  

class VDevices{ // список устройств рендеринга
public:
	LPDIRECT3D9	lpd3d9;
	VDevice     devices[2];
	DWORD       adapter;
	DWORD       count;
	DWORD       current;

	VDevices();
   ~VDevices();
	
    HRESULT   init(LPDIRECT3D9 plpd3d9,int ad_index);
	VDevice*  operator[](DWORD index){
					if(index < count)return &(devices[index]);
					else return NULL;
				}
	VDevice*  curr(){
					if(current >0 && current < count)return &(devices[current]);
					else return NULL;
				}
};

class VModes{ // список видеорежимов для конкретного устройства рендеринга
public:
	LPDIRECT3D9		lpd3d9;  
	DWORD			adapter;
	DWORD			count;
	DWORD			current;
	VMode          *modes;
	
	VModes();
   ~VModes();

	HRESULT init(LPDIRECT3D9 plpd3d9,int vd_index);
	VMode* operator[](DWORD index){
						if(index < count)return &(modes[index]);
						else return NULL;
					}
	VMode* curr(){
						if(current > 0 && current < count)return &(modes[current]);
						else return NULL;
					}
	void sort();
};

#define VFORMAT_ZBUFFER	1
#define VFORMAT_TEXTURE 2

class VFormats{
public:
	LPDIRECT3D9		lpd3d9;  
	DWORD			va;
	VDevice			*vd;
	VMode			*md;
	DWORD			count;
	DWORD			current;
	DWORD			format_type;
	VFormat			*format;
	VFormats();
	~VFormats();
	
	HRESULT			init(LPDIRECT3D9 plpd3d9,DWORD va,VDevice *pvd,VMode *pmd,DWORD fmt_type);
	VFormat*		operator[](DWORD i){
						if(i < count)return &format[i];
						else return NULL;
					}
	VFormat*		curr(){
						if(current < count)return &format[current];
						else return NULL;
					}
	void			clear();
};

typedef struct{
	D3DTEXTUREFILTERTYPE	tftype;
	char					name[64];
}TFType;

class TFTypes{
public:
	LPDIRECT3D9		lpd3d9;  
	DWORD			va;
	VDevice			*vd;
	VMode			*md;
	DWORD			count;
	DWORD			current;
	TFType			*filter_type;
	
	TFTypes();
	~TFTypes();

	HRESULT			init(LPDIRECT3D9 plpd3d9,DWORD va,VDevice *pvd,VMode *pmd);
	
	TFType*			operator[](DWORD i){
		if(i < count)return &filter_type[i];
		else return NULL;
	}
	TFType*		curr(){
		if(current < count)return &filter_type[current];
		else return NULL;
	}
	void			clear();
};

DWORD    FormatToBpp(D3DFORMAT d3dfm);
void format_to_zstencil(D3DFORMAT fmt, char *buf);
void format_to_texture(D3DFORMAT fmt,char *buf);
void format_to_filter_type(D3DTEXTUREFILTERTYPE ftype,char *name);

#endif

