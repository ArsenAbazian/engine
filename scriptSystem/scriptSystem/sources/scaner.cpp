#include "scaner.h"
#include <stdio.h>
#include <io.h>

/************************************************************************/
/* ������ ����������� �� ��� ���������						            */
/* ������� �������� ��������� � �.�. ��� �������						*/
/************************************************************************/
BYTE	preprocOperatorType[] =
{
	0,  // unrecognized
	1,  // ~
	1,	// !
	0,	// #
	0,	// $
	2,	// %
	2,	// ^
	2,	// &
	2,	// *
	0,	// (
	0,	// )
	2,	// -
	2,	// +
	0,	// =
	0,	// [
	0,	// ]
	0,	// {
	0,	// }
	0,	// :
	0,	// ;
	2,	// |
	0,	// 
	0,	// ,
	0,	// .
	3,	// ?
	2,	// /

	0,	// ::
	0,	// ->
	1,	// ++
	1,	// --
	0,	// .*
	0,	// ->*
	2,	// <<
	2,	// >>
	2,	// <
	2,	// >
	2,	// <=
	2,	// >=
	2,	// ==
	2,	// !=

	2,	// &&
	2,	// ||

	0,	// *=
	0,	// /=
	0,	// %=
	0,	// +=
	0,	// -=
	0,	// <<=
	0,	// >>=
	0,	// &=
	0,	// |=
	0,	// ^=
};

/************************************************************************/
/* �������� ���� �������� - �������������� ���������� ��� ���������		*/
/* 0 - �������															*/
/* 1 - ��������������													*/
/* 2 - ����������														*/
/* 3 - ���������														*/
/************************************************************************/
BYTE	preprocOperationType[] =
{
	0, // unrecognized
	3, // ~
	2,	// !
	0,	// #
	0,	// $
	1,	// %
	3,	// ^
	2,	// &
	1,	// *
	0,	// (
	0,	// )
	1,	// -
	1,	// +
	0,	// =
	0,	// [
	0,	// ]
	0,	// {
	0,	// }
	0,	// :
	0,	// ;
	3,	// |
	0,	// 
	0,	// ,
	0,	// .
	0,	// ?
	1,	// /
	0,	// ::
	0,	// ->
	1,	// ++
	1,	// --
	0,	// .*
	0,	// ->*
	3,	// <<
	3,	// >>
	2,	// <
	2,	// >
	2,	// <=
	2,	// >=
	2,	// ==
	2,	// !=
	2,	// &&
	2,	// ||

	0,	// *=
	0,	// /=
	0,	// %=
	0,	// +=
	0,	// -=
	0,	// <<=
	0,	// >>=
	0,	// &=
	0,	// |=
	0,	// ^=
};

/************************************************************************/
/* ������� �����������                                                  */
/* �������������� �������� �� ���������									*/
/************************************************************************/
scriptScaner::scriptScaner()
{
	state						= SSS_NONE;
	this->nextScaner			= NULL;
	this->parent				= this;
	this->prevScaner			= NULL;

	this->enableFormatString		= true;
	this->skipPreprocDirectives		= false;
}

/************************************************************************/
/* ��������� - ������� ��� �������, ����������� ������ � �. �.          */
/************************************************************************/
scriptScaner::~scriptScaner()
{
	this->clear();
}

/************************************************************************/
/* ������������� ��������� ���������� �������                           */
/* ������� �������� - ������ ����� - ����� ���������� � ����������      */
/* ������������� �������												*/
/************************************************************************/
scriptRetCode scriptScaner::initialize( std::list< shString > &incDir )
{

	// ������� ��� �������
	clear();

	std::list< shString >::iterator		shsi;
	
	// ����������� ������ ��������� ����������
	for( shsi = incDir.begin(); shsi != incDir.end(); shsi ++ )
		this->includeDirList.push_back( *shsi );
	
	// �������� ���������� � ���������
	programmDirectory.getModuleDirectory();
	programmFileName.getModuleFileName();
	programmFullPathName.getModuleFullPathName();

	return SRV_SUCCESS;
}

/************************************************************************/
/* ������������� ������� ��� ������ ��������� ����������				*/
/*													                    */
/************************************************************************/
scriptRetCode scriptScaner::initialize( )
{
	std::list< shString >	emptyList;
	return this->initialize( emptyList );
}

/************************************************************************/
/* ������������� �������, ��� ���������� ��� ������� �������            */
/* ������� ��������: ��������� �� ���������� ������                     */
/************************************************************************/
scriptRetCode scriptScaner::initialize( scriptScaner *prev )
{

	// ������� ��� �������
	clear();

	if( prev->parent == NULL )
		this->parent		= prev;
	else
		this->parent		= prev->parent;
	
	this->prevScaner		= prev;
	
	return SRV_SUCCESS;
}

/************************************************************************/
/* �������� ����������� ������������                                    */
/* ������ ������, ������� ���������������, �����, ������ ������,		*/
/* ��������																*/
/************************************************************************/
scriptRetCode scriptScaner::clearScanerResult()
{

	std::list< scriptMacros* >::iterator	smi;
	std::list< scriptError* >::iterator		sei;
	std::list< scriptLexeme* >::iterator	sli;
	
	// ������������� �� �������
	if( !this->macros.empty() )
	{
		// ������� ������� ���� ��� ������� �������
		for( smi = this->macros.begin(); smi != this->macros.end(); smi ++ )
			delete *smi;
				
		// ������ ������������� �� ������ ������
		this->macros.clear();
	
	}
	
	// ������ ������ � ��������������
	if( !this->errorList.empty() )
	{
		// ������� ������� ������
		for( sei = this->errorList.begin(); sei != this->errorList.end(); sei ++ )
			delete (*sei);
				
		// ����� ��� ������
		this->errorList.clear();
	}
			
	this->state					= SSS_NONE;
	
	// ������� ���� ���� ��������� ������ - �� ������� ������� ���
	if( this->nextScaner )
		delete	this->nextScaner;
		
	this->parent			= this;
	this->nextScaner		= NULL;
	this->prevScaner		= NULL;

	// ������ ������������� �� ������
	clearLexList( this->lexemes );
	// ���������������
	clearLexList( this->ids );
	// � ��������� ��������
	clearLexList( this->constStrings );
	// ������ ������ ��������� �������������
	clearLexList( this->directiveLexemes );	
	
	// ������ ������ ��������
	clearLexList( this->inputMacrosList );	
	clearLexList( this->outputMacrosList );
	
	// ������� ������ ����� �������� ����������
	((std::deque<scanSkipType>&)this->skipScanning._Get_container()).clear();

	// ������� ������ ���������� �� ����� �����
	this->scanedFiles.clear();
		
	// ������� ��������� ������
	this->bError = false;

	return SRV_SUCCESS;
}

/************************************************************************/
/* ������ ������� ������: ����������� ������������ � ���������          */
/* ����������															*/
/************************************************************************/
scriptRetCode scriptScaner::clear()
{
	
	std::list< shString >::iterator			shsi;
		
	// ������� ������������� �� ����������
	if( !this->includeDirList.empty() )
	{
		// ������� ������� ������
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
			shsi->clear();
		
		// ����� ��� ������
		this->includeDirList.clear();
	}
	
	// �� ���������� � ���������
	this->programmDirectory.clear();
	this->programmFileName.clear();
	this->programmFullPathName.clear();
	
	this->clearScanerResult();

	return SRV_SUCCESS;
}

/************************************************************************/
/* �������� ������ ����� � ������� ������                               */
/* ������� ��������� ���������� � ����� � ��������� �� ���������� �����	*/
/************************************************************************/
void scriptScaner::getFileIndex( fileInfo &fi, int *fileInd )
{
	
	std::list< fileNameInfo >::iterator	fni;
	*fileInd = -1;
	
	if( this->parent->scanedFiles.empty() ) return;
	for( *fileInd = 0, fni = this->parent->scanedFiles.begin(); fni != this->parent->scanedFiles.end(); fni ++, (*fileInd) ++ );
	{
		if( fi.fullPathName == fni->fullPathName )
			return ;
	}
	
	return ;
}

void scriptScaner::addFileToList( fileInfo &fi )
{

	fileNameInfo	fNameInfo;
	int				fileInd;

	// �������� �������� ������ �����
	this->getFileIndex( fi, &fileInd );
	
	if( fileInd == -1 )
	{
		// ������ �����
		fi.fileIndex	= (long) this->parent->scanedFiles.size();
		
		// ������� ��������� - ���� �� �����
		fNameInfo.directory		= fi.directory;
		fNameInfo.fileName		= fi.fileName;
		fNameInfo.fullPathName	= fi.fullPathName;

		// ��������� �� � ����
		this->parent->scanedFiles.push_back( fNameInfo );
	}
	else 
	{
		fi.fileIndex = fileInd;
	}

	return ;
}

scriptError& scriptScaner::addError( scriptErrorType type, scriptRetCode code, TCHAR *text, long fileIndex, int line, int col )
{
	
	scriptError	*scrError	= new scriptError;

	scrError->type			= type;
	scrError->code			= code;
	scrError->text			= text;
	scrError->fileIndex		= fileIndex;
	scrError->line			= line;
	scrError->col			= col;

	this->parent->errorList.push_back( scrError );

	if( scrError->type == SE_SCANER_ERROR || scrError->type == SE_SCANER_FATAL_ERROR ||
		scrError->type == SE_PREPROCESSOR_ERROR || scrError->type == SE_PREPROCESSOR_FATAL_ERROR )
		this->bError = true;
	
	return *( this->parent->errorList.back() );
}

scriptError& scriptScaner::addError( scriptErrorType type, scriptRetCode code, TCHAR *text, int line, int col )
{
	return this->addError( type, code, text, fInfo.fileIndex, line, col );
}

scriptError& scriptScaner::addError( scriptErrorType type, scriptRetCode code, TCHAR *text )
{
	return this->addError( type, code, text, this->fInfo.fileIndex, this->fInfo.line, this->fInfo.col );
}

scriptError& scriptScaner::addError( scriptErrorType type, scriptRetCode code, TCHAR *text, scriptLexeme &lex )
{
	return this->addError( type, code, text, this->fInfo.fileIndex, lex.line, lex.col );
}

scriptError& scriptScaner::addError( scriptErrorType type, scriptRetCode code, scriptLexeme &lex )
{
	return this->addError( type, code, lex.getLexemeDesc( this->parent->ids ).getCString(), fInfo.fileIndex, lex.line, lex.col );
}

scriptRetCode scriptScaner::openFile( TCHAR *fileName )
{

	// ������� ������� ��� ���������� ���������
	this->clearScanerResult();
	
	// �������� ������� ����
	if( fInfo.openFile( fileName ) == SRV_FAIL )
		return SSRV_CANT_OPEN_FILE;

	// ����� ��������� ������� � ����� � ������ �������
	this->state						= SSS_NONE;
	
	// ��������� ���� � �������
	this->addFileToList( this->fInfo );
	
	return SRV_SUCCESS;
}


scriptRetCode scriptScaner::openIncludeFile( shString &fileName, bool inSystem ){

	std::list< shString >::iterator		shsi;
	shString							fullPathName;

	std::list< shString >::iterator		dirIter;
	TCHAR								*fileNameBegin;
	FILE								*fp;
			
	// ����� �� ������ ���������� ���������� ������ ��� �����, ���, ����� ��� ����� ���� �������
	// ���� ���� ������ � ��������� ������
	if( inSystem )
	{	
	
		// ���������� �� ���� ����������� � ���� ������ ����
		for( shsi = this->parent->includeDirList.begin(); shsi != this->parent->includeDirList.end(); shsi ++ )
		{
			// ���������� ������ ��� �����
			fullPathName	= *shsi;
			fullPathName	+= TEXT("\\");
			fullPathName	+= fileName;
			
			// �������� ��� ������ ��� �������
			_tfopen_s( &fp, fullPathName.getCString(), TEXT("rb") );
			if( fp == NULL )
				continue;
			
			// ���������� ����������
			dirIter = shsi;
			break;
		}

		// �������������� ���� ������� ���� ������ �� 
		// ���� �� ������ �� � ����� ��������� �����
		if( shsi == this->parent->includeDirList.end() )
			return SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR;

		// ���� ������ ������� �� �� �����, �� ����� ���� ��� ������
		if( this->isFileOpened( fullPathName ) )
		{
			fclose( fp );
			return SPPRV_FILE_RECURSION;
		}

		// ��� �������� �������� ���� �������� ����
		this->fInfo.openFile( *dirIter, fileName, fullPathName, fp );
		fclose( fp );
		
		// ��������� ���� � �������
		this->addFileToList( this->fInfo );
	}
	else
	{
		// ���� �� ��������� ����, �� ���������� ����������� �� ��������
		// �.�. ���� �� ������ ������ ���� � �����, �� ����������, ����������� �� ����������, ���������� 
		// ��������� ����� �������� ������ ���� � �����
		
		// ��������� - ������ �� ������ ����
		if( ( ( fileName.getCString()[0] >= 'A' && fileName.getCString()[0] <= 'Z' ) || ( fileName.getCString()[0] >= 'a' && fileName.getCString()[0] <= 'z' ) ) &&
			fileName.getCString()[1] == ':' && fileName.getCString()[2] == '\\' )
		{
			// ��, ������ ������ ����
			fullPathName = fileName;
		}
		else 
		{
			// ��� ������ ���� �� ������, �������� ������� ������ ����
			
			// ���� ����� �� ���� ��� ������, �� ��������� �� ����,
			if( this->prevScaner != NULL )
				fullPathName = this->prevScaner->fInfo.directory;
			else	// � ��������� ������ ��������� �� ������� ����������
				fullPathName = this->programmDirectory;
			
			fullPathName += TEXT( "\\" );
			fullPathName += fileName;
				
		}

		// �������� ���������� ��� �����
		if( GetFullPathName( fullPathName.getCString(), 2048, this->tempBuffer, &fileNameBegin ) == NULL )
			return SPPRV_CANT_OPEN_READ_FILE;

		fullPathName = this->tempBuffer;
		
		if( this->isFileOpened( fullPathName ) )
			return SPPRV_FILE_RECURSION;

		if( fInfo.openFile( fullPathName.getCString() ) == SRV_FAIL )
			return SPPRV_CANT_OPEN_READ_FILE;

		// ��������� ���� � �������
		this->addFileToList( this->fInfo );
			
	}

	return SRV_SUCCESS;		
}

