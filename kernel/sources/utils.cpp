#include "utils.h"

void RGB2Float( DWORD color, float *fcol )
{

	DWORD a, r, g, b;

	a =   color >> 24;
	r = ( color >> 16 ) & 0xff;
	g = ( color >> 8  ) & 0xff;
	b = ( color ) & 0xff;

	fcol[0] = a * ( 1.0f / 255.0f );
	fcol[1] = r * ( 1.0f / 255.0f );
	fcol[2] = g * ( 1.0f / 255.0f );
	fcol[3] = b * ( 1.0f / 255.0f );

	return ;
}

DWORD Float2RGB( float *fcol )
{

	DWORD a, r, g, b;
	DWORD color;

	a = (DWORD)(fcol[0] * 255.0f);
	r = (DWORD)(fcol[1] * 255.0f);
	g = (DWORD)(fcol[2] * 255.0f);
	b = (DWORD)(fcol[3] * 255.0f);
	
	color = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;

	return color;	
}

void InterpolateColors(DWORD *dwr, DWORD *dwg, DWORD *dwb, DWORD dwr1, DWORD dwg1, DWORD dwb1, DWORD dwr2, DWORD dwg2, DWORD dwb2, float fdr, float fdg, float fdb, bool inv){

	float r, g, b;
	
	r = (float)*dwr;
	g = (float)*dwg;
	b = (float)*dwb;
		
	float r1, r2, g1, g2, b1, b2;

	r1 = (float)dwr1;
    r2 = (float)dwr2;
	g1 = (float)dwg1;
	g2 = (float)dwg2;
	b1 = (float)dwb1;
	b2 = (float)dwb2;
	
	if(!inv){
	
		// граничные условия
		if(fdr > 0.0f){
			if( (DWORD)(r + fdr) > r2)r = r2;
			else r += fdr;
		}
		else {
			if( (DWORD)(r + fdr) < r2)r = r2;
			else r += fdr;
		}

		if(fdg > 0.0f){
			if( (DWORD)(g + fdg) > g2)g = g2;
			else g += fdg;
		}
		else {
			if( (DWORD)(g + fdg) < g2)g = g2;
			else g += fdg;
		}
			
		if(fdb > 0.0f){
			if( (DWORD)(b + fdb) > b2)b = b2;
			else b += fdb;
		}
		else {
			if( (DWORD)(b + fdb) < b2)b = b2;
			else b += fdb;
		}
	}

	else {
	
		// граничные условия
		if(fdr > 0.0f){
			if( (DWORD)(r - fdr) < r1)r = r1;
			else r -= fdr;
		}
		else {
			if( (DWORD)(r - fdr) > r1)r = r1;
			else r -= fdr;
		}

		if(fdg > 0.0f){
			if( (DWORD)(g - fdg) < g1)g = g1;
			else g -= fdg;
		}
		else {
			if( (DWORD)(g - fdg) > g1)g = g1;
			else g -= fdg;
		}
			
		if(fdb > 0.0f){
			if( (DWORD)(b - fdb) < b1)b = b1;
			else b -= fdb;
		}
		else {
			if( (DWORD)(b - fdb) > b1)b = b1;
			else b -= fdb;
		}

	}
	
	*dwr = (DWORD)r;
	*dwg = (DWORD)g;
	*dwb = (DWORD)b;

	return ;
}

void InterpolateFloat(float *f, float f1, float f2, float df, bool inv){

	if(!inv){
	
		if(df > 0.0f){
			if(*f + df > f2)*f = f2;			
			else *f += df; 
		}
		else {
			if(* f + df < f2)*f = f2;
			else *f += df;
		}
	}
	else {
		
		if(df > 0.0f){
			if(*f - df < f1)*f = f1;			
			else *f -= df; 
		}
		else {
			if(* f - df > f1)*f = f1;
			else *f -= df;
		}
	}
}