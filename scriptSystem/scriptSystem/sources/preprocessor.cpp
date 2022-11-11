#include "preprocessor.h"
#include "io.h"


scriptPreProcessor::scriptPreProcessor()
{
	// просто очищаем структуру
	this->fileCount		= 0;
		
	ZeroMemory( this->files, sizeof( FILE* ) * SPP_FILE_MAX_COUNT );
	this->fpOut			= NULL;

}


scriptPreProcessor::~scriptPreProcessor()
{
	// полная очистка структуры
	clear();
}


scriptRetCode scriptPreProcessor::initialize( std::list< shString > &incDir, shString &progPath )
{

	// сначала все очищаем
	clear();

	std::list< shString >::iterator		shsi;
	
	// копирование списка системных директорий
	for( shsi = incDir.begin(); shsi != incDir.end(); shsi ++ )
		this->includeDirList.push_back( *shsi );
	
	// копирование директории программы
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

	// сначала освобождаемся от директорий
	if( !this->includeDirList.empty() )
	{
		// сначала очищаем строки
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
			shsi->clear();
		
		// затем сам список
		this->includeDirList.clear();
	}
	
	// далее список ошибок и предупреждений
	if( !this->errorList.empty() )
	{
		// сначала очищаем строки
		for( sei = this->errorList.begin(); sei != this->errorList.end(); sei ++ )
		{
			(*sei)->text.clear();
			delete *sei;
		}
		
		// затем сам список
		this->errorList.clear();
	}

	// далее список ошибок и предупреждений
	if( !this->warningList.empty() )
	{
		// сначала очищаем строки
		for( sei = this->warningList.begin(); sei != this->warningList.end(); sei ++ )
		{
			(*sei)->text.clear();
			delete *sei;
		}
		
		// затем сам список
		this->warningList.clear();
	}

	// освобождаемся от макроса
	if( !this->macros.empty() )
	{
		// сначала очищаем инфу для каждого макроса
		for( smi = this->macros.begin(); smi != this->macros.end(); smi ++ )
		{
			
			// для каждого макроса освобождаем его информацию
			smi->called = false;
			smi->line	= 0;
			smi->col	= 0;
			smi->name.clear();

			// освобождаемся от списка параметров
			for( sli = smi->params.begin(); sli != smi->params.end(); sli ++ )
				(*sli)->clear();
		
			// освобождаемся от тела макроса
			for( sli = smi->body.begin(); sli != smi->body.end(); sli ++ )
				(*sli)->clear();

		}
		
		// теперь освобождаемся от самого списка
		this->macros.clear();
	
	}

	// теперь файлы
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
	
	// и файл выхода 
	if( this->fpOut != NULL )
		fclose( this->fpOut );
	this->fpOut = NULL;
	
	this->programmPath = TEXT("");
	this->outFileName  = TEXT("");

	// в принципе всегда возвращается успешно
	return SRV_SUCCESS;
}