scriptRetCode scriptScaner::addBuffer( BYTE *buffer, DWORD bufLength, int line, int col )
{

	// ������� �������
	this->clearScanerResult();

	// ��������� ������
	this->fInfo.openBuffer( buffer, bufLength, line, col );

	// ����� ��������� ������� � ����� � ������ �������
	this->state					= SSS_NONE;
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::getNextLexeme( scriptLexeme &lex )
{

	int					readSymbolCount		= 0;	// ����� ��������� �� �������� ������ ��������		
	TCHAR				*endptr;				
	TCHAR				symbol[16];					// 16 ���� ��� ������ �������
	TCHAR				temp[16];					// 16 ���� ��� ��������� ������
	shString			name;						// ��� �������������� ��� ������������������ �����
	symbolClassCode		scc;						// ����� ������
	
	lex.useInGrammar = this->useInGrammar;
	
	// ������ ���� ������  
	while( 1 )
	{
	
		// ������� � ����� ��������� ��������� 
		switch( this->state )
		{
		
		case SSS_NONE:
			
			// �������� �� ����� �����, 
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;
			
			scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// ���������� ������
			// ���������� ������ ���������
			// ���������� ������ 0x0a
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x20 || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x09 || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
			{	
				if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
					this->fInfo.col		= 0;
				else if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 ) 
				{
					this->fInfo.col		+= 4;
					this->fInfo.col		&= 0xfffffffc;
				}
				else
					this->fInfo.col		++;

				this->fInfo.filePos ++;
				
				continue;
			}

			// ������������ ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.filePos ++;
				this->fInfo.line	++;
				return SSRV_END_OF_LINE;
			}

			
			// ��������: ����� ��� �����
			if( scc == SCC_LETTER )
			{
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		= 1;
				this->state			= SSS_ID_RWORD;
				
				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;

				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				continue;
			}

			// ��������: ����� ���� ����� ��� ����������� �����
			if( scc == SCC_DIGIT )
			{
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount = 1;
				this->state = SSS_NUMERIC_CONST;

				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;

				continue;
			}

			// ������������ �����
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && this->fInfo.fp[ this->fInfo.filePos ] == '.' && symbolClass [ this->fInfo.fp[ this->fInfo.filePos + 1 ] ] == ( BYTE ) SCC_DIGIT )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount = 1;

				this->state			= SSS_FLOAT_CONST;

				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;

				continue;
			}

			// ��������: ����������� �����
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '/' && 
				this->fInfo.fp[ this->fInfo.filePos + 1 ] == '/' )
			{
				// ����������� �����
				this->state = SSS_COMMENT_LINE;
				
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				continue;
			}

			// ��������: ����������� �����
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '/' && 
				this->fInfo.fp[ this->fInfo.filePos + 1 ] == '*' )
			{
				// ����������� �����
        		this->state = SSS_COMMENT_BLOCK;
				
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				continue;
			}

			// ��������: ������
			// ��������: ���������� ���������
			if( this->fInfo.fp[ this->fInfo.filePos ] == '\"' || 
				this->fInfo.fp[ this->fInfo.filePos ] == '\'' )
			{
			
				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				
				if( this->fInfo.fp[ this->fInfo.filePos ] == '\"' )
					this->state			= SSS_STRING;
				else
					this->state			= SSS_CHAR_CONST;

				this->fInfo.filePos ++;
				this->fInfo.col		++;	

				continue;
			}
		
			// �������� - ������� ����������� - ��������
			if( scc == SCC_PARSER )
			{
				
				// �������������� �������
				lex.type			= LT_PARSER;
				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				lex.setParser( this->fInfo.fp[ this->fInfo.filePos ] );
				
				lex.name.clear();

				this->fInfo.filePos ++;
				this->fInfo.col		++;

				// ������ �������: � �� �������� �� ��� ��������� include
				if( this->isSystemIncludeDir( lex ) )
				{
					readSymbolCount = 0;
					this->state = SSS_SCAN_INCLUDE;
				}
				
				return SRV_SUCCESS;

			}
			
			// �������� ������ ������
			if( scc == SCC_BAD_SYMBOL )
			{
				lex.type			= LT_BAD_CHARACTER;
				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				lex.byteValue		= this->fInfo.fp[ this->fInfo.filePos ];
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;	

				this->state				= SSS_NONE;
				
				// ������� ������
				_stprintf_s( this->tempBuffer, TEXT( "'%c' (0x%2.2x)" ), lex.byteValue, (DWORD)lex.byteValue );
				return this->addError( SE_SCANER_ERROR, SSRV_BAD_CHARACTER, this->tempBuffer, lex ).code;
		
			}
			
			break; // SSS_NONE
		
		case SSS_SCAN_INCLUDE:
			
			// �������� �� ����� �����, 
			// ����� ������
			// ������ '>'
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				this->fInfo.fp[ this->fInfo.filePos ] == '>' )
			{
				
				// ���������� �������� �������
				
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_STRING;
				lex.fileIndex	= this->fInfo.fileIndex;
				lex.name		= this->tempBuffer;
				// ������� ������� ����� � ������
				lex.name.cutSpacesLeft();
				lex.name.cutSpacesRight();

				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}

			// �� ���� ��������� ������� ���������� �������� � ������ � ������� � ����. ���������
			else 
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				
				if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 ) 
				{
					this->fInfo.col		+= 4;
					this->fInfo.col		&= 0xfffffffc;
				}
				else
					this->fInfo.col		++;
				
				this->fInfo.filePos ++;
				readSymbolCount ++;
			}

			break; // SSS_SCAN_INCLUDE2
		
		case SSS_ID_RWORD:

			if( this->fInfo.filePos < this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
				
			// �������� �� ����� �����,
			// �����������
			// ��� ������� ������ ( + ������� ������ + ������ + ��������� )
			if( this->fInfo.filePos == this->fInfo.fileLength || scc == SCC_PARSER )
			{
				
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = 0;
				lex.type		= LT_ID;
				lex.name		= this->tempBuffer;
				
				// ������� ������������� � ����������������� �����
				lex.convertIfRWord( );
								
				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}
						
			// ��������: ��� ���� ����� ��� ����� ��� ������ ������
			// ��� ����� ��� �������������
			if( scc == SCC_LETTER || scc == SCC_DIGIT || scc == SCC_BAD_SYMBOL )
			{
				// ��������� ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];

				readSymbolCount ++;
				
				this->fInfo.filePos ++;
				this->fInfo.col ++;

				// ���� ������ ������ - ������������ � ��������� ������
				if( scc == SCC_BAD_SYMBOL ){
					
					_stprintf_s( temp, TEXT( "'%c' (0x%2.2x)" ), lex.byteValue, (DWORD)lex.byteValue );
					this->addError( SE_SCANER_ERROR, SSRV_BAD_CHARACTER, temp, lex );
										
					this->state = SSS_ERROR;
				}
				
				continue;
			}
			
			break; // SSS_ID_RWORD

		case SSS_COMMENT_LINE:
			
			// �������� �� ����� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state			= SSS_NONE;
				return SSRV_END_OF_FILE;
			}
			
			// �������� �� ����� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->state			= SSS_NONE;
				continue;
			}

			this->fInfo.filePos ++;
			this->fInfo.col		++;

			break; // SSS_COMMENT_LINE

		case SSS_COMMENT_BLOCK:
			
			// �������� �� ����� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state			= SSS_NONE;
				
				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_COMMENT_BLOCK_NOT_FINISHED, TEXT("") ).code;
			}

			// �������� ����� �����������
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '*' && 
				this->fInfo.fp[ this->fInfo.filePos + 1] == '/' )
			{
			
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				this->state			= SSS_NONE;
				
				continue;
			}
 
			// �������� �� ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line	++;
				this->fInfo.col		= 0;
			}

			// ���������� ������ 0x0a
			if( this->fInfo.fp[ this->fInfo.filePos ] != 0x0a )
				this->fInfo.col ++;

			this->fInfo.filePos ++;
			
			break; // SSS_COMMENT_BLOCK

		case SSS_STRING:
			
			// ��������: ����� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state							= SSS_NONE;
				this->tempBuffer[ readSymbolCount ] = 0;
				
				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_EOF_IN_STRING_CONSTANT, this->tempBuffer ).code;
								
				return SSRV_EOF_IN_STRING_CONSTANT;			
			}
			
			// ��������: ����� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == '\"'  && !this->isEscapeSequence( this->tempBuffer, readSymbolCount ) )
			{
			
				
				this->tempBuffer[ readSymbolCount ] = 0;
				// ���� ����� ������������� ������
				if( this->enableFormatString )
					this->formatString( this->tempBuffer, this->tempBuffer, readSymbolCount + 1, lex );

				lex.type		= LT_STRING;
				lex.name		= this->tempBuffer;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;

				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}

			// ��������: ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->tempBuffer[ readSymbolCount ] = 0;
				this->addError( SE_SCANER_FATAL_ERROR, SSRV_NEWLINE_IN_STRING_CONSTANT, this->tempBuffer );
				
				this->fInfo.filePos ++;
				this->fInfo.line	++;
				this->fInfo.col		= 0;

				this->state			= SSS_NONE;
				
				return SSRV_NEWLINE_IN_STRING_CONSTANT;
			}
		
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;	
			this->fInfo.col		++;

			break; // SSS_STRING

		case SSS_CHAR_CONST:
			
			// ��������: ����� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				
				this->state							= SSS_NONE;
				this->tempBuffer[ readSymbolCount ] = 0;

				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_EOF_IN_CHAR_CONSTANT, this->tempBuffer ).code;	

			}

			// ��������: ����� ���������� ���������
			if( this->fInfo.fp[ this->fInfo.filePos ] == '\'' && !this->isEscapeSequence( this->tempBuffer, readSymbolCount ) )
			{
			
				this->tempBuffer[ readSymbolCount ] = 0;
				this->formatString( this->tempBuffer, this->tempBuffer, readSymbolCount + 1, lex );
				
				lex.type		= LT_CONST;
				lex.constType	= LCT_SYMBOL;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;

				this->state			= SSS_NONE;
				
				if( readSymbolCount == 0 )
				{
					// �� ���� ��������� ������ �������
					lex.type				= LT_NONE;
					
					return this->addError( SE_SCANER_ERROR, SSRV_EMPTY_CHAR_CONSTANT, TEXT( "''" ), lex ).code;
				}
				else if( readSymbolCount < 16 )
				{
					_stprintf_s( symbol, ( const TCHAR* ) this->tempBuffer );
					lex.byteValue	= (BYTE)(symbol[0]);	// ����� ������ ������ ������	
				}
				
				if( readSymbolCount >= 16 || _tcslen( symbol ) > 1 )
				{
					lex.name		= this->tempBuffer;
					
					this->tempBuffer[ readSymbolCount ] = 0;
					return this->addError( SE_SCANER_ERROR, SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT, this->tempBuffer ).code;
				}

				return SRV_SUCCESS;
			}

			// ��������: ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				
				this->tempBuffer[ readSymbolCount ] = 0;
				this->addError( SE_SCANER_FATAL_ERROR, SSRV_NEWLINE_IN_CHAR_CONSTANT, this->tempBuffer );
								
				this->fInfo.filePos ++;
				this->fInfo.line	++;
				this->fInfo.col		= 0;

				this->state			= SSS_NONE;
				
				return SSRV_NEWLINE_IN_CHAR_CONSTANT;
			}
		
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;	
			this->fInfo.col		++;
			
			break; // SSS_CHAR_CONST
			
		// ���� ��� ����� ������������ ���������
		case SSS_NUMERIC_CONST:
			
			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// �������� �� ����� �����
			// ��� ����� ������
			// ��� ����������� �� �� �����
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				( scc == SCC_PARSER && this->fInfo.fp[ this->fInfo.filePos ] != '.' ) )
			{
				
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_CONST;
				
				if( this->tempBuffer[ 1 ] == 'x' || this->tempBuffer[ 1 ] == 'X' )
					lex.constType	= LCT_HEX;
				else if( this->tempBuffer[ 1 ] == 'o' || this->tempBuffer[ 1 ] == 'O' )
					lex.constType	= LCT_OCTAL;
				else if( this->tempBuffer[ 1 ] == 'b' || this->tempBuffer[ 1 ] == 'B' )
					lex.constType	= LCT_BINARY;
				else
					lex.constType   = LCT_DECIMAL;

				lex.dwordValue	= this->toDword( this->tempBuffer, readSymbolCount );

				this->state		= SSS_NONE;
				return SRV_SUCCESS;
			}
			
			// ���� ��� ����������� ������� ���������
						
			// ����������� �����
			// �������� ��������, ������������ ��� 16 ������ �����
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'b' || this->fInfo.fp[ this->fInfo.filePos ] == 'B' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'o' || this->fInfo.fp[ this->fInfo.filePos ] == 'O' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'x' || this->fInfo.fp[ this->fInfo.filePos ] == 'X' )
			{
				
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				// ���� ��� ���� ����� ���� - �� ���������
				// ��� ���� b � ��� ����������������� �����
				if( readSymbolCount == 2 && this->tempBuffer[0] == '0' || 
					( ( this->fInfo.fp[ this->fInfo.filePos - 1 ] == 'b' || this->fInfo.fp[ this->fInfo.filePos - 1 ] == 'B' ) && ( this->tempBuffer[1] == 'x' || this->tempBuffer[1] == 'X' ) ))
					continue;
				else // ����� ������
				{
					this->tempBuffer[ readSymbolCount ] = 0;
					
					// ������� b o x ������ ���� ����� ���� � ����� �����
					// ������� ������� ������
					if( this->tempBuffer[1] == 'b' || this->tempBuffer[1] == 'B' )
						this->addError( SE_SCANER_ERROR, SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_BINARY_FORMAT, this->tempBuffer, lex );
					else if( this->tempBuffer[1] == 'o' || this->tempBuffer[1] == 'O' )
						this->addError( SE_SCANER_ERROR, SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_OCTAL_FORMAT, this->tempBuffer, lex );
					else if( this->tempBuffer[1] == 'x' || this->tempBuffer[1] == 'X' )
						this->addError( SE_SCANER_ERROR, SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT, this->tempBuffer, lex );			
					
					this->state		= SSS_ERROR;
					continue;
				}
			}

			// ����� ������������������ �����
			if( ( this->fInfo.fp[ this->fInfo.filePos ] >= 'a' && this->fInfo.fp[ this->fInfo.filePos ] <= 'f' ) || 
				( this->fInfo.fp[ this->fInfo.filePos ] >= 'A' && this->fInfo.fp[ this->fInfo.filePos ] <= 'F' ) )
			{
			
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				// ���� ��� ���� ����� ���� - �� ��������� 
				if( this->tempBuffer[1] == 'x' || this->tempBuffer[1] == 'X' )
					continue;
				else // ����� ������
				{
					// ��� ���� ������������ ������ ������������������ �����, ���� ������ �������
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT, this->tempBuffer, lex );
										
					this->state				= SSS_ERROR;
					continue;
				}

			}
			
			// ��������: ����� ��� �����
			if( scc == SCC_DIGIT )
			{
				
				// ���������� ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
				
				// ���� ��� �������� ����� 
				// �� ������ ���� ������ 0 � 1
				if( readSymbolCount >= 2 && ( this->tempBuffer[1] == 'b' || this->tempBuffer[1] == 'B' ) && this->tempBuffer[ readSymbolCount - 1 ] > '1' )
				{
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_BINARY_FORMAT, this->tempBuffer, lex );
				
					this->state			= SSS_ERROR;
					continue;
				}
				
				// ���� ������������ ����� �� 0 .. 7
				if( readSymbolCount >= 2 && ( this->tempBuffer[1] == 'o' || this->tempBuffer[1] == 'O' ) && this->tempBuffer[ readSymbolCount - 1 ] > '7' )
				{											
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_OCTAL_FORMAT, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;
				}
			
				// ���� ����� ���� ������� - ��������
				if( readSymbolCount >= 2 && \
					( this->tempBuffer[ readSymbolCount - 2 ] == 'L' || 
						this->tempBuffer[ readSymbolCount - 2 ] == 'l' || 
						this->tempBuffer[ readSymbolCount - 2 ] == 'U' || 
						this->tempBuffer[ readSymbolCount - 2 ] == 'u' ) )
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_INVALID_SUFFIX_POSITION, this->tempBuffer, lex );
					
					this->state			= SSS_ERROR;
					continue;
				}

				continue;
			}
			
			// ��������: ���������� �����
			if( this->fInfo.fp[ this->fInfo.filePos ] == '.' )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;		
			
				this->state				= SSS_FLOAT_CONST;
				
				continue ;
			}

			// ��������: ������� ����������� L l U u UL ul
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'L' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'l' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'U' ||
				this->fInfo.fp[ this->fInfo.filePos ] == 'u' )

			{
				
				// ���������� ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				// ���� ����� ������ u ����� ��� ���� �������
				if( ( this->tempBuffer[ readSymbolCount - 1 ] == 'U' || this->tempBuffer[ readSymbolCount - 1 ] == 'u' ) && 
					( this->tempBuffer[ readSymbolCount - 2 ] == 'L' || this->tempBuffer[ readSymbolCount - 2 ] == 'l' || this->tempBuffer[ readSymbolCount - 2 ] == 'U' || this->tempBuffer[ readSymbolCount - 2 ] == 'u' ) )
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_INVALID_U_SUFFIX_POSITION, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;	
				}

				// ���� ����� ������ l �� ����� ����� ��� ����� U ��� u - ������
				if( ( this->tempBuffer[ readSymbolCount - 1 ] == 'L' || this->tempBuffer[ readSymbolCount - 1 ] == 'l' ) && 
					( this->tempBuffer[ readSymbolCount - 2 ] == 'L' || this->tempBuffer[ readSymbolCount - 2 ] == 'l' ) 
					)
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_INVALID_L_SUFFIX_POSITION, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;	
				}

				// ���� ����� ����� u ��� U - �� �����������
				if( this->tempBuffer[ readSymbolCount - 1 ] == 'U' || this->tempBuffer[ readSymbolCount ] == 'u' )
					lex.isUnsigned = true;	
								
				continue;
			}

			// ��� ��������� - ������ 
			// ���������� ��������� ������
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;							
			
			this->tempBuffer[ readSymbolCount ] = 0;
			this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_NUMBER_FORMAT, this->tempBuffer, lex );
						
			this->state			= SSS_ERROR;
			continue;	

			break; // SSS_NUMERIC_CONSTANT

		case SSS_FLOAT_CONST:
			
			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// ��������: ����� �����
			// ����� ������ 
			// �����������
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_CONST;
				
				lex.constType	= LCT_DOUBLE;
				lex.doubleValue	= _tcstod( this->tempBuffer, &endptr );
				
				this->state		= SSS_NONE;
				return SRV_SUCCESS;
			}

			// ��������: ������� ������ e E - ����������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'E' || this->fInfo.fp[ this->fInfo.filePos ] == 'e' )
			{
				// ���������� ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							

				this->state			= SSS_FLOAT_CONST2;
				
				continue;
			}

			// ���� ��� ����� f ��� F
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'f' || this->fInfo.fp[ this->fInfo.filePos ] == 'F' )
			{
			
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				this->state			= SSS_FLOAT_CONST4; 

				continue;

			}

			// ��������: �����
			if( scc == SCC_DIGIT )
			{
				
				// ���������� ��������� ������
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				continue;
			}

			// ��� ��������� - ������ 
			// ���������� ��������� ������
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;							
			
			this->tempBuffer[ readSymbolCount ] = 0;
			this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX, this->tempBuffer, lex );
						
			this->state			= SSS_ERROR;
			continue;	

			break; // SSS_FOAT_CONST

		case SSS_FLOAT_CONST2:
			
			// � ���� ��������� �� ��������� ���� ������ ��� ����� ��� ������ �����
			
			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// ��������: ����� �����
			// ����� ������
			// �����������
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				
				this->tempBuffer[ readSymbolCount ] = 0;
				this->addError( SE_SCANER_ERROR, SSRV_EXPECTED_EXPONENT_VALUE, this->tempBuffer, lex );
							
				this->state			= SSS_ERROR;
				continue;	
			}

			// ��������: ������� �����������
			// �������� �����
			if( this->fInfo.fp[ this->fInfo.filePos ] == '+' || 
				this->fInfo.fp[ this->fInfo.filePos ] == '-' ||
				scc == SCC_DIGIT )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
							
				this->state			= SSS_FLOAT_CONST3;
				continue;	

			}

			// ��� ��������� ������
			// ��������� ������
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;							
			
			this->tempBuffer[ readSymbolCount ] = 0;
			this->addError( SE_SCANER_ERROR, SSRV_EXPECTED_EXPONENT_VALUE, this->tempBuffer, lex );
									
			lex.type				= LT_NONE;
			this->state				= SSS_ERROR;
			
			continue;	

			break; // SSS_FLOAT_CONST2

		case SSS_FLOAT_CONST3:

			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// ��������: ����� �����
			// ����� ������
			// �����������
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
				
				// ���� ���������� �� ����� - ������
				if( symbolClass [ this->tempBuffer[ readSymbolCount - 1 ] ] != ( BYTE )SCC_DIGIT )
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_EXPECTED_EXPONENT_VALUE, this->tempBuffer, lex );
									
					lex.type				= LT_NONE;
					this->state				= SSS_ERROR;
					continue;

				}
				else
				{
					// ��� ���������
					// ��������� ������
					this->tempBuffer[ readSymbolCount ] = 0;
				
					lex.type		= LT_CONST;
				
					lex.constType	= LCT_DOUBLE;
					lex.doubleValue	= _tcstod( this->tempBuffer, &endptr );
				
					this->state		= SSS_NONE;
					return SRV_SUCCESS;

				}

			}

			// ��������: �����
			if( scc == SCC_DIGIT )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				continue;					
			}
			
			// ���� ��� ����� f ��� F
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'f' ||
				this->fInfo.fp[ this->fInfo.filePos ] == 'F' )
			{
			
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				this->state			= SSS_FLOAT_CONST4; 

				continue;

			}

			break; // SSS_FLOAT_CONST3

		case SSS_FLOAT_CONST4:
			
			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// ��������: ����� �����
			// ����� ������
			// �����������
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
			
				// ��������� ������
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_CONST;
				
				lex.constType	= LCT_FLOAT;
				lex.floatValue	= ( float ) _tcstod( this->tempBuffer, &endptr );
				
				this->state		= SSS_NONE;
				return SRV_SUCCESS;	

			}
			
			// ��� ��������� ������
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;							

			this->tempBuffer[ readSymbolCount ] = 0;
			this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX, this->tempBuffer, lex );
						
			this->state			= SSS_ERROR;
			break; // SSS_FLOAT_CONST4
		
		case SSS_ERROR:
			
			// �������� ����� �������
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// ��������: ����� �����
			// ����� ������
			// �����������
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
			
				readSymbolCount = 0;
				this->state		= SSS_NONE;
				continue;

			}

			// ���������� ��������� ������
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;	
			
			break; // SSS_ERROR

		} // switch( this->state )
	} // while (1)
	
	return SRV_SUCCESS;
}

