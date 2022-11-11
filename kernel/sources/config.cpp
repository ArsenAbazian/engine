#include "config.h"

WORD	checkWord13;
#define CHECK_WORD13

WORD	checkWord13_1;
#define CHECK_WORD13_1

void Config::Clear(){

	ZeroMemory(&VParams,sizeof(VideoParams));
	
	return ;
}

bool Config::Load(TCHAR *filename){
	
	shString		str_video(TEXT("VideoConfiguration")), 
					str_device(TEXT("Device" )),
					str_adapter(TEXT("Adapter")),
					str_mode_width(TEXT("VMWidth")),
					str_mode_height(TEXT("VMHeight")),
					str_mode_format(TEXT("VMFormat")),
					str_mode_refrate(TEXT("RefreshRate")),
					str_zformat(TEXT("ZBufferFormat")),
					str_tformat(TEXT("TextureFormat")),
					str_tquality(TEXT("TextureQuality")),
					str_fmetod(TEXT("FiltrationMetod")),
					str_full(TEXT("Fullscreen"));
	
	shString		str_screensaver( TEXT( "ScreenSaverConfiguration" ) ),
					str_enable_clock_tick( TEXT( "enableClockTick" ) ),
					str_enable_clock_strike( TEXT( "enableClockStrike" ) ),
					str_enable_back_image( TEXT( "enableBackImage" ) ),
					str_enable_particle( TEXT( "enableParticle" ) ),
					str_smooth_moving( TEXT( "smoothMoving" ) ),
					str_move_camera( TEXT( "moveCamera" ) ),
					
					str_clock_tick_name( TEXT( "clockTickName" ) ),
					str_clock_strike_name( TEXT( "clockStrikeName" ) ),
					str_back_image_name01( TEXT( "backImageName01" ) ),
					str_back_image_name02( TEXT( "backImageName02" ) ),
					str_back_image_name03( TEXT( "backImageName03" ) ),
					str_back_image_name04( TEXT( "backImageName04" ) ),
					str_back_image_name05( TEXT( "backImageName05" ) ),
					str_back_image_name06( TEXT( "backImageName06" ) ),
					str_back_image_name07( TEXT( "backImageName07" ) ),
					str_back_image_name08( TEXT( "backImageName08" ) ),
					str_back_image_name09( TEXT( "backImageName09" ) ),
					str_back_image_name10( TEXT( "backImageName10" ) ),
					str_back_image_name11( TEXT( "backImageName11" ) ),
					str_back_image_name12( TEXT( "backImageName12" ) ),
					str_back_image_name13( TEXT( "backImageName13" ) ),
					str_back_image_name14( TEXT( "backImageName14" ) ),
					str_back_image_name15( TEXT( "backImageName15" ) ),
					str_back_image_name16( TEXT( "backImageName16" ) ),
					str_back_image_name17( TEXT( "backImageName17" ) ),
					str_back_image_name18( TEXT( "backImageName18" ) ),
					str_back_image_name19( TEXT( "backImageName19" ) ),
					str_back_image_name20( TEXT( "backImageName20" ) ),
					str_back_image_name21( TEXT( "backImageName21" ) ),
					str_back_image_name22( TEXT( "backImageName22" ) ),
					str_back_image_name23( TEXT( "backImageName23" ) ),
					str_back_image_name24( TEXT( "backImageName24" ) ),
					str_particle_name( TEXT( "particleName" ) ),
					
					str_max_particle_count( TEXT( "maxParticleCount" ) ),
					str_sound_volume( TEXT( "soundVolume" ) );
	
	RCManager		rcmanager;
	DwordValue		*value;
	shString		*strValue;

	if(rcmanager.Load(filename) != RC_OK){
		MessageBox(NULL, TEXT("Error: can't load configuration set all values to default"), TEXT("BoogiePerets screensaver"), MB_ICONERROR);
		return true;
	}
	else {
	
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_adapter, false);
		if(value != NULL)
			VParams.dwAdapter = value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_device, false); 
		if(value != NULL)
			VParams.DeviceType = (D3DDEVTYPE)value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_mode_width, false); 
		if(value != NULL)
			VParams.VideoMode.Width = value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_mode_height, false); 	
		if(value != NULL)
			VParams.VideoMode.Height		= value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_mode_format, false); 	
		if(value != NULL)
			VParams.VideoMode.Format = (D3DFORMAT)value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_mode_refrate, false); 	
		if(value != NULL)
			VParams.VideoMode.RefreshRate = value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_zformat, false); 	
		if(value != NULL)
			VParams.ZBufferFormat = (D3DFORMAT)value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_tformat, false); 		
		if(value != NULL)
			VParams.TextureFormat = (D3DFORMAT)value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_tquality, false); 		
		if(value != NULL)
			VParams.dwTextureQuality = value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_fmetod, false); 		
		if(value != NULL)
			VParams.dwFiltrationMetod = value->Val;
		
		value = (DwordValue*)rcmanager.GetElementByName(str_video, RC_DWORD, str_full, false); 
		if(value != NULL)
			VParams.bFullscreen = value->Val != 0;
			
		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_enable_clock_tick, false );
		if( value != NULL )
			this->SParams.enableClockTick = value->Val;

		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_enable_clock_strike, false );
		if( value != NULL )
			this->SParams.enableClockStrike = value->Val;

		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_enable_back_image, false );
		if( value != NULL )
			this->SParams.enableBackImage = value->Val;

		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_enable_particle, false );
		if( value != NULL )
			this->SParams.enableParticle = value->Val;

		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_smooth_moving, false );
		if( value != NULL )
			this->SParams.smoothMoving = value->Val;
	
		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_move_camera, false );
		if( value != NULL )
			this->SParams.moveCamera = value->Val;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_clock_tick_name, false );
		if( strValue != NULL )
			this->SParams.clockTickName = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_clock_strike_name, false );
		if( strValue != NULL )
			this->SParams.clockStrikeName = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name01, false );
		if( strValue != NULL )
			this->SParams.backImageName[0] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name02, false );
		if( strValue != NULL )
			this->SParams.backImageName[1] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name03, false );
		if( strValue != NULL )
			this->SParams.backImageName[2] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name04, false );
		if( strValue != NULL )
			this->SParams.backImageName[3] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name05, false );
		if( strValue != NULL )
			this->SParams.backImageName[4] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name06, false );
		if( strValue != NULL )
			this->SParams.backImageName[5] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name07, false );
		if( strValue != NULL )
			this->SParams.backImageName[6] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name08, false );
		if( strValue != NULL )
			this->SParams.backImageName[7] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name09, false );
		if( strValue != NULL )
			this->SParams.backImageName[8] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name10, false );
		if( strValue != NULL )
			this->SParams.backImageName[9] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name11, false );
		if( strValue != NULL )
			this->SParams.backImageName[10] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name12, false );
		if( strValue != NULL )
			this->SParams.backImageName[11] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name13, false );
		if( strValue != NULL )
			this->SParams.backImageName[12] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name14, false );
		if( strValue != NULL )
			this->SParams.backImageName[13] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name15, false );
		if( strValue != NULL )
			this->SParams.backImageName[14] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name16, false );
		if( strValue != NULL )
			this->SParams.backImageName[15] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name17, false );
		if( strValue != NULL )
			this->SParams.backImageName[16] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name18, false );
		if( strValue != NULL )
			this->SParams.backImageName[17] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name19, false );
		if( strValue != NULL )
			this->SParams.backImageName[18] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name20, false );
		if( strValue != NULL )
			this->SParams.backImageName[19] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name21, false );
		if( strValue != NULL )
			this->SParams.backImageName[20] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name22, false );
		if( strValue != NULL )
			this->SParams.backImageName[21] = *strValue;
		
		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name23, false );
		if( strValue != NULL )
			this->SParams.backImageName[22] = *strValue;

		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_back_image_name24, false );
		if( strValue != NULL )
			this->SParams.backImageName[23] = *strValue;
		
		strValue = (shString*)rcmanager.GetElementByName( str_screensaver, RC_STRING, str_particle_name, false );
		if( strValue != NULL )
			this->SParams.particleName = *strValue;
		
		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_max_particle_count, false );
		if( value != NULL )
			this->SParams.maxParticleCount = value->Val;

		value = (DwordValue*)rcmanager.GetElementByName( str_screensaver, RC_DWORD, str_sound_volume, false );
		if( value != NULL )
			this->SParams.soundVolume = value->Val;
	}
		
	return true;
} 

