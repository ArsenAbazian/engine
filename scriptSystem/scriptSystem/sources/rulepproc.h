#ifndef __RULE_POST_PROCESSOR_INC
#define __RULE_POST_PROCESSOR_INC

#include < windows.h >
#include "lexeme.h"
#include "scrcode.h"
#include "screrror.h"
#include <list>
#include <stack>

// описание класса, который после применения правила осуществляет
// дополнительные действия не предусмотренные грамматикой
class rulePostProcessor
{
	
public:
	rulePostProcessor(){};
	~rulePostProcessor(){};

	// обработать правило
	scriptRetCode	processRule( );

};

#endif