bool scriptScaner::isFileOpened( shString &fileName )
{

	shString		str1 = fileName;
	shString		str2;
	scriptScaner	*scaner = this->prevScaner;

	
	str1.toLowerCase();

	while( scaner!= NULL )
	{
		str2 = scaner->fInfo.fullPathName;
		str2.toLowerCase();

		if( str1 == str2 )
			return true;
	
		scaner = scaner->prevScaner;
	}

	return false;
}

bool scriptScaner::idExist( shString &name, int &index )
{
	
	std::list< scriptLexeme* >::iterator	sli;

	for( index=0, sli = this->parent->ids.begin(); sli != this->parent->ids.end(); sli++, index++ )
	{
		if( (*sli)->name == name )
			return true;
	}
	
	return false;
}

bool scriptScaner::stringExist( shString &name, int &index )
{
	
	std::list< scriptLexeme* >::iterator	sli;

	for( index=0, sli = this->parent->constStrings.begin(); sli != this->parent->constStrings.end(); sli++, index++ )
	{
		if( (*sli)->name == name )
			return true;
	}
	
	return false;
}

scriptMacros* scriptScaner::macroExist( shString &name )
{

	std::list< scriptMacros* >::iterator	smi;
	
	for( smi = this->parent->macros.begin(); smi != this->parent->macros.end(); smi++ )
	{
		if( name == (*smi)->name )
			return *smi;
	}

	return NULL;
}

DWORD scriptScaner::toDword( TCHAR *szString, DWORD dwLength )
{

	DWORD	dwVal, index;
	TCHAR	*endptr;

	if( szString == NULL )
		return 0;

	// ��������: �������� �����
	if( dwLength >= 3 && szString[1] == 'b' || szString[1] == 'B' )
	{
		dwVal = 0;
		for( index = dwLength - 1; index > 1; index-- )
		{
			if( szString[ index ] == '1' )
				dwVal |= 1 << ( dwLength - 1 - index );
					
		}
	}
	else if( dwLength >= 3 && szString[1] == 'o' || szString[1] == 'O' ) 
		dwVal = _tcstoul( szString, &endptr, 8 );
	
	else if( dwLength >= 3 && szString[1] == 'x' || szString[1] == 'X' )
		dwVal = _tcstoul( szString, &endptr, 16 );
	
	else
		dwVal = _tcstol( szString, &endptr, 10 );
	
	return dwVal;

}

