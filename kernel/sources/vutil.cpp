#include "vutil.h"
#include <stdio.h>
#include <dxerr9.h>

// перегоняет формат разрешения в число бит на пиксел
DWORD    FormatToBpp(D3DFORMAT d3dfm){
	switch(d3dfm){
		case 0:
			return 0;
		
		case D3DFMT_X8R8G8B8:
			return 32;
		
		case D3DFMT_R5G6B5:
			return 16;
	}
	return 0;
}

void format_to_zstencil(D3DFORMAT fmt, char *buf){

	switch(fmt){
		case D3DFMT_D16_LOCKABLE:
			sprintf_s(buf, 256, "D16_LOCKABLE");
		break;
		case D3DFMT_D32:
			sprintf_s(buf, 256, "D32");
		break;
		case D3DFMT_D15S1:
			sprintf_s(buf, 256, "D15S1");
		break;
		case D3DFMT_D24S8:
			sprintf_s(buf, 256, "D24S8");
		break;
		case D3DFMT_D24X8:
			sprintf_s(buf, 256, "D24X8");
		break;
		case D3DFMT_D24X4S4:
			sprintf_s(buf, 256, "D24X4S4");
		break;
		case D3DFMT_D32F_LOCKABLE:
			sprintf_s(buf, 256, "D32F_LOCKABLE");
		break;
		case D3DFMT_D24FS8:
			sprintf_s(buf, 256, "D24FS8");
		break;
		case D3DFMT_D16:	
			sprintf_s(buf, 256, "D16");
		break;
		default:
			sprintf_s(buf, 256, "UNKNOWN");
		break;
	}
}

void format_to_filter_type(D3DTEXTUREFILTERTYPE ftype,char *name){

	switch(ftype){	
		case D3DTEXF_NONE:
			sprintf_s(name, 256, "D3DTEXF_NONE");
		break;
		case D3DTEXF_POINT:
			sprintf_s(name, 256, "D3DTEXF_POINT");
		break;
		case D3DTEXF_LINEAR:
			sprintf_s(name, 256, "D3DTEXF_LINEAR");
		break;
		case D3DTEXF_ANISOTROPIC:
			sprintf_s(name, 256, "D3DTEXF_ANISOTROPIC");
		break;
		case D3DTEXF_PYRAMIDALQUAD:
			sprintf_s(name, 256, "D3DTEXF_PYRAMIDALQUAD");
		break;
		case D3DTEXF_GAUSSIANQUAD:
			sprintf_s(name, 256, "D3DTEXF_GAUSSIANQUAD");
		break;
		default:
			sprintf_s(name, 256, "UNKNOWN");
		break;
	}
	return ;
}

void format_to_texture(D3DFORMAT fmt,char *buf){
	
	switch(fmt){
		case D3DFMT_R8G8B8:
			sprintf_s(buf, 256, "R8G8B8");
		break;
		case D3DFMT_A8R8G8B8:
			sprintf_s(buf, 256, "A8R8G8B8");
		break;
		case D3DFMT_X8R8G8B8:
			sprintf_s(buf, 256, "X8R8G8B8");
		break;
		case D3DFMT_R5G6B5:
			sprintf_s(buf, 256, "R5G6B5");
		break;
		case D3DFMT_X1R5G5B5:
			sprintf_s(buf, 256, "X1R5G5B5");
		break;
		case D3DFMT_A1R5G5B5:
			sprintf_s(buf, 256, "A1R5G5B5");
		break;
		case D3DFMT_A4R4G4B4:
			sprintf_s(buf, 256, "A4R4G4B4");
		break;
		case D3DFMT_R3G3B2:
			sprintf_s(buf, 256, "R3G3B2");
		break;
		case D3DFMT_A8:
			sprintf_s(buf, 256, "A8");
		break;
		case D3DFMT_A8R3G3B2:
			sprintf_s(buf, 256, "A8R3G3B2");
		break;
		case D3DFMT_X4R4G4B4:
			sprintf_s(buf, 256, "X4R4G4B4");
		break;
		case D3DFMT_A2B10G10R10:
			sprintf_s(buf, 256, "A2B10G10R10 (RIGHT)");
		break;
		case D3DFMT_A8B8G8R8:
			sprintf_s(buf, 256, "A8R8G8B8 (RIGHT)");
		break;
		case D3DFMT_X8B8G8R8:
			sprintf_s(buf, 256, "X8R8G8B8 (RIGHT)");
		break;
		case D3DFMT_G16R16:
			sprintf_s(buf, 256, "G16R16");
		break;
		case D3DFMT_A2R10G10B10:
			sprintf_s(buf, 256, "A2R10G10B10");
		break;
		case D3DFMT_A16B16G16R16:
			sprintf_s(buf, 256, "A16B16G16R16 (RIGHT)");
		break;
		case D3DFMT_A8P8:
			sprintf_s(buf, 256, "A8P8");
		break;
		case D3DFMT_P8:
			sprintf_s(buf, 256, "P8");
		break;
		case D3DFMT_L8:
			sprintf_s(buf, 256, "L8");
		break;
		case D3DFMT_L16:
			sprintf_s(buf, 256, "L16");
		break;
		case D3DFMT_A8L8:
			sprintf_s(buf, 256, "A8L8");
		break;
		case D3DFMT_A4L4:
			sprintf_s(buf, 256, "A4L4");
		break;
		default:
			sprintf_s(buf, 256, "UNKNOWN");
		break;
	}
}

