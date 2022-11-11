/*********************************************************************
*	
*		class StateBlock	- render states representaion
*		created by Abazyan Arsen
*		
*		Tula 02.11.2004		12:29
*		e-mail: ars_s@uic.tula.ru
*		
*		information:
*			- supports render states loading from file
*			- supports render states editor manage2d
**********************************************************************/

#ifndef A_STATE_BLOCK_INC
#define A_STATE_BLOCK_INC

#include <windows.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3d9caps.h>


typedef union _StateValue{
	DWORD			dvalue;
	D3DCOLOR		color;
	float			fvalue;
}StateValue;

// Render state 
#define SB_DWORD			1
#define SB_FLOAT			2
#define SB_COLOR			3
#define SB_FLAGS			4
#define SB_INT				5
#define SB_ZBUFFERTYPE		6
#define SB_FILLMODE			7
#define SB_SHADEMODE		8
#define SB_BOOL				9
#define SB_BLEND			10
#define SB_CULL				11
#define SB_CMPFUNC			12
#define SB_FOGMODE			13
#define SB_STENCILCAPS		14
#define SB_WRAP				15
#define SB_MATCOLORSOURCE	16
#define SB_VBELNDFLAGS		17
#define SB_CLIPPLANES		18
#define SB_PATHEDGESTYLE	19
#define SB_MONITORTOKENS	20
#define SB_COLWRITEENABLE	21
#define SB_BLENDOP			22
#define SB_DEGREETYPE		23
#define SB_MESCCAPS			24
#define SB_COLORWRENABLE	25

// TextureStageState
#define	SB_TEXTUREOP		1
#define	SB_TEXTUREARG		2
#define	SB_TEXCOORINDEX		3
#define	SB_TEXTRANSFLAGS	4	
#define SB_TEXFLOAT			5
#define SB_TEXDWORD			6
#define SB_TEXBOOL			7
#define SB_TEXCOLOR			8

// SamplerState
#define SB_TEXTUREADDRESS	1
#define SB_TEXFILTERTYPE	2
#define SB_SMRFLOAT			3
#define SB_SMRDWORD			4
#define SB_SMRBOOL			5
#define SB_SMRCOLOR			6

static D3DRENDERSTATETYPE	RenderStateType[] = {
	D3DRS_ZENABLE                   ,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	D3DRS_FILLMODE                  ,    /* D3DFILLMODE */
	D3DRS_SHADEMODE                 ,    /* D3DSHADEMODE */
	D3DRS_ZWRITEENABLE              ,   /* TRUE to enable z writes */
	D3DRS_ALPHATESTENABLE           ,   /* TRUE to enable alpha tests */
	D3DRS_LASTPIXEL                 ,   /* TRUE for last-pixel on lines */
	D3DRS_SRCBLEND                  ,   /* D3DBLEND */
	D3DRS_DESTBLEND                 ,   /* D3DBLEND */
	D3DRS_CULLMODE                  ,   /* D3DCULL */
	D3DRS_ZFUNC                     ,   /* D3DCMPFUNC */
	D3DRS_ALPHAREF                  ,   /* D3DFIXED */
	D3DRS_ALPHAFUNC                 ,   /* D3DCMPFUNC */
	D3DRS_DITHERENABLE              ,   /* TRUE to enable dithering */
	D3DRS_ALPHABLENDENABLE          ,   /* TRUE to enable alpha blending */
	D3DRS_FOGENABLE                 ,   /* TRUE to enable fog blending */
	D3DRS_SPECULARENABLE            ,   /* TRUE to enable specular */
	D3DRS_FOGCOLOR                  ,   /* D3DCOLOR */
	D3DRS_FOGTABLEMODE              ,   /* D3DFOGMODE */
	D3DRS_FOGSTART                  ,   /* Fog start (for both vertex and pixel fog) */
	D3DRS_FOGEND                    ,   /* Fog end      */
	D3DRS_FOGDENSITY                ,   /* Fog density  */
	D3DRS_RANGEFOGENABLE            ,   /* Enables range-based fog */
	D3DRS_STENCILENABLE             ,   /* BOOL enable/disable stenciling */
	D3DRS_STENCILFAIL               ,   /* D3DSTENCILOP to do if stencil test fails */
	D3DRS_STENCILZFAIL              ,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
	D3DRS_STENCILPASS               ,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
	D3DRS_STENCILFUNC               ,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	D3DRS_STENCILREF                ,   /* Reference value used in stencil test */
	D3DRS_STENCILMASK               ,   /* Mask value used in stencil test */
	D3DRS_STENCILWRITEMASK          ,   /* Write mask applied to values written to stencil buffer */
	D3DRS_TEXTUREFACTOR             ,   /* D3DCOLOR used for multi-texture blend */
	D3DRS_WRAP0                     ,  /* wrap for 1st texture coord. set */
	D3DRS_WRAP1                     ,  /* wrap for 2nd texture coord. set */
	D3DRS_WRAP2                     ,  /* wrap for 3rd texture coord. set */
	D3DRS_WRAP3                     ,  /* wrap for 4th texture coord. set */
	D3DRS_WRAP4                     ,  /* wrap for 5th texture coord. set */
	D3DRS_WRAP5                     ,  /* wrap for 6th texture coord. set */
	D3DRS_WRAP6                     ,  /* wrap for 7th texture coord. set */
	D3DRS_WRAP7                     ,  /* wrap for 8th texture coord. set */
	D3DRS_CLIPPING                  ,
	D3DRS_LIGHTING                  ,
	D3DRS_AMBIENT                   ,
	D3DRS_FOGVERTEXMODE             ,
	D3DRS_COLORVERTEX               ,
	D3DRS_LOCALVIEWER               ,
	D3DRS_NORMALIZENORMALS          ,
	D3DRS_DIFFUSEMATERIALSOURCE     ,
	D3DRS_SPECULARMATERIALSOURCE    ,
	D3DRS_AMBIENTMATERIALSOURCE     ,
	D3DRS_EMISSIVEMATERIALSOURCE    ,
	D3DRS_VERTEXBLEND               ,
	D3DRS_CLIPPLANEENABLE           ,
	D3DRS_POINTSIZE                 ,   /* float point size */
	D3DRS_POINTSIZE_MIN             ,   /* float point size min threshold */
	D3DRS_POINTSPRITEENABLE         ,   /* BOOL point texture coord control */
	D3DRS_POINTSCALEENABLE          ,   /* BOOL point size scale enable */
	D3DRS_POINTSCALE_A              ,   /* float point attenuation A value */
	D3DRS_POINTSCALE_B              ,   /* float point attenuation B value */
	D3DRS_POINTSCALE_C              ,   /* float point attenuation C value */
	D3DRS_MULTISAMPLEANTIALIAS      ,  // BOOL - set to do FSAA with multisample buffer
	D3DRS_MULTISAMPLEMASK           ,  // DWORD - per-sample enable/disable
	D3DRS_PATCHEDGESTYLE            ,  // Sets whether patch edges will use float style tessellation
	D3DRS_DEBUGMONITORTOKEN         ,  // DEBUG ONLY - token to debug monitor
	D3DRS_POINTSIZE_MAX             ,   /* float point size max threshold */
	D3DRS_INDEXEDVERTEXBLENDENABLE  ,
	D3DRS_COLORWRITEENABLE          ,  // per-channel write enable
	D3DRS_TWEENFACTOR               ,   // float tween factor
	D3DRS_BLENDOP                   ,   // D3DBLENDOP setting
	D3DRS_POSITIONDEGREE            ,   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
	D3DRS_NORMALDEGREE              ,   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
	D3DRS_SCISSORTESTENABLE         ,
	D3DRS_SLOPESCALEDEPTHBIAS       ,
	D3DRS_ANTIALIASEDLINEENABLE     ,
	D3DRS_MINTESSELLATIONLEVEL      ,
	D3DRS_MAXTESSELLATIONLEVEL      ,
	D3DRS_ADAPTIVETESS_X            ,
	D3DRS_ADAPTIVETESS_Y            ,
	D3DRS_ADAPTIVETESS_Z            ,
	D3DRS_ADAPTIVETESS_W            ,
	D3DRS_ENABLEADAPTIVETESSELLATION,
	D3DRS_TWOSIDEDSTENCILMODE       ,   /* BOOL enable/disable 2 sided stenciling */
	D3DRS_CCW_STENCILFAIL           ,   /* D3DSTENCILOP to do if ccw stencil test fails */
	D3DRS_CCW_STENCILZFAIL          ,   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	D3DRS_CCW_STENCILPASS           ,   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
	D3DRS_CCW_STENCILFUNC           ,   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	D3DRS_COLORWRITEENABLE1         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_COLORWRITEENABLE2         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_COLORWRITEENABLE3         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_BLENDFACTOR               ,   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
	D3DRS_SRGBWRITEENABLE           ,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
	D3DRS_DEPTHBIAS                 ,
	D3DRS_WRAP8                     ,   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
	D3DRS_WRAP9                     ,
	D3DRS_WRAP10                    ,
	D3DRS_WRAP11                    ,
	D3DRS_WRAP12                    ,
	D3DRS_WRAP13                    ,
	D3DRS_WRAP14                    ,
	D3DRS_WRAP15                    ,
	D3DRS_SEPARATEALPHABLENDENABLE  ,  /* TRUE to enable a separate blending function for the alpha channel */
	D3DRS_SRCBLENDALPHA             ,  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	D3DRS_DESTBLENDALPHA            ,  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	D3DRS_BLENDOPALPHA              ,  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
};

