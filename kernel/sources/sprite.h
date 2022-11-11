/*********************************************************************
*	
*		class Sprite	- sprite representation
*		created by Abazyan Arsen
*		
*		Tula 02.11.2004		12:29
*		e-mail: ars_s@uic.tula.ru
*		
*		information:
*			- supported formats: BMP TGA JPG PNG DDS
*			- 
**********************************************************************/

#ifndef SPRITE_INC
#define SPRITE_INC

#include "anim.h"
#include "texture.h"

#define SPRITEVERTEX	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

typedef struct{
	Vector3     SCoor;
    float       RHW;
    DWORD       Color;
    Vector2     T1Coor;	
}SpriteVertex;

typedef struct{
	
	float                       fPosX0;
	float                       fPosY0;
	float                       fPosZ0;
	
	float                       fImagePosX0;
	float                       fImagePosY0;

	float                       fRotationAngle;

	float						fScaleX;
	float						fScaleY;
	
	float                       u0;
	float						u1;
	float						v0;
	float						v1;
	
	float						fWidth;
	float						fHeight;

	DWORD						dwColor;

}SpriteAnimationParams;

class Sprite{
	
	LPDIRECT3DVERTEXBUFFER9		lpVert;
	SpriteVertex				*lpBuffer;
	bool                        bSaved;

public:	
	Kernel						*SpriteKernel;
	Texture						*SpriteTexture;

	float                       fPosX0;
	float                       fPosY0;
	float                       fPosZ0;
	
	float                       fImagePosX0;
	float                       fImagePosY0;

	float                       fRotationAngle;

	float						fScaleX;
	float						fScaleY;
	
	float                       u0;
	float						u1;
	float						v0;
	float						v1;
	
	float						fWidth;
	float						fHeight;
	
	DWORD						dwColor;
	
	Sprite(){ ZeroMemory(this,sizeof(Sprite)); }
	~Sprite(){ Clear(); }

	HRESULT Init();
	HRESULT Clear();

	HRESULT	Save();
	HRESULT Restore();

	HRESULT Update();
	HRESULT Render();

	void    ApplyAnimWithoutPos(SpriteAnimationParams &spAnimPars);
	
	void    ScreenToLocal(float x, float y, float *loc_x, float *loc_y);
	bool    HitTest(float x,float y);
	bool    HitTestEx(float x, float y);
	
	void SetKernel(Kernel *krn){SpriteKernel = krn; }

	void Copy(const Sprite &sp);
	void CopyWithoutPos(const Sprite &sp);
};

#endif