void VAdapters::clear(){
	if(adapters)delete[] adapters;
	lpd3d9   = NULL;
	count    = 0;
	current  = 0;
	adapters = NULL;
}

HRESULT VAdapters::init(LPDIRECT3D9 plpd3d9){
	UINT index;
	HRESULT rv;
	
	lpd3d9 = plpd3d9;
		
	count = lpd3d9->GetAdapterCount();
	adapters = new VAdapter[count];	
	
	for(index=0;index<count;index++){	
		rv = lpd3d9->GetAdapterIdentifier(index,0,&adapters[index].identifier);
		if(FAILED(rv))return rv;
		
		rv = lpd3d9->GetAdapterDisplayMode(index,&adapters[index].curr_mode);
		if(FAILED(rv))return rv;
		
		adapters[index].index = index; 
	}
	
	return D3D_OK;
}

VAdapters::VAdapters(){
	lpd3d9   = NULL;
	count    = 0;
	current  = 0;
	adapters = NULL;
}

VAdapters::~VAdapters(){
	clear();
}

HRESULT VDevices::init(LPDIRECT3D9 plpd3d9,int ad_index){
	HRESULT rv;
	
	lpd3d9  = plpd3d9;
	adapter = ad_index;
	count   = 2;
		
	rv = lpd3d9->GetDeviceCaps(adapter,D3DDEVTYPE_HAL,&devices[0].caps); // получить описание устройства рендеринга
	if(rv == D3DERR_INVALIDDEVICE || rv == D3DERR_NOTAVAILABLE)devices[0].present = false;
	else if(FAILED(rv))return rv;
	else devices[0].present = true;
	
	rv = lpd3d9->GetDeviceCaps(adapter,D3DDEVTYPE_REF,&devices[1].caps);
	if(rv == D3DERR_INVALIDDEVICE || rv == D3DERR_NOTAVAILABLE)devices[1].present = false;
	else if(FAILED(rv))return rv;
	else devices[1].present = true;
	
	devices[0].desc = "HAL device"; 
	devices[1].desc = "REF device";
	
	return D3D_OK;
}

VDevices::VDevices(){
	lpd3d9  = NULL;
	count   = 0;
	current = 0;
	
	devices[0].present = false;
	devices[1].present = false;
}

VDevices::~VDevices(){}

