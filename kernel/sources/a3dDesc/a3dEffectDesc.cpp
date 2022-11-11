#include "a3dEffectDesc.h"
#include "..\\hlslman.h"
#include "..\\a3dScene.h"
#include <dxerr9.h>
#include "..\\a3dUtils\\a3dTextUtils.h"

int a3dEffectDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->fileName.calcSize(); 
}

bool a3dEffectDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dEffectDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;

	return true;
}

HRESULT	a3dEffectDesc::loadEffect( a3dScene *scene ) { 

	HLSLManager manager;
	HRESULT rv;

	this->kernel = scene->kernel;
	this->fullPathName = this->fileName;
	this->fullPathName.appendPath( scene->effectDirectory );

	rv = manager.createEffectFromFile( this->kernel->lpDevice, this->fullPathName );
	if( FAILED( rv ) ) {
		if( manager.result != null ) {
			_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT("%s"), manager.result.getCString() );
			this->kernel->ErrorBox( TEXT( "a3dEffectDesc" ) );
			a3dOutput::Default->printError( TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		}
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't load effect from file '%s' D3DXCreateEffect -> (%s)" ), this->fullPathName.getCString(), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dEffectDesc" ) );
		a3dOutput::Default->printError( TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);

		return rv;
	}

	this->lpEffect = manager.lpEffect;

	rv = lpEffect->OnResetDevice();
	if(FAILED(rv))
	{
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't load effect from file '%s' OnResetDevice -> (%s)" ), this->fullPathName.getCString(), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dEffectDesc" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);

		lpEffect->Release();
		return rv;
	}	

	return D3D_OK;
}