static BYTE	RenderStateFunc[] = {
	SB_ZBUFFERTYPE, // D3DRS_ZENABLE
	SB_FILLMODE, // D3DRS_FILLMODE
	SB_SHADEMODE, // D3DRS_SHADEMODE
	SB_BOOL, // D3DRS_ZWRITEENABLE
	SB_BOOL, // D3DRS_ALPHATESTENABLE
	SB_BOOL, // D3DRS_LASTPIXEL
	SB_BLEND, // D3DRS_SRCBLEND
	SB_BLEND, // D3DRS_DESTBLEND 
	SB_CULL, // D3DRS_CULLMODE 
	SB_CMPFUNC, // D3DRS_ZFUNC 
	SB_DWORD, // D3DRS_ALPHAREF 
	SB_CMPFUNC, // D3DRS_ALPHAFUNC 
	SB_BOOL, // D3DRS_DITHERENABLE 12
	SB_BOOL, // D3DRS_ALPHABLENDENABLE 
	SB_BOOL, // D3DRS_FOGENABLE 
	SB_BOOL, // D3DRS_SPECULARENABLE 
	SB_COLOR, // D3DRS_FOGCOLOR 
	SB_FOGMODE, // D3DRS_FOGTABLEMODE 
	SB_FLOAT, // D3DRS_FOGSTART 
	SB_FLOAT, // D3DRS_FOGEND 
	SB_FLOAT, // D3DRS_FOGDENSITY 
	SB_BOOL, // D3DRS_RANGEFOGENABLE  21
	SB_BOOL, // D3DRS_STENCILENABLE 
	SB_STENCILCAPS, // D3DRS_STENCILFAIL 
	SB_STENCILCAPS, // D3DRS_STENCILZFAIL 
	SB_STENCILCAPS, // D3DRS_STENCILPASS 
	SB_CMPFUNC, // D3DRS_STENCILFUNC 
	SB_INT,	  // D3DRS_STENCILREF 
	SB_DWORD, // D3DRS_STENCILMASK 
	SB_DWORD, // D3DRS_STENCILWRITEMASK 
	SB_COLOR, // D3DRS_TEXTUREFACTOR 
	SB_WRAP, // D3DRS_WRAP0 
	SB_WRAP, // D3DRS_WRAP1 
	SB_WRAP, // D3DRS_WRAP2 
	SB_WRAP, // D3DRS_WRAP3 
	SB_WRAP, // D3DRS_WRAP4 
	SB_WRAP, // D3DRS_WRAP5 
	SB_WRAP, // D3DRS_WRAP6 
	SB_WRAP, // D3DRS_WRAP7 
	SB_BOOL, // D3DRS_CLIPPING 39
	SB_BOOL, // D3DRS_LIGHTING 
	SB_COLOR, // D3DRS_AMBIENT 
	SB_FOGMODE, // D3DRS_FOGVERTEXMODE 
	SB_BOOL, // D3DRS_COLORVERTEX 43
	SB_BOOL, // D3DRS_LOCALVIEWER 
	SB_BOOL, // D3DRS_NORMALIZENORMALS 
	SB_MATCOLORSOURCE, // D3DRS_DIFFUSEMATERIALSOURCE 
	SB_MATCOLORSOURCE, // D3DRS_SPECULARMATERIALSOURCE 
	SB_MATCOLORSOURCE, // D3DRS_AMBIENTMATERIALSOURCE 
	SB_MATCOLORSOURCE, // D3DRS_EMISSIVEMATERIALSOURCE 
	SB_VBELNDFLAGS, // D3DRS_VERTEXBLEND 
	SB_CLIPPLANES, // D3DRS_CLIPPLANEENABLE 
	SB_FLOAT, // D3DRS_POINTSIZE 
	SB_FLOAT, // D3DRS_POINTSIZE_MIN 
	SB_BOOL, // D3DRS_POINTSPRITEENABLE 54
	SB_BOOL, // D3DRS_POINTSCALEENABLE 
	SB_FLOAT, // D3DRS_POINTSCALE_A 
	SB_FLOAT, // D3DRS_POINTSCALE_B 
	SB_FLOAT, // D3DRS_POINTSCALE_C 
	SB_BOOL, // D3DRS_MULTISAMPLEANTIALIAS 59
	SB_DWORD, // D3DRS_MULTISAMPLEMASK 
	SB_PATHEDGESTYLE, // D3DRS_PATCHEDGESTYLE 
	SB_MONITORTOKENS, // D3DRS_DEBUGMONITORTOKEN 
	SB_FLOAT, // D3DRS_POINTSIZE_MAX 
	SB_BOOL, // D3DRS_INDEXEDVERTEXBLENDENABLE 64
	SB_COLORWRENABLE, // D3DRS_COLORWRITEENABLE 
	SB_FLOAT, // D3DRS_TWEENFACTOR 
	SB_BLENDOP, // D3DRS_BLENDOP 
	SB_DEGREETYPE, // D3DRS_POSITIONDEGREE 
	SB_DEGREETYPE, // D3DRS_NORMALDEGREE 
	SB_BOOL, // D3DRS_SCISSORTESTENABLE 70
	SB_DWORD, // D3DRS_SLOPESCALEDEPTHBIAS 
	SB_BOOL, // D3DRS_ANTIALIASEDLINEENABLE 72
	SB_FLOAT, // D3DRS_MINTESSELLATIONLEVEL 
	SB_FLOAT, // D3DRS_MAXTESSELLATIONLEVEL 
	SB_FLOAT, // D3DRS_ADAPTIVETESS_X 
	SB_FLOAT, // D3DRS_ADAPTIVETESS_Y 
	SB_FLOAT, // D3DRS_ADAPTIVETESS_Z 
	SB_FLOAT, // D3DRS_ADAPTIVETESS_W 
	SB_BOOL, // D3DRS_ENABLEADAPTIVETESSELATION 79
	SB_BOOL, // D3DRS_TWOSIDEDSTENCILMODE 80
	SB_STENCILCAPS, // D3DRS_CCW_STENCILFAIL 
	SB_STENCILCAPS, // D3DRS_CCW_STENCILZFAIL 
	SB_STENCILCAPS, // D3DRS_CCW_STENCILPASS 
	SB_CMPFUNC, // D3DRS_CCW_STENCILFUNC 
	SB_COLORWRENABLE, // D3DRS_COLORWRITEENABLE1 
	SB_COLORWRENABLE, // D3DRS_COLORWRITEENABLE2 
	SB_COLORWRENABLE, // D3DRS_COLORWRITEENABLE3 
	SB_COLOR, // D3DRS_BLENDFACTOR 
	SB_BOOL, // D3DRS_SRGBWRITEENABLE 89
	SB_FLOAT, // D3DRS_DEPTHBIAS 
	SB_WRAP, // D3DRS_WRAP8 
	SB_WRAP, // D3DRS_WRAP9 
	SB_WRAP, // D3DRS_WRAP10 
	SB_WRAP, // D3DRS_WRAP11 
	SB_WRAP, // D3DRS_WRAP12 
	SB_WRAP, // D3DRS_WRAP13 
	SB_WRAP, // D3DRS_WRAP14 
	SB_WRAP, // D3DRS_WRAP15 
	SB_BOOL, // D3DRS_SEPARATEALPHABLENDENABLE 
	SB_BLEND, // D3DRS_SRCBLENDALPHA 
	SB_BLEND, // D3DRS_DESTBLENDALPHA 
	SB_BLENDOP, // D3DRS_BLENDOPALPHA 
};

