#include "fileinfo.h"

fileInfo::fileInfo()
{
	
	this->fp			= NULL;
	this->fileLength	= 0;
	this->filePos		= 0;
	this->line			= 0;
	this->col			= 0;
	this->ppState		= PPS_GET_SYMBOL;
}

fileInfo::~fileInfo()
{
	this->clear();
}

void fileInfo::clear()
{

	// если собственный буффер, т. е. выделили память сами 
	if( this->isOwnBuffer && this->fp != NULL )
		delete[] this->fp;
	
	this->isOwnBuffer			= false;

	this->col				= 0;
	this->fileLength		= 0;
	this->filePos			= 0;
	this->fp				= NULL;
	this->line				= 0;
	
	this->directory.clear();
	this->fileName.clear();
	this->fullPathName.clear();

	return ;
}

scriptRetCode fileInfo::openFile( TCHAR *fileName )
{
	TCHAR	tempBuffer[2048];
	TCHAR	*fileNameBegin;

	// получаем нормальное имя файла
	if( GetFullPathName( fileName, 2048, tempBuffer, &fileNameBegin ) == NULL )
		return SRV_FAIL;
	
	FILE	*filep;
	
	_tfopen_s( &filep, tempBuffer, TEXT("rb") );
	if( filep == NULL )
		return SRV_FAIL;

	// инициализация параметров
	this->isOwnBuffer			= true;
	this->fullPathName			= tempBuffer;
	this->directory				= this->fullPathName;
	this->directory.extractDirectory();
	this->fileName				= fileNameBegin;
	this->fileLength			= this->getFileSize( filep );

	// наконец считываем данные из файла
	this->fp					= new BYTE[ this->fileLength ];
	fread( this->fp, sizeof( BYTE ), this->fileLength, filep );
	
	fclose( filep );

	return SRV_SUCCESS;
}

DWORD fileInfo::getFileSize( FILE *filep )
{
	DWORD oldPos, size;

	oldPos = ftell( filep );
	fseek( filep, 0, SEEK_END );
	size = ftell( filep );
	fseek( filep, oldPos, SEEK_SET );

	return size;
}

scriptRetCode fileInfo::openFile( shString &dir, shString &file, shString &filePath, FILE *filep )
{
	
	this->directory				= dir;
	this->fileName				= file;
	this->fullPathName			= filePath;
	this->line					= 0;
	this->col					= 0;
	this->filePos				= 0;

	this->fileLength			= this->getFileSize( filep );
	this->fp					= new BYTE[ this->fileLength ];
	fread( this->fp, sizeof( BYTE ), this->fileLength, filep );
		
	this->isOwnBuffer			= true;

	return SRV_SUCCESS;
}

scriptRetCode fileInfo::openBuffer( BYTE *buffer, DWORD bufLength, int ln, int cl )
{

	// поскольку буффер нам предоставлен со стороны, то не нам его удалять
	this->isOwnBuffer		= false;
	this->directory			= TEXT("");
	this->fileName			= TEXT("");
	this->fullPathName		= TEXT("");
	this->fp				= buffer;
	this->fileLength		= bufLength;

	this->line				= ln;
	this->col				= cl;
	
	return SRV_SUCCESS;
}