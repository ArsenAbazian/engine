#ifndef A3D_FX_FILE_PARSER
#define A3D_FX_FILE_PARSER

#include "hstring.h"
#include "scaner.h"
#include <list>
#include "vecmat.h"

typedef enum { 
	FXPT_NONE,
	FXPT_BOOL,
	FXPT_INT,
	FXPT_FLOAT,
	FXPT_VECTOR,
	FXPT_MATRIX,
	FXPT_MATRIX_ARRAY,
	FXPT_TEXTURE,
	FXPT_TECHNIQUE
} fxParamType;

typedef enum { 
	SPWT_NONE, 
	SPWT_SLIDER, 
	SPWT_COLOR 
} a3dShaderParamWidgetType;

class fxParamInfo {

public:
	static shString sliderString;
	static shString colorString;
	static shString ColorString;
	
	fxParamType type;
	shString paramName;
	shString resourceName;
	shString uiName;
	shString resourceType;
	a3dShaderParamWidgetType uiWidget;
	shString object;
	shString space;
	shString token;
	float uiMin;
	float uiMax;
	float uiStep;
	int row, col;
	int arrayDimension; // max = 3
	int itemCount;
	int itemCount2;
	int itemCount3;
	int bufferSize;

	union {
		BYTE *dataBuffer;
		BOOL *boolBuffer;
		int  *intBuffer;
		float *floatBuffer;
		Matrix4 *matrixBuffer;
	};

	fxParamInfo() { this->initializeDefault(); }
	fxParamInfo(fxParamType type, shString &paramName);
	~fxParamInfo() { this->clear(); }

	void initializeDefault();
	void clear();

	int calcItemCount();
	int calcItemSize();
	void allocateBuffer();
	bool isSingleValue();
	bool isBoolValue();
	bool isIntValue();
	bool isFloatValue();
	bool isVectorValue();
	bool isMatrixValue();
	bool isBoolArrayValue();
	bool isIntArrayValue();
	bool isFloatArrayValue();
	bool isVectorArrayValue();
	bool isMatrixArrayValue();
	bool isSliderWidget() { return this->uiWidget == SPWT_SLIDER; }
	bool isColorWidget() { return this->uiWidget == SPWT_COLOR; }
};

class fxFileParser { 
public:
	static shString floatString;
	static shString techniqueString;
	static shString textureString;
	static shString stringString;
	static shString objectString;
	static shString uiNameString;
	static shString spaceString;
	static shString uiWidgetString;
	static shString uiMinString;
	static shString uiMaxString;
	static shString uiStepString;
	static shString resourceNameString;
	static shString resourceTypeString;
	static shString vectorString;
	static shString matrixString;
	static shString boolString;
	static shString intString;
	
	static shString scriptClassString;
	static shString scriptOrderString;
	static shString scriptOutputString;
	static shString scriptString;

private:
	bool parseLexemes( std::list<scriptLexeme*> &lexList );
	bool findNextParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseParam(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseBool(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseInt(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseTechnique(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseTexture(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseFloatCore(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseFloat(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseVector(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	fxParamInfo* parseMatrix(fxParamInfo *info, std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	bool readInfo(fxParamInfo *info,  std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	bool readData(fxParamInfo *info,  std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	bool readArray(fxParamInfo *info,  std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	void extractSelectedTechniqueName();
	bool parseStringParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li, shString &infoName, shString &stringValue );
	bool parseFloatParam( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li, shString &infoName, float *floatValue );
	void getRowColInfo(fxParamInfo *info, shString &str, int beginIndex);
	bool parseParamAttributes(fxParamInfo *info,  std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
	bool isFunction(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li);
public:
	shString scriptClass;
	shString scriptOrder;
	shString scriptOutput;
	shString script;
	shString selectedTechnique;

	std::list<fxParamInfo*> paramList;
	std::list<fxParamInfo*> techniquesList;

	shString* getSelectedTechnique();

	fxFileParser() { this->initializeDefault(); }
	~fxFileParser() { this->clear(); }

	void initializeDefault();
	void clear();

	bool parseFile( shString &file );
};

#endif