static char	*DBGRenderStateType[] = {
	"D3DRS_ZENABLE"                   ,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	"D3DRS_FILLMODE"                  ,    /* D3DFILLMODE */
	"D3DRS_SHADEMODE"                 ,    /* D3DSHADEMODE */
	"D3DRS_ZWRITEENABLE"              ,   /* TRUE to enable z writes */
	"D3DRS_ALPHATESTENABLE"           ,   /* TRUE to enable alpha tests */
	"D3DRS_LASTPIXEL"                 ,   /* TRUE for last-pixel on lines */
	"D3DRS_SRCBLEND"                  ,   /* D3DBLEND */
	"D3DRS_DESTBLEND"                 ,   /* D3DBLEND */
	"D3DRS_CULLMODE"                  ,   /* D3DCULL */
	"D3DRS_ZFUNC"                     ,   /* D3DCMPFUNC */
	"D3DRS_ALPHAREF"                  ,   /* D3DFIXED */
	"D3DRS_ALPHAFUNC"                 ,   /* D3DCMPFUNC */
	"D3DRS_DITHERENABLE"              ,   /* TRUE to enable dithering */
	"D3DRS_ALPHABLENDENABLE"          ,   /* TRUE to enable alpha blending */
	"D3DRS_FOGENABLE"                 ,   /* TRUE to enable fog blending */
	"D3DRS_SPECULARENABLE"            ,   /* TRUE to enable specular */
	"D3DRS_FOGCOLOR"                  ,   /* D3DCOLOR */
	"D3DRS_FOGTABLEMODE"              ,   /* D3DFOGMODE */
	"D3DRS_FOGSTART"                  ,   /* Fog start (for both vertex and pixel fog) */
	"D3DRS_FOGEND"                    ,   /* Fog end      */
	"D3DRS_FOGDENSITY"                ,   /* Fog density  */
	"D3DRS_RANGEFOGENABLE"             ,   /* Enables range-based fog */
	"D3DRS_STENCILENABLE"             ,   /* BOOL enable/disable stenciling */
	"D3DRS_STENCILFAIL"               ,   /* D3DSTENCILOP to do if stencil test fails */
	"D3DRS_STENCILZFAIL"              ,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
	"D3DRS_STENCILPASS"               ,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
	"D3DRS_STENCILFUNC"               ,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	"D3DRS_STENCILREF"                ,   /* Reference value used in stencil test */
	"D3DRS_STENCILMASK"               ,   /* Mask value used in stencil test */
	"D3DRS_STENCILWRITEMASK"          ,   /* Write mask applied to values written to stencil buffer */
	"D3DRS_TEXTUREFACTOR"             ,   /* D3DCOLOR used for multi-texture blend */
	"D3DRS_WRAP0"                     ,  /* wrap for 1st texture coord. set */
	"D3DRS_WRAP1"                     ,  /* wrap for 2nd texture coord. set */
	"D3DRS_WRAP2"                     ,  /* wrap for 3rd texture coord. set */
	"D3DRS_WRAP3"                     ,  /* wrap for 4th texture coord. set */
	"D3DRS_WRAP4"                     ,  /* wrap for 5th texture coord. set */
	"D3DRS_WRAP5"                     ,  /* wrap for 6th texture coord. set */
	"D3DRS_WRAP6"                     ,  /* wrap for 7th texture coord. set */
	"D3DRS_WRAP7"                     ,  /* wrap for 8th texture coord. set */
	"D3DRS_CLIPPING"                  ,
	"D3DRS_LIGHTING"                  ,
	"D3DRS_AMBIENT"                   ,
	"D3DRS_FOGVERTEXMODE"             ,
	"D3DRS_COLORVERTEX"               ,
	"D3DRS_LOCALVIEWER"               ,
	"D3DRS_NORMALIZENORMALS"          ,
	"D3DRS_DIFFUSEMATERIALSOURCE"     ,
	"D3DRS_SPECULARMATERIALSOURCE"    ,
	"D3DRS_AMBIENTMATERIALSOURCE"     ,
	"D3DRS_EMISSIVEMATERIALSOURCE"    ,
	"D3DRS_VERTEXBLEND"               ,
	"D3DRS_CLIPPLANEENABLE"           ,
	"D3DRS_POINTSIZE"                 ,   /* float point size */
	"D3DRS_POINTSIZE_MIN"             ,   /* float point size min threshold */
	"D3DRS_POINTSPRITEENABLE"         ,   /* BOOL point texture coord control */
	"D3DRS_POINTSCALEENABLE"          ,   /* BOOL point size scale enable */
	"D3DRS_POINTSCALE_A"              ,   /* float point attenuation A value */
	"D3DRS_POINTSCALE_B"              ,   /* float point attenuation B value */
	"D3DRS_POINTSCALE_C"              ,   /* float point attenuation C value */
	"D3DRS_MULTISAMPLEANTIALIAS"      ,  // BOOL - set to do FSAA with multisample buffer
	"D3DRS_MULTISAMPLEMASK"           ,  // DWORD - per-sample enable/disable
	"D3DRS_PATCHEDGESTYLE"            ,  // Sets whether patch edges will use float style tessellation
	"D3DRS_DEBUGMONITORTOKEN"         ,  // DEBUG ONLY - token to debug monitor
	"D3DRS_POINTSIZE_MAX"             ,   /* float point size max threshold */
	"D3DRS_INDEXEDVERTEXBLENDENABLE"  ,
	"D3DRS_COLORWRITEENABLE"          ,  // per-channel write enable
	"D3DRS_TWEENFACTOR"               ,   // float tween factor
	"D3DRS_BLENDOP"                   ,   // D3DBLENDOP setting
	"D3DRS_POSITIONDEGREE"            ,   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
	"D3DRS_NORMALDEGREE"              ,   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
	"D3DRS_SCISSORTESTENABLE"         ,
	"D3DRS_SLOPESCALEDEPTHBIAS"       ,
	"D3DRS_ANTIALIASEDLINEENABLE"     ,
	"D3DRS_MINTESSELLATIONLEVEL"      ,
	"D3DRS_MAXTESSELLATIONLEVEL"      ,
	"D3DRS_ADAPTIVETESS_X"            ,
	"D3DRS_ADAPTIVETESS_Y"            ,
	"D3DRS_ADAPTIVETESS_Z"            ,
	"D3DRS_ADAPTIVETESS_W"            ,
	"D3DRS_ENABLEADAPTIVETESSELLATION",
	"D3DRS_TWOSIDEDSTENCILMODE"       ,   /* BOOL enable/disable 2 sided stenciling */
	"D3DRS_CCW_STENCILFAIL"           ,   /* D3DSTENCILOP to do if ccw stencil test fails */
	"D3DRS_CCW_STENCILZFAIL"          ,   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	"D3DRS_CCW_STENCILPASS"           ,   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
	"D3DRS_CCW_STENCILFUNC"           ,   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	"D3DRS_COLORWRITEENABLE1"         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	"D3DRS_COLORWRITEENABLE2"         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	"D3DRS_COLORWRITEENABLE3"         ,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	"D3DRS_BLENDFACTOR"               ,   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
	"D3DRS_SRGBWRITEENABLE"           ,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
	"D3DRS_DEPTHBIAS"                 ,
	"D3DRS_WRAP8"                     ,   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
	"D3DRS_WRAP9"                     ,
	"D3DRS_WRAP10"                    ,
	"D3DRS_WRAP11"                    ,
	"D3DRS_WRAP12"                    ,
	"D3DRS_WRAP13"                    ,
	"D3DRS_WRAP14"                    ,
	"D3DRS_WRAP15"                    ,
	"D3DRS_SEPARATEALPHABLENDENABLE"  ,  /* TRUE to enable a separate blending function for the alpha channel */
	"D3DRS_SRCBLENDALPHA"             ,  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	"D3DRS_DESTBLENDALPHA"            ,  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	"D3DRS_BLENDOPALPHA"              ,  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
};

