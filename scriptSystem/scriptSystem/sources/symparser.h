#ifndef A3D_SYMANTEC_PARSER
#define A3D_SYMANTEC_PARSER

#include "scrparser.h"
#include "assembly\\assembly.h"
#include "assembly\\scopeInfo.h"
#include "arrayList.h"

class scriptSymantecParser { 
	scriptAssembly	*assembly;
	scriptParser	*parser;

	arrayList		usedScopes;
	arrayList		currentScopes;

	bool searchLexemeBackward( lexRWordType rword, std::list<scriptLexeme*>::iterator &sli, std::list<scriptLexeme*>::iterator &out);
	bool searchLexemeBackward( lexType type, std::list<scriptLexeme*>::iterator &sli, std::list<scriptLexeme*>::iterator &out);
	bool hasScopeInList( arrayList &arr, shString &scopeFullName );
	void buildQualifiedName( std::list<scriptLexeme*>::iterator &start, std::list<scriptLexeme*>::iterator &end, shString &out );
public:
	scriptSymantecParser( scriptParser *parser );
	~scriptSymantecParser() { 
		this->clear();
	}

	void clear();

	scriptRetCode			OnNamespaceDefinitionBegin();
	scriptRetCode			OnUsingDefinition();
};

#endif