#ifndef A3D_OBJECT_DESC_BASE_INC
#define A3D_OBJECT_DESC_BASE_INC

#include "..\\kernel.h"
#include "..\\headerCodes.h"

class a3dObjectDescBase : public a3dSerializableObject { 
public:
	Kernel		*kernel;
	bool		used;	// cashing
	shString	name;

	a3dObjectDescBase() { }
	~a3dObjectDescBase() { }

	int		getHeaderCode() { return a3dHeaders::a3dObjectDescBaseHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );
};

#endif