static D3DZBUFFERTYPE	ZBufferType[] = { 
	D3DZB_FALSE,
	D3DZB_TRUE,
	D3DZB_USEW,
};

static D3DFILLMODE		FillMode[] = {
	D3DFILL_POINT,
	D3DFILL_WIREFRAME,
	D3DFILL_SOLID,
};

static D3DSHADEMODE	ShadeMode[] = {
	D3DSHADE_FLAT,
	D3DSHADE_GOURAUD,
	D3DSHADE_PHONG,
};

static BOOL	D3DBoolValue[] = {
	FALSE,
	TRUE
};

static D3DBLEND		Blend[] = {
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_SRCALPHASAT,
	D3DBLEND_BOTHSRCALPHA,
	D3DBLEND_BOTHINVSRCALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR,
};

static D3DCULL		Cull[] = {
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW,
};

static D3DCMPFUNC	CmpFunc[] = {
	D3DCMP_NEVER,
	D3DCMP_LESS,
	D3DCMP_EQUAL,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_NOTEQUAL,
	D3DCMP_GREATEREQUAL,
	D3DCMP_ALWAYS,
};

static D3DFOGMODE	FogMode[] = {
	D3DFOG_NONE,
	D3DFOG_EXP,
	D3DFOG_EXP2,
	D3DFOG_LINEAR,
};

