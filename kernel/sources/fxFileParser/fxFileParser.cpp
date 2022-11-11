#include "fxFileParser.h"
#include "3dtypes.h"
#include "..\\a3dTextFileTokens.h"

fxParamInfo::fxParamInfo(fxParamType type, shString &paramName) { 
	this->initializeDefault();
	this->type = type;
	this->paramName = paramName;
}

void fxParamInfo::initializeDefault() { 
	
	this->type = FXPT_NONE;
	this->uiWidget = SPWT_NONE;
	this->bufferSize = 0;
	this->dataBuffer = null;
	this->uiMin = -100000.0f;
	this->uiMax = 100000.0f;
	this->uiStep = 1.0f;
	this->row = 0;
	this->col = 0;
	this->arrayDimension = 0;
	this->itemCount = 0;
	this->itemCount2 = 0;
	this->itemCount3 = 0;
}

void fxParamInfo::clear() { 
	
	if( this->dataBuffer != null )
		delete[] this->dataBuffer;
	this->dataBuffer = null;
	this->bufferSize = 0;
	this->paramName.clear();
	this->uiName.clear();
	this->resourceName.clear();
	this->resourceType.clear();
	this->object.clear();
	this->space.clear();
	this->row = 0;
	this->col = 0;
	this->arrayDimension = 0;
	this->itemCount = 0;
	this->itemCount2 = 0;
	this->itemCount3 = 0;
	this->type = FXPT_NONE;
	this->uiWidget = SPWT_NONE;
}

void fxFileParser::clear() { 
	
	std::list<fxParamInfo*>::iterator li;
	for(li = this->paramList.begin(); li!= this->paramList.end(); li++) { 
		delete (*li);
	}
	for(li = this->techniquesList.begin(); li != this->techniquesList.end(); li++) {
		delete (*li);
	}
	this->paramList.clear();
	this->techniquesList.clear();
}

bool fxFileParser::parseFile( shString &file ) { 
	
	scriptScaner scaner;

	scaner.skipPreprocDirectives = true;
	if(scaner.openFile(file.getCString()) != SRV_SUCCESS ) 
		return false;
	
	if( scaner.scan() != SRV_SUCCESS ) 
		return false;

	return parseLexemes( scaner.getLexemesList() );
}

bool fxFileParser::parseLexemes(std::list<scriptLexeme*> &lexList) { 

	std::list<scriptLexeme*>::iterator li;
	fxParamInfo *info;

	li = lexList.begin();
	while(li != lexList.end()) { 
		if(findNextParam(lexList, li)) { 
			info = this->parseParam(lexList, li);
			if(info == null) 
				return false;
			if(info->type == FXPT_TECHNIQUE) 
				this->techniquesList.push_back(info);
			else
				this->paramList.push_back(info);
		}
	}
	return true;
}

void fxFileParser::initializeDefault() { 
	
}

shString fxFileParser::floatString = TEXT( "float" );
shString fxFileParser::boolString = TEXT( "bool" );
shString fxFileParser::intString = TEXT( "int" );
shString fxFileParser::techniqueString = TEXT( "technique" );
shString fxFileParser::textureString = TEXT( "texture" );
shString fxFileParser::stringString = TEXT( "string" );
shString fxFileParser::vectorString = TEXT( "vector" );
shString fxFileParser::matrixString = TEXT( "matrix" );
shString fxFileParser::objectString = TEXT( "Object" );
shString fxFileParser::uiNameString = TEXT( "UIName" );
shString fxFileParser::spaceString = TEXT( "Space" );
shString fxFileParser::uiWidgetString = TEXT( "UIWidget" );
shString fxFileParser::uiMinString = TEXT( "UIMin" );
shString fxFileParser::uiMaxString = TEXT( "UIMax" );
shString fxFileParser::uiStepString = TEXT( "UIStep" );
shString fxFileParser::resourceNameString = TEXT( "ResourceName" );
shString fxFileParser::resourceTypeString = TEXT( "ResourceType" );
shString fxFileParser::scriptClassString = TEXT( "ScriptClass" );
shString fxFileParser::scriptOrderString = TEXT( "ScriptOrder" );
shString fxFileParser::scriptOutputString = TEXT( "ScriptOutput" );
shString fxFileParser::scriptString = TEXT( "Script" );