scriptRetCode scriptScaner::scan()
{
	
	scriptRetCode							retCode;
	scriptLexeme							lex, *lex2;
	
	std::list< scriptLexeme* >::iterator	sli; 
	scriptMacros								*scrMacro;
	shString								name;

	// �������� �� ���������� ������� �� ����� �����
	this->skipLine		= false;
	
	// �������� ����� ��������� ����
	this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );

	// �������� ��� �����
	_tprintf( TEXT("directory = '%s'   file = '%s'   fullPathname = '%s';\n"), this->fInfo.directory.getCString(), this->fInfo.fileName.getCString(), this->fInfo.fullPathName.getCString() );
	
	// ��������� �������� ������� � ����� ���������� �� � �������
	while( 1 )
	{
		// ���� ���� ���������� ������������
		if( this->skipScanning.top() != SST_DO_NOT_SKIP_SCANNING )
		{
			this->skipUntilDirective();
			continue;
		}

		lex.clear();							// ������� �������
		retCode = this->getNextLexeme( lex );	// �������� ��������� �������

		// ���� ��� ������ ����� ������
		if( retCode == SSRV_END_OF_LINE )
			continue;
				
		// ���� ��������� ����� ���������
		if( retCode == SSRV_END_OF_FILE )
			return SRV_SUCCESS;
			
		// ��������� � ������ �� �� ���������� �������
		if( skipLine && lex.line == skipLineIndex )
			continue;
		else 
			skipLine = false;

		// ���� �� ���������� - �� ��������� ������� � ������ ������
		// ��������� ������� � ������ ������
		if( lex.type != LT_NONE )
		{
			
			// ��������� �� ������� �������
			if( lex.type == LT_ID || lex.type == LT_RWORD )
			{
				// ���� ��� ������
				if( ( scrMacro = this->macroExist( lex.getLexemeName() ) ) != NULL )
				{
					// ������� ������� � �������� ������
					// ������ �������
					clearLexList( this->inputMacrosList );
					clearLexList( this->outputMacrosList );

					// ��������� ������� ��� ���������� �������
					this->scanMacros( lex, *scrMacro );
					
					// ��������� �� ���������� ������ ������
					this->parseMacros( lex, *scrMacro );
					
					// ��������� ������
					this->executeMacros( lex, *scrMacro, this->parent->lexemes );
					
					// ���������� 
					continue;
				}

			}
			
			// ����� ��������� � ������ ...
			lex2 = new scriptLexeme( lex );
			lex2->scaner = this;

			// ��������� �������������
			if( lex2->type == LT_PARSER && lex2->parser == LPT_PREPROC_DIRECTIVE && !this->skipPreprocDirectives)
			{
				
				// �������� ���������� �������
				sli = this->parent->lexemes.end();
				sli--;
				
				// ���� ������ ��������� ����������� �� ������ � ������
				if( this->parent->lexemes.size() != 0 && (*sli)->line == lex2->line )
				{
					// ���������� ������ ������ �� ���� � ���������� ��� ���������
					this->addError( SE_SCANER_ERROR, SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST, *lex2 );
										
					// �� ������ ���������� ��� ������� �� ����� ������
					skipLine				= true;
					skipLineIndex			= lex2->line;
					
					// ������� ������� 
					delete lex2;
					
					continue;
				}
				else // ������ ��������� ����������� ������ � ������
				{
														
					// ������� ������ ������ ���������
					clearLexList( this->directiveLexemes );
					
					this->directiveLexemes.push_back( lex2 );
					
					retCode = this->scanDirective();
					if( retCode == SRV_SUCCESS ) // ����������� ���������
						retCode = this->parseDirective();
										
					if( retCode != SRV_SUCCESS )
					{
						// �������� ��� ���������
						sli = this->directiveLexemes.begin();
						sli ++;
						
						if( (*sli)->type == LT_RWORD || (*sli)->type == LT_ID )
							this->addError( SE_PREPROCESSOR_ERROR, SPPRV_CANT_EXECUTE_DIRECTIVE, (*sli)->getLexemeName().getCString(), *(this->directiveLexemes.front()) );
						else
							this->addError( SE_PREPROCESSOR_ERROR, SPPRV_CANT_EXECUTE_DIRECTIVE, *(this->directiveLexemes.front()) );
					
						continue;
					}
					
					// ���������� ����������
					continue;
				}
			}
			
			this->parent->lexemes.push_back( lex2 );
		}

	}
	
	if( this->prevScaner )
		_tprintf( TEXT("directory = '%s'   file = '%s'   fullPathname = '%s';\n"), this->prevScaner->fInfo.directory.getCString(), this->prevScaner->fInfo.fileName.getCString(), this->prevScaner->fInfo.fullPathName.getCString() );
				
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::scanDirective()
{
	
	scriptRetCode							retCode;
	scriptLexeme							lex, *lex2;
		
	std::list< scriptLexeme* >::iterator	sli; 
			
	// �������� �� ���������� �������
	skipLine								= false;

	// ��������� �������� ������� � ����� ���������� �� � �������
	while( 1 )
	{
		lex.clear();							// ������� �������
		retCode = this->getNextLexeme( lex );	// �������� ��������� �������

		// ���� ������� ������� error
		if( lex.type == LT_RWORD && lex.rwordType == LRW_ERROR )
		{
			
			// ��������� ������� � 
			lex2 = new scriptLexeme(lex);
			lex2->scaner = this;
			this->directiveLexemes.push_back( lex2 );
			
			return SRV_SUCCESS;
		}
		
		// ���� ��� ������ ����� ������
		if( retCode == SSRV_END_OF_LINE || retCode == SSRV_END_OF_FILE )
		{
		
			// ������� - ���� ���������� ������� - �������� ���� - 
			// �� ��� ���� ����������� ������� � ���� ���������� ���������
				
			sli = this->directiveLexemes.end();
			sli--;
				
			if( this->directiveLexemes.size() != 0 && (*sli)->type == LT_PARSER && (*sli)->parser == LPT_BACKSLASH )
			{
				// ���� ������� ���������� ������� � ���������� ����������
				delete *sli;
				this->directiveLexemes.erase( sli );
				continue;
			}
			else
				return SRV_SUCCESS;
		}
			
		if( lex.type != LT_NONE )
		{
			lex2 = new scriptLexeme( lex );
			lex2->scaner = this;
			this->directiveLexemes.push_back( lex2 );
		}
	}

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::scanMacros( scriptLexeme &lex, scriptMacros &scrMacro )
{

	std::list< macrosInputParam >::iterator	mipi;
	std::list< scriptLexeme* >::iterator	sli;

	scriptLexeme		lex2, *lex3;
	scriptRetCode		retCode;
	int					macroScanState	= 0,
						bracketCount	= 0;

	bool				bMustScan = true;

	// ���� ��� ���������� - �� � ��������
	if( scrMacro.params.size() == 0 )
		return SRV_SUCCESS;
		
	// ���� ���� ��������� ���� �� �������
	// ��������� � ������
	while( bMustScan )
	{
		lex2.clear();							// ������� �������
		retCode = this->getNextLexeme( lex2 );	// �������� ��������� �������

		// ���� ��� ������ ����� ������
		if( retCode == SSRV_END_OF_LINE )
			continue;

		// ���� ��������� ����� ��������� 
		// �� �������������� �� ��� ����������������
		if( retCode == SSRV_END_OF_FILE )
		{
			// ������� ������ ������� ����
			scrMacro.inputList.begin	= this->inputMacrosList.begin();
			scrMacro.inputList.end		= this->inputMacrosList.end(); 
			scrMacro.inputList.size		= (int) this->inputMacrosList.size();
						
			return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EOF_IN_MACRO_PARAMS, lex ).code;
		}
		
		// �������, � ����� ��������� ���������
		if( macroScanState == 0 )
		{
			// ��������� (
			// ��������� �������
			if( lex2.type != LT_PARSER || lex2.parser != LPT_LEFT_BRACKET )
			{
				// �������� ������� ��������� ��� ������� ���� ����� ����������
				this->fInfo.filePos = lex.filePos + lex.name.getLength();
				this->fInfo.line	= lex.line;
				this->fInfo.col		= lex.col;
								
				// ��� ������ �������� ������
				scrMacro.inputList.begin	= this->inputMacrosList.begin();
				scrMacro.inputList.end		= this->inputMacrosList.end(); 
				scrMacro.inputList.size		= (int) this->inputMacrosList.size();

				// ������ ������, � ���, ��� ������ ���� ���� ������
				return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO, lex2 ).code;
			}
			else
			{
				macroScanState = 1;
				bracketCount   = 1;
			}
		}
		else // ��������� ��������� �������
		{
			
			// ���� ��� ����������� ������
			if( lex2.type == LT_PARSER && lex2.parser == LPT_LEFT_BRACKET )
				bracketCount ++;
			
			// ���� ��� ����������� ������
			else if( lex2.type == LT_PARSER && lex2.parser == LPT_RIGHT_BRACKET )
			{
				// ���� ��� ����������� ������ ������
				if( bracketCount == 1 )
					bMustScan = false;	// ���������� ������������
				else
					bracketCount --;	// ��������� ����� �������� ������
			}
		}
	
		// ��������� ������� � ������
		lex3 = new scriptLexeme( lex2 );
		lex3->scaner = this;
		this->inputMacrosList.push_back( lex3 );
	}
			
	// ������ �������������� �����
	scrMacro.inputList.begin	= this->inputMacrosList.begin();
	scrMacro.inputList.end		= this->inputMacrosList.end();
	scrMacro.inputList.size		= (int) this->inputMacrosList.size();

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::scanMacros( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme *> &lexList, scriptMacros& scrMacro )
{
	
	std::list< scriptLexeme *>::iterator	sli2;
	int							bracketCount = 0;

	// ���� ������ ��� ���������� - �� � �������
	if( scrMacro.params.size() == 0 )
		return SRV_SUCCESS;

	// ��������� �� ����� ������� � ������ ���������� ���� ��� ����
	sli2 = sli;
	sli ++;
		
	// ��������� ������� ������ ���� ����������� ����������� �������
	if( sli == lexList.end() || (*sli)->type != LT_PARSER || (*sli)->parser != LPT_LEFT_BRACKET )
	{
		// �������� � ����� �����
		sli --;

		// ��� ������� ������ � �������
		scrMacro.inputList.begin = sli;
		scrMacro.inputList.end	 = sli;
		scrMacro.inputList.size	 = 0;
		
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO, *(*sli) ).code;
	}
	
	// ���������� ������ ����������
	scrMacro.inputList.begin = sli;
	// ������
	scrMacro.inputList.size  = 1;

	// ����� ����������� ������
	bracketCount = 1;
	
	// ������ ������ ����� ����������� ������
	for( sli++; sli != this->inputMacrosList.end(); sli ++ )
	{
		if( sli == lexList.end() )
		{
			// ����� ����������
			scrMacro.inputList.end = sli;
			return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_RIGHT_BRACKET_IN_MACRO, *(*sli2) ).code;
		}
		
		if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_LEFT_BRACKET )
			bracketCount ++;
		else if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_RIGHT_BRACKET )
			bracketCount --;
		
		// ����� ������
		scrMacro.inputList.size ++;
		
		if( bracketCount == 0 )
			break;
	}
	
	// ����� ����������
	scrMacro.inputList.end = sli;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseMacros( scriptLexeme &lex, scriptMacros &scrMacro )
{

	std::list< scriptLexeme* >::iterator	sli;
	std::list< macrosInputParam >::iterator mipi;
	macrosInputParam						mip;
	int		bracketCount = 0;	// ����� �������� ������
	
	// ������� ��������� ������ �� ����������
	
	// ������� ������ ������� �������� ����������
	scrMacro.input.clear();

	// ������ ������� ���������
	mip.begin = scrMacro.inputList.begin;
	
	// ���� �� ���� �������� �� ������� ������
	for( sli = scrMacro.inputList.begin; sli != scrMacro.inputList.end ; sli++ )
	{
		// ���� �� �����������
		if( (*sli)->type != LT_PARSER )
			continue;
		
		// (
		if( (*sli)->parser == LPT_LEFT_BRACKET )
			bracketCount ++;
		// )
		else if( (*sli)->parser == LPT_RIGHT_BRACKET )
			bracketCount --;		
		
		// , ��� ��������� ������
		if( ( (*sli)->parser == LPT_COMMA && bracketCount == 1 ) || bracketCount == 0 )
		{
			// ����� ���������
			mip.end = sli;
			// ������������ ������
			mip.begin ++;
			// ��������� � ������
			scrMacro.input.push_back( mip );
		
			// �������������� ������ ���� ��� ���� ����� ��� ���������
			if( bracketCount != 0 )
				mip.begin = sli;
			else // ����� ��������� ����������
				break;
		}
	}

	// ����� ���� ��� ��������� �� ��������� �����������
	// ������� ����� ������ ��� ������� ���������
	// ������������ ����� ������ � ������ ���������		
	for( mipi = scrMacro.input.begin(); mipi != scrMacro.input.end(); mipi ++ )
	{
		mipi->size = 0;	
		for( sli = mipi->begin; sli != mipi->end; sli ++ )
			mipi->size ++;
	}
	
	// ������ ��������� ����� ����������
	if( scrMacro.input.size() != scrMacro.params.size() )
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_NOT_ENOUGH_ACTUAL_PARAMS_FOR_MACRO, lex ).code;
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeMacros( scriptLexeme &lex, scriptMacros &scrMacro, std::list< scriptLexeme* >	&lexList )
{

	std::list< scriptLexeme* >::iterator	sli, sli2, sli3;
	scriptLexeme							*lex2;
	std::list< macrosInputParam >::iterator	mipi;
	
	scriptMacros								*scrMacro2;

	if( scrMacro.called )
		this->addError( SE_SCANER_ERROR, SPPRV_MACRO_RECURSION, lex ).code;
		
	// ������������� �����
	scrMacro.called = true;

	// ����������� �����
			
	// ���������� �� ���� �������
	for( sli2 = scrMacro.body.begin(); sli2 != scrMacro.body.end(); sli2++ )
	{
						
		// ������� ��������� �� ������� ���������
		if( scrMacro.isParam( *(*sli2), mipi ) )
		{
			if( mipi == scrMacro.input.end() )
				continue;
			
			for( sli = mipi->begin; sli != mipi->end; sli ++ )
			{
								
				scrMacro2 = NULL;
				if( (*sli)->type == LT_ID || (*sli)->type == LT_RWORD )
					scrMacro2 = this->macroExist( (*sli)->getLexemeName() );
							
				if( scrMacro2 != NULL )
				{
					// ���� ��� �� � ��������� ���� ������
					// ���������� ������
					sli3 = sli;

					// ������������ �����
					// ��������� ��� ���������� ���������
					this->scanMacros( sli3, this->inputMacrosList, *scrMacro2 );
					// ����������� ��������� ���� ����
					this->parseMacros( *(*sli), *scrMacro2 );
					// ��������� ������ ���� ����
					this->executeMacros( *(*sli), *scrMacro2, this->parent->lexemes );
					// ������������� �� ������� ��������� �� ��������
					sli = sli3;
					
					// ���� �������� �����
					if( sli == mipi->end )
						break;
					else
						continue; // ���������� ���������
				}
				else
				{
					lex2 = new scriptLexeme( *( *sli ) );
					lex2->scaner = this;
					lexList.push_back( lex2 );
				}
			}
			
			continue;
		}
		
		// �������� �������� ��� �������
		scrMacro2 = NULL;
		if( (*sli2)->type == LT_ID || (*sli2)->type == LT_RWORD )
			scrMacro2 = this->macroExist( (*sli2)->getLexemeName() );

		// ���� ��� ��� �������
		if( scrMacro2 != NULL )
		{
			// ���������� ������ �������
			sli3 = sli2;
						
			// ������������ �����		
			// ��������� ��� ���������� ���������
			this->scanMacros( sli3, scrMacro.body, *scrMacro2 );
			// ����������� ��������� ���� ����
			this->parseMacros( *(*sli2), *scrMacro2 );
			// ��������� ������ ���� ����
			this->executeMacros( *(*sli2), *scrMacro2, this->parent->lexemes );
			// ������������� �� ������� ��������� �� ��������
			sli2 = sli3;
			
			// ���������� ���������
			continue;
		}
		else 
		{
			// ������ �������� �������
			lex2 = new scriptLexeme ( *( *sli2 ) );
			lex2->scaner = this;
			lexList.push_back( lex2 );
		}	
	}
		
	// ���������� �����
	scrMacro.called = false;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::skipUntilDirective()
{

	// �� ������ ���������� ��� ������� �� #else ��� �� #elsif ��� �� endif
	scriptScanerState	skipState = SSS_FIND_DIRECTIVE_SYMBOL;
	symbolClassCode		scc;
	scriptLexeme		lex, lex2, *lex3;
	bool				firstSymbolRead;
	int					byteReadCount;

	firstSymbolRead = false;
	
	while( 1 )
	{
	
		switch( skipState )
		{
		case SSS_FIND_DIRECTIVE_SYMBOL:
			
			// ������� ��������� �� ��������� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;

			// �������� ��� �������
			scc = (symbolClassCode) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// ���� ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x20 )
			{
				this->fInfo.col ++;
				this->fInfo.filePos ++;
				continue;
			}

			// ���� ������� 0x0a
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
			{
				this->fInfo.col = 0;
				this->fInfo.filePos ++;
				continue;
			}

			// ���� ������� ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line ++;
				this->fInfo.filePos ++;
				firstSymbolRead = false;
				continue;
			}

			// ������ ���������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 )
			{
				this->fInfo.col		+= 4;
				this->fInfo.col		&= 0xfffffffc;
				this->fInfo.filePos	++;
				continue;
			}

			// ���� ������ ������ #
			if( this->fInfo.fp[ this->fInfo.filePos ] == '#' )
			{
				// ���� ������ �� ������ � ������
				if( firstSymbolRead == true )
				{
					// ������ ���������� ���
					this->fInfo.filePos ++;
					this->fInfo.col		++;
					
					continue;
				}
				else 
				{
					lex.type		= LT_PARSER;
					lex.parser		= LPT_PREPROC_DIRECTIVE;
					lex.line		= this->fInfo.line;
					lex.col			= this->fInfo.col;

					this->fInfo.filePos ++;
					this->fInfo.col		++;
					
					skipState = SSS_WAIT_FOR_ENDIF_ANALOG;
					continue;
				}
			}

			// ���� ������ �� ����� �� ������ ���������� ������
			firstSymbolRead = true;
			this->fInfo.filePos ++;
			this->fInfo.col		++;

			continue;
			
			break;
		
		case SSS_WAIT_FOR_ENDIF_ANALOG:
						
			// ������� ��������� �� ��������� �����
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;

			// �������� ��� �������
			scc = (symbolClassCode) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// ���� ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x20 )
			{
				this->fInfo.col ++;
				this->fInfo.filePos ++;
				continue;
			}

			// ���� ������� ������� ������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line ++;
				this->fInfo.filePos ++;
				firstSymbolRead = false;
				
				skipState = SSS_FIND_DIRECTIVE_SYMBOL;
				
				continue;
			}

			// ������ ���������
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 )
			{
				this->fInfo.col		+= 4;
				this->fInfo.col		&= 0xfffffffc;
				this->fInfo.filePos	++;
			}
			
			// ���� ����� �����
			if( scc == SCC_LETTER )
			{
				lex2.line			= this->fInfo.line;
				lex2.col			= this->fInfo.col;
				
				this->tempBuffer[0] = this->fInfo.fp[ this->fInfo.filePos ];			
				byteReadCount		= 1;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				skipState = SSS_SCAN_ENDIF_ANALOG;
				
				continue;
			}
			
			// � ��������� ������� ��������� � ������
			this->fInfo.filePos ++;
			this->fInfo.col		++;
			firstSymbolRead		= false;
			skipState = SSS_FIND_DIRECTIVE_SYMBOL;

			break;
			
		case SSS_SCAN_ENDIF_ANALOG:
			
			// �������� �� ��������� �����
			// ������� ��������� �� ��������� �����
			// ��� �� ������� �����������
			if( this->fInfo.filePos == this->fInfo.fileLength ||
				( scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ] ) == SCC_PARSER )
			{
			
				// ���������� ���������� ��� ��� �� �������������
				this->tempBuffer[ byteReadCount ] = 0;
				
				// �������������� ��� �������
				lex2.name	= this->tempBuffer;
			
				// �������� ������ ����������������� �����
				if( lex2.convertIfRWord() )
				{
					// ���� ��� ����������������� ����� 
					// endif - �� ���������� ��� � ����� ������
					// else elif
					if( lex2.rwordType == LRW_ENDIF || lex2.rwordType == LRW_ELSE || lex2.rwordType == LRW_ELIF )
					{
						// �� ���� ������������� ��������� � ���� ��� ������ ��������� ��
						
						// ������� ������� ������ ��������
						clearLexList( this->directiveLexemes );

						// ��������� ���� ������ �������
						lex3 = new scriptLexeme( lex );
						lex3->scaner = this;
						this->directiveLexemes.push_back( lex3 );
																	
						// ��������� ���� ������ �������
						lex3 = new scriptLexeme( lex2 );
						lex3->scaner = this;
						this->directiveLexemes.push_back( lex3 );
					
						// ����������� ���������
						this->scanDirective();

						// ���������� ���������
						this->parseDirective();

						// ������������ 
						return SRV_SUCCESS;
					}
				}
			
				// �� ���� ��������� ������� ���������� �������
				skipState = SSS_FIND_DIRECTIVE_SYMBOL;
				continue;
			}

			// � ��������� ������� ���������� ��������� �������
			this->tempBuffer[ byteReadCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			byteReadCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;

			break;
		}
	}
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::optimizeParsers( std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli1;
	std::list< scriptLexeme* >::iterator	sli2;
	std::list< scriptLexeme* >::iterator	sli3;
		
	scriptLexeme							*lex1;
	scriptLexeme							*lex2;
	scriptLexeme							*lex3;
	
	bool									optimize;
	
	// ��������������� ����������� � ����� ������� ����������� 
	for( sli1 = lexList.begin(); sli1 != lexList.end(); sli1 ++ )
	{
		if( (*sli1)->type != LT_PARSER )
			continue;		
		
		// �������� �������
		lex1		= lex2 = lex3 = NULL;
		optimize	= true;
				
		// �������� ������ 
		lex1 = *sli1;
		
		sli2 = sli1;
		sli2 ++;

		// ���� ���� ������, �������� ������
		if( sli2 != lexList.end() )
		{
			// ���� ������ ������� �� ����������� - �� ���������� �����
			if( (*sli2)->type != LT_PARSER )
				continue;
			
			lex2 = *sli2;
			sli3 = sli2;
			sli3 ++;

			// ���� ���� ������ �������� ������
			if( sli3 != lexList.end() )
				lex3 = *sli3;
		}
		else
			continue; 
				
		if( lex3 != NULL && lex3->type == LT_PARSER )
		{
			
			// �������������� ������� 
			if( lex1->parser == LPT_SUBTRACTION && lex2->parser == LPT_GREATER_THAN && lex3->parser == LPT_MULTIPLICATION )
				lex1->parser = LPT_DEREFERENCE_POINTER_TO_CLASS_MEMBER;
			else if( lex1->parser == LPT_LESS_THAN && lex2->parser == LPT_LESS_THAN && lex3->parser == LPT_ASSIGNMENT )
				lex1->parser = LPT_LEFT_SHIFT_ASSIGNMEN;
			else if( lex1->parser == LPT_GREATER_THAN && lex2->parser == LPT_GREATER_THAN && lex3->parser == LPT_ASSIGNMENT )
				lex1->parser = LPT_RIGHT_SHIFT_ASSIGNMENT;
			else if( lex1->parser == LPT_DOT && lex2->parser == LPT_DOT && lex3->parser == LPT_DOT )
				lex1->parser = LPT_DOT3;
			else
				optimize = false;

			if( optimize )
			{
				// ������� ������ �������
				delete lex2;
				lexList.erase( sli2 );

				// ������� ������ �������
				delete lex3;
				lexList.erase( sli3 );
		
				// ���������� ������
				continue;
			}	
		}
		
		optimize = true;

		// ��������� 2 �������
		// �������, ��� � ��� ����� ��������� 
		if( lex1->parser == LPT_DOT2 && lex2->parser == LPT_DOT2 && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_SCOPE_RES;
		else if( lex1->parser == LPT_SUBTRACTION && lex2->parser == LPT_GREATER_THAN && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_MEMBER_SEL_POINTER;
		else if( lex1->parser == LPT_ADDITION && lex2->parser == LPT_ADDITION && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_INC;
		else if( lex1->parser == LPT_SUBTRACTION && lex2->parser == LPT_SUBTRACTION && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_DEC;
		else if( lex1->parser == LPT_DOT && lex2->parser == LPT_MULTIPLICATION && lex2->isParsersNear( *lex1 )  )
			lex1->parser = LPT_APPLY_POINTER_TO_CLASS_MEMBER;
		else if( lex1->parser == LPT_LESS_THAN && lex2->parser == LPT_LESS_THAN && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_LEFT_SHIFT;
		else if( lex1->parser == LPT_GREATER_THAN && lex2->parser == LPT_GREATER_THAN && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_RIGHT_SHIFT;
		else if( lex1->parser == LPT_LESS_THAN && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_LESS_THAN_OR_EQUAL_TO;
		else if( lex1->parser == LPT_GREATER_THAN && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_GREATER_THAN_OR_EQUAL_TO;
		else if( lex1->parser == LPT_ASSIGNMENT && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_EQUALITY;
		else if( lex1->parser == LPT_LOGICAL_NOT && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_INEQUALITY;
		else if( lex1->parser == LPT_BITWISE_AND && lex2->parser == LPT_BITWISE_AND && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_LOGICAL_AND;
		else if( lex1->parser == LPT_BITWISE_OR && lex2->parser == LPT_BITWISE_OR && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_LOGICAL_OR;
		else if( lex1->parser == LPT_MULTIPLICATION && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_MUL_ASSIGNMENT;
		else if( lex1->parser == LPT_DIVISION && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_DIV_ASSIGNMENT;
		else if( lex1->parser == LPT_REMAINDER && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_MOD_ASSIGNMENT;
		else if( lex1->parser == LPT_ADDITION && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_ADD_ASSIGNMENT;
		else if( lex1->parser == LPT_SUBTRACTION && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_SUB_ASSIGNMENT;
		else if( lex1->parser == LPT_BITWISE_AND && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_BITWISE_AND_ASSIGNMENT;
		else if( lex1->parser == LPT_BITWISE_OR && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_BITWISE_OR_ASSIGNMENT;
		else if( lex1->parser == LPT_BITWISE_EXCLUSIVE_OR && lex2->parser == LPT_ASSIGNMENT && lex2->isParsersNear( *lex1 ) )
			lex1->parser = LPT_BITWISE_EXCLUSIVE_OR_ASSIGNMENT;
		else
			optimize = false;
		
		// ���� ����������� ����������� 2 ������������
		if( optimize )
		{
			// ������� ������ �������
			delete lex2;
			lexList.erase( sli2 );
		
			// ���������� ������
			continue;
		}
	}

	// ������ �������������� ������� ���������, ��� �� ��� ����� �� ��������
	// ��������� ���������� ������� �������� ��� ��� ����� ������� �� ������ ������������
	// �������� ������
	// -- - 123
	// ����� ���� ������� ����� 
	// ���� ������ ���� 
	// -- - 123
	// ��� �������� ����� - ���� ������ ���� 
	// id -- - 123
	// �� ������� �������� - �� ��� �������������� ��������� ��� �����
	// � �������������� ���������� ������
	// ������� ������������� �� ������ ������������ �����������
	// ��� �������� ������������ �� ������ ������������ �������

	// ������������ ������� ��������� - � + ��� ��������
	for( sli1 = lexList.begin(); sli1 != lexList.end(); sli1 ++ )
	{
	
		// ������ ������� �� ������ ������������� �� �����
		if( sli1 == lexList.begin() )
			continue;

		// ���������� �������
		sli2 = sli1;
		sli2 --;
 
		// ���������� - � ������� - � + � ������� + 
		if( (*sli1)->type == LT_PARSER && ( (*sli1)->parser == LPT_SUBTRACTION || (*sli1)->parser == LPT_ADDITION ) )
		{
			if( (*sli2)->type == LT_PARSER )
			{
				if( (*sli1)->parser == LPT_SUBTRACTION )
					(*sli1)->parser = LPT_UNARY_MINUS;
				else 
					(*sli1)->parser = LPT_UNARY_PLUS;
			}
		}

		/*
		// ������ ������� ����� ������������� �� �����
		if( sli2 == lexList.begin() )
			continue;
		
		sli3 = sli2;
		sli3 --;
		
		lex1 = *sli1;
		lex2 = *sli2;
		lex3 = *sli3;
		

		optimize = false;
		
		// ���� ������� ������� �� ��������� - �� �� ����� �������������� ����� �������
		if( lex1->isNumericConst() && lex2->type == LT_PARSER && lex3->type == LT_PARSER && 
			lex3->parser == )
		{
			if( lex2->parser == LPT_SUBTRACTION )
			{
				// ������� �� ������ �����
				if( lex->isUnsigned )
				{
					this->addError( SE_SCANER_WARNING, SSRV_APPLY_UNARI_MINUS_TO_UNSIGNED, lex->getLexemeDesc().getCString(), *lex );
					lex->isUnsigned = false;
				}
				lex1->longValue = - lex1->longValue;
				optimize = true;
			}
			else if( lex2->parser == LPT_ADDITION )
				optimize = true;
							
			if( optimize )
			{
				// ������� ������ �������
				delete lex2;
				lexList.erase( sli2 );

				// ��������� �� ���� ������� ����� ��� ��������� ��������
				sli1 = sli3;
			}
		}
		*/
	}
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDirective()
{

	scriptRetCode	retCode;
	std::list< scriptLexeme* >::iterator	sli;

	// ������ ������� - # ������ ��� ����������������� �����
	sli = this->directiveLexemes.begin();
	sli ++;
	
	// ���� ������ �������� �������� ����������������� �����
	if( (*sli)->type == LT_RWORD )
	{
		
		// ������� �����
		switch( (*sli)->rwordType )
		{
		case LRW_DEFINE:
			retCode = this->parseDefineDirective();
			return retCode;
		case LRW_ELIF:
			retCode = this->parseIfDirective();
			return retCode;
		case LRW_ELSE:
			retCode = this->parseElseDirective();
			return retCode;
		case LRW_ENDIF:
			retCode = this->parseEndIfDirective();
			return retCode;
		case LRW_ERROR:
			retCode = this->executeErrorDirective();
			return retCode;
		case LRW_IF:
			retCode = this->parseIfDirective();
			return retCode;
		case LRW_IFDEF:
			retCode = this->parseIfDefDirective();
			return retCode;
		case LRW_IFNDEF:
			retCode = this->parseIfNdefDirective();
			return retCode;
		case LRW_IMPORT:
			break;
		case LRW_INCLUDE:
			retCode = this->parseIncludeDirective();
			return retCode;
		case LRW_PRAGMA:
			break;
		case LRW_UNDEF:
			retCode = this->parseUndefDirective();
			return retCode;
		default :
			return SRV_FAIL;
		}

	}
	
	return SRV_FAIL;	
}

scriptRetCode scriptScaner::executeIfDirective()
{
	
	scriptRetCode		retCode;
	scriptLexeme		*lex;

	_tprintf( TEXT( "*************************************\n" ) );
	retCode = this->executeIfDirective( this->outputMacrosList.begin(), this->outputMacrosList.end(), this->outputMacrosList );
	if( retCode != SRV_SUCCESS )
	{
		this->skipScanning.push( SST_SKIP_UNTIL_NEW_CONDITION );
		return retCode;
	}
	_tprintf( TEXT( "*************************************\n" ) );

	// ���� ��� ��������� �������� ��������� ��������� �������
	lex = this->outputMacrosList.front();
	if( lex->isZeroConst() )
		// ���������� ������������
		this->skipScanning.push( SST_SKIP_UNTIL_NEW_CONDITION );
	else 
		// ����� ������ ����������� 
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );

	return SRV_SUCCESS;
}

// begin - �������� � �������� ���� ��������
// end - ��������, �� ���������� �������� ���� ��� �� ���������
scriptRetCode scriptScaner::executeIfDirective( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli, sli2, sli3, sliTmp;
	scriptLexeme							*lex, *lex2;
	int										bracketCount, opCode;
	scriptRetCode							retCode;
	bool									foundBinary;

	// ���� ������ ����� ���e� - ���� ()
	if( begin == end )
	{
		lex = *begin;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR03, *lex ).code;	
	}

	// ���������� � ���� ������
	// ���� ������ ������������� ������
	// ��� ��� ������ ������������� ������
	// �������� ��������� ( begin ... )- end
	// ������ ������ ( � ) - ������� ��������� ��������� � �������
	// ��� ������ ��������� �� ����
	// � �� ������������� � ���������� �������
	// ������� ����� ��������� ������ ������
	// ����� ������� ����� � ������ ���� �� ������
	for( sli = begin; sli != end; sli ++ )
	{	
		// ���� ����������� ������
		lex = *sli;
		if( lex->type == LT_PARSER && lex->parser == LPT_LEFT_BRACKET )
		{
			sli2 = sli;
			for( sli2 ++, bracketCount = 1; sli2 != end; sli2 ++ )
			{
				lex2 = *sli2;
				if( lex2->type == LT_PARSER && lex2->parser == LPT_LEFT_BRACKET )
					bracketCount ++;
				if( lex2->type == LT_PARSER &&lex2->parser == LPT_RIGHT_BRACKET )
					bracketCount --;
			
				if( bracketCount == 0 )
					break;
			}

			// ��� ����������� ������ �� ����� ����������� ������
			if( sli2 == end )
				return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR01, *lex ).code;	
		
			// ����� ������ � ����� ������
			sli ++;
			retCode = this->executeIfDirective( sli, sli2, lexList );
			if( retCode != SRV_SUCCESS )
				return retCode;
			
			// ������� ���������� �������, ������� ��������� ������ ������
			sli3 = sli;
			sli3 --;
	
			// ���������� ������������� ������, ������������� ������ 
			delete lex;
			delete lex2;
			
			lexList.erase( sli3 );
			lexList.erase( sli2 );
			
			// ���� ������ ���� ((
			if( sli3 == begin )
				begin = sli;
			
			// ����� ���� ������������ ��������� ��� �������
			this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
		}
	}
			
	// ����� ���� ������������ ��������� ��� �������
	this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
	
	// ��������� ������� ���������� ���������� 
	// ��� ���������� (
	// �� ��� �� ��������� �����, �� ��� ������, ��� ��� ����������� ������ �� ����� ��� �������

	// �������� ����������
	// 0)	defined
	// 1)   ~  ! ( u+ u- ������������� �� ����� ��� ��� ��� ���� �������������� )
	// 2)	*  /  %
	// 3)	+ -
	// 4)	>> <<
	// 5)	>  <  >=  <=
	// 6)	== !=
	// 7)	&
	// 8)	^
	// 9)	|
	// 10)	&&
	// 11)  ||
	// 12)  e1 ? e2: e3
	
	// ���������� �� ���� �����������, ���� �� ������ � �� �������� ����
	for( opCode = 0; opCode < 13; opCode ++ )
	{
		lex = *begin;
		sli = begin;
		sli ++;
		
		// ���������� ������
		_tprintf( TEXT( "op = %d    " ), opCode );
		for( sliTmp = this->outputMacrosList.begin(); sliTmp != this->outputMacrosList.end(); sliTmp ++ )
			_tprintf( TEXT( "%s " ), (*sliTmp)->getLexemeDesc().getCString() );

		_tprintf( TEXT( "\n" ) );

		// �������� ( const ) ��� begin const end
		if( lex->type == LT_CONST && sli == end )
			break;	
			
		// ���� �������� �� �����������
		retCode		= SRV_SUCCESS;
		foundBinary	= false;
		for( sli = begin; sli != end; sli++ )
		{
			
			lex = *sli;
			
			// ���� �� defined ��� �� ����������� �� ����������
			if( lex->type != LT_PARSER && ( lex->type != LT_RWORD || lex->rwordType != LRW_DEFINED ) )
				continue ;
			
			// 0 ::= defined
			if( opCode == 0 && lex->type == LT_RWORD && lex->rwordType == LRW_DEFINED )
				retCode = this->executeDefinedOp( sli, lexList );
			// ������ ������������ ��� ������� ��������
			else if( opCode == 1 && preprocOperatorType[ lex->parser ] == 1 )
				retCode = this->executeUnaryOp( sli, lexList );
			// ������ ������������ ��� �������� ��������
			else if( opCode == 2 && ( lex->parser == LPT_MULTIPLICATION || lex->parser == LPT_DIVISION || lex->parser == LPT_REMAINDER ) )
				foundBinary = true;
			else if( opCode == 3 && ( lex->parser == LPT_ADDITION || lex->parser == LPT_SUBTRACTION ) )
				foundBinary = true;
			else if( opCode == 4 && ( lex->parser == LPT_LEFT_SHIFT || lex->parser == LPT_RIGHT_SHIFT ) )
				foundBinary = true;
			else if( opCode == 5 && ( lex->parser == LPT_GREATER_THAN || lex->parser == LPT_GREATER_THAN_OR_EQUAL_TO || lex->parser == LPT_LESS_THAN || lex->parser == LPT_LESS_THAN_OR_EQUAL_TO ) )
				foundBinary = true;
			else if( opCode == 6 && ( lex->parser == LPT_EQUALITY || lex->parser == LPT_INEQUALITY ) )
				foundBinary = true;
			else if( opCode == 7 && lex->parser == LPT_BITWISE_AND )
				foundBinary = true;
			else if( opCode == 8 && lex->parser == LPT_BITWISE_EXCLUSIVE_OR )
				foundBinary = true;
			else if( opCode == 9 && lex->parser == LPT_BITWISE_OR )
                foundBinary = true;
			else if( opCode == 10 && lex->parser == LPT_LOGICAL_AND )
				foundBinary = true;
			else if( opCode == 11 && lex->parser == LPT_LOGICAL_OR )
				foundBinary = true;
			// ������ ������������ ��������� ��������
			//else if( opCode == 12 && lex->type == LT_PARSER && lex->parser == LPT_QUESTION )
			//	retCode = this->executeQuickIfOp( sli, lexList );
			else continue;

			if( foundBinary )
				retCode = this->executeBinaryOp( sli, lexList );
			
			// ���� �� ����� - ������ ��� �� �����
			if( retCode != SRV_SUCCESS )
				return retCode;
			else 
			{
				// ������������ ������� ����� � ������	
				this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
				// � ����� ������ �������� ������������ ����������
				opCode --;
				break;
			}
		}
	}
	
	// ����� ��������� ������ ���� opCode == 12
	if( opCode == 13 )
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR18, *(*begin) ).code;
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeQuickIfOp( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{

	// ������� ������ ��������� � ������������ ������
	std::list< scriptLexeme* >::iterator	condSli, expr1Sli, expr2Sli, colonSli;
	scriptLexeme							*cond, *expr1, *expr2;

	// ������� ������� � ���� �� ������ ������e
	if( sli == begin )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR01, *(*sli) ).code;

	// �������� ������e
	condSli = sli;
	condSli--;
	cond = *condSli;

	// ��������������� ������������� � ����
	if( cond->type == LT_ID )
	{
		cond->type			= LT_CONST;
		cond->constType		= LCT_DECIMAL;
		cond->isUnsigned	= false;
		cond->longValue		= 0;
	}

	// ��������� �������
	if( cond->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR02, *cond ).code;

	// �������� ������ ���������
	// ������� ��������� � ���� �� ������ ���������
    if( sli == end )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR03, *cond ).code;

	expr1Sli = sli;
	expr1Sli ++;

	expr1 = *expr1Sli;

	// ���� ��� �������������
	if( expr1->type == LT_ID )
	{
		expr1->type			= LT_CONST;
		expr1->constType	= LCT_DECIMAL;
		expr1->isUnsigned	= false;
		expr1->longValue	= 0;
	}

	// ���� ��� �� �����
	if( expr1->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR04, *expr1 ).code;

	// ������ ������ ��������� ������� ���������
	// ���� ������ ��������� �����������
	colonSli = expr1Sli;
	if( colonSli == end )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR05, *expr1 ).code;
	
	// ���� ������ ��������� ����� �� �����
	if( (*colonSli)->type != LT_PARSER || (*colonSli)->parser != LPT_DOT2 )	
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR06, *(*colonSli) ).code;
	
	expr2Sli = colonSli;
	expr2Sli ++;

	// ���� ��������� �����������
	if( expr2Sli == end )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR07, *(*colonSli) ).code;
	
	expr2 = *expr2Sli;

	if( expr2->type == LT_ID )
	{		
		expr1->type			= LT_CONST;
		expr1->constType	= LCT_DECIMAL;
		expr1->isUnsigned	= false;
		expr1->longValue	= 0;
	}
	
	if( expr2->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR08, *expr2 ).code;

	// ������ ��������� �������
	if( !cond->isZeroConst() )
		*cond = *expr1;
	else
		*cond = *expr2;
	
	if( expr2Sli == end )
		end = condSli;
	
	// ������� ��������� ��������� ������ ���������
	delete expr2;
	delete *colonSli;
	delete expr1;
	delete *sli;

	lexList.erase( expr2Sli );
	lexList.erase( expr1Sli );
	lexList.erase( colonSli );
	lexList.erase( sli ); 
	
	sli = condSli;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeBinaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{
	
	std::list< scriptLexeme* >::iterator	leftOpSli, rightOpSli;
	scriptLexeme							*leftOp, *rightOp, *opCode;
	lexConstType							newConstType;
	bool									bNewUnsigned;
	
	// �������� ������� ���������
	opCode = *sli;
	
	// ��������, ��� ����� ����� ������� 
	if( sli == lexList.begin() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR10, *(*sli) ).code;
	
	// �������� ����� �������
	leftOpSli	= sli;
	leftOpSli --;
	leftOp		= *leftOpSli;	
	
	// �������� ������ �������
	rightOpSli = sli;
	rightOpSli ++;

	// �������� �� �� ��� ������ ���� �������
	if( rightOpSli == lexList.end() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR11, *(*rightOpSli) ).code;	
	
	rightOp		= *rightOpSli;
	
	if( leftOp->type == LT_ID )
	{
		leftOp->name.clear();
		leftOp->type			= LT_CONST;
		leftOp->constType		= LCT_DECIMAL;
		leftOp->isUnsigned		= false;
		leftOp->longValue		= 0;
	}
	
	if( rightOp->type == LT_ID )
	{
		rightOp->name.clear();
		rightOp->type			= LT_CONST;
		rightOp->constType		= LCT_DECIMAL;
		rightOp->isUnsigned		= false;
		rightOp->longValue		= 0;
	}
	
	// ����� ��� ��������
	// ��������� ������ ������� �� �������������� �����
	if( leftOp->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR12, *leftOp ).code;
	
	if( rightOp->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR13, *rightOp ).code;	
	
	// ������ ��������� �������� �������� ��������� ���������
	// ���� / ��� % ���������, ����� �� ��� 0 ����� 
	// ������: ������� �� ����!
	if( ( opCode->parser == LPT_DIVISION || opCode->parser == LPT_REMAINDER ) && rightOp->isZeroConst() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR14, *rightOp ).code;	
	    
	
	// �������� ��������� ��������
	
	// ���� �������� ��������������
	// + - * / %
	if( preprocOperationType[ opCode->parser ] == 1 )
	{
		// �������� ��� ����������
		leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );
			
		// ��������������� ����� �������
		leftOp->convertData( newConstType, bNewUnsigned );
	
		// ��������������� ������ �������
		rightOp->convertData( newConstType, bNewUnsigned );

		// ��������� �������������� ���������
		this->ppExecuteArithmeticOp( *leftOp, *opCode, *rightOp );
	}

	// ������� ��������, ��������� ��������� ��� �����
	// � ��������� ���� � �� �� �� ������������ ���� ������
	else if( preprocOperationType[ opCode->parser ] == 2 )
	{
		// �������� ��� ����������
		leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );

		// ��������������� ����� �������
		leftOp->convertData( newConstType, bNewUnsigned );

		// ��������������� ������ �������
		rightOp->convertData( newConstType, bNewUnsigned );
		
		// ��������� ��������
		this->ppExecuteLogicalOp( *leftOp, *opCode, *rightOp );
	}
	
	// ����� ��������� ��������
	// ���� & | ^ �������� >> << ��������� 
	// �������� �� >> << ���� ����������� ������
	else if( preprocOperationType[ opCode->parser ] == 3 )
	{
		// ���� ����� ��� - ������� 
		if( leftOp->constType == LCT_BOOL )
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING02, *leftOp );

		// ��� (�) ������ ��� �������
		if( rightOp->constType == LCT_BOOL )
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING03, *rightOp );
		
		// ����� �������� - float ������
		if( leftOp->constType == LCT_FLOAT )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR14, *leftOp ).code;

		// ������ �������� - float ������
		if( rightOp->constType == LCT_FLOAT )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR15, *rightOp ).code;

		// ����� �������� - double ������
		if( leftOp->constType == LCT_DOUBLE )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR16, *leftOp ).code;

		// ������ �������� - double ������
		if( rightOp->constType == LCT_DOUBLE )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR17, *rightOp ).code;

		if( opCode->parser != LPT_LEFT_SHIFT && opCode->parser != LPT_RIGHT_SHIFT )
		{
			// �������� ��� ����������
			leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );

			// ��������������� ����� �������
			leftOp->convertData( newConstType, bNewUnsigned );

			// ��������������� ������ �������
			rightOp->convertData( newConstType, bNewUnsigned );
		}
		
		// ��������� ��������
		this->ppExecuteBinaryOp( *leftOp, *opCode, *rightOp );
	}
	
	// ������� ���� �������� � ������ �������
	delete opCode;
	delete rightOp;

	lexList.erase( sli );
	lexList.erase( rightOpSli );

	sli = leftOpSli;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeUnaryOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli2;
	scriptLexeme							*lex, *lex2;
	scriptRetCode							code;

	// ������� ��������� ������� - ������ ���� ����������
	sli2 = sli;
	sli2 ++;
	
	// �������� �������
	lex = *sli;
	
	// ������� � ���� �� ���� ����� ������ �������
	if( sli2 == lexList.end() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR06, *(*sli2) ).code;
	
	lex2 = *sli2;	
		
	// ���� ��� ������������� � �� ������ - �� �� �������� ��� �������� ����� 0 
	if( lex2->type == LT_ID )
	{
		lex2->name.clear();
		lex2->type			= LT_CONST;
		lex2->constType		= LCT_DECIMAL; 
		lex2->isUnsigned	= false;
		lex2->longValue		= 0;
	}
	
	// ���� ��� ��� �� ������������� - �� 
	// ������ ���� ����������� ���������
    if( lex2->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR07, *lex2 ).code;
		
	lex->type		= LT_CONST;
	
	// �������� ����� 
	// ���� ��� ~
	if( lex->parser == LPT_BITWISE_NOT_OR_DESTRUCTOR )
	{
		lex->constType	= lex2->constType;	
		
		if( lex2->constType == LCT_BOOL )
		{
			// ��������� ����� ��������� �����
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING01, *lex2 );
			lex->boolValue	= !lex->boolValue; // �������� ��������� ����� ����������
		}
		// ���� - ������
		else if( lex2->constType == LCT_SYMBOL )
			lex->byteValue	= ~lex2->byteValue;
		else if( lex2->constType == LCT_BINARY || lex2->constType == LCT_OCTAL ||
				 lex2->constType == LCT_DECIMAL || lex2->constType == LCT_HEX )
		{	
			if( lex2->isUnsigned )
				lex->dwordValue	= ~lex2->dwordValue;
			else
				lex->longValue  = ~lex2->longValue;
		}
		else if( lex2->constType == LCT_FLOAT || lex2->constType == LCT_DOUBLE )
		{
			if( lex2->constType == LCT_FLOAT )
				code			= SPPRV_IF_DIRECTIVE_ERROR08;
			else 
				code			= SPPRV_IF_DIRECTIVE_ERROR09;
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, code, *lex2 ).code;
		}
	}		
	else  // ��� ! �������� � ����� ����� ��������
	{
		lex->constType	= LCT_BOOL;

		if( lex2->constType == LCT_BOOL )
			lex->boolValue = !lex2->boolValue;
		else if( lex2->constType == LCT_SYMBOL )
			lex->boolValue	= !lex2->byteValue;
		else if( lex2->constType == LCT_BINARY || lex2->constType == LCT_OCTAL ||
			lex2->constType == LCT_DECIMAL || lex2->constType == LCT_HEX )
		{
			if( lex2->isUnsigned )
				lex->dwordValue	= !lex2->dwordValue;
			else
				lex->longValue = !lex2->longValue;
		}
		else if( lex2->constType == LCT_FLOAT )
			lex->floatValue = !lex2->floatValue;
		else if( lex2->constType == LCT_DOUBLE )
			lex->doubleValue = !lex2->doubleValue;

	}

	// ������� ������� � ����������
	delete (lex2);
	lexList.erase( sli2 );

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeDefinedOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli2;
	scriptLexeme							*lex, *lex2;

	// ������� ��������� ������� - ������ ���� ������
	sli2 = sli;
	sli2 ++;

	lex		= *sli;
	
	// ������� � ���� �� ���� ����� ������ �������
	if( sli2 == lexList.end() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR05, *(*sli2) ).code;
		
	lex2	= *sli2;
	
	// ���� ��� �����
	if( lex2->type != LT_RWORD && lex2->type != LT_ID )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR04,  *lex2 ).code;
	
	// ��������������� � ���������
	lex->type		= LT_CONST;
	lex->constType	= LCT_BOOL;
	
	// ������� � ���� �� ����� ������
	if( this->macroExist( (*sli2)->getLexemeName() ) )
		lex->boolValue	= true;
	else
		lex->boolValue	= false;
	
	// ������� �������������
	delete lex2;
	lexList.erase( sli2 );

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIfDirective()
{

	std::list< scriptLexeme* >::iterator	sli, sli2, sli3;
	scriptMacros								*scrMacro;
	scriptLexeme							*lex;

	// ��������� � ������ ���������
	sli = this->directiveLexemes.begin();
	
	// ���������� #
	// ���������� if
	sli ++;
	sli ++;
	
	// ������� ������
	clearLexList( this->outputMacrosList );
	
	// ������ ���������� ��������� � �������� ������
	for( ; sli != this->directiveLexemes.end(); sli++ )
	{
	
		// �������� �������� ��� �������
		scrMacro = NULL;
		if( (*sli)->type == LT_ID || (*sli)->type == LT_RWORD )
			scrMacro = this->macroExist( (*sli)->getLexemeName() );

		// ���� ��� ��� ������� � ��� ��� ���� �� ����� defined
		// �� ���������� ������
		if( scrMacro != NULL && ( this->outputMacrosList.back()->type != LT_RWORD || this->outputMacrosList.back()->rwordType != LRW_DEFINED ) )
		{
			// ���������� ������ �������
			sli2 = sli;

			// ������������ �����		
			// ��������� ��� ���������� ���������
			this->scanMacros( sli2, this->directiveLexemes, *scrMacro );
			// ����������� ��������� ���� ����
			this->parseMacros( *(*sli), *scrMacro );
			// ��������� ������ ���� ����
			this->executeMacros( *(*sli), *scrMacro, this->outputMacrosList );
			// ������������� �� ������� ��������� �� ��������
			sli = sli2;

			// ���������� ���������
			continue;
		}
		else 
		{
			// ������ �������� �������
			lex = new scriptLexeme ( *( *sli ) );
			lex->scaner = this;
			this->outputMacrosList.push_back( lex );
		}	
	}
	
	// ������������ �����������
	this->optimizeParsers( this->outputMacrosList );
	this->optimizeMacroUnaryMinusAndPlus( this->outputMacrosList.begin(), this->outputMacrosList.end(), this->outputMacrosList );	
				
	// ��������� ���������
	this->executeIfDirective();
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineDirective()
{

	std::list< scriptLexeme* >::iterator	sli, sli2;
	scriptMacros								*scrMacro;
	scriptRetCode							retCode;
	bool									redef = false;

	// �������� ������� ��������� �� define
	sli = this->directiveLexemes.begin();
	sli ++;	// define
	sli ++;	// ��� �������

	// ���� �� ����� ���� ������ �� ������� ������
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_SCANER_ERROR, SPPRV_DEFINE_DIRECTIVE_ERROR01, *(*sli) ).code;
	}
	
	if( (*sli)->type == LT_RWORD )
		scrMacro = this->macroExist( scriptRWordArray [ (*sli)->rwordType ] );
	else if( (*sli)->type == LT_ID )
		scrMacro = this->macroExist( (*sli)->name );
	else
		return this->addError( SE_SCANER_ERROR, SPPRV_DEFINE_DIRECTIVE_ERROR02, *(*sli) ).code;
	
	// ���� ����� ������ ���� ���������� �������� ��������������
    if( scrMacro != NULL )
	{
		_stprintf_s( this->tempBuffer, TEXT( "'%s'" ), (*sli)->name.getCString() );
		this->addError( SE_SCANER_WARNING, SPPRV_MACRO_REDEFINITION, this->tempBuffer, *(*sli) );
				
		// ������� ������
		scrMacro->clear();
		redef = true;
	}
	else {
		scrMacro = new scriptMacros();
	}
	
	// ������ �������������� ������
	scrMacro->name			= (*sli)->name;
	scrMacro->line			= (*sli)->line;
	scrMacro->col			= (*sli)->col;
	scrMacro->fileIndex		= this->fInfo.fileIndex;
						
	// ���� ������� ���������
	retCode = this->parseDefineParams( *scrMacro, sli );
	
	// ���� ������� ���� �������
	retCode = this->parseDefineBody( *scrMacro, sli );

	// ���������� ������
	if( redef == false )
		this->parent->macros.push_back( scrMacro );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineParams( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli )
{
	std::list< scriptLexeme* >::iterator	sli2; 
	scriptLexeme							*lex;
	
	// ������� ������ ����������
	clearLexList( scrMacro.params );
	
	sli2 = sli;
	sli2 ++;	// ��������� ����� ������
	
	if( sli2 == this->directiveLexemes.end() )
	{
		sli ++;
		return SRV_SUCCESS;
	}
	
	// ���� ��� ������ � �����������
	if( (*sli2)->type != LT_PARSER || (*sli2)->parser != LPT_LEFT_BRACKET || !(*sli)->isParserNearId( *(*sli2) ) )
	{
		sli ++;
		return SRV_SUCCESS;
	}
	
	bool mustFoundId = true;
	
	// ������ ��������� ���������
	sli = sli2;
	for( sli ++; sli != this->directiveLexemes.end(); sli++ )
	{
		
		// ���� �� ����� )
		if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_RIGHT_BRACKET )
			break;
		
		// ���� ��� ������ ���� ����� ��������
		if( mustFoundId )
		{
			if( (*sli)->type != LT_ID && (*sli)->type != LT_RWORD )
			{
				this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR01, *(*sli) );
				continue;
			
			}
			else
			{
				// ��������� �������
				lex = new scriptLexeme( *(*sli) );
				lex->scaner = this;
				scrMacro.params.push_back( lex );
			}
		}
		else // ���� ����� �������
		{
			if( (*sli)->type != LT_PARSER || (*sli)->parser != LPT_COMMA )
			{
				this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR02, *(*sli) );
				continue;
				// SPPRV_PARAM_LIST_ERROR02;
			}
		}
	
		// ������ ��������� ����������
		mustFoundId = !mustFoundId;
	}

	// ������ ���������� ��� �������: ��� ����������� ������
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR03, *(*sli) ).code;
	}
	
	// ���������� ����������� ������
	sli ++;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineBody( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli )
{
	
	scriptLexeme *lex;

	// ������� ���� �������
	clearLexList( scrMacro.body );

	// ������� � �� ���������� �� ������
	if( sli == this->directiveLexemes.end() )
		return SRV_SUCCESS;

	// ���� �� ����������
	// ��������� ������� � ������
	for( ; sli != this->directiveLexemes.end(); sli ++ )
	{
		lex = new scriptLexeme ( *(*sli) );
		lex->scaner = this;
		scrMacro.body.push_back( lex );	
	}
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIfDefDirective()
{

	// �������� ������� �������� �� ifdef
	std::list< scriptLexeme* >::iterator	sli;
	scriptMacros								*scrMacro;

	sli = this->directiveLexemes.begin();
	sli ++;	// ifdef
	sli ++;	// ��� �������

	// ���� �� ����� ���� ������ �� ������� ������
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_SCANER_ERROR, SPPRV_IFDEF_DIRECTIVE_ERROR01, *(*sli) ).code;
	}
	
	if( (*sli)->type == LT_RWORD )
		scrMacro = this->macroExist( scriptRWordArray [ (*sli)->rwordType ] );
	else if( (*sli)->type == LT_ID )
		scrMacro = this->macroExist( (*sli)->name );
	else
		return this->addError( SE_SCANER_ERROR, SPPRV_IFDEF_DIRECTIVE_ERROR02, *(*sli) ).code;
	
	// ���� ������ ���������� - �� ������� ��������� � ��������� ������ �����
	if( scrMacro != NULL )
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );
	else // ����� ������ ���� �������
		this->skipScanning.push( SST_SKIP_UNTIL_END );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIfNdefDirective()
{

	// �������� ������� �������� �� ifdef
	std::list< scriptLexeme* >::iterator	sli;
	scriptMacros							*scrMacro;

	sli = this->directiveLexemes.begin();
	sli ++;	// ifdef
	sli ++;	// ��� �������

	// ���� �� ����� ���� ������ �� ������� ������
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_SCANER_ERROR, SPPRV_IFNDEF_DIRECTIVE_ERROR01, *(*sli) ).code;
	}
	
	if( (*sli)->type == LT_RWORD )
		scrMacro = this->macroExist( scriptRWordArray [ (*sli)->rwordType ] );
	else if( (*sli)->type == LT_ID )
		scrMacro = this->macroExist( (*sli)->name );
	else
		return this->addError( SE_SCANER_ERROR, SPPRV_IFNDEF_DIRECTIVE_ERROR02, *(*sli) ).code;
	
	// ���� ������ �� ���������� - �� ������� ��������� � ��������� ������ �����
	if( scrMacro != NULL )
		this->skipScanning.push( SST_SKIP_UNTIL_END );
	else // ����� ������ ���� �������
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseEndIfDirective()
{
	
	std::list< scriptLexeme* >::iterator	sli;
	
	// ���� �� endif ���� ��� ����� �� ������� ��� endif ����������� ��� �� ����
	if( this->directiveLexemes.size() > 2 || this->skipScanning.size() == 1 )
	{
		if( this->directiveLexemes.size() > 2 )
		{
			sli = this->directiveLexemes.begin();
			sli ++;
			sli ++;
			
			this->addError( SE_SCANER_ERROR, SPPRV_ENDIF_DIRECTIVE_ERROR01, *(*sli) );
			
		}
		else
			this->addError( SE_SCANER_ERROR, SPPRV_ENDIF_DIRECTIVE_ERROR02, *this->directiveLexemes.front() );
	}
	else // ��� ���������
		this->skipScanning.pop();
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseElseDirective()
{
	
	std::list< scriptLexeme* >::iterator	sli;
		
	// ���� �� endif ���� ��� ����� �� ������� ��� endif ����������� ��� �� ����
	if( this->directiveLexemes.size() > 2 || this->skipScanning.size() == 1 )
	{
		if( this->directiveLexemes.size() > 2 )
		{
			sli = this->directiveLexemes.begin();
			sli ++;
			sli ++;
			this->addError( SE_SCANER_ERROR, SPPRV_ELSE_DIRECTIVE_ERROR01, *(*sli) );
		}
		else
			this->addError( SE_SCANER_ERROR, SPPRV_ELSE_DIRECTIVE_ERROR02, *this->directiveLexemes.front() );
	}
	else // ��� ���������
	{
		if( this->skipScanning.top() == SST_DO_NOT_SKIP_SCANNING )
			this->skipScanning.top() = SST_SKIP_UNTIL_END;
		else if( this->skipScanning.top() == SST_SKIP_UNTIL_NEW_CONDITION )
			this->skipScanning.top() = SST_DO_NOT_SKIP_SCANNING;		
	}
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseUndefDirective()
{

	std::list< scriptLexeme* >::iterator	sli;
	std::list< scriptMacros* >::iterator		smi;
	scriptMacros			*scrMacro;
	shString			name;

	// �������� ������ �������
	sli = this->directiveLexemes.begin();
	sli++; // undef
	sli++; // macro name

	// ���� # undef
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR01, *(*sli) ).code;
	}
	
	// ���� ������� �� ��� � ���� ����� ���
	if( (*sli)->type != LT_ID && (*sli)->type != LT_RWORD )
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR02, *(*sli) ).code;
	
	// �������� ��� �������
	if( (*sli)->type == LT_ID )
		name = (*sli)->name;
	else
		name = scriptRWordArray [ (*sli)->rwordType ];

	// ���� ����� ������� ���
	if( ( scrMacro = this->macroExist( name ) ) == NULL )
	{
		// ������ ����������� � ���, ��� ��� ����� ���
		this->addError( SE_PREPROCESSOR_WARNING, SPPRV_UNDEFINED_MACRO, name.getCString(), *(*sli) );
		return SRV_SUCCESS;
	}

	// ������� ������
	for( smi = this->parent->macros.begin(); smi != this->parent->macros.end(); smi++ )
	{
		if( *smi == scrMacro )
		{
			delete *smi;
			this->parent->macros.erase( smi );
			break;
		}
	}
	
	// ���� ���� ��� ����� �� �������
	if( this->directiveLexemes.size() > 3 )
		this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR03, *this->directiveLexemes.front() );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeErrorDirective()
{

	// ����� ��� ���������� ������� ������ ��� ������	
	int	readSymbolCount = 0;
	
	// ��������� ���������� ����� 
	while( 1 )
	{
		
		// �������� �� ����� �����
		if( this->fInfo.filePos == this->fInfo.fileLength )
			break;
				
		// ���� �������� ����� ������
		if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
		{
			this->fInfo.line ++;
			// ���� ����������� ������
			if( this->fInfo.fp[ this->fInfo.filePos - 1 ] == '\\'  )
			{
				readSymbolCount --;
				this->fInfo.filePos ++;
				continue;
			}
			else {
				this->fInfo.filePos ++;
				break;
			}
		}
		
		// ���� �������� 0x0a
		if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
		{
			this->fInfo.filePos ++;
			this->fInfo.col		= 0;
			continue;
		}

		// ��� ��������� ��������
		this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
		this->fInfo.filePos ++;
		this->fInfo.col ++;
		readSymbolCount ++;
	
	}

	this->tempBuffer[ readSymbolCount ] = 0; 
	this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_ERROR_DIRECTIVE, this->tempBuffer, *this->directiveLexemes.front() );
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIncludeDirective()
{
	
	std::list< scriptLexeme* >::iterator	sli1;
	std::list< scriptLexeme* >::iterator	sli2;
	std::list< scriptLexeme* >::iterator	sli3;
	
	shString								fileName;

	// �������� ������ �������
	sli1 = this->directiveLexemes.begin();
	// ���������� ������ #
	sli1 ++;

	// �������� ������ �������
	sli2 = sli1;
	sli2 ++;

	// include ...
	if( sli2 == this->directiveLexemes.end() )
		return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR01, *(*sli1) ).code;
	
	// �������� ������ �������
	sli3 = sli2;
	sli3++;

	// ���� ������ ���� include "file.h"
	if( (*sli2)->type == LT_STRING )
	{
		// ���� ��� ��� �� ���� ����� �����
		if( sli3 != this->directiveLexemes.end() )
			this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR02, *(*sli3) );
			
		return this->executeIncludeDirective( (*sli2)->name, false );
	}
	
	// ���� ������ �� ������ ��� include < my file.h > �.�. < "my file.h" >
	if( (*sli2)->type == LT_PARSER && (*sli2)->parser == LPT_LESS_THAN )
	{
		// �������� �������� ������ �������������, ����� '.', ������ ������������� � ����� ����������� ������
		sli1 = sli2;
		sli1 ++;
		
		// ���� ������ ������� �������������� ������ ����� ������
		if( sli1 == this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR04, *(*sli2) ).code;
		// ���� �� ������� ������ 
		else if( (*sli1)->type != LT_STRING )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR05, *(*sli2) ).code;
				
		// ������� �� ������ �������� ����������� ����������� ������
		sli2 = sli1;
		sli2 ++;

		// ��������� > �� ������ ������� ������
		if( sli2 == this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR06, *(*sli2) ).code;
		else if( (*sli2)->type != LT_PARSER || (*sli2)->parser != LPT_GREATER_THAN )// ������� ��� �� ������
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR07, *(*sli2) ).code;
				
		// ������� ����� ������
		sli3 = sli2;
		sli3 ++;

		// ���� ����� ������ �� ������
		if( sli3 != this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR08, *(*sli2) ).code;
			
		// ������ ��������� ���������
		return this->executeIncludeDirective( (*sli1)->name, true );

	}
	else // ����� - ������ 
		return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR03, *(*sli2) ).code;
			
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeIncludeDirective( shString &str, bool inSysDir )
{
	
	// ���������� ��������� ��������� ������ � �������������� ���� ���� ��������
	scriptRetCode		retCode;
		
	this->nextScaner = new scriptScaner();
	this->nextScaner->prevScaner	= this;
	this->nextScaner->parent		= this->parent;
	
	retCode = this->nextScaner->openIncludeFile( str, inSysDir );
	if( retCode == SRV_SUCCESS )
	{
		// ��������� ��������� ����
		retCode = this->nextScaner->scan();
		// ������� ����� ���������� 
		delete this->nextScaner;
		this->nextScaner = NULL;
		// ���������� ��� ���		
		return retCode;
	}

	// ���� �� ��������� ����� �� ��������� ������ ��� ��������
	else
	{
		// �������� ������ � ��������� �������
		this->addError( SE_PREPROCESSOR_FATAL_ERROR, retCode, *this->directiveLexemes.front() );
		
		// ������� ����� ���������� 
		delete this->nextScaner;
		this->nextScaner = NULL;
		return retCode;
	}

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::optimize( std::list< scriptLexeme* > &lexList )
{

	scriptRetCode	retCode;
	
	// ������������ �����������
	retCode = this->optimizeParsers( lexList );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// ����������� �����
	retCode = this->optimizeStrings( lexList );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// ����������� ���������������
	retCode = this->optimizeIds( lexList );
	if( retCode != SRV_SUCCESS )
		return retCode;

	return SRV_SUCCESS;
}


scriptRetCode scriptScaner::optimize()
{
	return this->optimize( this->lexemes );
}


bool scriptScaner::isEscapeSequence( TCHAR *szString, DWORD dwLength )
{
	int index, escapeCount = 0;

	// ������� ����� ����� ���� ����� ��������
	for( index = dwLength - 1; index >= 0; index -- )
	{
		if( szString[ index ] == '\\' )
			escapeCount++;
		else
			break;
	}

	// ���� ����� ������� - �� ����������� escape �����������������
	if( escapeCount & 0x01 )
		return true;
	 
	// ����������� �������
	return false;
}

scriptRetCode scriptScaner::formatString( TCHAR *szString, TCHAR *szString2, DWORD dwLength, scriptLexeme &lex )
{

	DWORD			ind1, ind2, charVal;
	bool			escapeStatement;
	TCHAR			buf[16];

	// ������� �� ��������� �� � escape ������������������
	escapeStatement = false;
	
	// ���� �� ����� �������� � ������ ������
	for( ind1 = 0, ind2 = 0; ind1 < dwLength; )
	{
		// ���� �� escape ������
		if( escapeStatement == false )
		{
			if( szString[ ind1 ] != '\\' )
			{
				// ������ ���������� ��� � �������� ������
				szString2[ ind2 ] = szString[ ind1 ];
				ind1 ++;
				ind2 ++;
			}
			else 
			{
				escapeStatement = true;	
				ind1 ++;
			}
		}
		else
		{
			escapeStatement = false;	// ������� �� ������������ ������
			switch( szString[ ind1 ] )
			{
			case 'n':	// \n
				szString2[ ind2 ] = 0x0d;	
				break;
			case 't':	// \t
				szString2[ ind2 ] = 0x09;	
				break;
			case 'v':	// \v
				szString2[ ind2 ] = 0x0b;
				break;
			case 'b':	// \b
				szString2[ ind2 ] = 0x08;	
				break;
			case 'r':	// \r
				szString2[ ind2 ] = 0x0d;	
				break;
			case 'f':	// \f
				szString2[ ind2 ] = 0x0c;
				break;
			case 'a':	// \a
				szString2[ ind2 ] = 0x07;
				break;
			case '\\':	// '\\'
				szString2[ ind2 ] = 0x5c;	
				break;
			case '\?':	//	\?
				szString2[ ind2 ] = 0x3f;	
				break;
			case '\'':	// \'
				szString2[ ind2 ] = 0x27;
				break;
			case '\"':	// \"
				szString2[ ind2 ] = 0x22;
				break;
			case '0':	// \0
				szString2[ ind2 ] = 0;
				break;
			
			// ������������ �����
			case '1': case '2':	case '3': case '4': case '5': case '6': case '7':
					
				// ����������� ������ �����
				charVal = szString[ ind1 ] - 0x30;
				
				// ��������� ������ �����
				if( szString[ ind1 + 1 ] < '0' || szString[ ind1 + 1 ] > '7' )
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
					break;
				}
				
				ind1 ++;	
				
				// ���������� ������ �����
				charVal = charVal * 8 + szString[ ind1 ] - 0x30;

				// ��������� ������ �����
				if( szString[ ind1 + 1 ] < '0' || szString[ ind1 + 1 ] > '7' ) 
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
					break;
				}
				
				ind1 ++;	
				charVal = charVal * 8 + szString[ ind1 ] - 0x30;
				if( charVal > 255 )
				{
					// ����� ������ ��� 255
					// ���������� ������
					_stprintf_s( buf, TEXT( "'%d'" ), charVal );
					this->addError( SE_SCANER_ERROR, SSRV_TOO_BIG_FOR_CHARACTER, buf, lex.line, lex.col - ind1 - 2 );
					
					ind1 ++;
					continue;
				}
				else
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
				}
			break;
			case 'x':
				 
				// ������ �����
				if( szString[ ind1 + 1 ] >= '0' && szString[ ind1 + 1 ] <= '9' )
					charVal = szString[ ind1 + 1 ] - '0';
				else if( szString[ ind1 + 1 ] >= 'a' && szString[ ind1 + 1 ] <= 'f' )
					charVal = szString[ ind1 + 1 ] - 'a' + 0x0a;
				else if( szString[ ind1 + 1 ] >= 'A' && szString[ ind1 + 1 ] <= 'F' )
					charVal = szString[ ind1 + 1 ] - 'A' + 0x0a;
				else 
				{
					szString2[ ind2 ] = szString[ ind1 ];
					_stprintf_s( buf, TEXT( "'\\x%c'" ), szString[ ind1 + 1 ] );
					this->addError( SE_SCANER_WARNING, SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE, buf, lex.line, lex.col + ind1 );
				
					break;
				}

				ind1 ++;

				if( szString[ ind1 + 1 ] >= '0' && szString[ ind1 + 1 ] <= '9' )
				{
					charVal = charVal * 16 + szString[ ind1 + 1 ] - '0';
					szString2[ ind2 ] = ( TCHAR ) charVal;
					ind1 ++;
				}
				else if( szString[ ind1 + 1 ] >= 'a' && szString[ ind1 + 1 ] <= 'f' )
				{
					charVal = charVal * 16 + szString[ ind1 + 1 ] - 'a' + 0x0a;
					szString2[ ind2 ] = ( TCHAR ) charVal;
					ind1 ++;
				}
				else if( szString[ ind1 + 1 ] >= 'A' && szString[ ind1 + 1 ] <= 'F' )
				{
					charVal = charVal * 16 + szString[ ind1 + 1 ] - 'A' + 0x0a;
					szString2[ ind2 ] = ( TCHAR ) charVal;
					ind1 ++;
				}
				else 
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
					break;
				}
				
				break;
			default:
				// ���������� ��������� �������
				szString2[ ind2 ] = szString[ ind1 ];
				_stprintf_s( buf, TEXT( "'\\%c'" ), szString[ ind1 ] );

				this->addError( SE_SCANER_WARNING, SSRV_UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE, buf, lex.line, lex.col + ind1 );
				break;
			}
			
			ind1 ++;
			ind2 ++;
		}
	}
	
	return SRV_SUCCESS;
}