HRESULT VModes::init(LPDIRECT3D9 plpd3d9,int va_index){
	UINT    index,ext,i;
	HRESULT rv;
	
	DWORD count1,count2;
	
	lpd3d9  = plpd3d9;
	adapter = va_index;

	count1 = count = lpd3d9->GetAdapterModeCount(adapter,D3DFMT_X8R8G8B8);
	count += count2 = lpd3d9->GetAdapterModeCount(adapter,D3DFMT_R5G6B5);
	modes = new VMode[count];	

	current = 0;
	for(index=0,i=0;i<count1;i++){
		rv = lpd3d9->EnumAdapterModes(adapter,D3DFMT_X8R8G8B8,i,&modes[index].desc); // получить описание видеорежима
		if(FAILED(rv))return rv;
	
		
		for(ext=0;ext<index;ext++){ // избавляемя от уже существующих режимов
			if(modes[ext].desc.Width == modes[index].desc.Width && 
				modes[ext].desc.Height == modes[index].desc.Height &&
				modes[ext].desc.Format == modes[index].desc.Format &&
				modes[ext].desc.RefreshRate == modes[index].desc.RefreshRate)break;
			
		}	
		if(ext == index){ // этого режима еще не было
		
			modes[index].bpp = FormatToBpp(modes[index].desc.Format);
			sprintf_s(modes[index].name, 64, "%d x %d  %d bit  %d hz",modes[index].desc.Width,
													 modes[index].desc.Height,
													 modes[index].bpp, modes[index].desc.RefreshRate);
			//modes[index].desc.RefreshRate = 0;
			index++;
		}	
	}
	for(i=0;i<count2;i++){
		rv = lpd3d9->EnumAdapterModes(adapter,D3DFMT_R5G6B5,i,&modes[index].desc); // получить описание видеорежима
		if(FAILED(rv))return rv;
		
		
		for(ext=0;ext<index;ext++){ // избавляемя от уже существующих режимов
			if(modes[ext].desc.Width == modes[index].desc.Width && 
				modes[ext].desc.Height == modes[index].desc.Height &&
				modes[ext].desc.Format == modes[index].desc.Format &&
				modes[ext].desc.RefreshRate == modes[index].desc.RefreshRate)break;

		}	
		if(ext == index){ // этого режима еще не было
		
			modes[index].bpp = FormatToBpp(modes[index].desc.Format);
			sprintf_s(modes[index].name, 64, "%d x %d  %d bit  %d hz",modes[index].desc.Width,
				modes[index].desc.Height,
				modes[index].bpp, modes[index].desc.RefreshRate);
			//modes[index].desc.RefreshRate = 0;
			index++;
		}	
	}
	
	count = index; // если режимы повторялись то число режимов изменилось
	
	// отсортируем режимы
	sort();
	
	return D3D_OK;
}

VModes::VModes(){
	lpd3d9  = NULL;
	count   = 0;
	current = 0;
	modes   = NULL;
}

VModes::~VModes(){
	if(modes)delete[] modes;
}