shString fxParamInfo::sliderString = TEXT( "slider" );
shString fxParamInfo::colorString = TEXT( "color" );
shString fxParamInfo::ColorString = TEXT( "Color" );

bool fxFileParser::isFunction(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	// param type
	li++;
	// skip param name
	li++;
	// check if left bracket
	bool res = (*li)->type == LT_PARSER && (*li)->parser == LPT_LEFT_BRACKET;
	// return name
	li--;
	// return type
	li--;

	return res;
}

bool fxFileParser::findNextParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	int openBracketCount = 0;
	
	while( li != lexList.end() ) { 
		if( (*li)->type == LT_PARSER ) {
			if( (*li)->parser == LPT_LEFT_SQUARE_BRACKET || (*li)->parser == LPT_LEFT_BRACKET || (*li)->parser == LPT_LEFT_FIGURE_BRACKET )
				openBracketCount++;	
			else if( (*li)->parser == LPT_RIGHT_SQUARE_BRACKET || (*li)->parser == LPT_RIGHT_BRACKET || (*li)->parser == LPT_RIGHT_FIGURE_BRACKET )
				openBracketCount--;
		}
		
		// 
		else if( openBracketCount == 0 && (*li)->type == LT_ID ) { 
			if( (*li)->name.hasSubstr( this->floatString, 0 ) && !this->isFunction(lexList, li)) 
				return true;
			if( (*li)->name == this->techniqueString )
				return true;
			if( (*li)->name == this->textureString )
				return true;
			if( (*li)->name == this->vectorString && !this->isFunction(lexList, li) )
				return true;
			if( (*li)->name == this->matrixString && !this->isFunction(lexList, li) )
				return true;
			if( (*li)->name.hasSubstr( this->boolString,0 ) && !this->isFunction(lexList, li) )
				return true;
			if( (*li)->name.hasSubstr( this->intString, 0 ) && !this->isFunction(lexList, li) )
				return true;
		}
		
		li++;
	}
	
	return false;
}

bool fxFileParser::readArray(fxParamInfo *info,  std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	// array dimension
	for( info->arrayDimension = 0; info->arrayDimension < 3; info->arrayDimension++ ) {
		if( (*li)->isParser( LPT_LEFT_SQUARE_BRACKET ) ) { 
			if(!safeNextLex(lexList, li) || !(*li)->isNumericConst()) { 
				return false;
			}
			if(info->arrayDimension == 0)
				info->itemCount = (*li)->longValue;
			else if(info->arrayDimension == 1) {
				info->itemCount2 = info->itemCount;
				info->itemCount = (*li)->longValue;
			}
			else if(info->arrayDimension == 2) {
				info->itemCount3 = info->itemCount2;
				info->itemCount2 = info->itemCount;
				info->itemCount = (*li)->longValue;
			}
			safeNextLex(lexList, li); // skip constant
			safeNextLex(lexList, li); // skip ]
		}
		else 
			break;
	}

	return true;
}

fxParamInfo* fxFileParser::parseTechnique(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	if(!safeNextLex( lexList, li )) { 
		delete info;
		return false;
	}
	if((*li)->type != LT_ID ) { 
		delete info;
		return false;
	}

	info->type = FXPT_TECHNIQUE;
	info->paramName = (*li)->name;
	return info;

}

