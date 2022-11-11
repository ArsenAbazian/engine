#include "a3dShaderMaterial.h"
#include "memoryFile.h"
#include "..\\a3dShaderParam\\a3dShaderParams.h"
#include "..\\a3dScene.h"
#include "..\\texture.h"
#include "..\\a3dTextFileTokens.h"
#include <d3dx9effect.h>
#include <dxerr9.h>
#include "..\\fxFileParser\\fxFileParser.h"

void a3dShaderMaterial::initializeDefault() { 
	this->version = a3dVersion(1, 2);
	this->type = MT_SHADER;
	this->paramList = null;
	this->paramCount = 0;
	this->renderEnabled = true;
	this->effect = null;
	this->skipErrors = false;
	this->techniqueCount = 0;
	this->techniqueList = null;
	this->worldXf = TEXT("");
	this->worldITXf = TEXT("");
	this->wvpXf = TEXT("");
	this->viewIXf = TEXT("");
	*(this->worldXfParam) = 0;
	*(this->worldITXfParam) = 0;
	*(this->wvpXfParam) = 0;
	*(this->viewIXfParam) = 0;
	this->scene = null;
}

int a3dShaderMaterial::calcSizeCore() {

	int totalSize = 0;

	totalSize += a3dMaterialBase::calcSizeCore() + this->fileName.calcSize()
		+ sizeof( bool ) + sizeof( int ); // paramCount

	if(this->version >= a3dVersion(1,2)) {
		totalSize += this->worldXf.calcSize();
		totalSize += this->worldITXf.calcSize();
		totalSize += this->wvpXf.calcSize();
		totalSize += this->viewIXf.calcSize();
	}
	
	int pi;
	for( pi = 0; pi < this->paramCount; pi ++ ) { 
		totalSize += this->paramList[pi]->calcSize(); // size + content
	}

	return totalSize;	
}

HRESULT a3dShaderMaterial::initializeData( a3dScene *scene ) { 

	a3dEffectDesc *edesc = scene->getEffect( this->fileName );
	if( edesc == null ) return D3DERR_NOTFOUND;	

	a3dTextureDesc *tdesc;
	a3dTextureShaderParam *tparam;
	a3dTechniqueShaderParam *techParam;
	
	this->effect = edesc;
	this->setScene( scene );

	shString timeString( TEXT("TIME") );
	
	// not init textures
	for( int i = 0; i < this->paramCount; i++ ) { 
		if( this->paramList[i]->type == SPT_TEXTURE ) {
			tparam = (a3dTextureShaderParam*) this->paramList[i];

			tdesc = scene->getTexture( tparam->fileName );
			if( tdesc == null && !scene->designMode) return D3DERR_NOTFOUND;

			tparam->textureDesc = tdesc;
		}
		else if( this->paramList[i]->type == SPT_TECHNIQUE ) { 
			techParam = dynamic_cast<a3dTechniqueShaderParam*>(this->paramList[i]);
			techParam->initializeHandle(this->effect);
		}
		
		a3dFloatShaderParam *floatParam = dynamic_cast<a3dFloatShaderParam*>(this->paramList[i]);
		if( floatParam != null && floatParam->token == timeString ) { 
			floatParam->floatValueProperty->simpleBind( scene->timeProvider.timeProperty );
		}
	}

	return D3D_OK;
}

void a3dShaderMaterial::updateParametersDependency() { 
	
	shString timeString( TEXT("TIME") );

	for( int i = 0; i < this->paramCount; i++ ) { 
		a3dFloatShaderParam *floatParam = dynamic_cast<a3dFloatShaderParam*>(this->paramList[i]);
		if( floatParam != null && floatParam->token == timeString && this->scene != null) { 
			floatParam->floatValueProperty->simpleBind( this->scene->timeProvider.timeProperty );
		}
	}
}

bool a3dShaderMaterial::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	if( !isToken( li, a3dTextFileTokens::a3dNameString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringValue( li, this->name ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
   
	if( !this->importParams( lexList, li ) ) return false;

	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dDirectX_9_ShaderString ) ) return false;	

	return true;
}