bool scriptScaner::isSystemIncludeDir( scriptLexeme &lex )
{

	std::list< scriptLexeme*>::iterator	sli1;
		
	if( lex.type != LT_PARSER || lex.parser != LPT_LESS_THAN )
		return false;
	
	// ���� ������ ������ 3 - �� �� �������
	if( this->directiveLexemes.size() < 1 )
		return false;

	// �������� ��������� �������
	sli1 = this->directiveLexemes.end();
	sli1--;

	// include
	if( (*sli1)->type != LT_RWORD || (*sli1)->rwordType != LRW_INCLUDE )
		return false;
		
	return true;
}

void scriptScaner::createIdReference( scriptLexeme &lex )
{
	
	int				tableIndex;
	scriptLexeme	*lex2;

	// ���� ������������� ��� ���� � �������
	if( this->parent->idExist( lex.name, tableIndex ) )
	{
		// �� ������ �������� ��� ������
		lex.index = tableIndex;
	}
	else 
	{
		// ������� �������� ��� � ������ ��������������� � ����� �������� ��� ������
		lex2 = new scriptLexeme( lex );
		lex2->scaner = this;
		lex.index = (int) this->parent->ids.size();
		
		this->parent->ids.push_back( lex2 );
	}

	// ������� ��� ��������������
	lex.name.clear();

	return ;
}

