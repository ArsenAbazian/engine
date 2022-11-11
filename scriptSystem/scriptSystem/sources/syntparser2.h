#ifndef __SCRIPT_SYNTAX_PARSER_INC
#define __SCRIPT_SYNTAX_PARSER_INC

#include "scaner.h"
#include "rulepproc.h"
#include "objDesc.h"
#include "scopeDesc.h"

// �������� ���� ���� �� ������
#define SAFE_LOG( X ) if( this->fpLog ) X

typedef enum {

	RR_FALSE	= 0,
	RR_TRUE		= 1,
	RR_WRONG	= 2

}ruleResult;

class scriptSyntaxParser2;

typedef ruleResult ( scriptSyntaxParser2::*rulePointer ) ();

// ����� ��������� ������ ���� ��������
class scriptModule;

// �������� ������ ��������������� �����������
// ������������ ������ ���� ����������� �������
class scriptSyntaxParser2
{
	
	std::list< scriptError*	>				errorList;			// ������ ������
	bool									bError;				// ��������� ������
	
	bool									ruleError;			// ������ � �������� �������
	bool									skipSymantic;		// ���������� ������������� ��������

	// 2 ���� ��� ��������� ������
	TCHAR									tempBuffer[ 2048 ];	

	// ��� ���������� ����������
	FILE									*fpLog;
	shString								logFileName;		// ��� ����� �������

	// ������, ������� ���������� ����������� �������
	scriptScaner							*scaner;
	
	// ������ ������ �������
	std::list< scriptLexeme* >				*lexemeList;
	
	// ������ ��������������� � �����
	std::list< scriptLexeme* >				*idsList;
	std::list< scriptLexeme* >				*stringList;
	
	// ������ ������
	std::list< fileNameInfo >				*fileList;
	
	// ������� �������
	std::list< scriptLexeme* >::iterator	currentLexeme;

	std::list< scriptLexeme* >::iterator	tmpSli1;
	
	// ������ � ������� ����� ������������ ��� ����������
	scriptModule							*module;			
	
	// ���� ��������� �������
	rulePostProcessor						rulepp;

	// �������� �������
	scriptObjectDesc						objDesc;
	
	// �������� ����
	scriptTypeDesc							typeDesc;
	
	// ���� ������� ����
	std::list< scriptScopeDesc* >			scopeList;

public:

	scriptSyntaxParser2();
	~scriptSyntaxParser2(){ this->clear(); }

	// �������������� ������
	void								setModule( scriptModule *mod ){ this->module = mod; }
	// �������������� ���� ������
	bool								initializeLog( shString fileName );
	// ������ ���� ������
	void								closeLog();
	// �������� ���
	void								clear();

	// ������������� ���
	scriptRetCode						parse( scriptScaner &scaner );
	
	// ��������� �� ����� ������ ������
	bool								isEnd(){ return this->currentLexeme == this->lexemeList->end(); }
	
	// ���������� ������ ���� � ��������� ������� ��������� ���� ������ �������������
	bool								getVariableInScope( scriptScopeDesc &scope, shString idName, int *objIndex );
	bool								getVariableInScope( scriptScopeDesc &scope, int idIndex, int *objIndex );

	// ����� ������� ������������ ������ �������� ����������!!!
	
	// ��������������� ��������
	ruleResult							isConstValue();
	ruleResult							isParser( lexParserType &pt );
	ruleResult							isParser( const lexParserType &pt );
	ruleResult							isParser( int pCount, lexParserType *pt );
	ruleResult							isId( );
	ruleResult							isRword( lexRWordType &rw );
	ruleResult							isRword( const lexRWordType &rw );
	ruleResult							isBuildInTypeName();
	ruleResult							isOneOfRules( int ruleCount, rulePointer *rulesArray );	// ���� �� �����������
	ruleResult							isTypedefName( );				// ����� �� ��� typedef
	ruleResult							isClassName( );					// ��� ��� ������	
	ruleResult							isTemplateClassName( );			// ��� ��� ������� ������
	ruleResult							isTypedefClassName( );			// ��� ��� typedef ���������
	ruleResult							isTypedefTemplateClassName( );	// ��� ��� typedef ���������� ������� ������
	ruleResult							isNamespaceName();				// ��� ��� ������������ ����
	ruleResult							isTypedefNamespaceName();		// ��� typedef ��� ������������ ����
	ruleResult							isMemberFunction();				// ��� ���� �������
	ruleResult							isMemberOperator();				// ��� ���� ��������
	ruleResult							isConstructor();				// ����� ��� �����������
	ruleResult							isOperator();					// ��������
	
	// ����� ������� ������� - ���������
	ruleResult							programmModule( );
};

#endif