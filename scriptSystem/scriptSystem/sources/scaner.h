#ifndef __SCRIPT_SCANER_INC
#define __SCRIPT_SCANER_INC

#include "scrcode.h"
#include "fileinfo.h"
#include "hstring.h"
#include "lexeme.h"
#include "screrror.h"
#include "scrmacro.h"
#include <stack>

typedef enum _scriptScanerState		// �������� ��������� �������
{
	SSS_NONE							= 0,	// ��������� ���������
	SSS_ID_RWORD						= 1,	// ��������� ���������� ��������������
	SSS_NUMERIC_CONST					= 2,	// ��������� ���������� ��������� ���������
	SSS_COMMENT_LINE					= 3,	// ����������� �����
	SSS_COMMENT_BLOCK					= 4,	// ����������� �����
	SSS_STRING							= 5,	// ��������� ���������
	SSS_ERROR							= 6,	// ��������� ������� ��������������
	SSS_CHAR_CONST						= 7,	// ���������� ���������
	SSS_FLOAT_CONST						= 8,	// ������������ ���������
	SSS_FLOAT_CONST2					= 9,	// ����������� ���������� ������������ ���������
	SSS_FLOAT_CONST3					= 10,	// ����������� ���������� ������������ ���������
	SSS_FLOAT_CONST4					= 11,	// ��������� ���������� ������������ ���������
	SSS_SCAN_INCLUDE					= 12,	// ��������� � ������� ����������� ������ � ������� ������� < ... >
	SSS_FIND_DIRECTIVE_SYMBOL			= 13,	// ��������� � ������� ������������ ��� ������� � �������� ������ #
	SSS_WAIT_FOR_ENDIF_ANALOG			= 14,	// ��������� � ������� �� ������� ��������� endif else � elif
	SSS_SCAN_ENDIF_ANALOG				= 15,	// ��������� � ������� �� ��������� ���������
	SSS_FORCE_DWORD						= 0xffffffff
}scriptScanerState;

typedef enum						// �������� ���� �������� ������������
{
	SST_DO_NOT_SKIP_SCANNING		= 0,
	SST_SKIP_UNTIL_NEW_CONDITION	= 1,
	SST_SKIP_UNTIL_END				= 2,
	SST_FORCE_DWORD					= 0xffffffff
}scanSkipType;


class scriptScaner	// ������ - ����������� ���������� ������
{

	// ������������ �������
	std::list< shString >					includeDirList;					// ��������� ��� ���������� ������
	shString								programmDirectory;				// ���� ���������
	shString								programmFileName;				// ��� ����� ���������
	shString								programmFullPathName;			// ������ ���� �����

	// ������ ��������
	std::list< scriptMacros* >				macros;							
		
	// ������ ������ � ��������������
	std::list< scriptError* >				errorList;						
		
	// ���������� � ����� � �������� �������
	fileInfo								fInfo;
	
	// ��������� �������
	scriptScanerState						state;
	
	// 2 ���� ��� ��������� ������
	TCHAR									tempBuffer[ 2048 ];	

	// ��� ����������� �������� ������������
	scriptScaner							*parent;		// ������������ ������
	scriptScaner							*prevScaner;	// ���������� ������
	scriptScaner							*nextScaner;	// ��������� ������
	
	// ������ ������ ���������� � ���������� ������������
	std::list< scriptLexeme* >				lexemes;										
	// ������ ��������������� ���������� � ���������� ������������
	std::list< scriptLexeme* >				ids;	
	// ������ ��������� �������� ���������� � ���������� ������������
	std::list< scriptLexeme* >				constStrings;

	// ������ ������ � ��������� �������������
	// � ����� ������ ���������� �������
	std::list< scriptLexeme* >				directiveLexemes;
	
	// ������� � �������� ������ ������ ��������
	std::list< scriptLexeme* >				inputMacrosList;
	std::list< scriptLexeme* >				outputMacrosList;
	
	// ������ ��������������� ������		
	std::list< fileNameInfo >				scanedFiles;		