void scriptScaner::createStringReference( scriptLexeme &lex )
{

	int				tableIndex;
	scriptLexeme	*lex2;
	
	// ���� ���� ��� ����� ������ � �������
	if( this->parent->stringExist( lex.name, tableIndex ) )
	{	
		// �� ������ �������� �� ������
		lex.index = tableIndex;
	}
	else 
	{
		// ��������� �� � ������� ����� � ����� ������ �������� �� ������
		lex2 = new scriptLexeme( lex );
		lex2->scaner = this;
		lex.index = (int) this->parent->constStrings.size();
		
		this->parent->constStrings.push_back( lex2 );
	}
	
	// ������� ����� ������
	lex.name.clear();
				
	return ;
}

scriptRetCode scriptScaner::optimizeStrings( std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli, sli2;
		
	// �������� �� � �������� �������� � � �������
	sli = lexList.begin();
	sli ++;
	for( ; sli != lexList.end(); sli ++ )
	{
		
		// ���� �� ������ - ���������� ������
		if( (*sli)->type != LT_STRING )
			continue;
		
		// ���������� �������		
		sli2 = sli;
		sli2 --;
	
		// ���� ���������� ������� �� ������ - ���������� ������
		if( (*sli2)->type != LT_STRING )
			continue;
		
		// ������������ ������� ������� � ����������
		(*sli2)->name.concat( (*sli)->name );
		// ������� ������� �������
		delete *sli;
		// ������ ������� ���������� ������� �� sli2
		lexList.erase( sli );
		sli = sli2;
	}

	// ������ �� ������� ������ � ������� �����
	for( sli = lexList.begin(); sli != lexList.end(); sli++ )
	{
		if( (*sli)->type == LT_STRING )
			this->createStringReference( *(*sli) );
	}
	
	return SRV_SUCCESS;	
}