static DWORD	StencilCaps[] = {
	D3DSTENCILCAPS_KEEP, 
	D3DSTENCILCAPS_ZERO,
	D3DSTENCILCAPS_REPLACE,
	D3DSTENCILCAPS_INCRSAT,
	D3DSTENCILCAPS_DECRSAT,
	D3DSTENCILCAPS_INVERT,
	D3DSTENCILCAPS_INCR,
	D3DSTENCILCAPS_DECR,
	D3DSTENCILCAPS_TWOSIDED
};

static DWORD	Wrap[] = {
	D3DWRAP_U, 
	D3DWRAP_V, 
	D3DWRAP_W, 
	D3DWRAPCOORD_3
};

static D3DMATERIALCOLORSOURCE	MatColorSource[] = {
	D3DMCS_MATERIAL,
	D3DMCS_COLOR1,
	D3DMCS_COLOR2,
};

static D3DVERTEXBLENDFLAGS	VBlendFlags[] = {
	D3DVBF_DISABLE,
	D3DVBF_1WEIGHTS,
	D3DVBF_2WEIGHTS,
	D3DVBF_3WEIGHTS,
	D3DVBF_TWEENING,
	D3DVBF_0WEIGHTS
};

static DWORD	ClipPlanes[] = {
	D3DCS_LEFT,
	D3DCS_RIGHT,
	D3DCS_TOP,
	D3DCS_BOTTOM,
	D3DCS_FRONT,
	D3DCS_BACK,
	D3DCS_PLANE0,
	D3DCS_PLANE1,
	D3DCS_PLANE2,
	D3DCS_PLANE3,
	D3DCS_PLANE4,
	D3DCS_PLANE5,
	D3DCS_ALL
};

static D3DPATCHEDGESTYLE	PatchEdgeStyle[] = {
	D3DPATCHEDGE_DISCRETE,
	D3DPATCHEDGE_CONTINUOUS,
};

static D3DDEBUGMONITORTOKENS	MonitorTokens[] = {
	D3DDMT_ENABLE,
	D3DDMT_DISABLE,
};

static DWORD	ColWriteEnable[] = {
	D3DCOLORWRITEENABLE_RED,
	D3DCOLORWRITEENABLE_GREEN,
	D3DCOLORWRITEENABLE_BLUE, 
	D3DCOLORWRITEENABLE_ALPHA 
};

static D3DBLENDOP	BlendOp[] = {
	D3DBLENDOP_ADD,
	D3DBLENDOP_SUBTRACT,
	D3DBLENDOP_REVSUBTRACT,
	D3DBLENDOP_MIN,
	D3DBLENDOP_MAX,
};

static D3DDEGREETYPE	DegreeType[] = {
	D3DDEGREE_LINEAR,
	D3DDEGREE_QUADRATIC,
	D3DDEGREE_CUBIC,
	D3DDEGREE_QUINTIC,
};

static DWORD	MiscCaps[] = {
	D3DPMISCCAPS_MASKZ,
	D3DPMISCCAPS_CULLNONE,
	D3DPMISCCAPS_CULLCW,
	D3DPMISCCAPS_CULLCCW,
	D3DPMISCCAPS_COLORWRITEENABLE,
	D3DPMISCCAPS_CLIPPLANESCALEDPOINTS,
	D3DPMISCCAPS_CLIPTLVERTS,
	D3DPMISCCAPS_TSSARGTEMP,
	D3DPMISCCAPS_BLENDOP,
	D3DPMISCCAPS_NULLREFERENCE,
	D3DPMISCCAPS_INDEPENDENTWRITEMASKS,
	D3DPMISCCAPS_PERSTAGECONSTANT,
	D3DPMISCCAPS_FOGANDSPECULARALPHA,
	D3DPMISCCAPS_SEPARATEALPHABLEND,
	D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS,
	D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING,
	D3DPMISCCAPS_FOGVERTEXCLAMPED
};

// =============================== SetTextureStageState ===============================

static D3DTEXTURESTAGESTATETYPE	TextureStageStateType[] = {
	D3DTSS_COLOROP,
	D3DTSS_COLORARG1,
	D3DTSS_COLORARG2,
	D3DTSS_ALPHAOP,
	D3DTSS_ALPHAARG1,
	D3DTSS_ALPHAARG2,
	D3DTSS_BUMPENVMAT00,
	D3DTSS_BUMPENVMAT01,
	D3DTSS_BUMPENVMAT10,
	D3DTSS_BUMPENVMAT11,
	D3DTSS_TEXCOORDINDEX,
	D3DTSS_BUMPENVLSCALE,
	D3DTSS_BUMPENVLOFFSET,
	D3DTSS_TEXTURETRANSFORMFLAGS,
	D3DTSS_COLORARG0,
	D3DTSS_ALPHAARG0,
	D3DTSS_RESULTARG,
	D3DTSS_CONSTANT,
};