bool	Config::Save(TCHAR *filename){

	shString		str_video(TEXT("VideoConfiguration")), 
					str_device(TEXT("Device" )),
					str_adapter(TEXT("Adapter")),
					str_mode_width(TEXT("VMWidth")),
					str_mode_height(TEXT("VMHeight")),
					str_mode_format(TEXT("VMFormat")),
					str_mode_refrate(TEXT("RefreshRate")),
					str_zformat(TEXT("ZBufferFormat")),
					str_tformat(TEXT("TextureFormat")),
					str_tquality(TEXT("TextureQuality")),
					str_fmetod(TEXT("FiltrationMetod")),
					str_full(TEXT("Fullscreen"));
	
	shString		str_screensaver( TEXT( "ScreenSaverConfiguration" ) ),
					str_enable_clock_tick( TEXT( "enableClockTick" ) ),
					str_enable_clock_strike( TEXT( "enableClockStrike" ) ),
					str_enable_back_image( TEXT( "enableBackImage" ) ),
					str_enable_particle( TEXT( "enableParticle" ) ),
					str_smooth_moving( TEXT( "smoothMoving" ) ),
					str_move_camera( TEXT( "moveCamera" ) ),
					
					str_clock_tick_name( TEXT( "clockTickName" ) ),
					str_clock_strike_name( TEXT( "clockStrikeName" ) ),
					str_back_image_name01( TEXT( "backImageName01" ) ),
					str_back_image_name02( TEXT( "backImageName02" ) ),
					str_back_image_name03( TEXT( "backImageName03" ) ),
					str_back_image_name04( TEXT( "backImageName04" ) ),
					str_back_image_name05( TEXT( "backImageName05" ) ),
					str_back_image_name06( TEXT( "backImageName06" ) ),
					str_back_image_name07( TEXT( "backImageName07" ) ),
					str_back_image_name08( TEXT( "backImageName08" ) ),
					str_back_image_name09( TEXT( "backImageName09" ) ),
					str_back_image_name10( TEXT( "backImageName10" ) ),
					str_back_image_name11( TEXT( "backImageName11" ) ),
					str_back_image_name12( TEXT( "backImageName12" ) ),
					str_back_image_name13( TEXT( "backImageName13" ) ),
					str_back_image_name14( TEXT( "backImageName14" ) ),
					str_back_image_name15( TEXT( "backImageName15" ) ),
					str_back_image_name16( TEXT( "backImageName16" ) ),
					str_back_image_name17( TEXT( "backImageName17" ) ),
					str_back_image_name18( TEXT( "backImageName18" ) ),
					str_back_image_name19( TEXT( "backImageName19" ) ),
					str_back_image_name20( TEXT( "backImageName20" ) ),
					str_back_image_name21( TEXT( "backImageName21" ) ),
					str_back_image_name22( TEXT( "backImageName22" ) ),
					str_back_image_name23( TEXT( "backImageName23" ) ),
					str_back_image_name24( TEXT( "backImageName24" ) ),
					str_particle_name( TEXT( "particleName" ) ),
					
					str_max_part_count( TEXT( "maxParticleCount" ) ),
					str_sound_volume( TEXT( "soundVolume" ) );
	
	RCManager		rcmanager;
			
	rcmanager.AddCategory(str_video);
	rcmanager.SelectCategory(str_video);
	rcmanager.AddValue(str_adapter,		 (DWORD)VParams.dwAdapter, 10);
	rcmanager.AddValue(str_device,		 (DWORD)VParams.DeviceType, 10);
	rcmanager.AddValue(str_mode_width,   (DWORD)VParams.VideoMode.Width, 10);
	rcmanager.AddValue(str_mode_height,  (DWORD)VParams.VideoMode.Height, 10);
	rcmanager.AddValue(str_mode_format,  (DWORD)VParams.VideoMode.Format, 10);
	rcmanager.AddValue(str_mode_refrate, (DWORD)VParams.VideoMode.RefreshRate, 10);
	rcmanager.AddValue(str_zformat,		 (DWORD)VParams.ZBufferFormat, 10);
	rcmanager.AddValue(str_tformat,      (DWORD)VParams.TextureFormat, 10);
	rcmanager.AddValue(str_tquality,     (DWORD)VParams.dwTextureQuality, 10);
	rcmanager.AddValue(str_fmetod,	 	 (DWORD)VParams.dwFiltrationMetod, 10);
	rcmanager.AddValue(str_full,		 (DWORD)VParams.bFullscreen, 10);
	
	rcmanager.AddCategory( str_screensaver );
	rcmanager.SelectCategory( str_screensaver );
	
	rcmanager.AddValue( str_enable_clock_tick,		(DWORD)this->SParams.enableClockTick, 10 );
	rcmanager.AddValue( str_enable_clock_strike,	(DWORD)this->SParams.enableClockStrike, 10 );
	rcmanager.AddValue( str_enable_back_image,		(DWORD)this->SParams.enableBackImage, 10 );
	rcmanager.AddValue( str_enable_particle,		(DWORD)this->SParams.enableParticle, 10 );
	rcmanager.AddValue( str_smooth_moving,			(DWORD)this->SParams.smoothMoving, 10 );
	rcmanager.AddValue( str_move_camera,			(DWORD)this->SParams.moveCamera, 10 );
	
	rcmanager.AddValue( str_clock_tick_name,		this->SParams.clockTickName );
	rcmanager.AddValue( str_clock_strike_name,		this->SParams.clockStrikeName );
	rcmanager.AddValue( str_back_image_name01,		this->SParams.backImageName[0] );
	rcmanager.AddValue( str_back_image_name02,		this->SParams.backImageName[1] );
	rcmanager.AddValue( str_back_image_name03,		this->SParams.backImageName[2] );
	rcmanager.AddValue( str_back_image_name04,		this->SParams.backImageName[3] );
	rcmanager.AddValue( str_back_image_name05,		this->SParams.backImageName[4] );
	rcmanager.AddValue( str_back_image_name06,		this->SParams.backImageName[5] );
	rcmanager.AddValue( str_back_image_name07,		this->SParams.backImageName[6] );
	rcmanager.AddValue( str_back_image_name08,		this->SParams.backImageName[7] );
	rcmanager.AddValue( str_back_image_name09,		this->SParams.backImageName[8] );
	rcmanager.AddValue( str_back_image_name10,		this->SParams.backImageName[9] );
	rcmanager.AddValue( str_back_image_name11,		this->SParams.backImageName[10] );
	rcmanager.AddValue( str_back_image_name12,		this->SParams.backImageName[11] );
	rcmanager.AddValue( str_back_image_name13,		this->SParams.backImageName[12] );
	rcmanager.AddValue( str_back_image_name14,		this->SParams.backImageName[13] );
	rcmanager.AddValue( str_back_image_name15,		this->SParams.backImageName[14] );
	rcmanager.AddValue( str_back_image_name16,		this->SParams.backImageName[15] );
	rcmanager.AddValue( str_back_image_name17,		this->SParams.backImageName[16] );
	rcmanager.AddValue( str_back_image_name18,		this->SParams.backImageName[17] );
	rcmanager.AddValue( str_back_image_name19,		this->SParams.backImageName[18] );
	rcmanager.AddValue( str_back_image_name20,		this->SParams.backImageName[19] );
	rcmanager.AddValue( str_back_image_name21,		this->SParams.backImageName[20] );
	rcmanager.AddValue( str_back_image_name22,		this->SParams.backImageName[21] );
	rcmanager.AddValue( str_back_image_name23,		this->SParams.backImageName[22] );
	rcmanager.AddValue( str_back_image_name24,		this->SParams.backImageName[23] );
	rcmanager.AddValue( str_particle_name,			this->SParams.particleName );

	rcmanager.AddValue( str_max_part_count,			(DWORD)this->SParams.maxParticleCount, 10 );
	rcmanager.AddValue( str_sound_volume,			(DWORD)this->SParams.soundVolume, 10 );

	rcmanager.Save(filename);
	
	return true;
}