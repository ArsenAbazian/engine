#include "symparser.h"

#define null NULL

scriptSymantecParser::scriptSymantecParser(scriptParser *parser) { 
	this->parser = parser;
	this->assembly = new scriptAssembly();
	this->usedScopes.initialize(1024);
	this->currentScopes.initialize(128);
}

void scriptSymantecParser::clear() { 

	this->parser = null;
	if( this->assembly != null )
		delete this->assembly;
	this->assembly = null;

	this->usedScopes.clear();
	this->currentScopes.clear();
}

bool scriptSymantecParser::hasScopeInList( arrayList &arr, shString &scopeFullName ) { 
	
	for(int i = 0; i < arr.getCount(); i++) { 
		scriptScopeInfo *info = (scriptScopeInfo*)arr.getItem(i);
		if(info->fullName == scopeFullName) 
			return true;
	}
	return false;
}

void scriptSymantecParser::buildQualifiedName( std::list<scriptLexeme*>::iterator &start, std::list<scriptLexeme*>::iterator &end, shString &out ) { 
	
	std::list<scriptLexeme*>::iterator sli;
	out = TEXT("");
	for(sli = start; sli != end; sli++) { 
		if( (*sli)->type == LT_ID ) { 
			if(out.getLength() > 0) out += TEXT(".");
			out += (*sli)->getLexemeName();
		}
	}
}

scriptRetCode scriptSymantecParser::OnNamespaceDefinitionBegin() { 
	
	std::list<scriptLexeme*>::iterator sli;
	std::list<scriptLexeme*>::iterator sliStart = this->parser->sli;
	sliStart--;
	if(!this->searchLexemeBackward(LRW_NAMESPACE, sliStart, sli))
		return SRV_FAIL;

	sli++; // skip namespace;
	
	shString scopeName = TEXT("");
	shString scopeFullName = TEXT("");
	
	// get full namespace name;
	this->buildQualifiedName(sli, this->parser->sli, scopeFullName);
	
	scriptScopeInfo *info = new scriptScopeInfo();
	info->name = scopeFullName;
	info->fullName = scopeFullName;
	
	this->currentScopes.addItem(info);

	return SRV_SUCCESS;
}

scriptRetCode scriptSymantecParser::OnUsingDefinition() { 
	
	std::list<scriptLexeme*>::iterator sli;
	std::list<scriptLexeme*>::iterator sliStart = this->parser->sli;
	sliStart--;
	if(!this->searchLexemeBackward(LRW_USING, sliStart, sli))
		return SRV_FAIL;

	sli++; // skip namespace;

	shString scopeName = TEXT("");
	shString scopeFullName = TEXT("");
	
	// get full namespace name;
	this->buildQualifiedName(sli, this->parser->sli, scopeFullName);

	scriptScopeInfo *info = new scriptScopeInfo();
	info->name = scopeFullName;
	info->fullName = scopeFullName;
	
	this->usedScopes.addItem(info);

	return SRV_SUCCESS;
}

bool scriptSymantecParser::searchLexemeBackward( lexType type, std::list<scriptLexeme*>::iterator &sli, std::list<scriptLexeme*>::iterator &out) { 
	
	out = sli;
	if(out == this->parser->scaner->getLexemesList().end())
		out--;
	while( true ) { 
		if((*out)->type == type) break;
		if(out == this->parser->scaner->getLexemesList().begin())
			return false;
		out--;
	}
	return true;
}

bool scriptSymantecParser::searchLexemeBackward( lexRWordType rword, std::list<scriptLexeme*>::iterator &sli, std::list<scriptLexeme*>::iterator &out) { 
	
	out = sli;
	if(out == this->parser->scaner->getLexemesList().end())
		out--;
	while( true ) { 
		if((*out)->type == LT_RWORD && (*out)->rwordType == rword) break;
		if(out == this->parser->scaner->getLexemesList().begin())
			return false;
		out--;
	}
	return true;
}