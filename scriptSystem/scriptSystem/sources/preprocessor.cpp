#include "preprocessor.h"
#include "io.h"


scriptPreProcessor::scriptPreProcessor()
{
	// ������ ������� ���������
	this->fileCount		= 0;
		
	ZeroMemory( this->files, sizeof( FILE* ) * SPP_FILE_MAX_COUNT );
	this->fpOut			= NULL;

}


scriptPreProcessor::~scriptPreProcessor()
{
	// ������ ������� ���������
	clear();
}


scriptRetCode scriptPreProcessor::initialize( std::list< shString > &incDir, shString &progPath )
{

	// ������� ��� �������
	clear();

	std::list< shString >::iterator		shsi;
	
	// ����������� ������ ��������� ����������
	for( shsi = incDir.begin(); shsi != incDir.end(); shsi ++ )
		this->includeDirList.push_back( *shsi );
	
	// ����������� ���������� ���������
	this->programmPath = progPath;

	return SRV_SUCCESS;
}


scriptRetCode scriptPreProcessor::clear()
{
	
	std::list< shString >::iterator		shsi;
	std::list< scriptLexeme* >::iterator sli;
	std::list< scriptMacros >::iterator	smi;
	std::list< scriptError* >::iterator	sei;
	int									fileIndex;

	// ������� ������������� �� ����������
	if( !this->includeDirList.empty() )
	{
		// ������� ������� ������
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
			shsi->clear();
		
		// ����� ��� ������
		this->includeDirList.clear();
	}
	
	// ����� ������ ������ � ��������������
	if( !this->errorList.empty() )
	{
		// ������� ������� ������
		for( sei = this->errorList.begin(); sei != this->errorList.end(); sei ++ )
		{
			(*sei)->text.clear();
			delete *sei;
		}
		
		// ����� ��� ������
		this->errorList.clear();
	}

	// ����� ������ ������ � ��������������
	if( !this->warningList.empty() )
	{
		// ������� ������� ������
		for( sei = this->warningList.begin(); sei != this->warningList.end(); sei ++ )
		{
			(*sei)->text.clear();
			delete *sei;
		}
		
		// ����� ��� ������
		this->warningList.clear();
	}

	// ������������� �� �������
	if( !this->macros.empty() )
	{
		// ������� ������� ���� ��� ������� �������
		for( smi = this->macros.begin(); smi != this->macros.end(); smi ++ )
		{
			
			// ��� ������� ������� ����������� ��� ����������
			smi->called = false;
			smi->line	= 0;
			smi->col	= 0;
			smi->name.clear();

			// ������������� �� ������ ����������
			for( sli = smi->params.begin(); sli != smi->params.end(); sli ++ )
				(*sli)->clear();
		
			// ������������� �� ���� �������
			for( sli = smi->body.begin(); sli != smi->body.end(); sli ++ )
				(*sli)->clear();

		}
		
		// ������ ������������� �� ������ ������
		this->macros.clear();
	
	}

	// ������ �����
	if( this->fileCount != 0 )
	{
		for( fileIndex = 0; fileIndex < this->fileCount; fileIndex ++ )
		{
			this->files[ fileIndex ].directory.clear();
			this->files[ fileIndex ].fileName.clear();
			this->files[ fileIndex ].fullPathName.clear();
			this->files[ fileIndex ].line		= 0;
			this->files[ fileIndex ].col		= 0;
			this->files[ fileIndex ].fileLength = 0;
			this->files[ fileIndex ].filePos	= 0;

			if( this->files[ fileIndex ].fp != NULL )
				delete[] this->files[ fileIndex ].fp;
			this->files[ fileIndex ].fp = NULL;
			
		}
	}
	this->fileCount		= 0;
	
	// � ���� ������ 
	if( this->fpOut != NULL )
		fclose( this->fpOut );
	this->fpOut = NULL;
	
	this->programmPath = TEXT("");
	this->outFileName  = TEXT("");

	// � �������� ������ ������������ �������
	return SRV_SUCCESS;
}


