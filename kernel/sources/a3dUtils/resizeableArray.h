#ifndef A3D_RESIZEABLE_ARRAY
#define A3D_RESIZEABLE_ARRAY

#include "3dtypes.h"

class resizeableArray { 
	void	increaseSize();
public:
	int		*lpArray;
	int		arraySize;
	int		count;
	
	resizeableArray() { 
		this->lpArray = null;
		this->arraySize = 0;
		this->count = 0;
	}
	~resizeableArray() { this->clear(); }

	void	clear();
	void	addValue(int value);
	void	removeValueByIndex(int index);
};

#endif