scriptRetCode scriptScaner::optimizeIds( std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli;
	
	for( sli = lexList.begin(); sli != lexList.end(); sli++ )
	{
		if( (*sli)->type == LT_ID )
			this->createIdReference( *(*sli) );
	}
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::optimizeMacroUnaryMinusAndPlus( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList )
{

	bool	bOptimize;

	std::list< scriptLexeme* >::iterator	sli1;
	std::list< scriptLexeme* >::iterator	sli2;
	std::list< scriptLexeme* >::iterator	sli3;

	scriptLexeme							*lex1;
	scriptLexeme							*lex2;
	scriptLexeme							*lex3;
	
	// ������ �������������� ������� ���������, ��� �� ��� ����� �� ��������
	// ��������� ���������� ������� �������� ��� ��� ����� ������� �� ������ ������������
	// �������� ������
	// -- - 123
	// ����� ���� ������� ����� 
	// ���� ������ ���� 
	// -- - 123
	// ��� �������� ����� - ���� ������ ���� 
	// id -- - 123
	// �� ������� �������� - �� ��� �������������� ��������� ��� �����
	// � �������������� ���������� ������
	// ������� ������������� �� ������ ������������ �����������
	// ��� �������� ������������ �� ������ ������������ �������

	// � ��� ��� �������� ��� ������, ��������� �������� -- ��� ++ � ��������� 
	// ��� if � elif ���� �� ������

	// ������������ ������� ��������� - � + ��� ��������
	for( sli1 = begin; sli1 != end; )
	{

		// ������ ������� �� ������ ������������� �� �����
		if( sli1 == begin )
		{
			sli1 ++;
			continue;
		}

		// ���������� �������
		sli2 = sli1;
		sli2 --;

		// ������ ������� ����� ������������� �� �����
		if( sli2 != begin )
		{	
		
			sli3 = sli2;
			sli3 --;
			
			lex3 = *sli3;
		}
		
		lex1 = *sli1;
		lex2 = *sli2;
		
		bOptimize = false;

		// ... parser - const
		// ... parser + const
		// - const
		// + const
		if( lex1->isNumericConst() && lex2->type == LT_PARSER && ( sli2 == begin || lex3->type == LT_PARSER ) )
		{
			if( lex2->parser == LPT_SUBTRACTION )
			{
				// ������� �� ������ �����
				if( lex1->isUnsigned )
				{
					this->addError( SE_SCANER_WARNING, SSRV_APPLY_UNARY_MINUS_TO_UNSIGNED, *lex1 );
					lex1->isUnsigned = false;
				}
				lex1->longValue = - lex1->longValue;
				bOptimize = true;
			}
			else if( lex2->parser == LPT_ADDITION )
				bOptimize = true;

			if( bOptimize )
			{
				// ������� ������ �������
				delete lex2;
				lexList.erase( sli2 );

				// ��������� �� ���� ������� ����� ��� ��������� ��������
				sli1 = sli3;
			}
			else
				sli1 ++;
		}
		else // ���� ������ �� ����� �� ������ ��������� ������
			sli1 ++;
	}
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::ppExecuteArithmeticOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp )
{
	switch( opCode.parser )
	{
	case LPT_ADDITION:
		leftOp.operatorPlus( rightOp );
		break;
	case LPT_SUBTRACTION:
		leftOp.operatorMinus( rightOp );
		break;
	case LPT_MULTIPLICATION:
		leftOp.operatorMul( rightOp );
		break;
	case LPT_DIVISION:
		leftOp.operatorDiv( rightOp );  
		break;
	case LPT_REMAINDER:
		leftOp.operatorModulus( rightOp );
		break;
	}	

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::ppExecuteLogicalOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp )
{

	switch( opCode.parser )
	{
	case LPT_GREATER_THAN:
		leftOp.operatorGreater( rightOp );
		break;
	case LPT_GREATER_THAN_OR_EQUAL_TO:
		leftOp.operatorGreaterEqual( rightOp );
		break;
	case LPT_LESS_THAN:
		leftOp.operatorLess( rightOp );
		break;
	case LPT_LESS_THAN_OR_EQUAL_TO:
		leftOp.operatorLessEqual( rightOp );
		break;
	case LPT_EQUALITY:
		leftOp.operatorEquality( rightOp );
		break;
	case LPT_INEQUALITY:
		leftOp.operatorInEquality( rightOp );
		break;
	case LPT_LOGICAL_AND:
		leftOp.operatorLogAnd( rightOp );
		break;
	case LPT_LOGICAL_OR:
		leftOp.operatorLogOr( rightOp );
		break;
	}	

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::ppExecuteBinaryOp( scriptLexeme &leftOp, scriptLexeme &opCode, scriptLexeme &rightOp )
{

	switch( opCode.parser )
	{
	case LPT_BITWISE_AND:
		leftOp.operatorBitAnd( rightOp );
		break;
	case LPT_BITWISE_OR:
		leftOp.operatorBitOr( rightOp );
		break;
	case LPT_BITWISE_EXCLUSIVE_OR:
		leftOp.operatorBitXor( rightOp );
		break;
	case LPT_LEFT_SHIFT:
		leftOp.operatorLeftShift( rightOp );
		break;
	case LPT_RIGHT_SHIFT:
		leftOp.operatorRightShift( rightOp );
		break;
	}	

	return SRV_SUCCESS;
}

fileNameInfo* scriptScaner::getFileNameInfo(int fileIndex) { 
	
	std::list<fileNameInfo>::iterator fii;
	int i;
	for(fii = this->getFilesList().begin(), i = 0; fii != this->getFilesList().end(); fii++, i++ ) { 
		if(i == fileIndex) return &(*fii);
	}
	
	return NULL;
}