fxParamInfo* fxFileParser::parseTexture(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	if(!safeNextLex( lexList, li )) { 
		delete info;
		return null;
	}
	if((*li)->type != LT_ID) { 
		delete info;
		return null;
	}

	info->type = FXPT_TEXTURE;
	info->paramName = (*li)->name;
	if(!findNextParserUntilParser(lexList, li, LPT_LESS_THAN, LPT_DOTCOMMA)) return info;
	this->readInfo(info, lexList, li);
	return info;
}

bool fxFileParser::parseParamAttributes(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	if(!safeNextLex(lexList, li))
		return false;

	if( (*li)->type == LT_ID )
		info->paramName = (*li)->name;

	if(!safeNextLex(lexList, li))
		return false;
	
	if(!this->readArray(info, lexList, li) ) 
		return false;

	if( (*li)->isParser( LPT_DOT2 ) ) { 
		if(!safeNextLex(lexList, li)) return false;
		if((*li)->type == LT_ID) { 
			info->token = (*li)->name;
		}
	}

	if(findNextParserUntilParser(lexList, li, LPT_LESS_THAN, LPT_DOTCOMMA, LPT_ASSIGNMENT)) {
		if(!this->readInfo(info, lexList, li))
			return false;
		if( !safeNextLex(lexList, li) ) 
			return false;
	}

	if( findNextParserUntilParser(lexList, li, LPT_ASSIGNMENT, LPT_DOTCOMMA, LPT_DOTCOMMA)) {
		this->readData(info, lexList, li);
	}

	if( findNextParserUntilParser(lexList, li, LPT_DOTCOMMA, LPT_DOTCOMMA, LPT_DOTCOMMA) ) { 
		return true;
	}

	return false;
}

fxParamInfo* fxFileParser::parseFloatCore(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	if(!this->parseParamAttributes(info, lexList, li)) { 
		delete info;
		return null;
	}

	return info;
}

void fxFileParser::getRowColInfo(fxParamInfo *info, shString &str, int beginIndex) { 
	
	info->row = 1; info->col = 1;
	if( beginIndex >= str.getLength() ) return;
	
	info->col = str.getCString()[beginIndex] - (int)'0';
	if( beginIndex + 2 >= str.getLength() ) return;
	info->row = info->col;
	info->col = str.getCString()[beginIndex+2] - (int)'0';
}

fxParamInfo* fxFileParser::parseFloat(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	info->type = FXPT_FLOAT;
	
	this->getRowColInfo(info, (*li)->name, 5);
	if( info->row > 1 || info->col > 1) info->type = FXPT_MATRIX;

	return this->parseFloatCore(info, lexList, li);	
}

fxParamInfo* fxFileParser::parseBool(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	info->type = FXPT_BOOL;
	
	this->getRowColInfo(info, (*li)->name, 4);
	if(!this->parseParamAttributes(info, lexList, li)) {
		delete info;
		return null;
	}
	return info;
}

fxParamInfo* fxFileParser::parseInt(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	info->type = FXPT_INT;

	this->getRowColInfo(info, (*li)->name, 3);
	if(!this->parseParamAttributes(info, lexList, li)) {
		delete info;
		return null;
	}
	return info;
}

fxParamInfo* fxFileParser::parseVector(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	info->row = 1;
	info->col = 4;

	return this->parseFloatCore(info, lexList, li);
}

fxParamInfo* fxFileParser::parseMatrix(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	info->row = 4;
	info->col = 4;

	return this->parseFloatCore(info, lexList, li);
}

fxParamInfo* fxFileParser::parseParam(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	fxParamInfo *info = new fxParamInfo();
	
	if( (*li)->name == this->textureString ) { 
		return this->parseTexture(info, lexList, li);
	}
	else if( (*li)->name == this->techniqueString ) { 
		return this->parseTechnique(info, lexList, li);		
	}
	else if( (*li)->name.hasSubstr( this->floatString, 0 ) ) { 
		return this->parseFloat(info, lexList, li);
	}
	else if( (*li)->name == this->vectorString ) { 
		return this->parseVector( info, lexList, li );
	}
	else if( (*li)->name == this->matrixString ) {
		return this->parseMatrix( info, lexList, li );  
	}
	else if( (*li)->name.hasSubstr( this->boolString, 0 ) ) { 
		return this->parseBool( info, lexList, li);
	}
	else if( (*li)->name.hasSubstr( this->intString, 0 ) ) { 
		return this->parseInt( info, lexList, li );
	}

	return info;
}

