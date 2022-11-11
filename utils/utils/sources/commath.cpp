#include "commath.h"

// реализация вычисления корня от nvidia

static float _0_47 = 0.47f;
static float _1_47 = 1.47f;

float __a3d_fastcall ulrsqrt(float x)
{
  DWORD y;
  float r;
  _asm
  {
    mov     eax, 07F000000h+03F800000h // (ONE_AS_INTEGER<<1) + ONE_AS_INTEGER
    sub     eax, x
    sar     eax, 1

    mov     y, eax                      // y
    fld     _0_47                       // 0.47
    fmul    DWORD PTR x                 // x*0.47

    fld     DWORD PTR y
    fld     st(0)                       // y y x*0.47
    fmul    st(0), st(1)                // y*y y x*0.47

    fld     _1_47                       // 1.47 y*y y x*0.47
    fxch    st(3)                       // x*0.47 y*y y 1.47
    fmulp   st(1), st(0)                // x*0.47*y*y y 1.47
    fsubp   st(2), st(0)                // y 1.47-x*0.47*y*y
    fmulp   st(1), st(0)                // result
    fstp    y
    and     y, 07FFFFFFFh
    
  }
  r = *(float *)&y;
  // optional
  r = (3.0f - x * (r * r)) * r * 0.5f; // remove for low accuracy
  return r;
}

/*
    sqrt(x) = x / sqrt(x)
*/
float __a3d_fastcall ulsqrt( float val )
{
	return val * ulrsqrt(val);
}

float __a3d_fastcall invSqrt(const float & x)
{
  
	DWORD   tmp = ((ONE_AS_INTEGER << 1) + ONE_AS_INTEGER - *(DWORD*)&x) >> 1;
	
	float y = *(float*)&tmp;
	
	return y * (1.47f - 0.47f * x * y * y);
}
