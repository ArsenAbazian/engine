#include "rand.h"
#include <windows.h>

float __a3d_fastcall generateValueFromRange( float minVal, float maxVal )
{

    float randVal = (float)rand();
    float disp = maxVal - minVal;
		
    randVal *= ( 1.0f / RAND_MAX ); 
    randVal  = randVal * disp + minVal;
	
    return randVal;

}

float __a3d_fastcall getValueWithDelta( float a, float da ) {
	
	float val;

	val  = ( float ) rand();
	val *= ( 1.0f / RAND_MAX );
	val *= da;
	val += a;

	return val;
}