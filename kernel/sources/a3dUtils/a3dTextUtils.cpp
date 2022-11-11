#include "a3dTextUtils.h"

a3dOutput* a3dOutput::Default = new a3dOutput();

void a3dOutput::printInfo( TCHAR *format, ... ) { 
	va_list args;

	va_start( args, format );
	_vstprintf_s( this->tempBuffer, this->bufferSize, format, args );
	this->outInfo(this->tempBuffer);
}

void a3dOutput::printWarning( TCHAR *format, ... ) { 
	va_list args;

	va_start( args, format );
	_vstprintf_s( this->tempBuffer, this->bufferSize, format, args );
	this->outWarning(this->tempBuffer);
}

void a3dOutput::printError( TCHAR *format, ... ) { 
	va_list args;

	va_start( args, format );
	_vstprintf_s( this->tempBuffer, this->bufferSize, format, args );
	this->outError(this->tempBuffer);
}
