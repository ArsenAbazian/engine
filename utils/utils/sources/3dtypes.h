#ifndef    TYPES_3D_INCLUDED
#define    TYPES_3D_INCLUDED

#include <d3d9.h>
#include "vecmat.h"

#define		null NULL

//=====================Color formats=================

#pragma pack(push, 1)
typedef struct{
    unsigned long b:8;
    unsigned long g:8;
    unsigned long r:8;
    unsigned long a:8;
}Color8888;

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}Color888;

typedef struct{
    unsigned short b:5;
    unsigned short g:6;
    unsigned short r:5;
}Color565;

typedef struct{
    unsigned short b:5;
    unsigned short g:5;
    unsigned short r:5;
    unsigned short a:1;
}Color1555;

typedef struct{
    unsigned short b:4;
    unsigned short g:4;
    unsigned short r:4;
    unsigned short a:4;
}Color4444;

#define SVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)
typedef struct{
    Vector3    SCoor;
    Vector3    Normal;
}Vertex;

#define T1VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
typedef struct{
    Vector3    SCoor;
    Vector3    Normal;
    Vector2    T1Coor;
}T1Vertex;

#define T2VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)
typedef  struct{
    Vector3    SCoor;
    Vector3    Normal;
    Vector2    T1Coor;
    Vector2    T2Coor;
}T2Vertex;

typedef struct{
	DWORD	va;
	DWORD	vb;
	DWORD   vc;
	Vector3	Normal;
}Plane;

typedef struct {
    WORD    Va_indx;
    WORD    Vb_indx;
    WORD    Vc_indx;
}Face;

typedef struct{
    Vertex   Va;
    Vertex   Vb;
    Vertex   Vc;
}Facet;

typedef struct{
    T2Vertex   Va;
    T2Vertex   Vb;
    T2Vertex   Vc;
}T2Facet;

typedef struct{
	float v0;
	float v1;
}v0v1;

typedef struct{
	float ang;
	float move;
}rot_move;

typedef struct{
	float u0,v0;
	float u1,v1;
}FRECT;

typedef struct { 
	int aIndex;
	int bIndex;
	int cIndex;
} a3dFace;

#pragma pack(pop)

#endif
