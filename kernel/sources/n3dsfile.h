#ifndef N3DS_INC
#define N3DS_INC

#include <windows.h>
#include <fstream>

#include "vecmat.h"
#include "hstring.h"


#define	MAIN3DS		  0x4D4D          // "заголовок" файла
#define VERSION       0x0002          // верси€ файла
// =============== подблоки MAIN3DS =============== 
#define EDIT3DS		  0x3D3D	        //  конфигураци€ объекта (вершины, грани, текстура, материал, свет, камера...)
#define KEYF3DS       0xB000            //  конфигураци€ анимации        

// =============== подблоки EDIT3DS ===============
#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100 
#define EDIT_OBJECT   0x4000

#define EDIT_UNKNW01  0x1100
#define EDIT_UNKNW02  0x1201
#define EDIT_UNKNW03  0x1300
#define EDIT_UNKNW04  0x1400
#define EDIT_UNKNW05  0x1420
#define EDIT_UNKNW06  0x1450 
#define EDIT_UNKNW07  0x1500
#define EDIT_UNKNW08  0x2200
#define EDIT_UNKNW09  0x2201
#define EDIT_UNKNW10  0x2210
#define EDIT_UNKNW11  0x2300
#define EDIT_UNKNW12  0x2302 
#define EDIT_UNKNW13  0x3000
#define EDIT_UNKNW14  0xAFFF
     
// =============== блоки процентов ====================
#define INT_PERCENT   0x0030
#define FLOAT_PERCENT 0x0031

// =============== подблоки EDIT_OBJECT ===============
#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

#define OBJ_UNKNWN01  0x4010
#define OBJ_UNKNWN02  0x4012 //---- может быть тень

// =============== подблоки OBJ_CAMERA ===============
#define CAM_UNKNWN01  0x4710 
#define CAM_UNKNWN02  0x4720 

// =============== подблоки JBJ_LIGHT ===============
#define LIT_SPOT      0x4610
#define LIT_OFF       0x4620
#define LIT_UNKNWN01  0x465A

// =============== подблоки OBJ_TRIMESH ===============
#define TRI_VERTEXL   0x4110
#define TRI_FACEL2    0x4111 
#define TRI_FACEL1    0x4120
#define TRI_MATLIST   0x4130
#define TRI_MAPCOOR   0x4140
#define TRI_SMOOTH    0x4150
#define TRI_LOCAL     0x4160
#define TRI_VISIBLE   0x4165

// =============== подблоки EDIT_MATERIAL ================
#define MAT_NAME            0xA000
#define MAT_AMBCOL          0xA010
#define MAT_DIFCOL          0xA020
#define MAT_SPCCOL          0xA030
#define MAT_SHININESS       0xA040
#define MAT_SHIN2PCN        0xA041
#define MAT_SHIN3PCT        0xA042
#define MAT_TRANSPARENCY    0xA050
#define MAT_MAPFILE         0xA300
#define MAT_TEX1            0xA200
#define MAT_BUMP			0xA230
#define MAT_OPACITY			0xA210
#define MAT_REFLECTION		0xA220

#define MAT_MAPNAME					0xA300
#define MAT_ACUBIC					0xA310
#define MAT_SXP_TEXT_DATA			0xA320
#define MAT_SXP_TEXT2_DATA			0xA321
#define MAT_SXP_OPAC_DATA			0xA322
#define MAT_SXP_BUMP_DATA			0xA324
#define MAT_SXP_SPEC_DATA			0xA325
#define MAT_SXP_SHIN_DATA 			0xA326
#define MAT_SXP_SELFI_DATA 			0xA328
#define MAT_SXP_TEXT_MASKDATA		0xA32A
#define MAT_SXP_TEXT2_MASKDATA		0xA32C
#define MAT_SXP_OPAC_MASKDATA		0xA32E
#define MAT_SXP_BUMP_MASKDATA		0xA330
#define MAT_SXP_SPEC_MASKDATA		0xA332
#define MAT_SXP_SHIN_MASKDATA		0xA334
#define MAT_SXP_SELFI_MASKDATA		0xA336
#define MAT_SXP_REFL_MASKDATA		0xA338
#define MAT_TEX2MAP 				0xA33A
#define MAT_SHINMAP 				0xA33C
#define MAT_SELFIMAP 				0xA33D
#define MAT_TEXMASK 				0xA33E
#define MAT_TEX2MASK 				0xA340
#define MAT_OPACMASK 				0xA342
#define MAT_BUMPMASK 				0xA344
#define MAT_SHINMASK 				0xA346
#define MAT_SPECMASK 				0xA348
#define MAT_SELFIMASK 				0xA34A
#define MAT_REFLMASK 				0xA34C
#define MAT_MAP_TILINGOLD 			0xA350
#define MAT_MAP_TILING 				0xA351
#define MAT_MAP_TEXBLUR_OLD 		0xA352
#define MAT_MAP_TEXBLUR 			0xA353
#define MAT_MAP_USCALE 				0xA354
#define MAT_MAP_VSCALE 				0xA356
#define MAT_MAP_UOFFSET 			0xA358
#define MAT_MAP_VOFFSET 			0xA35A
#define MAT_MAP_ANG 				0xA35C
#define MAT_MAP_COL1 				0xA360
#define MAT_MAP_COL2 				0xA362
#define MAT_MAP_RCOL 				0xA364
#define MAT_MAP_GCOL 				0xA366
#define MAT_MAP_BCOL 				0xA368

