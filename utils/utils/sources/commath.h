#ifndef COMMATH_INC
#define COMMATH_INC

#include <windows.h>
#include <math.h>
#include "defines.h"

#define ONE_AS_INTEGER ((DWORD)(0x3F800000))

// быстрый перевод из вещественного в целый.
__inline void myFtoL( int *intVal, float floatVal )
{
  __asm	fld floatVal;
  __asm	mov edx, intVal
  __asm	fistp [edx];
}
 
float __a3d_fastcall	ulrsqrt( float x );	// быстрое вычисление x / sqrtx
float __a3d_fastcall	ulsqrt( float x );	// быстрое вычисление корня

// быстрое обратное вычисление обратного корня
float __a3d_fastcall	invSqrt( const float & x );	// 1 / корень

#endif