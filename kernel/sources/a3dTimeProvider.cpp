#include "a3dTimeProvider.h"

shString a3dTimeProviderBase::className = shString( TEXT( "TimeProviderBase" ) );

void a3dTimeProviderBase::initializeDefault() { 
	this->fTime = 0.0f;	
}

void a3dTimeProviderBase::registerDependencyProperties() { 
	
	this->timeProperty = this->registerDependencyProperty(TEXT("Time"), TEXT("fTime"), DPROP_FLOAT, &(this->fTime));
}