static BYTE	TextureStageStateFunc[] = {
	SB_TEXTUREOP, // D3DTSS_COLOROP 
	SB_TEXTUREARG, // D3DTSS_COLORARG1 
	SB_TEXTUREARG, // D3DTSS_COLORARG2 
	SB_TEXTUREOP, // D3DTSS_ALPHAOP 
	SB_TEXTUREARG, // D3DTSS_ALPHAARG1 
	SB_TEXTUREARG, // D3DTSS_ALPHAARG2 
	SB_TEXFLOAT, // D3DTSS_BUMPENVMAT00 
	SB_TEXFLOAT, // D3DTSS_BUMPENVMAT01 
	SB_TEXFLOAT, // D3DTSS_BUMPENVMAT10 
	SB_TEXFLOAT, // D3DTSS_BUMPENVMAT11 
	SB_TEXCOORINDEX, // D3DTSS_TEXCOORDINDEX 
	SB_TEXFLOAT, // D3DTSS_BUMPENVLSCALE 
	SB_TEXFLOAT, // D3DTSS_BUMPENVLOFFSET 
	SB_TEXTRANSFLAGS	, // D3DTSS_TEXTURETRANSFORMFLAGS 
	SB_TEXTUREARG, // D3DTSS_COLORARG0 
	SB_TEXTUREARG, // D3DTSS_ALPHAARG0 
	SB_TEXTUREARG, // D3DTSS_RESULTARG 
	SB_TEXCOLOR, // D3DTSS_CONSTANT 
};

static char	*DBGTextureStageStateType[] = {
	"D3DTSS_COLOROP",
	"D3DTSS_COLORARG1",
	"D3DTSS_COLORARG2",
	"D3DTSS_ALPHAOP",
	"D3DTSS_ALPHAARG1",
	"D3DTSS_ALPHAARG2",
	"D3DTSS_BUMPENVMAT00",
	"D3DTSS_BUMPENVMAT01",
	"D3DTSS_BUMPENVMAT10",
	"D3DTSS_BUMPENVMAT11",
	"D3DTSS_TEXCOORDINDEX",
	"D3DTSS_BUMPENVLSCALE",
	"D3DTSS_BUMPENVLOFFSET",
	"D3DTSS_TEXTURETRANSFORMFLAGS",
	"D3DTSS_COLORARG0",
	"D3DTSS_ALPHAARG0",
	"D3DTSS_RESULTARG",
	"D3DTSS_CONSTANT",
};

static D3DTEXTUREOP	TextureOp[] = {
	D3DTOP_DISABLE,
	D3DTOP_SELECTARG1,
	D3DTOP_SELECTARG2,
	D3DTOP_MODULATE,
	D3DTOP_MODULATE2X,
	D3DTOP_MODULATE4X,
	D3DTOP_ADD,
	D3DTOP_ADDSIGNED,
	D3DTOP_ADDSIGNED2X,
	D3DTOP_SUBTRACT,
	D3DTOP_ADDSMOOTH,
	D3DTOP_BLENDDIFFUSEALPHA,
	D3DTOP_BLENDTEXTUREALPHA,
	D3DTOP_BLENDFACTORALPHA,
	D3DTOP_BLENDTEXTUREALPHAPM,
	D3DTOP_BLENDCURRENTALPHA,
	D3DTOP_PREMODULATE,
	D3DTOP_MODULATEALPHA_ADDCOLOR,
	D3DTOP_MODULATECOLOR_ADDALPHA,
	D3DTOP_MODULATEINVALPHA_ADDCOLOR,
	D3DTOP_MODULATEINVCOLOR_ADDALPHA,
	D3DTOP_BUMPENVMAP,
	D3DTOP_BUMPENVMAPLUMINANCE,
	D3DTOP_DOTPRODUCT3,
	D3DTOP_MULTIPLYADD,
	D3DTOP_LERP
};

static DWORD	TextureArg[] = { 
	D3DTA_CURRENT,
	D3DTA_DIFFUSE,
	D3DTA_SELECTMASK,
	D3DTA_SPECULAR,
	D3DTA_TEMP,
	D3DTA_TEXTURE,
	D3DTA_TFACTOR,
	D3DTA_ALPHAREPLICATE,
	D3DTA_COMPLEMENT
};

static DWORD	TextCoorIndex[] = { 
	D3DTSS_TCI_PASSTHRU,
	D3DTSS_TCI_CAMERASPACENORMAL,
	D3DTSS_TCI_CAMERASPACEPOSITION,
	D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR,
	D3DTSS_TCI_SPHEREMAP
};

static D3DTEXTURETRANSFORMFLAGS	TextureTransFlags[] = { 
	D3DTTFF_DISABLE,
	D3DTTFF_COUNT1,
	D3DTTFF_COUNT2,
	D3DTTFF_COUNT3,
	D3DTTFF_COUNT4,
	D3DTTFF_PROJECTED
};

// ================================ SetSamplerState ===================================

static D3DSAMPLERSTATETYPE		SamplerStateType[] = {
	D3DSAMP_ADDRESSU,
	D3DSAMP_ADDRESSV,
	D3DSAMP_ADDRESSW,
	D3DSAMP_BORDERCOLOR,
	D3DSAMP_MAGFILTER,
	D3DSAMP_MINFILTER,
	D3DSAMP_MIPFILTER,
	D3DSAMP_MIPMAPLODBIAS,
	D3DSAMP_MAXMIPLEVEL,
	D3DSAMP_MAXANISOTROPY,
	D3DSAMP_SRGBTEXTURE,
	D3DSAMP_ELEMENTINDEX,
	D3DSAMP_DMAPOFFSET,
};

static char	*DBGSamplerStateType[] = {
	"D3DSAMP_ADDRESSU",
	"D3DSAMP_ADDRESSV",
	"D3DSAMP_ADDRESSW",
	"D3DSAMP_BORDERCOLOR",
	"D3DSAMP_MAGFILTER",
	"D3DSAMP_MINFILTER",
	"D3DSAMP_MIPFILTER",
	"D3DSAMP_MIPMAPLODBIAS",
	"D3DSAMP_MAXMIPLEVEL",
	"D3DSAMP_MAXANISOTROPY",
	"D3DSAMP_SRGBTEXTURE",
	"D3DSAMP_ELEMENTINDEX",
	"D3DSAMP_DMAPOFFSET",
};