// �������� inSystem ���������, ��� ���� ������ � ��������� �����������
scriptRetCode scriptPreProcessor::openInputFile( TCHAR *fileName, bool inSystem )
{

	std::list< shString >::iterator		shsi;
	shString							hfileName( fileName );
	shString							fullPathName;

	std::list< shString >::iterator		dirIter;
	TCHAR								*fileNameBegin;
	FILE								*fp;
		
	// ��������: ����� �� ������� ����
	if( this->fileCount == SPP_FILE_MAX_COUNT )
		return SPPRV_FILE_LIMIT_COUNT;

	// ����� �� ������ ���������� ���������� ������ ��� �����, ���, ����� ��� ����� ���� �������
	// ���� ���� ������ � ��������� ������
	if( inSystem )
	{
	
		// ���������� �� ���� ����������� � ���� ������ ����
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
		{
			// ���������� ������ ��� �����
			getFullPathName( *shsi, hfileName, fullPathName ); 
		
			// �������� ��� ������ ��� �������
			fp = _tfopen( fullPathName.getCString(), TEXT("rb") );
			if( fp == NULL )
				continue;
			
			// ���������� ����������
			dirIter = shsi;
			break;
		}

		// �������������� ���� ������� ���� ������ �� 
		// ���� �� ������ �� � ����� ��������� �����
		if( shsi == this->includeDirList.end() )
			return SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR;

		// ���� ������ ������� �� �� �����, �� ����� ���� ��� ������
		if( this->isFileOpened( fullPathName ) )
		{
			fclose( fp );
			return SPPRV_FILE_RECURSION;
		}

		// ��� �������� �������� ���� �������� ����
		files[ fileCount ].directory		= *dirIter;
		files[ fileCount ].fileName			= hfileName;
		files[ fileCount ].fullPathName		= fullPathName;
		files[ fileCount ].line				= 0;
		files[ fileCount ].col				= 0;
		files[ fileCount ].filePos			= 0;

		files[ fileCount ].fileLength		= filelength( (int)fp );
		files[ fileCount ].fp				= new BYTE[ files[ fileCount ].fileLength ];
		fread( files[ fileCount ].fp, sizeof( BYTE ), files[ fileCount ].fileLength, fp );
		fclose( fp );
		
		
		// ����������� ����� ������
		fileCount++;
	}
	else
	{
		// ���� �� ��������� ����, �� ���������� ����������� �� ��������
		// �.�. ���� �� ������ ������ ���� � �����, �� ����������, ����������� �� ����������, ���������� 
		// ��������� ����� �������� ������ ���� � �����
		
		// ��������� - ������ �� ������ ����
		if( ( ( fileName[0] >= 'A' && fileName[0] <= 'Z' ) || ( fileName[0] >= 'a' && fileName[0] <= 'z' ) ) &&
			fileName[1] == ':' && fileName[2] == '\\' )
		{
			// ��, ������ ������ ����
			fullPathName = fileName;
		}
		else 
		{
			// ��� ������ ���� �� ������, �������� ������� ������ ����
			
			// ���� ����� �� ���� ��� ������, �� ��������� �� ����,
			if( this->fileCount > 0 )
				fullPathName = this->files[ this->fileCount - 1 ].directory;
			else	// � ��������� ������ ��������� �� ������� ����������
				fullPathName = this->programmPath;
			
			fullPathName += TEXT( "\\" );
			fullPathName += fileName;
		}

		// �������� ���������� ��� �����
		if( GetFullPathName( fullPathName.getCString(), 2048, tempBuffer, &fileNameBegin ) == NULL )
			return SPPRV_CANT_OPEN_READ_FILE;
		
		fullPathName = tempBuffer;		
						
		if( this->isFileOpened( fullPathName ) )
			return SPPRV_FILE_RECURSION;
		
		// �� ����� ���� ������ ��� ���� �� ������� �����
		// ����� �������� ��� ���������� � ��� �����
		this->files[ this->fileCount ].fullPathName = fullPathName;
		
		// ��� �����
		this->files[ this->fileCount ].fileName = this->files[ this->fileCount ].fullPathName;
		this->files[ this->fileCount ].fileName.extractFileName();

		// ����������
		this->files[ this->fileCount ].directory = fullPathName;
		this->files[ this->fileCount ].directory.extractDirectory();

		// ��������� ��������
		fp = _tfopen( this->files[ this->fileCount ].fullPathName.getCString(), TEXT("rb") );
		if( this->files[ this->fileCount ].fp == NULL )
		{
			return SPPRV_CANT_OPEN_READ_FILE; 
		}

		files[ fileCount ].filePos			= 0;

		files[ fileCount ].fileLength		= filelength( (int)fp );
		files[ fileCount ].fp				= new BYTE[ files[ fileCount ].fileLength ];
		fread( files[ fileCount ].fp, sizeof( BYTE ), files[ fileCount ].fileLength, fp );
		fclose( fp );
		
		this->files[ this->fileCount ].line = 0;
		this->files[ this->fileCount ].col	= 0;
		
		this->fileCount ++;
	}

	return SRV_SUCCESS;
}