bool fxParamInfo::isSingleValue() { return this->row == 1 && this->col == 1 && this->arrayDimension == 0; }

bool fxParamInfo::isBoolValue() { 
	return this->type == FXPT_BOOL && this->isSingleValue();
}

bool fxParamInfo::isIntValue() { 
	return this->type == FXPT_INT && this->isSingleValue();
}

bool fxParamInfo::isFloatValue() { 
	return this->type == FXPT_FLOAT && this->isSingleValue();
}

bool fxParamInfo::isMatrixValue() { 
	return this->type == FXPT_MATRIX && this->arrayDimension == 0;
}

bool fxParamInfo::isVectorValue() { 
	return this->type == FXPT_VECTOR && this->arrayDimension == 0;
}

bool fxParamInfo::isBoolArrayValue() { 
	return this->type == FXPT_BOOL && !this->isSingleValue();
}

bool fxParamInfo::isIntArrayValue() { 
	return this->type == FXPT_INT && !this->isSingleValue();
}

bool fxParamInfo::isFloatArrayValue() { 
	return this->type == FXPT_FLOAT && !this->isSingleValue();
}

bool fxParamInfo::isVectorArrayValue() { 
	return this->type == FXPT_VECTOR && this->arrayDimension > 0;
}

bool fxParamInfo::isMatrixArrayValue() { 
	return this->type == FXPT_MATRIX && this->arrayDimension > 0;
}

bool fxFileParser::parseStringParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li, shString &infoName, shString &stringValue ) { 
	
	if((*li)->type != LT_ID) return false;
	infoName = (*li)->name;
	if(!safeNextLex(lexList, li)) return false;
	if(!(*li)->isParser(LPT_ASSIGNMENT)) return false;
	if(!safeNextLex(lexList, li)) return false;
	if((*li)->type != LT_STRING) return false;
	stringValue = (*li)->name;

	return true;
}

bool fxFileParser::parseFloatParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li, shString &infoName, float *floatValue ) { 
	
	float sign = 1.0f;
	
	if((*li)->type != LT_ID) return false;
	infoName = (*li)->name;
	if(!safeNextLex(lexList, li)) return false;
	if(!(*li)->isParser(LPT_ASSIGNMENT)) return false;
	if(!safeNextLex(lexList, li)) return false;
	if((*li)->isParser(LPT_SUBTRACTION)) { 
		sign = -1.0f;
		if(!safeNextLex(lexList, li)) return false;
	}
	if((*li)->type != LT_CONST) return false;
	*floatValue = sign * (*li)->const2Float();

	return true;
}

void fxFileParser::extractSelectedTechniqueName() { 
	
	int techBeginIndex = this->script.findFromRight( '=' ) + 1;
	int dotCommaIndex = this->script.findFromRight( ';' );
	if(dotCommaIndex - techBeginIndex > 0)
		this->selectedTechnique.initialize( &this->script.getCString()[techBeginIndex], dotCommaIndex - techBeginIndex );
}