static BYTE	SamplerStateFunc[] = {
	SB_TEXTUREADDRESS, // D3DSAMP_ADDRESSU
	SB_TEXTUREADDRESS, // D3DSAMP_ADDRESSV
	SB_TEXTUREADDRESS, // D3DSAMP_ADDRESSW
	SB_SMRCOLOR, // D3DSAMP_BORDERCOLOR
	SB_TEXFILTERTYPE, // D3DSAMP_MAGFILTER
	SB_TEXFILTERTYPE, // D3DSAMP_MINFILTER
	SB_TEXFILTERTYPE, // D3DSAMP_MIPFILTER
	SB_SMRDWORD, // D3DSAMP_MIPMAPLODBIAS
	SB_SMRDWORD, // D3DSAMP_MAXMIPLEVEL
	SB_SMRDWORD, // D3DSAMP_MAXANISOTROPY
	SB_SMRFLOAT, // D3DSAMP_SRGBTEXTURE
	SB_SMRDWORD, // D3DSAMP_ELEMENTINDEX
	SB_SMRDWORD, // D3DSAMP_DMAPOFFSET
};

static D3DTEXTUREADDRESS	TextureAddress[] = {
	D3DTADDRESS_WRAP,
	D3DTADDRESS_MIRROR,
	D3DTADDRESS_CLAMP,
	D3DTADDRESS_BORDER,
	D3DTADDRESS_MIRRORONCE
};

static D3DTEXTUREFILTERTYPE	TextureFilterType[] = { 
	D3DTEXF_NONE,
	D3DTEXF_POINT,
	D3DTEXF_LINEAR,
	D3DTEXF_ANISOTROPIC,
	D3DTEXF_PYRAMIDALQUAD,
	D3DTEXF_GAUSSIANQUAD
};


// ============================ Debug information ===========================
static char	*DBGZBEnable[] = { 
	"D3DZB_FALSE",
	"D3DZB_TRUE",
	"D3DZB_USEW" 
};

static char	*DBGFMode[] = {	
	"D3DFILL_POINT",
	"D3DFILL_WIREFRAME",
	"D3DFILL_SOLID"
};

static char	*DBGSMode[] = { 
	"D3DSHADE_FLAT",
	"D3DSHADE_GOURAUD",
	"D3DSHADE_PHONG"
};

static char	*DBGTrueFalse[] = {
	"FALSE", 
	"TRUE"
};

static char	*DBGBlend[] = {	
	"D3DBLEND_ZERO",
	"D3DBLEND_ONE",
	"D3DBLEND_SRCCOLOR",
	"D3DBLEND_INVSRCCOLOR",
	"D3DBLEND_SRCALPHA",
	"D3DBLEND_INVSRCALPHA",
	"D3DBLEND_DESTALPHA",
	"D3DBLEND_INVDESTALPHA",
	"D3DBLEND_DESTCOLOR",
	"D3DBLEND_INVDESTCOLOR",
	"D3DBLEND_SRCALPHASAT",
	"D3DBLEND_BOTHSRCALPHA",
	"D3DBLEND_BOTHINVSRCALPHA",
	"D3DBLEND_BLENDFACTOR",
	"D3DBLEND_INVBLENDFACTOR"
};

static char	*DBGCull[] = {	
	"D3DCULL_NONE",
	"D3DCULL_CW",
	"D3DCULL_CCW"
};

static char	*DBGCmpFunc[] = { 
	"D3DCMP_NEVER",
	"D3DCMP_LESS",
	"D3DCMP_EQUAL",
	"D3DCMP_LESSEQUAL",
	"D3DCMP_GREATER",
	"D3DCMP_NOTEQUAL",
	"D3DCMP_GREATEREQUAL",
	"D3DCMP_ALWAYS"
};

static char	*DBGFogMode[] = { 
	"D3DFOG_NONE",
	"D3DFOG_EXP",
	"D3DFOG_EXP2",
	"D3DFOG_LINEAR"
};

static char	*DBGStencilCaps[] = {	
	"D3DSTENCILCAPS_KEEP", 
	"D3DSTENCILCAPS_ZERO",
	"D3DSTENCILCAPS_REPLACE",
	"D3DSTENCILCAPS_INCRSAT",
	"D3DSTENCILCAPS_DECRSAT",
	"D3DSTENCILCAPS_INVERT",
	"D3DSTENCILCAPS_INCR",
	"D3DSTENCILCAPS_DECR",
	"D3DSTENCILCAPS_TWOSIDED"
};

static char	*DBGWrap[] = {	
	"D3DWRAPCOORD_0 (D3DWRAP_U)", 
	"D3DWRAPCOORD_1 (D3DWRAP_V)", 
	"D3DWRAPCOORD_2 (D3DWRAP_W)", 
	"D3DWRAPCOORD_3" 
};

static char	*DBGMatColSource[] = { 
	"D3DMCS_MATERIAL",
	"D3DMCS_COLOR1",
	"D3DMCS_COLOR2"
};

static char	*DBGVBlendFlags[] = {
	"D3DVBF_DISABLE",
	"D3DVBF_1WEIGHTS",
	"D3DVBF_2WEIGHTS",
	"D3DVBF_3WEIGHTS",
	"D3DVBF_TWEENING",
	"D3DVBF_0WEIGHTS"
};

static char	*DBGClipPlanes[] = {
	"D3DCS_LEFT",
	"D3DCS_RIGHT",
	"D3DCS_TOP",
	"D3DCS_BOTTOM",
	"D3DCS_FRONT",
	"D3DCS_BACK",
	"D3DCS_PLANE0",
	"D3DCS_PLANE1",
	"D3DCS_PLANE2",
	"D3DCS_PLANE3",
	"D3DCS_PLANE4",
	"D3DCS_PLANE5",
	"D3DCS_ALL"
};

static char	*DBGPatchEdgeStyle[] = {
	"D3DPATCHEDGE_DISCRETE",
	"D3DPATCHEDGE_CONTINUOUS"
};

static char	*DBGMonitorTokens[] = {	
	"D3DDMT_ENABLE",
	"D3DDMT_DISABLE"
};

static char	*DBGColWriteEnable[] = { 
	"D3DCOLORWRITEENABLE_RED"
	"D3DCOLORWRITEENABLE_GREEN",
	"D3DCOLORWRITEENABLE_BLUE", 
	"D3DCOLORWRITEENABLE_ALPHA", 
};

