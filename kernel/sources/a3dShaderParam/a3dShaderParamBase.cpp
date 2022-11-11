#include "a3dShaderParamBase.h"
#include "memoryFile.h"

a3dShaderParamBase* a3dShaderParamBase::clone() { 

	a3dShaderParamBase *param = new a3dShaderParamBase( this->paramName );
	param->assign(this);
	return param;
}

void a3dShaderParamBase::assign(a3dShaderParamBase* p) { 
	
	this->material = p->material;
	this->uiWidget = p->uiWidget;
	this->used = p->used;
	this->type = p->type;
	this->row = p->row;
	this->col = p->col;
	this->arraySize1 = p->arraySize1;
	this->arraySize2 = p->arraySize2;
	this->arraySize3 = p->arraySize3;
	this->paramName = p->paramName;
	this->paramName.copyToChar( name );
	this->version = p->version;
	this->token = p->token;
}

int a3dShaderParamBase::calcSizeCore() { 

	int sz = sizeof( int ) + this->paramName.calcSize() + sizeof( bool ); 
	if( this->supportVersion( a3dVersion(1,1) ) ) {
		sz += sizeof(int); // widgetType
		sz += this->resourceType.calcSize();
		sz += this->uiName.calcSize();
		sz += this->object.calcSize();
		sz += this->space.calcSize();
		sz += sizeof(int) * 5; // row col arraySize1 - 3
	}
	if( this->supportVersion( a3dVersion(1,2) ) ) { 
		sz += this->token.calcSize();
	}
	if( this->supportVersion( a3dVersion(1,3) ) ) { 
		sz += a3dDependencyPropertyOwner::calcSizeCore();
	}
	return sz;
}

a3dShaderParamBase::a3dShaderParamBase(fxParamInfo *info) { 
	
	this->material = null;
	this->version = a3dVersion(1,2);
	
	this->used = true;
	this->type = SPT_BASE;
	this->uiWidget = SPWT_NONE;
	
	this->paramName = info->paramName;
	paramName.copyToChar( name );

	this->token = info->token;

	this->row = info->row;
	this->col = info->col;
	this->arraySize1 = info->itemCount;
	this->arraySize2 = info->itemCount2;
	this->arraySize3 = info->itemCount3;

	this->initializeUIInfo(info);
}

bool a3dShaderParamBase::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !file.writeInt( (int)this->type ) ) return false;
	if( !this->paramName.saveToMemory( file ) ) return false;
	if( !file.writeBool( this->used ) ) return false;
	if( this->supportVersion( a3dVersion(1,1) ) ) { 
		if(!file.writeInt( (int)this->uiWidget ) ) return false;
		if(!this->resourceType.saveToMemory(file)) return false;
		if(!this->uiName.saveToMemory(file)) return false;
		if(!this->object.saveToMemory(file)) return false;
		if(!this->space.saveToMemory(file)) return false;
		if(!file.writeInt(this->row)) return false;
		if(!file.writeInt(this->col)) return false;
		if(!file.writeInt(this->arraySize1)) return false;
		if(!file.writeInt(this->arraySize2)) return false;
		if(!file.writeInt(this->arraySize3)) return false;
	}
	if( this->supportVersion( a3dVersion(1,2) ) )
		if(!this->token.saveToMemory(file)) return false;
	
	if( this->supportVersion( a3dVersion(1,3)) ) {
		if(!a3dDependencyPropertyOwner::saveToMemoryCore(file)) 
			return false;
	}

	return true;
}

bool a3dShaderParamBase::loadFromMemoryCore( a3dMemoryFile &file ) { 

	int tp;

	if( !file.readInt( &tp ) ) return false;
	this->type = (a3dShaderParamType) tp;
	if( !this->paramName.loadFromMemory( file ) ) return false;
	if( !file.readBool( &this->used ) ) return false;
	if( this->supportVersion( a3dVersion(1,1) ) ) { 
		if(!file.readInt( &tp ) ) return false;
		this->uiWidget = (a3dShaderParamWidgetType)tp;
		if(!this->resourceType.loadFromMemory(file)) return false;
		if(!this->uiName.loadFromMemory(file)) return false;
		if(!this->object.loadFromMemory(file)) return false;
		if(!this->space.loadFromMemory(file)) return false;
		if(!file.readInt(&this->row)) return false;
		if(!file.readInt(&this->col)) return false;
		if(!file.readInt(&this->arraySize1)) return false;
		if(!file.readInt(&this->arraySize2)) return false;
		if(!file.readInt(&this->arraySize3)) return false;
	}

	if( this->supportVersion( a3dVersion(1,2) ) )
		if(!this->token.loadFromMemory(file))
			return false;

	this->paramName.copyToChar( this->name );
	
	if( this->supportVersion( a3dVersion(1,3) ) ) {
		if(!a3dDependencyPropertyOwner::loadFromMemoryCore(file))
			return false;
	
		this->registerDependencyProperties();	
	}

	return true;
}

void a3dShaderParamBase::initializeUIInfo(fxParamInfo *info) { 
	
	this->uiWidget = info->uiWidget;
	this->uiName = info->uiName;
	this->resourceType = info->resourceType;
	this->object = info->object;
	this->space = info->space;
}

void a3dShaderParamBase::registerDependencyProperties() { 
	
}