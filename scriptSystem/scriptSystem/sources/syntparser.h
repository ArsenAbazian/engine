#ifndef __SCRIPT_SYNTAX_PARSER_INC
#define	__SCRIPT_SYNTAX_PARSER_INC

#include "scaner.h"
#include "rulepproc.h"

// ��� ����� ���� ��������� �� ����� grmParser.h ������� scGrammar

// ------------>

typedef enum _relationCode
{
	RELCODE_NONE				= 0,
	RELCODE_EQUAL				= 1,
	RELCODE_GREATER				= 2,
	RELCODE_LESS				= 3,
	RELCODE_GREATEREQUAL		= 4,
	RELCODE_LESSEQUAL			= 5,
	RELCODE_GREATERLESS			= 6,
	RELCODE_LESSGREATER			= 7,
	RELCODE_LESSGREATEREQUAL	= 8,
	RELCODE_GREATERLESSEQUAL	= 9
}relationCode;

// ��������������� ������� ���������� ����� ���������
TCHAR	*relationCode2Text( relationCode relcode );

// ��� ����� � ������ �����
#define TRIADE_LEXEME_EMPTY		(-1)

// �������� ������
typedef struct
{
	int							s1;		// ������ �������
	int							s2;		// ������ �������
	int							s3;		// ������ �������
}sgpTriade;

// ��������������� ������� ��� ������ ������ ������ � �������
int	getTriadeIndex( sgpTriade *triadeList, int triadeCount, int s1, int s2, int s3 );

// <------------

class scriptModule;	// ���������� ������ ������ 

class scriptSyntaxParser
{
	std::list< scriptError*	>		errorList;			// ������ ������
	bool							bError;				// ��������� ������

	int								matrixSize;			// ������ �������
	BYTE							**matrix;			// ���� ������� ���������������

	int								lexemeCount;		// ����� ������, ��������� � ����������
	scriptLexeme					*lexemeList;		// ������ ������, ��������� � ����������
	
	int								ruleCount;			// ����� ������, ��������� � ����������
	int								*ruleLeftPart;		// ����� ����� ������
	int								*rightPartSize;		// ������ ������ ����� ������, �� ����� ������ � ���
	int								**ruleRightPart;	// ������ ����� ������
	
	int								leftTriadeCount;	// ����� ����� �����
	sgpTriade						*leftTriadeList;	// ������ ����� �����

	int								rightTriadeCount;	// ����� ������ �����
	sgpTriade						*rightTriadeList;	// ������ ������ �����

	
	std::list< scriptLexeme* >		lexemeStack;		// ���� ��� ������� ������ ������	
	std::list< int >				indexStack;			// ���� �������� ��� ������� ������

	// 2 ���� ��� ��������� ������
	TCHAR							tempBuffer[ 2048 ];	

	// ��� ���������� ����������
	FILE							*fpLog;
	shString						logFileName;		// ��� ����� �������

	// ������ � ������� ����� ������������ ��� ����������
	scriptModule					*module;			
	// ���� ��������� �������
	rulePostProcessor				rulepp;

public:

	scriptSyntaxParser(  );								// �����������
	~scriptSyntaxParser(){ clear(); }					// ����������					

	void							clear();			// �������� ������� ������
	
	// ������������� ����������� - ���� ���� ������� ����� �������������� �����������
	// scGrammarBuilder
	void							initialize(  );		// �������������
	
	// �������������� ������
	void							setModule( scriptModule *mod ){ this->module = mod; }

	// ��������������� ������� ��� �������
	// �������� ������ �������
	int								getLexemeIndex( scriptLexeme &lex );
	// ����� ������ ������ 
	int								getTriadeIndex( int s1, int s2, int s3, bool left );
	// �� �� ����� ������ � ������� ������
	int								getTriadeIndex( scriptLexeme *lex1, scriptLexeme *lex2, scriptLexeme *lex3, bool left );
	// ��������� �������
	scriptRetCode					findRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int &ruleIndex );
	scriptRetCode					applyRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list<int>::iterator &sliBeginIndex, std::list< scriptLexeme* >::iterator &sliEnd, std::list<int>::iterator &sliEndIndex, int ruleIndex );

	// ��������� ������
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col );	// �������� ������ � ������ ������
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex );

	// ������ ������ ������
	scriptRetCode					parse( scriptScaner &scaner );	
	// �������� ����� � �������� ������� � ������� ��������� <
	scriptRetCode					parseBack( scriptScaner &scaner, std::list< scriptLexeme *>::iterator &sliGreater );

	// ���������� ����������
	
	// �������������� ���� �������
	scriptRetCode					initLog( shString fileName );

	// �������� ���� ������ � ��������� �������
	void							printStack( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli, relationCode relCode, std::list< scriptLexeme* > scList );
	// �������� �������, ������� ����� ���������
	void							printStack( std::list< scriptLexeme* >::iterator &sli1, std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &sli2, relationCode relCode1, relationCode relCode2, std::list< scriptLexeme* > scList );
	// �������� ��������� �������
	void							printFoundRule( std::list< scriptLexeme* >::iterator &sliBegin, std::list< scriptLexeme* >::iterator &sliEnd, std::list< int >::iterator &sliBeginIndex, std::list<int>::iterator &sliEndIndex, int ruleIndex, std::list< scriptLexeme* > scList );
};

#endif