bool a3dShaderMaterial::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dMaterialBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;
	if( !file.readBool( &this->renderEnabled ) ) return false;
	if( !file.readInt( &this->paramCount ) ) return false;

	if(this->version >= a3dVersion(1,2)) { 
		if(!this->worldXf.loadFromMemory(file)) return false;
		if(!this->worldITXf.loadFromMemory(file)) return false;
		if(!this->wvpXf.loadFromMemory(file)) return false;
		if(!this->viewIXf.loadFromMemory(file)) return false;
		this->updatePredefinedMatrixParams();
	}

	int pi;
	if( this->paramCount > 0 ) { 
		this->paramList = new a3dShaderParamBase*[ this->paramCount ];
		for( pi = 0; pi < this->paramCount; pi ++ ) { 
			this->paramList[ pi ] = null;
		}
	}

	BYTE *pos = NULL;
	int tp;

	for( pi = 0; pi < this->paramCount; pi ++ ) { 

		// reading param type
		file.savePos();
		if( !file.readInt( &tp ) ) return false; // skip header
		if( !file.readInt( &tp ) ) return false; // skip size
		if( !file.readInt( &tp ) ) return false; // read type
		if( !file.readInt( &tp ) ) return false; // read type
		file.restorePos();

		this->paramList[ pi ] = this->createParam( (a3dShaderParamType) tp );
		this->paramList[ pi ]->registerDependencyProperties();
		this->paramList[ pi ]->material = this;

		if( this->paramList[ pi ] == null ) return false;
		if( !this->paramList[ pi ]->loadFromMemory( file ) ) return false;
	}

	return true;
}

a3dShaderParamBase* a3dShaderMaterial::createParam( a3dShaderParamType ptype ) { 

	shString emptyName;

	switch( ptype ) { 
		case SPT_BASE: 
			return new a3dShaderParamBase( emptyName );
		case SPT_BOOL:
			return new a3dBoolShaderParam( emptyName );
		case SPT_BOOL_ARRAY:
			return new a3dBoolArrayShaderParam( emptyName );
		case SPT_FLOAT: 
			return new a3dFloatShaderParam( emptyName );
		case SPT_FLOAT_ARRAY: 
			return new a3dFloatArrayShaderParam( emptyName );
		case SPT_COLOR:
			return new a3dColorShaderParam( emptyName );
		case SPT_VECTOR:
			return new a3dVectorShaderParam( emptyName );
		case SPT_INT:
			return new a3dIntShaderParam( emptyName );
		case SPT_INT_ARRAY:
			return new a3dIntArrayShaderParam( emptyName );
		case SPT_MATRIX:
			return new a3dMatrixShaderParam( emptyName );
		case SPT_MATRIX_ARRAY:
			return new a3dMatrixArrayShaderParam( emptyName );
		case SPT_TEXTURE:
			return new a3dTextureShaderParam( emptyName );
		case SPT_TECHNIQUE:
			return new a3dTechniqueShaderParam( emptyName );
	}

	return null;
}

bool a3dShaderMaterial::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dMaterialBase::saveToMemoryCore( file ) ) return false; 
	if( !this->fileName.saveToMemory( file ) ) return false;
	if( !file.writeBool( this->renderEnabled ) ) return false;
	if( !file.writeInt( this->paramCount ) ) return false;

	if(this->version >= a3dVersion(1,2)) { 
		if(!this->worldXf.saveToMemory(file)) return false;
		if(!this->worldITXf.saveToMemory(file)) return false;
		if(!this->wvpXf.saveToMemory(file)) return false;
		if(!this->viewIXf.saveToMemory(file)) return false;
	}

	for( int pi = 0; pi < this->paramCount; pi ++ ) { 
		if( !this->paramList[pi]->saveToMemory( file ) ) return false;
	}

	return true;
}


bool a3dShaderMaterial::importBooleanParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) { 

	shString *name;
	bool boolVal;

	if( !isBooleanClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolValue( li, &boolVal ) ) return false;

	// specific param
	if( (*name) == shString( TEXT("renderEnabled") ) ) {
		this->renderEnabled = boolVal;
		return true;
	}

	pList.push_back( new a3dBoolShaderParam( *name, boolVal ) );
	return true;
}

