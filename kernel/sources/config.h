#ifndef G3DECONFIG_INC
#define G3DECONFIG_INC

#include "rcmanager.h"
#include <d3d9.h>

typedef struct{
	DWORD			dwAdapter;
	D3DDISPLAYMODE	VideoMode;
	D3DDEVTYPE		DeviceType;
	D3DFORMAT		ZBufferFormat;
	D3DFORMAT		TextureFormat;
	DWORD			dwTextureQuality;
	DWORD			dwFiltrationMetod;
	bool			bFullscreen;
}VideoParams;

typedef struct
{
	int				enableClockTick;
	int				enableClockStrike;
	int				enableBackImage;
	int				enableParticle;
	int				smoothMoving;
	int				moveCamera;
		
	shString		clockTickName;
	shString		clockStrikeName;
	shString		backImageName[24];
	shString		particleName;

	long			maxParticleCount;
	long			soundVolume;
}ScreenSaverParams;

class Config{
public:
	VideoParams			VParams;
	ScreenSaverParams	SParams;

	Config(){ Clear(); }
	~Config(){ Clear(); }

	bool	Save(TCHAR *filename);
	bool	Load(TCHAR *filename);
	
	void	Clear();
};

#endif