scriptRetCode scriptPreProcessor::closeInputFile()
{

	if( this->fileCount )
	{
		
		this->files[ this->fileCount - 1 ].directory.clear();
		this->files[ this->fileCount - 1 ].fileName.clear();
		this->files[ this->fileCount - 1 ].fullPathName.clear();
		
		if( this->files[ this->fileCount - 1 ].fp != NULL)
			delete[] this->files[ this->fileCount - 1 ].fp;

		this->files[ this->fileCount - 1 ].line = 0;
		this->files[ this->fileCount - 1 ].col  = 0;
		
		this->fileCount --;
	
	}

	return SRV_SUCCESS;
}

scriptRetCode scriptPreProcessor::openOutputFile( TCHAR *fileName )
{

	if( this->fpOut != NULL )
		fclose( this->fpOut );

	this->fpOut = NULL;
	_tfopen_s( &this->fpOut, fileName, TEXT( "wb" ) );
	if( this->fpOut == NULL )
		return SPPRV_CANT_OPEN_WRITE_FILE;
	
	this->outFileName = TEXT("");
	
	return SRV_SUCCESS;
}

// ������� ����������� ���������� �����
// 
scriptRetCode scriptPreProcessor::preProcess()
{
	
	// ��������: ������ �� �������� ����
	if( this->fpOut == NULL )
		return SPPRV_OUTPUT_FILE_NOT_OPENED;
	
	
	fileInfo				*fInfo;								// ���������� � �����
	bool					firstSymbolInLineRead;				// ���� � ����� ������ ��� ��� �������� ������ - �� ������
	scriptError				*err;
	scriptMacros			mac;
	// ��������� ��������
	preProcessorState		ppState;							// ��������� �������������
	
	fInfo					= &this->files[ this->fileCount - 1 ]; 
	firstSymbolInLineRead	= false;					// ���� �� ���� ������ �� ��������
	ppState					= PPS_GET_SYMBOL;

	// ������ ����
	// � ���� ����� ���� ��������� �����
	for( ;; )
	{
	
		if( ppState == PPS_GET_SYMBOL )
		{
			
			// ���� ��� �� ��������� ������ � �������� �������� ������
			if( fInfo->filePos < fInfo->fileLength - 1 && fInfo->fp[ fInfo->filePos ] == 0x0d && fInfo->fp[ fInfo->filePos + 1] == 0x0a )
			{
				
				// ���������� ��� � �������� ����� 
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 2, fpOut );
				// ����������� ������� ����� �� 2
				fInfo->filePos += 2;
				// ��������� ������� ������
				firstSymbolInLineRead = false;

				// ��������� ���������� � �����
				fInfo->col = 0;
				fInfo->line ++;

				// ���������� ����������
				continue;
			}

			// �������� ����� ������ ��������� �������������
			//if( firstSymbolInLineRead == false && this->isDirective( fInfo->fp[ fInfo->filePos ] ) )
			//{
				
				// ������������ ���������
				// this->processDirective( fInfo );	

				// ���������� ����������
				// continue;

			//}
			
			// ���������, ����� ������� � ����� ������� ���� ������?
			//if( this->isMacros( fInfo->fp[ fInfo->filePos ], mac ) )
			//{
			
				// ������������ ������
				// this->processMacros( fInfo );
				
				// ���������� ����������
				//continue;
			//}
			
			// ��������� ���� ��� ������� �������
			if( fInfo->fp[ fInfo->filePos ] == '\"' )
			{
				// �� ���� �������� � �������� ����
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 1, this->fpOut );
				// ����������� ������� �����
				fInfo->filePos ++;
				// ��������� ������� ������
				firstSymbolInLineRead = true;
				// ��������� ���������� � �����
				fInfo->col ++;

				// ��������� � ��������� ���������� ������ ( �.�. ������������ ��� ������� )
				fInfo->ppState = PPS_GET_STRING;

				// ���������� ���������� 
				continue;
			
			}

			// ��������� - � ����� ��� ����������� ������ ������������� #
			if( fInfo->fp[ fInfo->filePos ] == '#' )
			{
				
				// ������ ������, ��������� ����������� ������ �� ����� ���� ����� ������� ��������
				err					= new scriptError;
				err->code			= SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST;
				err->fileIndex		= fInfo->fileIndex;
				err->text			= TEXT( "������ ��������� ������������� '#' ������ ���� ������ �������� � �����, ����� ������� � ������� ����������." );
				err->line			= fInfo->line;
				err->col			= fInfo->col;
				
				// ������� ������ � ������ ������
				this->errorList.push_back( err );

				// ���������� ��������� ����� ������ - 
				// ������ �� ����� ������ ��� ����� �����
				this->runUntilEOL( *fInfo );

				// ��������� ���������� 
				continue;

			}

			// ���� ������ ����� ����� ��������� � ���� ������
			{
			
				// �� ���� �������� � �������� ����
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 1, this->fpOut );
				// ����������� ������� �����
				fInfo->filePos ++;
				// ��������� ������� ������
				firstSymbolInLineRead = true;
				// ��������� ���������� � �����
				fInfo->col ++;
				
				// ���������� ���������� 
				continue;

			}

		} // if( fInfo->ppState == PPS_GET_SYMBOL )

	}
	
	return SRV_SUCCESS;
}

