#include "resizeableArray.h"
#include <windows.h>

void resizeableArray::clear() { 
	
	if(this->lpArray != null) 
		delete[] this->lpArray;
	this->lpArray = null;
	this->arraySize = 0;
	this->count = 0;
}

void resizeableArray::increaseSize() { 
	
	if(this->arraySize == 0) 
		this->arraySize = 1;
	else
		this->arraySize *= 2;
	
	int *tmp = new int[this->arraySize];
	memcpy( tmp, this->lpArray, this->count * sizeof(int));
	delete[] this->lpArray;
	this->lpArray = tmp;
}

void resizeableArray::addValue(int value) { 
	
	if(this->count == this->arraySize) 
		this->increaseSize();
	this->lpArray[this->count] = value;
	this->count++;
}

void resizeableArray::removeValueByIndex(int index) { 

	if(index < 0 || index >= this->count) return;
	
	if(index < this->count - 1)
		memcpy(&this->lpArray[index], &this->lpArray[index + 1], sizeof(int) * ( this->count - index - 1 ) );
	this->count--;
} 