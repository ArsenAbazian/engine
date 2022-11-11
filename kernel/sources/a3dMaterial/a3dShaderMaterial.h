#ifndef A3D_SHADER_MATERIAL_INC
#define A3D_SHADER_MATERIAL_INC

#include "a3dMaterialBase.h"
#include "..\\a3dShaderParam\\a3dShaderParamType.h"
#include <d3dx9effect.h>

class a3dShaderParamBase;
class a3dFloatShaderParam;
class a3dTechniqueShaderParam;
class a3dTextureShaderParam;
class fxFileParser;
class fxParamInfo;

class a3dShaderMaterial : public a3dMaterialBase { 

	bool importParams( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importShaderParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importBooleanParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importIntegerParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importBitMapParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	bool importStandardMaterialParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, std::list< a3dShaderParamBase* > &pList );
	int	 calcEffectParamCount( fxFileParser *fxFile );
	a3dShaderParamBase* createParam(fxParamInfo *paramInfo);
	void updatePredefinedMatrixParams();
	void updateParametersDependency();
public:
	shString fileName;
	a3dShaderParamBase** paramList;
	int	paramCount;
	bool  renderEnabled;
	a3dEffectDesc	*effect;	
	shString *techniqueList;	// since 1.1
	int techniqueCount;			// since 1.1

	shString worldXf;			// since 1.2
	shString worldITXf;			// since 1.2
	shString wvpXf;				// since 1.2
	shString viewIXf;			// since 1.2
	
	char	 worldXfParam[64];
	char     worldITXfParam[64];
	char	 wvpXfParam[64];
	char	 viewIXfParam[64];

	a3dScene *scene;

	bool skipErrors;

	a3dShaderMaterial() { this->initializeDefault(); }

	bool supportSave() { return true; }
	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void initializeDefault();
	void clear();
	a3dMaterialBase* clone();
	void			assign(a3dMaterialBase *mat);
	void		setScene( a3dScene *scene ) { this->scene = scene; }
	
	int			getHeaderCode() { return a3dHeaders::a3dShaderMaterialHeader; }
	int			calcSizeCore();
	bool		saveToMemoryCore( a3dMemoryFile &file );
	bool		loadFromMemoryCore( a3dMemoryFile &file );

	a3dShaderParamBase* createParam( a3dShaderParamType type );
	a3dShaderParamBase* createParam( D3DXPARAMETER_DESC *paramDesc );

	HRESULT		initializeData( a3dScene *scene );

	void		updateParameters();
	HRESULT		applyParameters();

	HRESULT		setNewEffect( a3dScene *scene, a3dEffectDesc *effect );
	void		clearParams();
	HRESULT		retreiveEffectParams();
	bool		isPredefinedParam(shString &paramName);
};


#endif