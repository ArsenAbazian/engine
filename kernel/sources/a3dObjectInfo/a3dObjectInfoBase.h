#ifndef A3D_OBJECT_INFO_BASE_INC
#define A3D_OBJECT_INFO_BASE_INC

#include "hstring.h"
#include "serialization.h"
#include "..\\headerCodes.h"

class a3dMemoryFile;
class a3dObjectInfoBase : public a3dSerializableObject { 
public:
	shString	name;

	a3dObjectInfoBase() { }
	~a3dObjectInfoBase() { }

	void	copyTo( a3dObjectInfoBase *obj );

	int		getHeaderCode() { return a3dHeaders::a3dObjectInfoHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);
};

#endif