void VModes::sort(){
	UINT  i,j;
	VMode tmp;
	
	// сначала сортируем по глубине цвета
	for(i=0;i<count-1;i++){
		for(j=i+1;j<count;j++){
			if(modes[i].bpp > modes[j].bpp){
				tmp.bpp         = modes[j].bpp;
				tmp.desc.Format = modes[j].desc.Format;
				tmp.desc.Width  = modes[j].desc.Width; 
				tmp.desc.Height = modes[j].desc.Height;
				tmp.desc.RefreshRate = modes[j].desc.RefreshRate;

				modes[j].bpp         = modes[i].bpp;
				modes[j].desc.Format = modes[i].desc.Format;
				modes[j].desc.Width  = modes[i].desc.Width;
				modes[j].desc.Height = modes[i].desc.Height;
				modes[j].desc.RefreshRate = modes[i].desc.RefreshRate;
			
				modes[i].bpp         = tmp.bpp;
				modes[i].desc.Format = tmp.desc.Format;
				modes[i].desc.Width  = tmp.desc.Width;
				modes[i].desc.Height = tmp.desc.Height;
				modes[i].desc.RefreshRate = tmp.desc.RefreshRate;
			}
		}
	}

	// теперь сортируем по ширине
	for(i=0;i<count-1;i++){
		for(j=i+1;j<count;j++){
			if(modes[i].bpp == modes[j].bpp && 
				modes[i].desc.Width > modes[j].desc.Width){
				tmp.bpp         = modes[j].bpp;
				tmp.desc.Format = modes[j].desc.Format;
				tmp.desc.Width  = modes[j].desc.Width; 
				tmp.desc.Height = modes[j].desc.Height;
				tmp.desc.RefreshRate = modes[j].desc.RefreshRate;
				
				modes[j].bpp         = modes[i].bpp;
				modes[j].desc.Format = modes[i].desc.Format;
				modes[j].desc.Width  = modes[i].desc.Width;
				modes[j].desc.Height = modes[i].desc.Height;
				modes[j].desc.RefreshRate = modes[i].desc.RefreshRate;
			
				modes[i].bpp         = tmp.bpp;
				modes[i].desc.Format = tmp.desc.Format;
				modes[i].desc.Width  = tmp.desc.Width;
				modes[i].desc.Height = tmp.desc.Height;
				modes[i].desc.RefreshRate = tmp.desc.RefreshRate;	
			}
		}
	}
	
	// теперь сортируем по высоте
	for(i=0;i<count-1;i++){
		for(j=i+1;j<count;j++){
			if(modes[i].bpp == modes[j].bpp && 
				modes[i].desc.Width == modes[j].desc.Width &&
				modes[i].desc.Height > modes[j].desc.Height){
				tmp.bpp         = modes[j].bpp;
				tmp.desc.Format = modes[j].desc.Format;
				tmp.desc.Width  = modes[j].desc.Width; 
				tmp.desc.Height = modes[j].desc.Height;
				tmp.desc.RefreshRate = modes[j].desc.RefreshRate;
				
				modes[j].bpp         = modes[i].bpp;
				modes[j].desc.Format = modes[i].desc.Format;
				modes[j].desc.Width  = modes[i].desc.Width;
				modes[j].desc.Height = modes[i].desc.Height;
				modes[j].desc.RefreshRate = modes[i].desc.RefreshRate;
			
				modes[i].bpp         = tmp.bpp;
				modes[i].desc.Format = tmp.desc.Format;
				modes[i].desc.Width  = tmp.desc.Width;
				modes[i].desc.Height = tmp.desc.Height;
				modes[i].desc.RefreshRate = tmp.desc.RefreshRate;
			}
		}
	}

	// теперь сортируем по развертке
	for(i=0;i<count-1;i++){
		for(j=i+1;j<count;j++){
			if(modes[i].bpp == modes[j].bpp && 
				modes[i].desc.Width == modes[j].desc.Width &&
				modes[i].desc.Height == modes[j].desc.Height &&
				modes[i].desc.RefreshRate > modes[j].desc.RefreshRate){
				
				tmp.bpp         = modes[j].bpp;
				tmp.desc.Format = modes[j].desc.Format;
				tmp.desc.Width  = modes[j].desc.Width; 
				tmp.desc.Height = modes[j].desc.Height;
				tmp.desc.RefreshRate = modes[j].desc.RefreshRate;
				
				modes[j].bpp         = modes[i].bpp;
				modes[j].desc.Format = modes[i].desc.Format;
				modes[j].desc.Width  = modes[i].desc.Width;
				modes[j].desc.Height = modes[i].desc.Height;
				modes[j].desc.RefreshRate = modes[i].desc.RefreshRate;
			
				modes[i].bpp         = tmp.bpp;
				modes[i].desc.Format = tmp.desc.Format;
				modes[i].desc.Width  = tmp.desc.Width;
				modes[i].desc.Height = tmp.desc.Height;
				modes[i].desc.RefreshRate = tmp.desc.RefreshRate;
			}
		}
	}
	
	// заново переписываем строки
	for(i=0;i<count;i++)
		sprintf_s(modes[i].name, 64, "%d x %d  %d bit  %d hz",modes[i].desc.Width,
													 modes[i].desc.Height,
													 modes[i].bpp, modes[i].desc.RefreshRate);
}

VFormats::VFormats(){

	count		= 0;
	current		= 0;
	format_type = 0;
	lpd3d9		= NULL;
	format		= NULL;
	vd			= NULL;
	md			= NULL;
}


VFormats::~VFormats(){

	if(format) delete[] format;
	
	vd			= NULL;
	md			= NULL;
	count		= 0;
	current		= 0;
	lpd3d9		= NULL;
	format		= NULL;
	format_type = 0;
}