	// �������������� ���������
	bool									skipLine;			// ����������, ��� ���� ���������� ��� ������� �� ����� ������
	int										skipLineIndex;		// ����� ������������ �����

	std::stack< scanSkipType >				skipScanning;		// ���������� ������������ ������, ���� �� ����� ��������� ������� ����������

	// ��������� ������ � ���������� ������������
	bool									bError;			

public:
	// ����������� �����
	bool									enableFormatString;		// ������������� ������ �� �������� c++
	bool									skipPreprocDirectives;	// ���������� ���������
	bool									useInGrammar;			// ����������� ��� ����������

	scriptScaner();					// ����������� 
	~scriptScaner();				// ����������

	// ������������� 
	scriptRetCode					initialize( std::list< shString > &incDir );
	scriptRetCode					initialize( scriptScaner *prev );
	scriptRetCode					initialize(  );

	// �������� ��������
	// ������� ��������� ���� ��� ������ ( ������ *.cpp ���� )
	scriptRetCode					openFile( TCHAR *fileName );							
	// ������� ���� � ���������� ���������� ��������� include
	scriptRetCode					openIncludeFile( shString &fileName, bool inSystem );		
	
	// ���������� ������������ �� ��������� ���� ������ ��������
	bool							isFileOpened( shString &fileName );	

	// ������������������� ������ ��� ������� ��������
	scriptRetCode					addBuffer( BYTE *buffer, DWORD bufLength, int line, int col );				
	scriptRetCode					addBuffer( BYTE *buffer, DWORD bufLength, int line, int col, scriptScaner *owner );
	
	// �������� ������������ �����������
	scriptRetCode					getNextLexeme( scriptLexeme	&lex );				// �������� ��������� ������� �� �����
	scriptRetCode					scan();											// ��������� ������������� ����
	scriptRetCode					scanDirective();								// ����������� ���������
	scriptRetCode					scanMacros( scriptLexeme &lex, scriptMacros& scrMacro );	// ��������� ������
	scriptRetCode					scanMacros( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList, scriptMacros& scrMacro );	// ��������� ������ ������� ��������� � ������ ������� 
	scriptRetCode					skipUntilDirective();							// ���������� ������������ ���� �� ����� ������� ��������� endif elif ��� else 
	
	// ������ ��������
	scriptRetCode					parseDirective();								// ������ ���������
	scriptRetCode					parseIncludeDirective();						// ������ ��������� include
	scriptRetCode					parseDefineDirective();							// ������ ��������� define 
	scriptRetCode					parseDefineParams( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli );	// ��������� ��������� �������
	scriptRetCode					parseDefineBody( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli );	// ��������� ���� �������
	scriptRetCode					parseIfDefDirective();							// ������ ��������� ifdef
	scriptRetCode					parseIfNdefDirective();							// ������ ��������� ifndef
	scriptRetCode					parseEndIfDirective();							// ������ ��������� endif
	scriptRetCode					parseIfDirective();
	scriptRetCode					parseElseDirective();							// ������ ��������� else
	scriptRetCode					parseUndefDirective();							// ������ ��������� undef
	scriptRetCode					parseErrorDirective();							// ������ ��������� error
	scriptRetCode					parseMacros( scriptLexeme &lex, scriptMacros &scrMacro );									// ����������� ������									
	
