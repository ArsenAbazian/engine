#ifndef A3DMODELFILE_INC
#define A3DMODELFILE_INC

#include "kernel.h"

static const WORD fileId = 0x0a3d;

class a3dMFileObject { 
public:
};

class a3dModelFile { 
public:
	int					objectCount;
	a3dMFileObject		*object;
	
};

#endif