static char	*DBGBlendOp[] = { 
	"D3DBLENDOP_ADD",
	"D3DBLENDOP_SUBTRACT",
	"D3DBLENDOP_REVSUBTRACT",
	"D3DBLENDOP_MIN",
	"D3DBLENDOP_MAX"
};

static char	*DBGDegreeType[] = { 
	"D3DDEGREE_LINEAR",
	"D3DDEGREE_QUADRATIC",
	"D3DDEGREE_CUBIC",
	"D3DDEGREE_QUINTIC"
};

static char	*DBGMiscCaps[] = { 
	"D3DPMISCCAPS_MASKZ",
	"D3DPMISCCAPS_CULLNONE",
	"D3DPMISCCAPS_CULLCW",
	"D3DPMISCCAPS_CULLCCW",
	"D3DPMISCCAPS_COLORWRITEENABLE",
	"D3DPMISCCAPS_CLIPPLANESCALEDPOINTS",
	"D3DPMISCCAPS_CLIPTLVERTS",
	"D3DPMISCCAPS_TSSARGTEMP",
	"D3DPMISCCAPS_BLENDOP",
	"D3DPMISCCAPS_NULLREFERENCE",
	"D3DPMISCCAPS_INDEPENDENTWRITEMASKS",
	"D3DPMISCCAPS_PERSTAGECONSTANT",
	"D3DPMISCCAPS_FOGANDSPECULARALPHA",
	"D3DPMISCCAPS_SEPARATEALPHABLEND",
	"D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS",
	"D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING",
	"D3DPMISCCAPS_FOGVERTEXCLAMPED"
};

// Texture debug information	Can't write russian!!

static char	*DBGTextureOp[] = {	
	"D3DTOP_DISABLE",
	"D3DTOP_SELECTARG1",
	"D3DTOP_SELECTARG2",
	"D3DTOP_MODULATE",
	"D3DTOP_MODULATE2X",
	"D3DTOP_MODULATE4X",
	"D3DTOP_ADD",
	"D3DTOP_ADDSIGNED",
	"D3DTOP_ADDSIGNED2X",
	"D3DTOP_SUBTRACT",
	"D3DTOP_ADDSMOOTH",
	"D3DTOP_BLENDDIFFUSEALPHA",
	"D3DTOP_BLENDTEXTUREALPHA",
	"D3DTOP_BLENDFACTORALPHA",
	"D3DTOP_BLENDTEXTUREALPHAPM",
	"D3DTOP_BLENDCURRENTALPHA",
	"D3DTOP_PREMODULATE",
	"D3DTOP_MODULATEALPHA_ADDCOLOR",
	"D3DTOP_MODULATECOLOR_ADDALPHA",
	"D3DTOP_MODULATEINVALPHA_ADDCOLOR",
	"D3DTOP_MODULATEINVCOLOR_ADDALPHA",
	"D3DTOP_BUMPENVMAP",
	"D3DTOP_BUMPENVMAPLUMINANCE",
	"D3DTOP_DOTPRODUCT3",
	"D3DTOP_MULTIPLYADD",
	"D3DTOP_LERP"
};

static char	*DBGTextureArg[] = { 
	"D3DTA_CURRENT",
	"D3DTA_DIFFUSE",
	"D3DTA_SELECTMASK",
	"D3DTA_SPECULAR",
	"D3DTA_TEMP",
	"D3DTA_TEXTURE",
	"D3DTA_TFACTOR",
	"D3DTA_ALPHAREPLICATE",
	"D3DTA_COMPLEMENT"};

static char	*DBGTextCoorIndex[] = { 
	"D3DTSS_TCI_PASSTHRU",
	"D3DTSS_TCI_CAMERASPACENORMAL",
	"D3DTSS_TCI_CAMERASPACEPOSITION",
	"D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR",
	"D3DTSS_TCI_SPHEREMAP"
};

static char	*DBGTextureTransFlags[] = { 
	"D3DTTFF_DISABLE",
	"D3DTTFF_COUNT1",
	"D3DTTFF_COUNT2",
	"D3DTTFF_COUNT3",
	"D3DTTFF_COUNT4",
	"D3DTTFF_PROJECTED"
};

// Debug Sampler state

static char	*DBGTextAddr[] = { 
	"D3DTADDRESS_WRAP",
	"D3DTADDRESS_MIRROR",
	"D3DTADDRESS_CLAMP",
	"D3DTADDRESS_BORDER",
	"D3DTADDRESS_MIRRORONCE"
};

static char	*DBGTextFilterType[] = { 
	"D3DTEXF_NONE",
	"D3DTEXF_POINT",
	"D3DTEXF_LINEAR",
	"D3DTEXF_ANISOTROPIC",
	"D3DTEXF_PYRAMIDALQUAD",
	"D3DTEXF_GAUSSIANQUAD"
};


class StateBlockInfo{
public:	
	bool					rsenabled[103];			// активно ли состояние рендеринга, указанного в RenderStateType
	StateValue				rstate_value[103];		// значение RenderState
	bool					tsenabled[8];			// 
	bool					tssenabled[8][18];		// активно ли состояние текстуры
	StateValue				tsstate_value[8][18];	// значение TextureStageStateType		
	bool					ssenabled[13];			// активно ли состояние StateSampler	
	StateValue				sstate_value[13];		// значение SamplerState
	char					*name;	
	
	DWORD					sel_rstate;
	DWORD					sel_stage;
	DWORD					sel_tsstate[8];
	DWORD					sel_sstate;
	
	StateBlockInfo();
	~StateBlockInfo();

	void	clear();					// уничтожить
	void	write_log(char *filename);
	void	write_xml(char *filename);
	bool	load(char *filename);
	bool	save(char *filename);
	bool	get_rs_values(DWORD	**values,char ***dbg_values,DWORD *count);	
	bool	get_ss_values(DWORD	**values,char ***dbg_values,DWORD *count);
	bool	get_tss_values(DWORD **values,char ***dbg_values,DWORD *count);
	bool	get_rs_values(DWORD rstate,DWORD	**values,char ***dbg_values,DWORD *count);	
	bool	get_ss_values(DWORD sstate,DWORD	**values,char ***dbg_values,DWORD *count);
	bool	get_tss_values(DWORD tsstate,DWORD **values,char ***dbg_values,DWORD *count);
	void	set_name(char *nm);
};

#endif