#ifndef ARS_UTILS_INC
#define ARS_UTILS_INC

#include <windows.h>

void InterpolateColors(DWORD *r, DWORD *g, DWORD *b, DWORD r1, DWORD g1, DWORD b1, DWORD r2, DWORD g2, DWORD b2, float fdr, float fdg, float fdb, bool inv);
void InterpolateFloat(float *f, float f1, float f2, float df, bool inv);
void RGB2Float( DWORD color, float *fcol );
DWORD Float2RGB( float *fcol );

#endif