void scriptPreProcessor::runUntilEOL( fileInfo &fInfo )
{
	
	while ( 1 )
	{
	
		if( fInfo.filePos < fInfo.fileLength - 1 && fInfo.fp[ fInfo.filePos ] == 0x0d && fInfo.fp[ fInfo.filePos ] == 0x0a )
			break;

		if( fInfo.filePos == fInfo.fileLength )
			break;
	
		fInfo.filePos ++;
	
	}

	return ;
}

bool scriptPreProcessor::isMacros( fileInfo &fInfo, scriptMacros &mac )
{
	
	
	
	return false;
}

scriptRetCode scriptPreProcessor::getFullPathName( shString &directory, shString &fileName, shString &fullPathName )
{

	fullPathName	= directory;
	fullPathName   += TEXT("\\");
	fullPathName   += fileName;	

	return SRV_SUCCESS;
}


bool scriptPreProcessor::isFileOpened( shString &fileName )
{

	shString str1 = fileName;
	shString str2;
	int		 fileIndex;

	str1.toLowerCase();

	for( fileIndex = 0; fileIndex < this->fileCount; fileIndex++ )
	{
		str2 = this->files[ fileIndex ].fullPathName;
		str2.toLowerCase();

		if( str1 == str2 )
			return true;
	}

	return false;
}