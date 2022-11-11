#ifndef A3D_TEXT_SCENEFILE_INC
#define A3D_TEXT_SCENEFILE_INC

#include "a3dSceneDefinitions.h"
#include <list>
#include "lexeme.h"

class a3dLightBase;
class a3dModelInfo;
class a3dLandscapeInfo;
class a3dParticleSystemInfo;
class a3dRailSystemInfo;
class a3dTextSceneFile { 

	bool	initializeModel( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	bool	initializeLand( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	bool	initializeParticleSystem( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	bool	initializeRailSystem( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	bool	initializeLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli, a3dLightBase *light );
	bool	initializeModels( std::list< scriptLexeme* > &lexList );
	bool	initializeLands( std::list< scriptLexeme* > &lexList );
	bool	initializeParticleSystems( std::list< scriptLexeme* > &lexList );
	bool	initializeRailSystems( std::list< scriptLexeme* > &lexList );
	bool	initializeLights( std::list< scriptLexeme* > &lexList );
	bool	initializeDirectories( std::list<scriptLexeme*> &lexList );

public:

	shString				textureDirectory;
	shString				modelDirectory;
	shString				effectDirectory;
	shString				landDirectory;
	shString				particleDirectory;
	shString				railSystemDirectory;
	shString				fontDirectory;
	
	a3dModelInfo			*modelList[SCENE_MODEL_MAX_COUNT];
	a3dLandscapeInfo		*landList[SCENE_LAND_MAX_COUNT];
	a3dParticleSystemInfo	*particleSystemList[SCENE_PS_MAX_COUNT];
	a3dRailSystemInfo		*railSystemList[SCENE_RS_MAX_COUNT];
	a3dLightBase			*lightList[SCENE_LIGHT_MAX_COUNT];

	int						modelCount;
	int						landCount;
	int						particleSystemCount;
	int						railSystemCount;
	int						lightCount;

	a3dTextSceneFile() { this->initializeDefault(); }
	~a3dTextSceneFile() { this->clear(); }

	void	initializeDefault();
	void	clear();

	bool	loadFromFile( shString &fileName );
	a3dParticleSystemInfo *createParticleInfo( shString &type );
};

#endif