HRESULT	VFormats::init(LPDIRECT3D9 plpd3d9,DWORD pva,VDevice *pvd,VMode *pmd,DWORD fmt_type){
	
	HRESULT rv;

	D3DFORMAT	zformat[] = {
		D3DFMT_D16,
		D3DFMT_D16_LOCKABLE,
		D3DFMT_D32,
		D3DFMT_D32F_LOCKABLE,
		D3DFMT_D15S1,
		D3DFMT_D24S8,
		D3DFMT_D24X8,
		D3DFMT_D24X4S4,
		D3DFMT_D24FS8
	};
	
	bool  zsupport[sizeof(zformat)>>2];
		
	D3DFORMAT	tformat[] = {
		D3DFMT_R8G8B8,
		D3DFMT_A8R8G8B8,
		D3DFMT_X8R8G8B8,
		D3DFMT_R5G6B5,
		D3DFMT_X1R5G5B5,
		D3DFMT_A1R5G5B5,
		D3DFMT_A4R4G4B4,
		D3DFMT_R3G3B2,
		D3DFMT_A8,
		D3DFMT_A8R3G3B2,
		D3DFMT_X4R4G4B4,
		D3DFMT_A2B10G10R10,
		D3DFMT_A8B8G8R8,
		D3DFMT_X8B8G8R8,
		D3DFMT_G16R16,
		D3DFMT_A2R10G10B10,
		D3DFMT_A16B16G16R16,
		D3DFMT_A8P8,
		D3DFMT_P8,
		D3DFMT_L8,
		D3DFMT_L16,
		D3DFMT_A8L8,
		D3DFMT_A4L4
	};
	bool  tsupport[sizeof(tformat)>>2];
		
	lpd3d9		= plpd3d9;
	vd			= pvd;
	md			= pmd;
	va			= pva;
	format_type = fmt_type;
	
	int	i,j;
	
	if(fmt_type == VFORMAT_ZBUFFER){
		ZeroMemory(zsupport,sizeof(zsupport));
		count = 0;
		for(i=0;i<(sizeof(zformat)>>2);i++){
			rv = lpd3d9->CheckDeviceFormat(va,
											vd->caps.DeviceType,
											md->desc.Format,
											D3DUSAGE_DEPTHSTENCIL,
											D3DRTYPE_SURFACE,
											zformat[i]);	
			if(rv == D3D_OK){
				count++;
				zsupport[i] = true;
			}
		}
		format = new VFormat[count];
		for(j=0,i=0;i<(sizeof(zformat)>>2);i++){
			if(zsupport[i]){	
				format[j].fmt = zformat[i];
				format_to_zstencil(zformat[i],format[j].name);
				j++;
			}
		}
	}
	else if(fmt_type == VFORMAT_TEXTURE){
		ZeroMemory(tsupport,sizeof(tsupport));
		count = 0;
		for(i=0;i<(sizeof(tformat)>>2);i++){
			rv = lpd3d9->CheckDeviceFormat(va,
				vd->caps.DeviceType,
				md->desc.Format,
				D3DUSAGE_AUTOGENMIPMAP,
				D3DRTYPE_TEXTURE,
				tformat[i]);	
			if(rv == D3D_OK || rv == D3DOK_NOAUTOGEN){
				count++;
				tsupport[i] = true;
			}
		}
		format = new VFormat[count];
		for(j=0,i=0;i<(sizeof(tformat)>>2);i++){
			if(tsupport[i]){	
				format[j].fmt = tformat[i];
				format_to_texture(tformat[i],format[j].name);
				j++;
			}
		}
	}	
		
	return D3D_OK;
}

void VFormats::clear(){
	if(format) delete[] format;

	vd			= NULL;
	md			= NULL;
	count		= 0;
	current		= 0;
	lpd3d9		= NULL;
	format		= NULL;
	format_type = 0;
};

void TFTypes::clear(){
	if(filter_type) delete[] filter_type;

	vd			= NULL;
	md			= NULL;
	count		= 0;
	current		= 0;
	lpd3d9		= NULL;
	filter_type	= NULL;
	
};

TFTypes::TFTypes(){

	this->filter_type = NULL;
	clear();

}

TFTypes::~TFTypes(){

	clear();
}

HRESULT TFTypes::init(LPDIRECT3D9 plpd3d9,DWORD pva,VDevice *pvd,VMode *pmd){

	DWORD	i,j;
	
	lpd3d9 = plpd3d9;
	md = pmd;
	va = pva;
	vd = pvd;

	D3DTEXTUREFILTERTYPE	ftype[]= {
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
		D3DTEXF_PYRAMIDALQUAD,
		D3DTEXF_GAUSSIANQUAD
	};
			
	bool tftsupport[6];
	ZeroMemory(tftsupport,sizeof(tftsupport));
	tftsupport[0] = tftsupport[1] = true;
		
	count = 2;
	if(vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR ||
		vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR){
		tftsupport[2] = true;
		count++;	
	}
	if(vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ||
		vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC){
		tftsupport[3] = true;
		count++;	
	}
	if(vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFPYRAMIDALQUAD ||
		vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFPYRAMIDALQUAD){
		tftsupport[4] = true;
		count++;	
	}
	if(vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFGAUSSIANQUAD ||
		vd->caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFGAUSSIANQUAD){
		tftsupport[5] = true;
		count++;	
	}
	
	filter_type = new TFType[count];
	for(j=0,i=0;i<count;i++){
		if(tftsupport[i]){	
			filter_type[i].tftype = ftype[i];
			format_to_filter_type(ftype[i],filter_type[j].name);
			j++;
		}
	}
		
	return D3D_OK;
}