bool a3dShaderMaterial::importFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	shString *name;
	float floatVal;

	if( !isFloatClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &floatVal ) ) return false;

	pList.push_back( new a3dFloatShaderParam( *name, floatVal ) );
	return true;
}

bool a3dShaderMaterial::importIntegerParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	shString *name;
	int intVal;

	if( !isIntegerClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntValue( lexList, li, &intVal ) ) return false;

	if( *name == TEXT( "technique" ) )
		pList.push_back( new a3dTechniqueShaderParam( *name ) );
	else
		pList.push_back( new a3dIntShaderParam( *name, intVal ) );
	return true;
}

bool a3dShaderMaterial::importStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	shString *name;
	shString stringVal;

	if( !isStringClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringValue( li, stringVal ) ) return false;

	// specific params
	if( (*name) == shString( TEXT("effectFile") ) || 
		(*name) == shString( TEXT("effectFilename") ) || 
		(*name) == shString( TEXT("effectFileName") ) ) { 
			this->fileName = stringVal;
			return true;
	} 

	return true;
}

bool a3dShaderMaterial::importColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	if( !isColorClass( li ) ) return false;
	shString *name;
	Vector4 vVal = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };

	if( !isColorClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readColorValue( lexList, li, vVal ) ) return false;

	pList.push_back( new a3dFloatArrayShaderParam( *name, vVal, vVal[3] != FLT_MAX? 4: 3 ) );

	return true;
}

bool a3dShaderMaterial::importBitMapParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	shString *name;
	shString stringVal;

	if( !isBitMapClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;

	name = &((*li)->name);

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringValue( li, stringVal ) ) return false;

	pList.push_back( new a3dTextureShaderParam( *name, stringVal ) );

	return true;
}

bool a3dShaderMaterial::importStandardMaterialParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) {

	if( !isStandardMaterialClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenClass( li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isString( li ) ) return false;

	return true;
}

HRESULT a3dShaderMaterial::applyParameters() { 

	HRESULT rv;
	
	int index;
	for( index = 0; index < this->paramCount; index++ ) {
		if( !this->paramList[index]->used ) continue;
		rv = this->paramList[index]->setParam( this->effect );
		if( FAILED( rv ) && !this->skipErrors )
			return rv;
	}

	return D3D_OK;
}

void a3dShaderMaterial::updateParameters() { 

	shString WorldXf = TEXT("World");
	shString WorldITXf = TEXT("WorldInverseTranspose");
	shString WvpXf = TEXT("WorldViewProjection");
	shString ViewIXf = TEXT("ViewInverse");
	
	int i;
	for( i = 0; i < this->paramCount; i++ ) { 
		if( this->paramList[i]->token == WorldXf ) { 
			this->paramList[i]->used = false;
			this->worldXf = this->paramList[i]->paramName;
			this->worldXf.copyToChar(this->worldXfParam);
		}
		else if( this->paramList[i]->token == WorldITXf ) { 
			this->paramList[i]->used = false;
			this->worldITXf = this->paramList[i]->paramName;
			this->worldITXf.copyToChar(this->worldITXfParam);
		}
		else if( this->paramList[i]->token == WvpXf ) { 
			this->paramList[i]->used = false;
			this->wvpXf = this->paramList[i]->paramName;
			this->wvpXf.copyToChar(this->worldXfParam);
		}
		else if( this->paramList[i]->token == ViewIXf ) { 
			this->paramList[i]->used = false;
			this->viewIXf = this->paramList[i]->paramName;
			this->viewIXf.copyToChar(this->viewIXfParam);
		}
	}

	return ;
}

bool a3dShaderMaterial::importShaderParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList ) { 

	if( isBooleanClass( li ) ) return this->importBooleanParam( lexList, li, pList );
	else if( isFloatClass( li ) ) return this->importFloatParam( lexList, li, pList );
	else if( isIntegerClass( li ) ) return this->importIntegerParam( lexList, li, pList );
	else if( isStringClass( li ) ) return this->importStringParam( lexList, li, pList );
	else if( isColorClass( li ) ) return this->importColorParam( lexList, li, pList );
	else if( isBitMapClass( li ) ) return this->importBitMapParam( lexList, li, pList);
	else if( isStandardMaterialClass( li ) ) return this->importStandardMaterialParam( lexList, li, pList );

	return false;
}

