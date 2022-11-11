#ifndef	IFONT2D_INC
#define IFONT2D_INC

#include "texture.h"

#define MAXLETTERS  300
#define FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

typedef struct{
    float x0;
    float y0;
    float x1;
    float x2;
    float y1;
    float x3;
}CRECT;

typedef struct{
    Vector3     SCoor;
    float       RHW;
    DWORD       Color;
    Vector2     T1Coor;
}FontVertex;

class a3dFont : public Texture{
    FontVertex							*lpVBuffer;	
	WORD								*lpIBuffer;
		
	bool								bSaved;
	
	LPDIRECT3DVERTEXBUFFER9             lpVert;
    LPDIRECT3DINDEXBUFFER9              lpIndex;
       
    CRECT                               Pos[160];
    
    DWORD                               FontRState;
    DWORD                               FontPRState;
    DWORD                               dwItalic;
	WORD								dwLength;
	WORD                                dwFaceCount;

public:
    a3dFont(){ ZeroMemory(this,sizeof(a3dFont)); }
   ~a3dFont(){ Clear(); } 
           
	float		fFontX;
	float		fFontY;
   
	float		fPrintedWidth;
	float		fPrintedHeight;

	bool		LoadPositions(TCHAR *FileName);
    
	HRESULT		Create();
    HRESULT		Clear();
    
	HRESULT		Print(TCHAR *String,float x,float y,DWORD Color);
    HRESULT		Print(TCHAR *String,float x,float y,float scalex,float scaley,DWORD Color);
    void		Italic(int);
	
	HRESULT		Save();
	HRESULT		Restore();
	
	HRESULT		Render();
};

#endif