bool fxFileParser::readInfo(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	shString infoName, stringValue;
	float floatValue;
	
	while( li != lexList.end() ) { 
		
		if( (*li)->isParser(LPT_GREATER_THAN) ) break;
		else if( (*li)->isId( this->stringString ) ) {
			
			if(!safeNextLex(lexList, li)) return false;
			if(!this->parseStringParam(lexList, li, infoName, stringValue)) return false;
			
			if( infoName == this->uiNameString ) info->uiName = stringValue;
			else if( infoName == this->objectString ) info->object = stringValue;
			else if( infoName == this->uiWidgetString ) { 
				if(stringValue == fxParamInfo::sliderString)
					info->uiWidget = SPWT_SLIDER;
				if(stringValue == fxParamInfo::colorString || stringValue == fxParamInfo::ColorString)
					info->uiWidget = SPWT_COLOR;
			}
			else if( infoName == this->spaceString ) info->space = stringValue;
			else if( infoName == this->resourceNameString ) info->resourceName = stringValue;
			else if( infoName == this->resourceTypeString ) info->resourceType = stringValue;
			else if( infoName == this->scriptClassString ) this->scriptClass = stringValue;
			else if( infoName == this->scriptOrderString ) this->scriptOrder = stringValue;
			else if( infoName == this->scriptOutputString ) this->scriptOutput = stringValue;
			else if( infoName == this->scriptString ) { 
				this->script = stringValue;
				this->extractSelectedTechniqueName();
			}
		
			// ;
			safeNextLex( lexList, li );
		}
		else if( (*li)->isId( this->floatString ) ) { 
			if(!safeNextLex(lexList, li)) return false;
			if(!this->parseFloatParam( lexList, li, infoName, &floatValue )) return false;
		
			if( infoName == this->uiMinString ) info->uiMin = floatValue;
			else if( infoName == this->uiMaxString ) info->uiMax = floatValue;
			else if( infoName == this->uiStepString ) info->uiStep = floatValue;
		
			// ;
			safeNextLex( lexList, li );
		}
		if(!safeNextLex(lexList, li))
			break;
	}

	return true;
}

int fxParamInfo::calcItemCount() { 
	
	int itemCount = 1;
	
	if(this->arrayDimension > 0) itemCount *= this->itemCount;
	if(this->arrayDimension > 1) itemCount *= this->itemCount2;
	if(this->arrayDimension > 2) itemCount *= this->itemCount3;

	return itemCount;
}

int fxParamInfo::calcItemSize() { 
	
	if( this->type == FXPT_FLOAT ) return this->col * this->row * sizeof( float );
	if( this->type == FXPT_INT ) return this->col * this->row * sizeof( int );
	if( this->type == FXPT_BOOL ) return this->col * this->row * sizeof( bool ); 
	if( this->type == FXPT_VECTOR ) return sizeof(float) * 4;
	if( this->type == FXPT_MATRIX ) return sizeof(float) * 16;

	return 0;
}

void fxParamInfo::allocateBuffer() { 

	int itemCount = this->calcItemCount();
	int itemSize = this->calcItemSize();

	if( itemCount * itemSize == 0 ) return;

	this->bufferSize = itemCount * itemSize;
	this->dataBuffer = new BYTE[ this->bufferSize ];
}

bool fxFileParser::readData(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	// skip assignment
	if( !safeNextLex(lexList, li) ) return false;
	
	int itemCount = info->calcItemCount();
	int itemSize = info->calcItemSize();

	info->allocateBuffer();

	int constCount = itemCount * info->row * info->col;
	int readConstCount = 0;
	
	while( readConstCount < constCount && !(*li)->isParser( LPT_DOTCOMMA ) ) { 
		
		if(!findNextConstantUntilParser(lexList, li, LPT_DOTCOMMA))
			break;

		if(info->type == FXPT_FLOAT || info->type == FXPT_MATRIX || info->type == FXPT_VECTOR)
			info->floatBuffer[readConstCount] = (*li)->const2Float();
		else if(info->type == FXPT_BOOL)
			info->boolBuffer[readConstCount] = (*li)->rwordType == LRW_TRUE? true: false;
		else if(info->type == FXPT_INT)
			info->intBuffer[readConstCount] = (*li)->const2Int();
		readConstCount++;
		if(!safeNextLex(lexList, li))
			return false;
	}

	return true;
}