#include "a3dModelDesc.h"
#include "..\\a3dScene.h"
#include "..\\a3dModel\\a3dModel.h"
#include "..\\a3dModelRef\\a3dModelRef.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include "..\\a3dUtils\\a3dTextUtils.h"

a3dModelDesc::~a3dModelDesc() {
	if(this->model != null)
		delete this->model;
	this->model = null;
};

int a3dModelDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->fileName.calcSize();
}

bool a3dModelDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dModelDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;

	return true;
}

void a3dModelDesc::updateModel( a3dScene *scene ) { 

	if(this->model == null) return;
	this->model->skipMaterialErrors(scene->designMode);
}

HRESULT a3dModelDesc::loadModel( a3dScene *scene ) { 

	HRESULT rv;

	this->kernel = scene->kernel;
	this->fullPathName = this->fileName;
	this->fullPathName.appendPath( scene->modelDirectory );

	this->model = new a3dModel();

	if( !this->model->loadFromFile( this->fullPathName ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't load from file '%s'" ), this->fullPathName.getCString() );	
		this->kernel->ErrorBox( TEXT( "a3dModelDesc" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return D3DERR_INVALIDCALL;
	}

	this->updateModel( scene );

	this->model->setKernel( this->kernel );
	rv = this->model->initialize();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't initialize model" ) );	
		this->kernel->ErrorBox( TEXT( "a3dModelDesc" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return D3DERR_INVALIDCALL;
	}

	rv = this->model->initializeData( scene );
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't initialize model data" ) );	
		this->kernel->ErrorBox( TEXT( "a3dModelDesc" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return D3DERR_INVALIDCALL;
	}

	return D3D_OK;
}

a3dModelRef* a3dModelDesc::createReference() { 
	return new a3dModelRef(this);
}