	// ���������� ��������
	scriptRetCode					executeIfDirective();
	scriptRetCode					executeIfDirective( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList );	// ��������� ��������� if
	scriptRetCode					executeErrorDirective();						// ���������� ��������� error
	scriptRetCode					executeIncludeDirective( shString &str, bool inSysDir );	// ���������� ��������� include
	scriptRetCode					executeMacros( scriptLexeme &lex, scriptMacros &scrMacro, std::list< scriptLexeme* >	&lexList );		// ���������� ����������������
	scriptRetCode					executeDefinedOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );		// ��������� �������� defined macroname
	scriptRetCode					executeUnaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );	// ��������� �������� ! ��� ~
	scriptRetCode					executeBinaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList );	// ��������� �������� ��������
	scriptRetCode					executeQuickIfOp( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList ); // ��������� ������� if

	// ����������� �� ������ �������
	scriptRetCode					optimizeMacroUnaryMinusAndPlus( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList );	// ����� �������� �� ��� ������� ���� �������� ������ ��� ����� ��������������
	scriptRetCode					optimizeParsers( std::list< scriptLexeme* > &lexList );		// �������������� ����������� ( ���������� ��������� ������� ����� ������� ������������ � ����������� )						
	scriptRetCode					optimizeStrings( std::list< scriptLexeme* > &lexList );		// ���������� ����� � ��������� ����� � �������
	scriptRetCode					optimizeIds( std::list< scriptLexeme* > &lexList );			// ������� ��� �������������� � �������
	scriptRetCode					optimize( std::list< scriptLexeme* > &lexList );			// �������������� ������� � ��������� ������
	scriptRetCode					optimize();													// �������������� ������ ������ ���������� � ���������� ������������

	// ������� ��������
	scriptRetCode					clear();									// �������� ������
	scriptRetCode					clearScanerResult();						// �������� ���������

	// ��������������� �������
	bool							idExist( shString &name, int &index );		// ���������, ������� �� ������������� � ������� ���������������
	bool							stringExist( shString &name, int &index );	// ���������, �������� �� ������ � ������� �����
	scriptMacros*					macroExist( shString &name );				// ���������, ���������� �� ����� ������
	void							createIdReference( scriptLexeme &lex );		// ������� ������������� � ������� ��������������� � �������� ��� �� ��� ������ � �������
	void							createStringReference( scriptLexeme &lex );	// ������� ������ � ������� ����� � �������� ����� ������ �� �� ����� � ������� 
	shString&						getLexemeName( scriptLexeme &lex );			// �������� ��� �������������� ��� ����� ������
	bool							isSystemIncludeDir( scriptLexeme &lex );	// ������� �� ������ # include <						// ���������� ������ ���� ������ ��������� #include < 
	DWORD							toDword( TCHAR *szString, DWORD dwLength );
	bool							isEscapeSequence( TCHAR *szString, DWORD dwLength );	// ���� ��� ������� ������ ������
	scriptRetCode					formatString( TCHAR *szString, TCHAR *szString2, DWORD dwLength, scriptLexeme &scrLex );		// ����������� ������
		
	// ��������� ������ ������
	int								isFileScaned( shString fullPathName );		// ��� �� ������ ��� ����� ����
	void							getFileIndex( fileInfo &fi, int *fileInd );				// �������� ������ ����� ���� ����, ���� ���, �� �������� � ����� �������� ������ �����
	void							addFileToList( fileInfo &fi );
	
	// ���������� �������������� �������� �������������
	scriptRetCode					ppExecuteArithmeticOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );
	scriptRetCode					ppExecuteLogicalOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );
	scriptRetCode					ppExecuteBinaryOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp );

	// ��������� ������
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col );	// �������� ������ � ������ ������	
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, int line, int col );	
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex );
	scriptError&					addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex );
	
	// ������� ������� � ������ ������
	std::list< scriptLexeme* >&		getLexemesList(){ return lexemes; }
	std::list< scriptLexeme* >&		getIdsList(){ return ids; }
	std::list< scriptLexeme* >&		getStringsList(){ return constStrings; }
	std::list< scriptError* >&		getErrorsList(){ return errorList; }
	std::list< scriptMacros* >&		getMacrosList(){ return macros; } 
	std::list< fileNameInfo >&		getFilesList(){ return scanedFiles; }	

	// ������� �������
	void							onError();			// ���������� ����� ������ ������ ������
	void							onNewLine();		// ���������� ����� ������ ��������� �� ��������� ������
	void							onNewFile();		// ���������� ����� ������ ��������� ����� ����
	void							onGetNewLexeme();	// ���������� ����� ������ ������� ����� �������

	bool							isError(){ return this->bError; };

	fileNameInfo*					getFileNameInfo(int fileIndex);	// �������� ���� ����� �� �������
	
	bool							checkLex(std::list<scriptLexeme*>::iterator &sli) { return sli != this->lexemes.end(); }
};

#endif