// параметр inSystem указывает, что надо искать в системных директориях
scriptRetCode scriptPreProcessor::openInputFile( TCHAR *fileName, bool inSystem )
{

	std::list< shString >::iterator		shsi;
	shString							hfileName( fileName );
	shString							fullPathName;

	std::list< shString >::iterator		dirIter;
	TCHAR								*fileNameBegin;
	FILE								*fp;
		
	// проверка: можно ли открыть файл
	if( this->fileCount == SPP_FILE_MAX_COUNT )
		return SPPRV_FILE_LIMIT_COUNT;

	// далее по списку директорий составляем полное имя файла, так, чтобы его можно было открыть
	// если надо искать в системных папках
	if( inSystem )
	{
	
		// проходимся по всем директориям и ищем данный файл
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
		{
			// составляем полное имя файла
			getFullPathName( *shsi, hfileName, fullPathName ); 
		
			// пытаемся для начала его открыть
			fp = _tfopen( fullPathName.getCString(), TEXT("rb") );
			if( fp == NULL )
				continue;
			
			// запоминаем директорию
			dirIter = shsi;
			break;
		}

		// соответственно если пройден весь список то 
		// файл не найден ни в одной системной папке
		if( shsi == this->includeDirList.end() )
			return SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR;

		// если массив пройден не до конца, то такой файл уже открыт
		if( this->isFileOpened( fullPathName ) )
		{
			fclose( fp );
			return SPPRV_FILE_RECURSION;
		}

		// все проверки пройдены надо добавить файл
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
		
		
		// увеличиваем число файлов
		fileCount++;
	}
	else
	{
		// если не системный файл, то необходимо действовать по рекурсии
		// т.е. если не указан полный путь к файлу, то необходимо, основываясь на директории, последнего 
		// открытого файла получить полный путь к файлу
		
		// проверяем - указан ли полный путь
		if( ( ( fileName[0] >= 'A' && fileName[0] <= 'Z' ) || ( fileName[0] >= 'a' && fileName[0] <= 'z' ) ) &&
			fileName[1] == ':' && fileName[2] == '\\' )
		{
			// да, указан полный путь
			fullPathName = fileName;
		}
		else 
		{
			// нет полный путь не указан, пытаемся получит полный путь
			
			// если какой то файл уже открыт, то действуем от него,
			if( this->fileCount > 0 )
				fullPathName = this->files[ this->fileCount - 1 ].directory;
			else	// в противном случае действуем от текущей директории
				fullPathName = this->programmPath;
			
			fullPathName += TEXT( "\\" );
			fullPathName += fileName;
		}

		// получаем нормальное имя файла
		if( GetFullPathName( fullPathName.getCString(), 2048, tempBuffer, &fileNameBegin ) == NULL )
			return SPPRV_CANT_OPEN_READ_FILE;
		
		fullPathName = tempBuffer;		
						
		if( this->isFileOpened( fullPathName ) )
			return SPPRV_FILE_RECURSION;
		
		// мы нашли файл теперь нам надо из полного имени
		// также получить имя директории и имя файла
		this->files[ this->fileCount ].fullPathName = fullPathName;
		
		// имя файла
		this->files[ this->fileCount ].fileName = this->files[ this->fileCount ].fullPathName;
		this->files[ this->fileCount ].fileName.extractFileName();

		// директория
		this->files[ this->fileCount ].directory = fullPathName;
		this->files[ this->fileCount ].directory.extractDirectory();

		// остальные атрибуты
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

// функция препроцесса последнего файла
// 
scriptRetCode scriptPreProcessor::preProcess()
{
	
	// проверка: открыт ли выходной файл
	if( this->fpOut == NULL )
		return SPPRV_OUTPUT_FILE_NOT_OPENED;
	
	
	fileInfo				*fInfo;								// информация о файле
	bool					firstSymbolInLineRead;				// если в новой строке был уже прочитан символ - то истина
	scriptError				*err;
	scriptMacros			mac;
	// состояние автомата
	preProcessorState		ppState;							// состояние препроцессора
	
	fInfo					= &this->files[ this->fileCount - 1 ]; 
	firstSymbolInLineRead	= false;					// пока ни один символ не прочитан
	ppState					= PPS_GET_SYMBOL;

	// вечный цикл
	// в этом цикле идет обработка файла
	for( ;; )
	{
	
		if( ppState == PPS_GET_SYMBOL )
		{
			
			// если это не последний символ и достигли перевода строки
			if( fInfo->filePos < fInfo->fileLength - 1 && fInfo->fp[ fInfo->filePos ] == 0x0d && fInfo->fp[ fInfo->filePos + 1] == 0x0a )
			{
				
				// записываем это в выходной поток 
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 2, fpOut );
				// увеличиваем позицию файла на 2
				fInfo->filePos += 2;
				// обновляем признак флагов
				firstSymbolInLineRead = false;

				// обновляем информацию о файле
				fInfo->col = 0;
				fInfo->line ++;

				// продолжаем считывание
				continue;
			}

			// пытаемся найти символ директивы препроцессора
			//if( firstSymbolInLineRead == false && this->isDirective( fInfo->fp[ fInfo->filePos ] ) )
			//{
				
				// обрабатываем директиву
				// this->processDirective( fInfo );	

				// продолжаем считывание
				// continue;

			//}
			
			// проверяем, может начиная с этого символа идет макрос?
			//if( this->isMacros( fInfo->fp[ fInfo->filePos ], mac ) )
			//{
			
				// обрабатываем макрос
				// this->processMacros( fInfo );
				
				// продолжаем считывание
				//continue;
			//}
			
			// проверяем если это двойные ковычки
			if( fInfo->fp[ fInfo->filePos ] == '\"' )
			{
				// их надо записать в выходной файл
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 1, this->fpOut );
				// увеличиваем позицию файла
				fInfo->filePos ++;
				// обновляем признак флагов
				firstSymbolInLineRead = true;
				// обновляем информацию о файле
				fInfo->col ++;

				// переходим в состояние считывания строки ( т.е. пропускаются все макросы )
				fInfo->ppState = PPS_GET_STRING;

				// продолжаем считывание 
				continue;
			
			}

			// проверяем - а вдруг это управляющий символ препроцессора #
			if( fInfo->fp[ fInfo->filePos ] == '#' )
			{
				
				// выдать ошибку, поскольку управляющий символ не может идти после обычных символов
				err					= new scriptError;
				err->code			= SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST;
				err->fileIndex		= fInfo->fileIndex;
				err->text			= TEXT( "Символ директивы препроцессора '#' должен быть первым символом в линии, кроме пробела и символа таблуляции." );
				err->line			= fInfo->line;
				err->col			= fInfo->col;
				
				// добавим ошибку в список ошибок
				this->errorList.push_back( err );

				// попытаемся очухаться после ошибки - 
				// дойдем до конца строки или конца файла
				this->runUntilEOL( *fInfo );

				// продолжим считывание 
				continue;

			}

			// этот символ можно смело добавлять в файл выхода
			{
			
				// их надо записать в выходной файл
				fwrite( &fInfo->fp[ fInfo->filePos ], sizeof( BYTE ), 1, this->fpOut );
				// увеличиваем позицию файла
				fInfo->filePos ++;
				// обновляем признак флагов
				firstSymbolInLineRead = true;
				// обновляем информацию о файле
				fInfo->col ++;
				
				// продолжаем считывание 
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