bool a3dShaderMaterial::importParams( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) {

	std::list< a3dShaderParamBase* > pList;

	while( 1 ) { 
		if( isToken( li, a3dTextFileTokens::a3dEndString ) ) break;	
		if( !this->importShaderParam( lexList, li, pList ) ) return false;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	// мы не должны импортировать параметры
	// поскольку сериализация параметров в файл у 3dmax реализована из рук вон плохо
	// она float3 сериализит как Integer
	// поэтому мы благополучно все пропустим и загрузим сразу из файла
	// конечно мы потеряем настройки макса да и хер с ней.
	// настроим все в sceneEditor
	if( FAILED(this->retreiveEffectParams())) return false;
	/*
	this->paramCount = (int)pList.size();
	this->paramList = new a3dShaderParamBase*[ this->paramCount ];

	int i;
	std::list< a3dShaderParamBase* >::iterator pi;

	for( pi = pList.begin(), i = 0; i < this->paramCount; i++, pi++ ) {
		this->paramList[ i ] = (*pi);
	}
	*/
	// update parameters usability
	this->updateParameters();

	return true;
}
a3dMaterialBase* a3dShaderMaterial::clone() { 

	a3dShaderMaterial *mat = new a3dShaderMaterial();
	mat->assign(this);

	return mat;
}

void a3dShaderMaterial::updatePredefinedMatrixParams() { 

	this->worldXf.copyToChar(this->worldXfParam);
	this->worldITXf.copyToChar(this->worldITXfParam);
	this->wvpXf.copyToChar(this->wvpXfParam);
	this->viewIXf.copyToChar(this->viewIXfParam);
	
	return ;
}

void a3dShaderMaterial::assign(a3dMaterialBase *mat) {
	
	a3dShaderMaterial *smat = dynamic_cast<a3dShaderMaterial*>(mat);
	if(smat == null) return;

	this->name = smat->name;
	this->paramCount = smat->paramCount;	
	this->paramList = new a3dShaderParamBase*[ this->paramCount ];
	this->renderEnabled = smat->renderEnabled;
	this->fileName = smat->fileName;
	this->skipErrors = smat->skipErrors;
	this->effect = smat->effect;
	this->setScene( smat->scene ); 

	this->worldXf = smat->worldXf;
	this->worldITXf = smat->worldITXf;
	this->wvpXf = smat->wvpXf;
	this->viewIXf = smat->viewIXf;
	this->updatePredefinedMatrixParams();

	int pi;
	for( pi = 0; pi < this->paramCount; pi ++ ) { 
		this->paramList[ pi ] = smat->paramList[ pi ]->clone();	
		this->paramList[ pi ]->material = this;
	}

	this->techniqueCount = smat->techniqueCount;
	this->techniqueList = new shString[this->techniqueCount];
	for(int i = 0; i < this->techniqueCount; i++) { 
		this->techniqueList[i] = smat->techniqueList[i];
	}
	this->updateParametersDependency();
}

void a3dShaderMaterial::clear() { 

	this->clearParams();
	this->effect = null;
	
	if( this->techniqueList != null )
		delete[] this->techniqueList;
	this->techniqueList = null;
	this->techniqueCount = 0;

	return;
}

void a3dShaderMaterial::clearParams() { 
	
	if( this->paramList != null ) { 
		for( int pi = 0; pi < this->paramCount; pi ++ ) { 
			if( this->paramList[ pi ] != null ) delete this->paramList[ pi ];
		}
		delete[] this->paramList;
	}
	this->paramList = null;
	this->paramCount = 0;
}

bool a3dShaderMaterial::isPredefinedParam(shString &paramName) { 
	
	return false;
	// skip all predefined params;
	static shString WorldITXf(TEXT("WorldITXf")), 
					WvpXf(TEXT("WvpXf")), 
					WorldXf(TEXT("WorldXf")), 
					ViewIXf(TEXT("ViewIXf")), 
					Script(TEXT("Script"));
	
	return paramName == WorldITXf || paramName == WvpXf || paramName == WorldXf || paramName == ViewIXf || paramName == Script;
}

a3dShaderParamBase* a3dShaderMaterial::createParam(D3DXPARAMETER_DESC *paramDesc ) { 
	
	shString pName;

	pName.copyFromChar( (char*)paramDesc->Name );
	return new a3dShaderParamBase( pName );
}

int a3dShaderMaterial::calcEffectParamCount(fxFileParser *fxFile) {

	int count = 0;
	std::list<fxParamInfo*>::iterator fli;
	
	for(fli = fxFile->paramList.begin(); fli != fxFile->paramList.end(); fli++) { 
		if(this->isPredefinedParam((*fli)->paramName)) continue;
		count++;
	}

	return count;
}

HRESULT a3dShaderMaterial::retreiveEffectParams() {

	fxFileParser fxFile;
	std::list<fxParamInfo*>::iterator fli;
	int paramIndex = 0;

	if(this->effect == null)
		fxFile.parseFile( this->fileName );
	else 
		fxFile.parseFile( this->effect->fullPathName );
	this->paramCount = this->calcEffectParamCount(&fxFile) + 1; // and one for TechniqueParam
	if( this->paramCount == 0 )
		return D3D_OK;
	this->paramList = new a3dShaderParamBase*[ this->paramCount ];
	
	for(fli = fxFile.paramList.begin(); fli != fxFile.paramList.end(); fli++) { 
		if(this->isPredefinedParam((*fli)->paramName)) continue;
		this->paramList[paramIndex] = this->createParam( *fli );
		this->paramList[paramIndex]->registerDependencyProperties();
		this->paramList[paramIndex]->material = this;
		paramIndex++;
	}
	this->paramList[paramIndex] = new a3dTechniqueShaderParam( fxFile.selectedTechnique );

	this->techniqueCount = fxFile.techniquesList.size();
	this->techniqueList = new shString[this->techniqueCount];
	int techIndex = 0;
	for(fli = fxFile.techniquesList.begin(); fli != fxFile.techniquesList.end(); fli++, techIndex++) { 
		this->techniqueList[techIndex] = (*fli)->paramName;
	}
	
	return D3D_OK;
}

a3dShaderParamBase* a3dShaderMaterial::createParam( fxParamInfo *info ) { 
	
	if(info->type == FXPT_TEXTURE)
		return new a3dTextureShaderParam( info );
	else if(info->type == FXPT_TECHNIQUE)
		return new a3dTechniqueShaderParam( info );
	else if(info->isBoolValue())
		return new a3dBoolShaderParam(info);
	else if(info->isIntValue())
		return new a3dIntShaderParam(info);
	else if(info->isFloatValue())
		return new a3dFloatShaderParam(info);
	else if(info->isVectorValue())
		return new a3dMatrixShaderParam(info);
	else if(info->isMatrixValue())
		return new a3dMatrixShaderParam(info);
	else if(info->isBoolArrayValue())
		return new a3dBoolArrayShaderParam(info);
	else if(info->isIntArrayValue())
		return new a3dIntArrayShaderParam(info);
	else if(info->isFloatArrayValue())
			return new a3dFloatArrayShaderParam(info);
	else if(info->isVectorArrayValue())
		return new a3dFloatArrayShaderParam(info);
	else if(info->isMatrixArrayValue())
		return new a3dMatrixArrayShaderParam(info);
	return null;
}

HRESULT a3dShaderMaterial::setNewEffect(a3dScene *scene, a3dEffectDesc *effect) { 

	HRESULT rv;
	
	this->clearParams();
	this->effect = effect;

	rv = this->retreiveEffectParams();
	if( FAILED( rv ) )
		return rv;

	rv = this->initializeData( scene );
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}