// =============== подблоки KEYF3DS ===============
#define KEYF_UNKNWN01				0xB009
#define KEYF_UNKNWN02				0xB00A
#define KEYF_FRAMES					0xB008
#define KEYF_OBJDES					0xB002

// =============== типы цветов в 3DS Max ===============
#define COL_FRGB					0x0010
#define COL_RGB						0x0011
#define COL_RGBG					0x0012
#define COL_FRGBG					0x0013

// =============== объ€влени€ дл€ viewport ================
#define TOP					0x0001
#define BOTTOM				0x0002
#define LEFT				0x0003
#define RIGHT				0x0004
#define FRONT				0x0005
#define BACK				0x0006
#define USER				0x0007
#define CAMERA				0x0008 // 0xFFFF актуальный код прочитанный из файла
#define LIGHT				0x0009
#define DISABLED			0x0010
#define BOGUS				0x0011

#define SPOTLIGHT			1
#define POINTLIGHT			2
#define DIRECTIONAL			3

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct{
	word	        Desc;
	dword	        Lenght;
}ChunkHdr3DS;

class Color3DS{
public:
    byte            alpha;
    byte            red;
    byte            green;
    byte            blue;
    
    Color3DS(){ alpha = 0;
                red   = 0;
                green = 0;
                blue  = 0;
    }
    ~Color3DS(){};
};

typedef struct{
    float           x;
    float           y;
    float           z;
}Vector3DS;

typedef struct{
    float           u;
    float           v;
}TextCoor3DS;

typedef struct{
    Vector3DS       SCoor;
    TextCoor3DS     TCoor;
}Vertex3DS;

typedef struct{
    word            Va_indx;
    word            Vb_indx;
    word            Vc_indx;
}Face3DS;

class FMatList3DS{
public:
    shString        Name;
    word            Id;

    word            FCount;
    word            *F_index;

    FMatList3DS(){
        Name    = NULL;
        Id      = 0;
        FCount  = 0;
        F_index = NULL;
    }

    ~FMatList3DS(){
        Name.clear();
        if(FCount)delete[] F_index;
    }
};

typedef struct{
    Vector3DS       x_axis;
    Vector3DS       y_axis;
    Vector3DS       z_axis;
    Vector3DS       point;
}Basis3DS;

#pragma pack(push, 1)
typedef struct
{
	short frame_number;
	DWORD unknown;
	float rotation;         /* in radians */
	float axis_x, axis_y, axis_z;
}Rotation3DS;

typedef struct
{
	short			flags;
	short			unknown[4];
	short			keys;
	short			unknown2;
	Rotation3DS		*rotation;
}RotTrackTag3DS;

typedef struct
{
	shString		name;
	Vector3DS		pivot;	
	RotTrackTag3DS	rotTag;
}KeyFrameObjectInfo3DS;

typedef struct
{
	KeyFrameObjectInfo3DS	*info;
}KeyFrame3DS; 
#pragma pack(pop)

typedef struct{
    dword           Type;
    Vector3DS       Pos;
    Color3DS        Color;
    Vector3DS       Target;
    float           HotSpot;
    float           FallOff;
}Light3DS;

class Object3DS{
public:
    shString        Name;
    
    Basis3DS        Basis;
    
    Vertex3DS       *Vert;
    word            VCount;
    
    Face3DS         *Face;
    word            FCount;
    
    FMatList3DS     *MList;    
    word            MLCount;
    
    Light3DS        *Light;
    word            LCount;

    Object3DS(){
        Name  = NULL;
        Vert  = NULL;
        Face  = NULL;
        Face  = NULL;
        MList = NULL;
        Light = NULL;
        VCount = FCount = MLCount = LCount = 0;
    }
    ~Object3DS(){
        Name.clear();
		if(Vert)delete[] Vert;
        if(Face)delete[] Face;
        if(MList)delete[] MList;
        if(Light)delete[] Light;
    }
};

class Map3DS{
public:
    shString        Name;
    float           percent;  
    word            tiling;   
    byte            ignorealpha;
    word            filter; 
    float           blur;
    byte            mirror;
    byte            negative;
    float           uscale;
    float           vscale;
    float           uoffset;
    float           voffset;
    float           rotation;
    word            source;
    Color3DS        tint1;
    Color3DS        tint2;
    Color3DS        redtint;
    Color3DS        greentint;
    Color3DS        bluetint;

	Map3DS(){ ZeroMemory(this,sizeof(Map3DS)); }
	~Map3DS(){ Name.clear(); }
};

class Material3DS{
public:    
    shString        Name;
    word            Id;
    
    Color3DS        Ambient;
    Color3DS        Diffuse;
    Color3DS        Specular;

    float           Shininess;
    float           ShinStrenght;
    
    float           Transparency;
    
    Map3DS			Texture1;
	Map3DS			Bump;
	Map3DS			Opacity;
	Map3DS			Reflection;
	
	Material3DS(){ Name = NULL; 
                   Shininess = 0;
                   ShinStrenght = 0;
                 }
   ~Material3DS(){ Name.clear(); }
};       

class File3DS{
	int				  real_obj_count;
public:
    Material3DS       *Material;
    Object3DS         *Object;
    word              MatCount;
    word              ObjCount;
    KeyFrame3DS		  keyFrame;
	word			  keyFrameCount;

    File3DS(){ Material  = NULL; 
               Object    = NULL;
               MatCount  = 0; 
               ObjCount  = 0; 
				real_obj_count = -1;
				keyFrame.info = NULL;
			}
    
    File3DS(TCHAR *FileName){ Load(FileName); };
   ~File3DS(){ Close(); }

    bool Load(TCHAR *FileName);
    void Close();

	void apply_texture_path(TCHAR *path);
	
	void basisToVectors(Basis3DS &b, Vector3 x, Vector3 y, Vector3 z);
	void mirrorVector(Vector3DS &v);
	void correct_for_dx();
	void transform(Matrix4	mat,DWORD obj_ind);
	void transform(Matrix4	mat);
	void scale(float x,float y,float z);
	void offset( float x, float y, float z );
	void offset( float x, float y, float z, DWORD obj_ind );
	void circle_deform_dx( float radius, int axis ); // 0 - x, 1 - y, 2 - z
	void circle_deform_dx( float radius, int axis, DWORD obj_ind );
	int	 get_real_obj_count();
	int  getRealObjectByIndex( int index );
	Material3DS*	 getMaterialByName( shString &name );
};

word GetChunkCount(std::ifstream &is,word ChunkId,dword Lenght);
std::ifstream& operator>>(std::ifstream &is,ChunkHdr3DS &ch);
std::ifstream& operator>>(std::ifstream &is,Color3DS &col);
std::ifstream& operator>>(std::ifstream &is,Vector3DS &vec);
std::ifstream& operator>>(std::ifstream &is,TextCoor3DS &tcoor);
std::ifstream& operator>>(std::ifstream &is,Face3DS &face);
std::ifstream& operator>>(std::ifstream &is,Basis3DS &bas);
std::ifstream& operator>>(std::ifstream &is,Light3DS &light);
std::ifstream& operator>>(std::ifstream &is,FMatList3DS &fml);
std::ifstream& operator>>(std::ifstream &us,Object3DS &obj);
std::ifstream& operator>>(std::ifstream &is,Material3DS &mat);
std::ifstream& operator>>(std::ifstream &is,Map3DS &mp);

std::ofstream& operator<<(std::ofstream &os,ChunkHdr3DS &ch);
std::ofstream& operator<<(std::ofstream &os,Color3DS &col);
std::ofstream& operator<<(std::ofstream &os,Vector3DS &vec);
std::ofstream& operator<<(std::ofstream &os,TextCoor3DS &tcoor);
std::ofstream& operator<<(std::ofstream &os,Face3DS &face);
std::ofstream& operator<<(std::ofstream &os,Basis3DS &bas);
std::ofstream& operator<<(std::ofstream &os,Light3DS &light);
std::ofstream& operator<<(std::ofstream &os,FMatList3DS &fml);
std::ofstream& operator<<(std::ofstream &os,Object3DS &obj);
std::ofstream& operator<<(std::ofstream &os,Material3DS &mat);
std::ofstream& operator<<(std::ofstream &os,Map3DS &mp);

#endif