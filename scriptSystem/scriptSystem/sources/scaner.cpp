#include "scaner.h"
#include <stdio.h>
#include <io.h>

/************************************************************************/
/* массив указывающий на тип операциий						            */
/* унарный бинарный тернарный и т.д. для макроса						*/
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
/* описание типа операции - арифметическая логическая или побитовая		*/
/* 0 - никакая															*/
/* 1 - арифметическая													*/
/* 2 - логическая														*/
/* 3 - побитовая														*/
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
/* простой конструктор                                                  */
/* инициализирует элементы по умолчанию									*/
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
/* десруктор - очищает все ресурсы, освобождает память и т. д.          */
/************************************************************************/
scriptScaner::~scriptScaner()
{
	this->clear();
}

/************************************************************************/
/* инициализация системных переменных сканера                           */
/* входной параметр - список строк - путей директорий с системными      */
/* заголовочными файлами												*/
/************************************************************************/
scriptRetCode scriptScaner::initialize( std::list< shString > &incDir )
{

	// сначала все очищаем
	clear();

	std::list< shString >::iterator		shsi;
	
	// копирование списка системных директорий
	for( shsi = incDir.begin(); shsi != incDir.end(); shsi ++ )
		this->includeDirList.push_back( *shsi );
	
	// получаем информацию о программе
	programmDirectory.getModuleDirectory();
	programmFileName.getModuleFileName();
	programmFullPathName.getModuleFullPathName();

	return SRV_SUCCESS;
}

/************************************************************************/
/* инициализация сканера без всяких системных директорий				*/
/*													                    */
/************************************************************************/
scriptRetCode scriptScaner::initialize( )
{
	std::list< shString >	emptyList;
	return this->initialize( emptyList );
}

/************************************************************************/
/* инициализация сканера, как вложенного для другого сканера            */
/* входной параметр: указатель на предыдущий сканер                     */
/************************************************************************/
scriptRetCode scriptScaner::initialize( scriptScaner *prev )
{

	// сначала все очищаем
	clear();

	if( prev->parent == NULL )
		this->parent		= prev;
	else
		this->parent		= prev->parent;
	
	this->prevScaner		= prev;
	
	return SRV_SUCCESS;
}

/************************************************************************/
/* очистака результатов сканирования                                    */
/* списка лексем, таблицы идентификаторов, строк, списка ошибок,		*/
/* макросов																*/
/************************************************************************/
scriptRetCode scriptScaner::clearScanerResult()
{

	std::list< scriptMacros* >::iterator	smi;
	std::list< scriptError* >::iterator		sei;
	std::list< scriptLexeme* >::iterator	sli;
	
	// освобождаемся от макроса
	if( !this->macros.empty() )
	{
		// сначала очищаем инфу для каждого макроса
		for( smi = this->macros.begin(); smi != this->macros.end(); smi ++ )
			delete *smi;
				
		// теперь освобождаемся от самого списка
		this->macros.clear();
	
	}
	
	// список ошибок и предупреждений
	if( !this->errorList.empty() )
	{
		// сначала очищаем строки
		for( sei = this->errorList.begin(); sei != this->errorList.end(); sei ++ )
			delete (*sei);
				
		// затем сам список
		this->errorList.clear();
	}
			
	this->state					= SSS_NONE;
	
	// смотрим если есть следующий сканер - то сначала очистим его
	if( this->nextScaner )
		delete	this->nextScaner;
		
	this->parent			= this;
	this->nextScaner		= NULL;
	this->prevScaner		= NULL;

	// теперь освобождаемся от лексем
	clearLexList( this->lexemes );
	// идентификаторов
	clearLexList( this->ids );
	// и строковых констант
	clearLexList( this->constStrings );
	// список лексем директивы препроцессора
	clearLexList( this->directiveLexemes );	
	
	// список лексем макросов
	clearLexList( this->inputMacrosList );	
	clearLexList( this->outputMacrosList );
	
	// очищаем список точек условных компиляций
	((std::deque<scanSkipType>&)this->skipScanning._Get_container()).clear();

	// очищаем список информации об имени файла
	this->scanedFiles.clear();
		
	// очищаем индикатор ошибки
	this->bError = false;

	return SRV_SUCCESS;
}

/************************************************************************/
/* полная очистка класса: результатов сканирования и системных          */
/* переменных															*/
/************************************************************************/
scriptRetCode scriptScaner::clear()
{
	
	std::list< shString >::iterator			shsi;
		
	// сначала освобождаемся от директорий
	if( !this->includeDirList.empty() )
	{
		// сначала очищаем строки
		for( shsi = this->includeDirList.begin(); shsi != this->includeDirList.end(); shsi ++ )
			shsi->clear();
		
		// затем сам список
		this->includeDirList.clear();
	}
	
	// от информации о программе
	this->programmDirectory.clear();
	this->programmFileName.clear();
	this->programmFullPathName.clear();
	
	this->clearScanerResult();

	return SRV_SUCCESS;
}

/************************************************************************/
/* получить индекс файла в таблице файлов                               */
/* входные параметры информация о файле и указатель на получаемый номер	*/
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

	// пытаемся получить индекс файла
	this->getFileIndex( fi, &fileInd );
	
	if( fileInd == -1 )
	{
		// индекс файла
		fi.fileIndex	= (long) this->parent->scanedFiles.size();
		
		// создаем структуру - инфа об файле
		fNameInfo.directory		= fi.directory;
		fNameInfo.fileName		= fi.fileName;
		fNameInfo.fullPathName	= fi.fullPathName;

		// добавляем ее в файл
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

	// сначала очищаем всю предыдущую инфрмацию
	this->clearScanerResult();
	
	// пытаемся открыть файл
	if( fInfo.openFile( fileName ) == SRV_FAIL )
		return SSRV_CANT_OPEN_FILE;

	// сброс состояния сканера в связи с новыми данными
	this->state						= SSS_NONE;
	
	// добавляем файл в таблицу
	this->addFileToList( this->fInfo );
	
	return SRV_SUCCESS;
}


scriptRetCode scriptScaner::openIncludeFile( shString &fileName, bool inSystem ){

	std::list< shString >::iterator		shsi;
	shString							fullPathName;

	std::list< shString >::iterator		dirIter;
	TCHAR								*fileNameBegin;
	FILE								*fp;
			
	// далее по списку директорий составляем полное имя файла, так, чтобы его можно было открыть
	// если надо искать в системных папках
	if( inSystem )
	{	
	
		// проходимся по всем директориям и ищем данный файл
		for( shsi = this->parent->includeDirList.begin(); shsi != this->parent->includeDirList.end(); shsi ++ )
		{
			// составляем полное имя файла
			fullPathName	= *shsi;
			fullPathName	+= TEXT("\\");
			fullPathName	+= fileName;
			
			// пытаемся для начала его открыть
			_tfopen_s( &fp, fullPathName.getCString(), TEXT("rb") );
			if( fp == NULL )
				continue;
			
			// запоминаем директорию
			dirIter = shsi;
			break;
		}

		// соответственно если пройден весь список то 
		// файл не найден ни в одной системной папке
		if( shsi == this->parent->includeDirList.end() )
			return SPPRV_NO_SUCH_FILE_IN_SYSTEM_INCLUDE_DIR;

		// если массив пройден не до конца, то такой файл уже открыт
		if( this->isFileOpened( fullPathName ) )
		{
			fclose( fp );
			return SPPRV_FILE_RECURSION;
		}

		// все проверки пройдены надо добавить файл
		this->fInfo.openFile( *dirIter, fileName, fullPathName, fp );
		fclose( fp );
		
		// добавляем файл в таблицу
		this->addFileToList( this->fInfo );
	}
	else
	{
		// если не системный файл, то необходимо действовать по рекурсии
		// т.е. если не указан полный путь к файлу, то необходимо, основываясь на директории, последнего 
		// открытого файла получить полный путь к файлу
		
		// проверяем - указан ли полный путь
		if( ( ( fileName.getCString()[0] >= 'A' && fileName.getCString()[0] <= 'Z' ) || ( fileName.getCString()[0] >= 'a' && fileName.getCString()[0] <= 'z' ) ) &&
			fileName.getCString()[1] == ':' && fileName.getCString()[2] == '\\' )
		{
			// да, указан полный путь
			fullPathName = fileName;
		}
		else 
		{
			// нет полный путь не указан, пытаемся получит полный путь
			
			// если какой то файл уже открыт, то действуем от него,
			if( this->prevScaner != NULL )
				fullPathName = this->prevScaner->fInfo.directory;
			else	// в противном случае действуем от текущей директории
				fullPathName = this->programmDirectory;
			
			fullPathName += TEXT( "\\" );
			fullPathName += fileName;
				
		}

		// получаем нормальное имя файла
		if( GetFullPathName( fullPathName.getCString(), 2048, this->tempBuffer, &fileNameBegin ) == NULL )
			return SPPRV_CANT_OPEN_READ_FILE;

		fullPathName = this->tempBuffer;
		
		if( this->isFileOpened( fullPathName ) )
			return SPPRV_FILE_RECURSION;

		if( fInfo.openFile( fullPathName.getCString() ) == SRV_FAIL )
			return SPPRV_CANT_OPEN_READ_FILE;

		// добавляем файл в таблицу
		this->addFileToList( this->fInfo );
			
	}

	return SRV_SUCCESS;		
}

scriptRetCode scriptScaner::addBuffer( BYTE *buffer, DWORD bufLength, int line, int col )
{

	// сначала очищаем
	this->clearScanerResult();

	// открываем буффер
	this->fInfo.openBuffer( buffer, bufLength, line, col );

	// сброс состояния сканера в связи с новыми данными
	this->state					= SSS_NONE;
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::getNextLexeme( scriptLexeme &lex )
{

	int					readSymbolCount		= 0;	// число считанных во временый буффер символов		
	TCHAR				*endptr;				
	TCHAR				symbol[16];					// 16 байт под буффер символа
	TCHAR				temp[16];					// 16 байт под временный буффер
	shString			name;						// имя идентификатора или зарезервированного слова
	symbolClassCode		scc;						// класс литеры
	
	lex.useInGrammar = this->useInGrammar;
	
	// вечный цикл работы  
	while( 1 )
	{
	
		// смотрим в каком состоянии находимся 
		switch( this->state )
		{
		
		case SSS_NONE:
			
			// проверка на конец файла, 
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;
			
			scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// игнорируем пробел
			// игнорируем символ табуляции
			// игнорируем символ 0x0a
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

			// обрабатываем перевод строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.filePos ++;
				this->fInfo.line	++;
				return SSRV_END_OF_LINE;
			}

			
			// проверка: может это буква
			if( scc == SCC_LETTER )
			{
				// сохраняем символ
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

			// проверка: может быть целая или вещесвенная цифра
			if( scc == SCC_DIGIT )
			{
				// сохраняем символ
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

			// вещественное число
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

			// проверка: комментарий линии
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '/' && 
				this->fInfo.fp[ this->fInfo.filePos + 1 ] == '/' )
			{
				// комментарий линии
				this->state = SSS_COMMENT_LINE;
				
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				continue;
			}

			// проверка: комментарий блока
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '/' && 
				this->fInfo.fp[ this->fInfo.filePos + 1 ] == '*' )
			{
				// комментарий блока
        		this->state = SSS_COMMENT_BLOCK;
				
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				continue;
			}

			// проверка: строка
			// проверка: символьная константа
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
		
			// проверка - обычный разделитель - оператор
			if( scc == SCC_PARSER )
			{
				
				// инициализируем лексему
				lex.type			= LT_PARSER;
				lex.fileIndex		= this->fInfo.fileIndex;
				lex.filePos			= this->fInfo.filePos;
				lex.line			= this->fInfo.line;
				lex.col				= this->fInfo.col;
				lex.setParser( this->fInfo.fp[ this->fInfo.filePos ] );
				
				lex.name.clear();

				this->fInfo.filePos ++;
				this->fInfo.col		++;

				// теперь смотрим: а не возможно ли это директива include
				if( this->isSystemIncludeDir( lex ) )
				{
					readSymbolCount = 0;
					this->state = SSS_SCAN_INCLUDE;
				}
				
				return SRV_SUCCESS;

			}
			
			// проверка плохой символ
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
				
				// создаем ошибку
				_stprintf_s( this->tempBuffer, TEXT( "'%c' (0x%2.2x)" ), lex.byteValue, (DWORD)lex.byteValue );
				return this->addError( SE_SCANER_ERROR, SSRV_BAD_CHARACTER, this->tempBuffer, lex ).code;
		
			}
			
			break; // SSS_NONE
		
		case SSS_SCAN_INCLUDE:
			
			// проверка на конец файла, 
			// конец строки
			// символ '>'
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				this->fInfo.fp[ this->fInfo.filePos ] == '>' )
			{
				
				// необходимо добавить лексему
				
				// закрываем строку
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_STRING;
				lex.fileIndex	= this->fInfo.fileIndex;
				lex.name		= this->tempBuffer;
				// удаляем пробелы слева и справа
				lex.name.cutSpacesLeft();
				lex.name.cutSpacesRight();

				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}

			// во всех остальных случаях необходимо добавить в список и перейти в след. состояние
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
				
			// проверка на конец файла,
			// разделитель
			// или перевод строки ( + перевод строки + пробел + табулятор )
			if( this->fInfo.filePos == this->fInfo.fileLength || scc == SCC_PARSER )
			{
				
				// завершаем строку
				this->tempBuffer[ readSymbolCount ] = 0;
				lex.type		= LT_ID;
				lex.name		= this->tempBuffer;
				
				// пытаемя преобразовать в зарезервированное слово
				lex.convertIfRWord( );
								
				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}
						
			// проверка: еще одна буква или цифра или плохой символ
			// все равно это идентификатор
			if( scc == SCC_LETTER || scc == SCC_DIGIT || scc == SCC_BAD_SYMBOL )
			{
				// считываем очередной символ
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];

				readSymbolCount ++;
				
				this->fInfo.filePos ++;
				this->fInfo.col ++;

				// если плохой символ - перепрыгнуть в состояние ошибки
				if( scc == SCC_BAD_SYMBOL ){
					
					_stprintf_s( temp, TEXT( "'%c' (0x%2.2x)" ), lex.byteValue, (DWORD)lex.byteValue );
					this->addError( SE_SCANER_ERROR, SSRV_BAD_CHARACTER, temp, lex );
										
					this->state = SSS_ERROR;
				}
				
				continue;
			}
			
			break; // SSS_ID_RWORD

		case SSS_COMMENT_LINE:
			
			// проверка на конец файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state			= SSS_NONE;
				return SSRV_END_OF_FILE;
			}
			
			// проверка на конец строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->state			= SSS_NONE;
				continue;
			}

			this->fInfo.filePos ++;
			this->fInfo.col		++;

			break; // SSS_COMMENT_LINE

		case SSS_COMMENT_BLOCK:
			
			// проверка на конец файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state			= SSS_NONE;
				
				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_COMMENT_BLOCK_NOT_FINISHED, TEXT("") ).code;
			}

			// проверка конец комментария
			if( this->fInfo.filePos < this->fInfo.fileLength - 1 && 
				this->fInfo.fp[ this->fInfo.filePos ] == '*' && 
				this->fInfo.fp[ this->fInfo.filePos + 1] == '/' )
			{
			
				this->fInfo.filePos += 2;
				this->fInfo.col		+= 2;

				this->state			= SSS_NONE;
				
				continue;
			}
 
			// проверка на перевод строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line	++;
				this->fInfo.col		= 0;
			}

			// игнорируем символ 0x0a
			if( this->fInfo.fp[ this->fInfo.filePos ] != 0x0a )
				this->fInfo.col ++;

			this->fInfo.filePos ++;
			
			break; // SSS_COMMENT_BLOCK

		case SSS_STRING:
			
			// проверка: конец файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				this->state							= SSS_NONE;
				this->tempBuffer[ readSymbolCount ] = 0;
				
				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_EOF_IN_STRING_CONSTANT, this->tempBuffer ).code;
								
				return SSRV_EOF_IN_STRING_CONSTANT;			
			}
			
			// проверка: конец строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == '\"'  && !this->isEscapeSequence( this->tempBuffer, readSymbolCount ) )
			{
			
				
				this->tempBuffer[ readSymbolCount ] = 0;
				// если можно форматировать строку
				if( this->enableFormatString )
					this->formatString( this->tempBuffer, this->tempBuffer, readSymbolCount + 1, lex );

				lex.type		= LT_STRING;
				lex.name		= this->tempBuffer;
				
				this->fInfo.filePos ++;
				this->fInfo.col		++;

				this->state			= SSS_NONE;
				
				return SRV_SUCCESS;
			}

			// проверка: перевод строки
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
			
			// проверка: конец файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
			{
				
				this->state							= SSS_NONE;
				this->tempBuffer[ readSymbolCount ] = 0;

				return this->addError( SE_SCANER_FATAL_ERROR, SSRV_EOF_IN_CHAR_CONSTANT, this->tempBuffer ).code;	

			}

			// проверка: конец символьной константы
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
					// не надо добавлять данную лексему
					lex.type				= LT_NONE;
					
					return this->addError( SE_SCANER_ERROR, SSRV_EMPTY_CHAR_CONSTANT, TEXT( "''" ), lex ).code;
				}
				else if( readSymbolCount < 16 )
				{
					_stprintf_s( symbol, ( const TCHAR* ) this->tempBuffer );
					lex.byteValue	= (BYTE)(symbol[0]);	// берем только первый символ	
				}
				
				if( readSymbolCount >= 16 || _tcslen( symbol ) > 1 )
				{
					lex.name		= this->tempBuffer;
					
					this->tempBuffer[ readSymbolCount ] = 0;
					return this->addError( SE_SCANER_ERROR, SSRV_MORE_THAN_ONE_SYMBOL_IN_CHAR_CONSTANT, this->tempBuffer ).code;
				}

				return SRV_SUCCESS;
			}

			// проверка: перевод строки
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
			
		// пока что целая десятеричная константа
		case SSS_NUMERIC_CONST:
			
			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// проверка на конец файла
			// или конец строки
			// или разделитель но не точка
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				( scc == SCC_PARSER && this->fInfo.fp[ this->fInfo.filePos ] != '.' ) )
			{
				
				// завершаем строку
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
			
			// если это управляющие символы константы
						
			// продолжение цифры
			// возможно двоичное, восьмеричное или 16 ричное число
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'b' || this->fInfo.fp[ this->fInfo.filePos ] == 'B' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'o' || this->fInfo.fp[ this->fInfo.filePos ] == 'O' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'x' || this->fInfo.fp[ this->fInfo.filePos ] == 'X' )
			{
				
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				// если это идет после нуля - то нормально
				// или если b и это шестнадцатеричное число
				if( readSymbolCount == 2 && this->tempBuffer[0] == '0' || 
					( ( this->fInfo.fp[ this->fInfo.filePos - 1 ] == 'b' || this->fInfo.fp[ this->fInfo.filePos - 1 ] == 'B' ) && ( this->tempBuffer[1] == 'x' || this->tempBuffer[1] == 'X' ) ))
					continue;
				else // иначе ошибка
				{
					this->tempBuffer[ readSymbolCount ] = 0;
					
					// символы b o x должны идти после нуля и никак иначе
					// поэтому создаем ошибку
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

			// часть шестнадцатеричного числа
			if( ( this->fInfo.fp[ this->fInfo.filePos ] >= 'a' && this->fInfo.fp[ this->fInfo.filePos ] <= 'f' ) || 
				( this->fInfo.fp[ this->fInfo.filePos ] >= 'A' && this->fInfo.fp[ this->fInfo.filePos ] <= 'F' ) )
			{
			
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;
				
				// если это идет после нуля - то нормально 
				if( this->tempBuffer[1] == 'x' || this->tempBuffer[1] == 'X' )
					continue;
				else // иначе ошибка
				{
					// это либо неправильный формат шестнадцатеричного числа, либо плохой суффикс
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_BAD_NUMBER_SUFFIX_OR_UNRECOGNIZED_HEX_FORMAT, this->tempBuffer, lex );
										
					this->state				= SSS_ERROR;
					continue;
				}

			}
			
			// проверка: какая это цифра
			if( scc == SCC_DIGIT )
			{
				
				// запоминаем очередной символ
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
				
				// если это вдоичное число 
				// то должны быть только 0 и 1
				if( readSymbolCount >= 2 && ( this->tempBuffer[1] == 'b' || this->tempBuffer[1] == 'B' ) && this->tempBuffer[ readSymbolCount - 1 ] > '1' )
				{
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_BINARY_FORMAT, this->tempBuffer, lex );
				
					this->state			= SSS_ERROR;
					continue;
				}
				
				// если восьмиричное число то 0 .. 7
				if( readSymbolCount >= 2 && ( this->tempBuffer[1] == 'o' || this->tempBuffer[1] == 'O' ) && this->tempBuffer[ readSymbolCount - 1 ] > '7' )
				{											
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_OCTAL_FORMAT, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;
				}
			
				// если перед нами суффикс - уродство
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
			
			// проверка: десятичная точка
			if( this->fInfo.fp[ this->fInfo.filePos ] == '.' )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;		
			
				this->state				= SSS_FLOAT_CONST;
				
				continue ;
			}

			// проверка: символы модификации L l U u UL ul
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'L' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'l' || 
				this->fInfo.fp[ this->fInfo.filePos ] == 'U' ||
				this->fInfo.fp[ this->fInfo.filePos ] == 'u' )

			{
				
				// запоминаем очередной символ
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				// если перед буквой u стоит еще один суффикс
				if( ( this->tempBuffer[ readSymbolCount - 1 ] == 'U' || this->tempBuffer[ readSymbolCount - 1 ] == 'u' ) && 
					( this->tempBuffer[ readSymbolCount - 2 ] == 'L' || this->tempBuffer[ readSymbolCount - 2 ] == 'l' || this->tempBuffer[ readSymbolCount - 2 ] == 'U' || this->tempBuffer[ readSymbolCount - 2 ] == 'u' ) )
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_INVALID_U_SUFFIX_POSITION, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;	
				}

				// если перед буквой l не стоит цифра или буква U или u - ошибка
				if( ( this->tempBuffer[ readSymbolCount - 1 ] == 'L' || this->tempBuffer[ readSymbolCount - 1 ] == 'l' ) && 
					( this->tempBuffer[ readSymbolCount - 2 ] == 'L' || this->tempBuffer[ readSymbolCount - 2 ] == 'l' ) 
					)
				{
					
					this->tempBuffer[ readSymbolCount ] = 0;
					this->addError( SE_SCANER_ERROR, SSRV_INVALID_L_SUFFIX_POSITION, this->tempBuffer, lex );
										
					this->state			= SSS_ERROR;
					continue;	
				}

				// если стоит буква u или U - то беззнаковая
				if( this->tempBuffer[ readSymbolCount - 1 ] == 'U' || this->tempBuffer[ readSymbolCount ] == 'u' )
					lex.isUnsigned = true;	
								
				continue;
			}

			// все остальное - ошибка 
			// запоминаем очередной символ
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
			
			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// проверка: конец файла
			// конец строки 
			// разделитель
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
				// завершаем строку
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_CONST;
				
				lex.constType	= LCT_DOUBLE;
				lex.doubleValue	= _tcstod( this->tempBuffer, &endptr );
				
				this->state		= SSS_NONE;
				return SRV_SUCCESS;
			}

			// проверка: получен символ e E - експонента
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'E' || this->fInfo.fp[ this->fInfo.filePos ] == 'e' )
			{
				// запоминаем очередной символ
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							

				this->state			= SSS_FLOAT_CONST2;
				
				continue;
			}

			// если это буква f или F
			if( this->fInfo.fp[ this->fInfo.filePos ] == 'f' || this->fInfo.fp[ this->fInfo.filePos ] == 'F' )
			{
			
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				this->state			= SSS_FLOAT_CONST4; 

				continue;

			}

			// проверка: число
			if( scc == SCC_DIGIT )
			{
				
				// запоминаем очередной символ
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				continue;
			}

			// все остальное - ошибка 
			// запоминаем очередной символ
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
			
			// в этом состоянии мы считываем знак минуса или плюса или просто цифру
			
			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// проверка: конец файла
			// конец строки
			// разделитель
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				
				this->tempBuffer[ readSymbolCount ] = 0;
				this->addError( SE_SCANER_ERROR, SSRV_EXPECTED_EXPONENT_VALUE, this->tempBuffer, lex );
							
				this->state			= SSS_ERROR;
				continue;	
			}

			// проверка: получен разделитель
			// получена цифра
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

			// все остальное ошибка
			// считываем символ
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

			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// проверка: конец файла
			// конец строки
			// разделитель
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
				
				// если предыдущее не число - ошибка
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
					// все нормально
					// завершаем строку
					this->tempBuffer[ readSymbolCount ] = 0;
				
					lex.type		= LT_CONST;
				
					lex.constType	= LCT_DOUBLE;
					lex.doubleValue	= _tcstod( this->tempBuffer, &endptr );
				
					this->state		= SSS_NONE;
					return SRV_SUCCESS;

				}

			}

			// проверка: цифра
			if( scc == SCC_DIGIT )
			{
				this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
				readSymbolCount		++;
				this->fInfo.filePos ++;
				this->fInfo.col		++;							
			
				continue;					
			}
			
			// если это буква f или F
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
			
			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// проверка: конец файла
			// конец строки
			// разделитель
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
			
				// завершаем строку
				this->tempBuffer[ readSymbolCount ] = 0;
				
				lex.type		= LT_CONST;
				
				lex.constType	= LCT_FLOAT;
				lex.floatValue	= ( float ) _tcstod( this->tempBuffer, &endptr );
				
				this->state		= SSS_NONE;
				return SRV_SUCCESS;	

			}
			
			// все остальное ошибка
			this->tempBuffer[ readSymbolCount ] = this->fInfo.fp[ this->fInfo.filePos ];
			readSymbolCount		++;
			this->fInfo.filePos ++;
			this->fInfo.col		++;							

			this->tempBuffer[ readSymbolCount ] = 0;
			this->addError( SE_SCANER_ERROR, SSRV_UNRECOGNIZED_FLOAT_NUMBER_FORMAT_OR_BAD_SUFFIX, this->tempBuffer, lex );
						
			this->state			= SSS_ERROR;
			break; // SSS_FLOAT_CONST4
		
		case SSS_ERROR:
			
			// получаем класс символа
			if( this->fInfo.filePos != this->fInfo.fileLength )
				scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];
			
			// проверка: конец файла
			// конец строки
			// разделитель
			if( this->fInfo.filePos == this->fInfo.fileLength || 
				this->fInfo.fp[ this->fInfo.filePos ] == 0x0d || 
				scc == SCC_PARSER )
			{
			
				readSymbolCount = 0;
				this->state		= SSS_NONE;
				continue;

			}

			// продолжаем считывать символ
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

	// проверка: бинарное число
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

	// поначалу не пропускаем лексему до конца линии
	this->skipLine		= false;
	
	// поначалу можно компилить файл
	this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );

	// печатаем имя файла
	_tprintf( TEXT("directory = '%s'   file = '%s'   fullPathname = '%s';\n"), this->fInfo.directory.getCString(), this->fInfo.fileName.getCString(), this->fInfo.fullPathName.getCString() );
	
	// постоянно получаем лексемы и затем записываем их в таблицу
	while( 1 )
	{
		// если надо пропустить сканирование
		if( this->skipScanning.top() != SST_DO_NOT_SKIP_SCANNING )
		{
			this->skipUntilDirective();
			continue;
		}

		lex.clear();							// очищаем лексему
		retCode = this->getNextLexeme( lex );	// получаем следующую лексему

		// если был найден конец строки
		if( retCode == SSRV_END_OF_LINE )
			continue;
				
		// если обработка файла завершена
		if( retCode == SSRV_END_OF_FILE )
			return SRV_SUCCESS;
			
		// проверяем а должны ли мы пропускать лексему
		if( skipLine && lex.line == skipLineIndex )
			continue;
		else 
			skipLine = false;

		// если не обработали - то добавляем лексему в список лексем
		// добавляем лексему в список лексем
		if( lex.type != LT_NONE )
		{
			
			// проверяем на наличие макроса
			if( lex.type == LT_ID || lex.type == LT_RWORD )
			{
				// если это макрос
				if( ( scrMacro = this->macroExist( lex.getLexemeName() ) ) != NULL )
				{
					// очищаем входной и выходной список
					// лексем макроса
					clearLexList( this->inputMacrosList );
					clearLexList( this->outputMacrosList );

					// сканируем лексемы для параметров макроса
					this->scanMacros( lex, *scrMacro );
					
					// разбираем по параметрам первый макрос
					this->parseMacros( lex, *scrMacro );
					
					// выполняем макрос
					this->executeMacros( lex, *scrMacro, this->parent->lexemes );
					
					// продолжаем 
					continue;
				}

			}
			
			// будем добавлять в список ...
			lex2 = new scriptLexeme( lex );
			lex2->scaner = this;

			// директива препроцессора
			if( lex2->type == LT_PARSER && lex2->parser == LPT_PREPROC_DIRECTIVE && !this->skipPreprocDirectives)
			{
				
				// получаем предыдущую лексему
				sli = this->parent->lexemes.end();
				sli--;
				
				// если символ директивы компилятора не первый в строке
				if( this->parent->lexemes.size() != 0 && (*sli)->line == lex2->line )
				{
					// необходимо выдать ошибку об этом и пропустить эту директиву
					this->addError( SE_SCANER_ERROR, SPPRV_DIRECTIVE_SYMBOL_NOT_FIRST, *lex2 );
										
					// мы должны пропускать все лексемы до конца строки
					skipLine				= true;
					skipLineIndex			= lex2->line;
					
					// удаляем лексему 
					delete lex2;
					
					continue;
				}
				else // символ директивы компилятора первый в строке
				{
														
					// очищаем список лексем директивы
					clearLexList( this->directiveLexemes );
					
					this->directiveLexemes.push_back( lex2 );
					
					retCode = this->scanDirective();
					if( retCode == SRV_SUCCESS ) // анализируем директиву
						retCode = this->parseDirective();
										
					if( retCode != SRV_SUCCESS )
					{
						// получаем код директивы
						sli = this->directiveLexemes.begin();
						sli ++;
						
						if( (*sli)->type == LT_RWORD || (*sli)->type == LT_ID )
							this->addError( SE_PREPROCESSOR_ERROR, SPPRV_CANT_EXECUTE_DIRECTIVE, (*sli)->getLexemeName().getCString(), *(this->directiveLexemes.front()) );
						else
							this->addError( SE_PREPROCESSOR_ERROR, SPPRV_CANT_EXECUTE_DIRECTIVE, *(this->directiveLexemes.front()) );
					
						continue;
					}
					
					// продолжаем считывание
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
			
	// поначалу не пропускаем лексему
	skipLine								= false;

	// постоянно получаем лексемы и затем записываем их в таблицу
	while( 1 )
	{
		lex.clear();							// очищаем лексему
		retCode = this->getNextLexeme( lex );	// получаем следующую лексему

		// если найдена лексема error
		if( lex.type == LT_RWORD && lex.rwordType == LRW_ERROR )
		{
			
			// добавляем лексему и 
			lex2 = new scriptLexeme(lex);
			lex2->scaner = this;
			this->directiveLexemes.push_back( lex2 );
			
			return SRV_SUCCESS;
		}
		
		// если был найден конец строки
		if( retCode == SSRV_END_OF_LINE || retCode == SSRV_END_OF_FILE )
		{
		
			// смотрим - если предыдущая лексема - обратный слеш - 
			// то это знак продолжения макроса и надо продолжать обработку
				
			sli = this->directiveLexemes.end();
			sli--;
				
			if( this->directiveLexemes.size() != 0 && (*sli)->type == LT_PARSER && (*sli)->parser == LPT_BACKSLASH )
			{
				// надо удалить предыдущую лексему и продолжить считывание
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

	// если нет параметров - ну и зашибись
	if( scrMacro.params.size() == 0 )
		return SRV_SUCCESS;
		
	// если есть параметры надо их считать
	// заключены в скобки
	while( bMustScan )
	{
		lex2.clear();							// очищаем лексему
		retCode = this->getNextLexeme( lex2 );	// получаем следующую лексему

		// если был найден конец строки
		if( retCode == SSRV_END_OF_LINE )
			continue;

		// если обработка файла завершена 
		// то просканировано не все макроопределение
		if( retCode == SSRV_END_OF_FILE )
		{
			// входной список сколько есть
			scrMacro.inputList.begin	= this->inputMacrosList.begin();
			scrMacro.inputList.end		= this->inputMacrosList.end(); 
			scrMacro.inputList.size		= (int) this->inputMacrosList.size();
						
			return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EOF_IN_MACRO_PARAMS, lex ).code;
		}
		
		// смотрим, в каком состоянии находимся
		if( macroScanState == 0 )
		{
			// ожидается (
			// проверяем лексему
			if( lex2.type != LT_PARSER || lex2.parser != LPT_LEFT_BRACKET )
			{
				// вернутся обратно поскольку эту лексему надо будет обработать
				this->fInfo.filePos = lex.filePos + lex.name.getLength();
				this->fInfo.line	= lex.line;
				this->fInfo.col		= lex.col;
								
				// нет вообще входного списка
				scrMacro.inputList.begin	= this->inputMacrosList.begin();
				scrMacro.inputList.end		= this->inputMacrosList.end(); 
				scrMacro.inputList.size		= (int) this->inputMacrosList.size();

				// выдать ошибку, о том, что должна была быть скобка
				return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO, lex2 ).code;
			}
			else
			{
				macroScanState = 1;
				bracketCount   = 1;
			}
		}
		else // считываем параметры макроса
		{
			
			// если это открывающая скобка
			if( lex2.type == LT_PARSER && lex2.parser == LPT_LEFT_BRACKET )
				bracketCount ++;
			
			// если это закрывающая скобка
			else if( lex2.type == LT_PARSER && lex2.parser == LPT_RIGHT_BRACKET )
			{
				// если это закрывающая макрос скобка
				if( bracketCount == 1 )
					bMustScan = false;	// прекратить сканирование
				else
					bracketCount --;	// уменьшаем число открытых скобок
			}
		}
	
		// добавляем лексемы в список
		lex3 = new scriptLexeme( lex2 );
		lex3->scaner = this;
		this->inputMacrosList.push_back( lex3 );
	}
			
	// теперь инициализируем макро
	scrMacro.inputList.begin	= this->inputMacrosList.begin();
	scrMacro.inputList.end		= this->inputMacrosList.end();
	scrMacro.inputList.size		= (int) this->inputMacrosList.size();

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::scanMacros( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme *> &lexList, scriptMacros& scrMacro )
{
	
	std::list< scriptLexeme *>::iterator	sli2;
	int							bracketCount = 0;

	// если макрос без параметров - ну и отлично
	if( scrMacro.params.size() == 0 )
		return SRV_SUCCESS;

	// переходим от имени макроса к списку параметров если они есть
	sli2 = sli;
	sli ++;
		
	// следующая лексема должна быть обязательно открывающей скобкой
	if( sli == lexList.end() || (*sli)->type != LT_PARSER || (*sli)->parser != LPT_LEFT_BRACKET )
	{
		// вернемся к имени макро
		sli --;

		// нет входной строки в скобках
		scrMacro.inputList.begin = sli;
		scrMacro.inputList.end	 = sli;
		scrMacro.inputList.size	 = 0;
		
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_LEFT_BRACKET_IN_MACRO, *(*sli) ).code;
	}
	
	// запоминаем начало параметров
	scrMacro.inputList.begin = sli;
	// размер
	scrMacro.inputList.size  = 1;

	// число открывающих скобок
	bracketCount = 1;
	
	// теперь должны найти закрывающую скобку
	for( sli++; sli != this->inputMacrosList.end(); sli ++ )
	{
		if( sli == lexList.end() )
		{
			// конец параметров
			scrMacro.inputList.end = sli;
			return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_EXPECTED_RIGHT_BRACKET_IN_MACRO, *(*sli2) ).code;
		}
		
		if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_LEFT_BRACKET )
			bracketCount ++;
		else if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_RIGHT_BRACKET )
			bracketCount --;
		
		// число лексем
		scrMacro.inputList.size ++;
		
		if( bracketCount == 0 )
			break;
	}
	
	// конец параметров
	scrMacro.inputList.end = sli;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseMacros( scriptLexeme &lex, scriptMacros &scrMacro )
{

	std::list< scriptLexeme* >::iterator	sli;
	std::list< macrosInputParam >::iterator mipi;
	macrosInputParam						mip;
	int		bracketCount = 0;	// число открытых скобок
	
	// сначала разбираем макрос по параметрам
	
	// очищаем список входных значений параметров
	scrMacro.input.clear();

	// начало первого параметра
	mip.begin = scrMacro.inputList.begin;
	
	// цикл по всем лексемам во входном списке
	for( sli = scrMacro.inputList.begin; sli != scrMacro.inputList.end ; sli++ )
	{
		// если не разделитель
		if( (*sli)->type != LT_PARSER )
			continue;
		
		// (
		if( (*sli)->parser == LPT_LEFT_BRACKET )
			bracketCount ++;
		// )
		else if( (*sli)->parser == LPT_RIGHT_BRACKET )
			bracketCount --;		
		
		// , или последняя скобка
		if( ( (*sli)->parser == LPT_COMMA && bracketCount == 1 ) || bracketCount == 0 )
		{
			// конец параметра
			mip.end = sli;
			// корректируем начало
			mip.begin ++;
			// добавляем в список
			scrMacro.input.push_back( mip );
		
			// инициализируем заново если еще есть место для параметра
			if( bracketCount != 0 )
				mip.begin = sli;
			else // иначе закончить считывание
				break;
		}
	}

	// после того как разбиение на параметры закончилось
	// считаем число лексем для каждого параметра
	// подсчитываем число лексем в каждом параметре		
	for( mipi = scrMacro.input.begin(); mipi != scrMacro.input.end(); mipi ++ )
	{
		mipi->size = 0;	
		for( sli = mipi->begin; sli != mipi->end; sli ++ )
			mipi->size ++;
	}
	
	// теперь проверяем число параметров
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
		
	// задействовать макро
	scrMacro.called = true;

	// подставляем макро
			
	// проходимся по телу макроса
	for( sli2 = scrMacro.body.begin(); sli2 != scrMacro.body.end(); sli2++ )
	{
						
		// сначала проверяем на наличие параметра
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
					// если где то в параметре есть макрос
					// запоминаем макрос
					sli3 = sli;

					// обрабатываем макро
					// сканируем уже полученные параметры
					this->scanMacros( sli3, this->inputMacrosList, *scrMacro2 );
					// анализируем параметры если есть
					this->parseMacros( *(*sli), *scrMacro2 );
					// выполняем макрос если есть
					this->executeMacros( *(*sli), *scrMacro2, this->parent->lexemes );
					// перепрыгиваем на лексему следующую за макросом
					sli = sli3;
					
					// если достигли конца
					if( sli == mipi->end )
						break;
					else
						continue; // продолжаем обработку
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
		
		// пытаемся получить имя макроса
		scrMacro2 = NULL;
		if( (*sli2)->type == LT_ID || (*sli2)->type == LT_RWORD )
			scrMacro2 = this->macroExist( (*sli2)->getLexemeName() );

		// если это имя макроса
		if( scrMacro2 != NULL )
		{
			// запоминаем начало макроса
			sli3 = sli2;
						
			// обрабатываем макро		
			// сканируем уже полученные параметры
			this->scanMacros( sli3, scrMacro.body, *scrMacro2 );
			// анализируем параметры если есть
			this->parseMacros( *(*sli2), *scrMacro2 );
			// выполняем макрос если есть
			this->executeMacros( *(*sli2), *scrMacro2, this->parent->lexemes );
			// перепрыгиваем на лексему следующую за макросом
			sli2 = sli3;
			
			// продолжаем обработку
			continue;
		}
		else 
		{
			// просто копируем лексему
			lex2 = new scriptLexeme ( *( *sli2 ) );
			lex2->scaner = this;
			lexList.push_back( lex2 );
		}	
	}
		
	// освободить макро
	scrMacro.called = false;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::skipUntilDirective()
{

	// мы должны пропускать все лексемы до #else или до #elsif или до endif
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
			
			// сначала проверяем на окончание файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;

			// получаем код символа
			scc = (symbolClassCode) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// если получен пробел
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x20 )
			{
				this->fInfo.col ++;
				this->fInfo.filePos ++;
				continue;
			}

			// если получен 0x0a
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
			{
				this->fInfo.col = 0;
				this->fInfo.filePos ++;
				continue;
			}

			// если получен перевод строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line ++;
				this->fInfo.filePos ++;
				firstSymbolRead = false;
				continue;
			}

			// символ табуляции
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 )
			{
				this->fInfo.col		+= 4;
				this->fInfo.col		&= 0xfffffffc;
				this->fInfo.filePos	++;
				continue;
			}

			// если найден символ #
			if( this->fInfo.fp[ this->fInfo.filePos ] == '#' )
			{
				// если символ не первый в строке
				if( firstSymbolRead == true )
				{
					// просто пропустить его
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

			// если ничего не вышло то просто пропускаем символ
			firstSymbolRead = true;
			this->fInfo.filePos ++;
			this->fInfo.col		++;

			continue;
			
			break;
		
		case SSS_WAIT_FOR_ENDIF_ANALOG:
						
			// сначала проверяем на окончание файла
			if( this->fInfo.filePos == this->fInfo.fileLength )
				return SSRV_END_OF_FILE;

			// получаем код символа
			scc = (symbolClassCode) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ];

			// если получен пробел
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x20 )
			{
				this->fInfo.col ++;
				this->fInfo.filePos ++;
				continue;
			}

			// если получен перевод строки
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
			{
				this->fInfo.line ++;
				this->fInfo.filePos ++;
				firstSymbolRead = false;
				
				skipState = SSS_FIND_DIRECTIVE_SYMBOL;
				
				continue;
			}

			// символ табуляции
			if( this->fInfo.fp[ this->fInfo.filePos ] == 0x09 )
			{
				this->fInfo.col		+= 4;
				this->fInfo.col		&= 0xfffffffc;
				this->fInfo.filePos	++;
			}
			
			// если нашли букву
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
			
			// в остальных случаях вернуться к началу
			this->fInfo.filePos ++;
			this->fInfo.col		++;
			firstSymbolRead		= false;
			skipState = SSS_FIND_DIRECTIVE_SYMBOL;

			break;
			
		case SSS_SCAN_ENDIF_ANALOG:
			
			// проверка на окончание файла
			// сначала проверяем на окончание файла
			// или на наличие разделителя
			if( this->fInfo.filePos == this->fInfo.fileLength ||
				( scc = ( symbolClassCode ) symbolClass[ this->fInfo.fp[ this->fInfo.filePos ] ] ) == SCC_PARSER )
			{
			
				// необходимо посмотреть что это за идентификатор
				this->tempBuffer[ byteReadCount ] = 0;
				
				// инициализируем имя лексемы
				lex2.name	= this->tempBuffer;
			
				// получаем индекс зарезевированного слова
				if( lex2.convertIfRWord() )
				{
					// если это зарезервированное слово 
					// endif - то обработать его в любом случае
					// else elif
					if( lex2.rwordType == LRW_ENDIF || lex2.rwordType == LRW_ELSE || lex2.rwordType == LRW_ELIF )
					{
						// то надо досканировать директиву и если нет ошибок выполнить ее
						
						// сначала очищаем список директив
						clearLexList( this->directiveLexemes );

						// добавляем туда первую лексему
						lex3 = new scriptLexeme( lex );
						lex3->scaner = this;
						this->directiveLexemes.push_back( lex3 );
																	
						// добавляем туда вторую лексему
						lex3 = new scriptLexeme( lex2 );
						lex3->scaner = this;
						this->directiveLexemes.push_back( lex3 );
					
						// досканируем директиву
						this->scanDirective();

						// распознаем директиву
						this->parseDirective();

						// возвращаемся 
						return SRV_SUCCESS;
					}
				}
			
				// во всех остальных случаях пропустить лексему
				skipState = SSS_FIND_DIRECTIVE_SYMBOL;
				continue;
			}

			// в остальных случаях продолжать считывать символы
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
	
	// преобразовываем разделители в более сложные разделители 
	for( sli1 = lexList.begin(); sli1 != lexList.end(); sli1 ++ )
	{
		if( (*sli1)->type != LT_PARSER )
			continue;		
		
		// обнуляем лексемы
		lex1		= lex2 = lex3 = NULL;
		optimize	= true;
				
		// получаем первую 
		lex1 = *sli1;
		
		sli2 = sli1;
		sli2 ++;

		// если есть вторая, получаем вторую
		if( sli2 != lexList.end() )
		{
			// если вторая лексема не разделитель - то продолжаем поиск
			if( (*sli2)->type != LT_PARSER )
				continue;
			
			lex2 = *sli2;
			sli3 = sli2;
			sli3 ++;

			// если есть третья получаем третью
			if( sli3 != lexList.end() )
				lex3 = *sli3;
		}
		else
			continue; 
				
		if( lex3 != NULL && lex3->type == LT_PARSER )
		{
			
			// простматриваем тройные 
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
				// удаляем вторую лексему
				delete lex2;
				lexList.erase( sli2 );

				// удаляем третью лексему
				delete lex3;
				lexList.erase( sli3 );
		
				// продолжаем искать
				continue;
			}	
		}
		
		optimize = true;

		// разбираем 2 лексемы
		// смотрим, что с чем можно соединить 
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
		
		// была произведена оптимизация 2 разделителей
		if( optimize )
		{
			// удаляем вторую лексему
			delete lex2;
			lexList.erase( sli2 );
		
			// продолжаем искать
			continue;
		}
	}

	// нельзя оптимизировать унарные опреаторы, как бы мне этого не хотелось
	// поскольку определить унарный оператор или нет очень геморно на стадии сканирования
	// например запись
	// -- - 123
	// может быть унарный минус 
	// если запись типа 
	// -- - 123
	// или бинарный минус - если запись типа 
	// id -- - 123
	// но главный аргумент - то что оптимизировать константы еще можно
	// а оптимизировать переменные нельзя
	// поэтому оптимизириуем на стадии лексического анализатора
	// для макросов оптимизируем на уровне сканирования макроса

	// оптимизируем унарные операторы - и + для констант
	for( sli1 = lexList.begin(); sli1 != lexList.end(); sli1 ++ )
	{
	
		// первую лексему мы вообще просматривать не будем
		if( sli1 == lexList.begin() )
			continue;

		// предыдущая лексема
		sli2 = sli1;
		sli2 --;
 
		// превращаем - в унарный - и + в унарный + 
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
		// вторую лексему также просматривать не будем
		if( sli2 == lexList.begin() )
			continue;
		
		sli3 = sli2;
		sli3 --;
		
		lex1 = *sli1;
		lex2 = *sli2;
		lex3 = *sli3;
		

		optimize = false;
		
		// если текущая лексема не константа - то не будем оптимизировать такую лексему
		if( lex1->isNumericConst() && lex2->type == LT_PARSER && lex3->type == LT_PARSER && 
			lex3->parser == )
		{
			if( lex2->parser == LPT_SUBTRACTION )
			{
				// лексема со знаком минус
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
				// удаляем вторую лексему
				delete lex2;
				lexList.erase( sli2 );

				// переходим на одну лексему назад для повторной проверки
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

	// первая лексема - # вторая уже зарезервированное слово
	sli = this->directiveLexemes.begin();
	sli ++;
	
	// если первым символом является зарезервированное слово
	if( (*sli)->type == LT_RWORD )
	{
		
		// смотрим какое
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

	// если все нормально осталось разобрать последнюю лексему
	lex = this->outputMacrosList.front();
	if( lex->isZeroConst() )
		// пропускаем сканирование
		this->skipScanning.push( SST_SKIP_UNTIL_NEW_CONDITION );
	else 
		// можно дальше сканировать 
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );

	return SRV_SUCCESS;
}

// begin - итератор с которого надо начинать
// end - итератор, по достижении которого надо тут же закончить
scriptRetCode scriptScaner::executeIfDirective( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli, sli2, sli3, sliTmp;
	scriptLexeme							*lex, *lex2;
	int										bracketCount, opCode;
	scriptRetCode							retCode;
	bool									foundBinary;

	// если начало равно конeц - бред ()
	if( begin == end )
	{
		lex = *begin;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR03, *lex ).code;	
	}

	// проходимся и ищем скобки
	// если найдем открывающуюся скобку
	// для нее найдем закрывающуюсю скобку
	// выполним выражение ( begin ... )- end
	// удалим скобки ( и ) - текущим стновится результат в скобках
	// его самого проверять не надо
	// и мы передвигаемся к следующему символу
	// который может оказаться концом строки
	// таким образом сбоев в работе быть не должно
	for( sli = begin; sli != end; sli ++ )
	{	
		// ищем открывающую скобку
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

			// для открывающей скобки не нашли закрывающую скобку
			if( sli2 == end )
				return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR01, *lex ).code;	
		
			// нашли начало и конец скобок
			sli ++;
			retCode = this->executeIfDirective( sli, sli2, lexList );
			if( retCode != SRV_SUCCESS )
				return retCode;
			
			// текущим становится лексема, которая находится внутри скобок
			sli3 = sli;
			sli3 --;
	
			// уничтожаем открывающиеся скобки, закрывающиеся скобки 
			delete lex;
			delete lex2;
			
			lexList.erase( sli3 );
			lexList.erase( sli2 );
			
			// если запись типа ((
			if( sli3 == begin )
				begin = sli;
			
			// после чего оптимизируем выражение для макроса
			this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
		}
	}
			
	// после чего оптимизируем выражение для макроса
	this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
	
	// поскольку функция вызывается рекурсивно 
	// при нахождении (
	// то раз мы оказались здесь, то это значит, что все открывающие скобки до этого уже найдены

	// пиоритет операторов
	// 0)	defined
	// 1)   ~  ! ( u+ u- рассматривать не будем так как они были оптимизированы )
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
	
	// проходимся по всем приоритетам, пока не найдем и не выполним один
	for( opCode = 0; opCode < 13; opCode ++ )
	{
		lex = *begin;
		sli = begin;
		sli ++;
		
		// отладочная печать
		_tprintf( TEXT( "op = %d    " ), opCode );
		for( sliTmp = this->outputMacrosList.begin(); sliTmp != this->outputMacrosList.end(); sliTmp ++ )
			_tprintf( TEXT( "%s " ), (*sliTmp)->getLexemeDesc().getCString() );

		_tprintf( TEXT( "\n" ) );

		// ситуация ( const ) или begin const end
		if( lex->type == LT_CONST && sli == end )
			break;	
			
		// ищем операции по приоритетам
		retCode		= SRV_SUCCESS;
		foundBinary	= false;
		for( sli = begin; sli != end; sli++ )
		{
			
			lex = *sli;
			
			// если не defined или не разделитель то продолжить
			if( lex->type != LT_PARSER && ( lex->type != LT_RWORD || lex->rwordType != LRW_DEFINED ) )
				continue ;
			
			// 0 ::= defined
			if( opCode == 0 && lex->type == LT_RWORD && lex->rwordType == LRW_DEFINED )
				retCode = this->executeDefinedOp( sli, lexList );
			// теперь обрабатываем все унарные операции
			else if( opCode == 1 && preprocOperatorType[ lex->parser ] == 1 )
				retCode = this->executeUnaryOp( sli, lexList );
			// теперь обрабатываем все бинарные операции
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
			// теперь обрабатываем тернарную операцию
			//else if( opCode == 12 && lex->type == LT_PARSER && lex->parser == LPT_QUESTION )
			//	retCode = this->executeQuickIfOp( sli, lexList );
			else continue;

			if( foundBinary )
				retCode = this->executeBinaryOp( sli, lexList );
			
			// если мы здесь - значит что то нашли
			if( retCode != SRV_SUCCESS )
				return retCode;
			else 
			{
				// оптимизируем унарные плюсы и минусы	
				this->optimizeMacroUnaryMinusAndPlus( begin, end, lexList );
				// а также заново начинаем сканирование операторов
				opCode --;
				break;
			}
		}
	}
	
	// здесь произошла ошибка если opCode == 12
	if( opCode == 13 )
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_IF_DIRECTIVE_ERROR18, *(*begin) ).code;
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeQuickIfOp( std::list< scriptLexeme* >::iterator &begin, std::list< scriptLexeme* >::iterator &end, std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{

	// сначала должны убедиться в правильности записи
	std::list< scriptLexeme* >::iterator	condSli, expr1Sli, expr2Sli, colonSli;
	scriptLexeme							*cond, *expr1, *expr2;

	// сначала смотрим а есть ли вообще условиe
	if( sli == begin )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR01, *(*sli) ).code;

	// получаем условиe
	condSli = sli;
	condSli--;
	cond = *condSli;

	// преобразовываем идентификатор в нуль
	if( cond->type == LT_ID )
	{
		cond->type			= LT_CONST;
		cond->constType		= LCT_DECIMAL;
		cond->isUnsigned	= false;
		cond->longValue		= 0;
	}

	// проверяем условие
	if( cond->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR02, *cond ).code;

	// получаем первое выражение
	// сначала проверяем а есть ли первое выражение
    if( sli == end )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR03, *cond ).code;

	expr1Sli = sli;
	expr1Sli ++;

	expr1 = *expr1Sli;

	// если это идентификатор
	if( expr1->type == LT_ID )
	{
		expr1->type			= LT_CONST;
		expr1->constType	= LCT_DECIMAL;
		expr1->isUnsigned	= false;
		expr1->longValue	= 0;
	}

	// если это не число
	if( expr1->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR04, *expr1 ).code;

	// теперь должны проверить наличие двоеточия
	// если вообще выражение прерывается
	colonSli = expr1Sli;
	if( colonSli == end )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR05, *expr1 ).code;
	
	// если вместо двоеточия какая то хрень
	if( (*colonSli)->type != LT_PARSER || (*colonSli)->parser != LPT_DOT2 )	
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_QUICKIF_ERROR06, *(*colonSli) ).code;
	
	expr2Sli = colonSli;
	expr2Sli ++;

	// если выражение прерывается
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

	// теперь выполняем условие
	if( !cond->isZeroConst() )
		*cond = *expr1;
	else
		*cond = *expr2;
	
	if( expr2Sli == end )
		end = condSli;
	
	// удаляем выражение оставляем только результат
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
	
	// получаем лексему оператора
	opCode = *sli;
	
	// проверка, что слева имеем операнд 
	if( sli == lexList.begin() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR10, *(*sli) ).code;
	
	// получаем левый операнд
	leftOpSli	= sli;
	leftOpSli --;
	leftOp		= *leftOpSli;	
	
	// получаем правый операнд
	rightOpSli = sli;
	rightOpSli ++;

	// проверка на то что справа есть операнд
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
	
	// имеем два операнда
	// проверяем каждый операнд на принадлежность числу
	if( leftOp->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR12, *leftOp ).code;
	
	if( rightOp->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR13, *rightOp ).code;	
	
	// теперь проверяем операнды учитывая специфику оператора
	// если / или % проверить, чтобы не был 0 иначе 
	// ошибка: деление на нуль!
	if( ( opCode->parser == LPT_DIVISION || opCode->parser == LPT_REMAINDER ) && rightOp->isZeroConst() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR14, *rightOp ).code;	
	    
	
	// начинаем выполнять действия
	
	// если операция арифметическая
	// + - * / %
	if( preprocOperationType[ opCode->parser ] == 1 )
	{
		// получаем тип результата
		leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );
			
		// преобразовываем левый операнд
		leftOp->convertData( newConstType, bNewUnsigned );
	
		// преобразовываем правый операнд
		rightOp->convertData( newConstType, bNewUnsigned );

		// выполняем арифметическое выражение
		this->ppExecuteArithmeticOp( *leftOp, *opCode, *rightOp );
	}

	// булевые операции, поскольку приоритет уже учтен
	// а обработка одна и та же то обрабатываем всем скопом
	else if( preprocOperationType[ opCode->parser ] == 2 )
	{
		// получаем тип результата
		leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );

		// преобразовываем левый операнд
		leftOp->convertData( newConstType, bNewUnsigned );

		// преобразовываем правый операнд
		rightOp->convertData( newConstType, bNewUnsigned );
		
		// выполняем операцию
		this->ppExecuteLogicalOp( *leftOp, *opCode, *rightOp );
	}
	
	// пошли побитовые операции
	// типа & | ^ исключая >> << поскольку 
	// проверка на >> << была произведена раньше
	else if( preprocOperationType[ opCode->parser ] == 3 )
	{
		// если левый тип - булевый 
		if( leftOp->constType == LCT_BOOL )
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING02, *leftOp );

		// или (и) правый тип булевый
		if( rightOp->constType == LCT_BOOL )
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING03, *rightOp );
		
		// левый параметр - float ошибка
		if( leftOp->constType == LCT_FLOAT )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR14, *leftOp ).code;

		// правый параметр - float ошибка
		if( rightOp->constType == LCT_FLOAT )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR15, *rightOp ).code;

		// левый параметр - double ошибка
		if( leftOp->constType == LCT_DOUBLE )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR16, *leftOp ).code;

		// правый параметр - double ошибка
		if( rightOp->constType == LCT_DOUBLE )
			return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR17, *rightOp ).code;

		if( opCode->parser != LPT_LEFT_SHIFT && opCode->parser != LPT_RIGHT_SHIFT )
		{
			// получаем тип результата
			leftOp->getResultType( *rightOp, newConstType, bNewUnsigned );

			// преобразовываем левый операнд
			leftOp->convertData( newConstType, bNewUnsigned );

			// преобразовываем правый операнд
			rightOp->convertData( newConstType, bNewUnsigned );
		}
		
		// выполняем операцию
		this->ppExecuteBinaryOp( *leftOp, *opCode, *rightOp );
	}
	
	// удаляем знак операции и правый операнд
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

	// смотрим следующую лексему - должна быть константой
	sli2 = sli;
	sli2 ++;
	
	// получаем лексемы
	lex = *sli;
	
	// смотрим а есть ли хоть какая нибудь лексема
	if( sli2 == lexList.end() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR06, *(*sli2) ).code;
	
	lex2 = *sli2;	
		
	// если это идентификатор и не макрос - то по правилам его значение равно 0 
	if( lex2->type == LT_ID )
	{
		lex2->name.clear();
		lex2->type			= LT_CONST;
		lex2->constType		= LCT_DECIMAL; 
		lex2->isUnsigned	= false;
		lex2->longValue		= 0;
	}
	
	// если это был не идентификатор - то 
	// должна быть обязательно константа
    if( lex2->type != LT_CONST )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR07, *lex2 ).code;
		
	lex->type		= LT_CONST;
	
	// проверка типов 
	// если это ~
	if( lex->parser == LPT_BITWISE_NOT_OR_DESTRUCTOR )
	{
		lex->constType	= lex2->constType;	
		
		if( lex2->constType == LCT_BOOL )
		{
			// непонятно какой результат будет
			this->addError( SE_PREPROCESSOR_WARNING, SPPRV_IF_DIRECTIVE_WARNING01, *lex2 );
			lex->boolValue	= !lex->boolValue; // бинарное отрицание через логическое
		}
		// байт - символ
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
	else  // это ! работает с любым типом констант
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

	// убиваем лексему с константой
	delete (lex2);
	lexList.erase( sli2 );

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeDefinedOp( std::list< scriptLexeme* >::iterator &sli, std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli2;
	scriptLexeme							*lex, *lex2;

	// смотрим следующую лексему - должна быть именем
	sli2 = sli;
	sli2 ++;

	lex		= *sli;
	
	// смотрим а есть ли хоть какая нибудь лексема
	if( sli2 == lexList.end() )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR05, *(*sli2) ).code;
		
	lex2	= *sli2;
	
	// если нет имени
	if( lex2->type != LT_RWORD && lex2->type != LT_ID )
		return this->addError( SE_PREPROCESSOR_FATAL_ERROR, SPPRV_IF_DIRECTIVE_ERROR04,  *lex2 ).code;
	
	// преобразовываем в константу
	lex->type		= LT_CONST;
	lex->constType	= LCT_BOOL;
	
	// смотрим а есть ли такой макрос
	if( this->macroExist( (*sli2)->getLexemeName() ) )
		lex->boolValue	= true;
	else
		lex->boolValue	= false;
	
	// удаляем идентификатор
	delete lex2;
	lexList.erase( sli2 );

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIfDirective()
{

	std::list< scriptLexeme* >::iterator	sli, sli2, sli3;
	scriptMacros								*scrMacro;
	scriptLexeme							*lex;

	// переходим к началу выражения
	sli = this->directiveLexemes.begin();
	
	// пропускаем #
	// пропускаем if
	sli ++;
	sli ++;
	
	// очищаем список
	clearLexList( this->outputMacrosList );
	
	// теперь записываем параметры в выходной список
	for( ; sli != this->directiveLexemes.end(); sli++ )
	{
	
		// пытаемся получить имя макроса
		scrMacro = NULL;
		if( (*sli)->type == LT_ID || (*sli)->type == LT_RWORD )
			scrMacro = this->macroExist( (*sli)->getLexemeName() );

		// если это имя макроса и это имя идет не после defined
		// то обработать макрос
		if( scrMacro != NULL && ( this->outputMacrosList.back()->type != LT_RWORD || this->outputMacrosList.back()->rwordType != LRW_DEFINED ) )
		{
			// запоминаем начало макроса
			sli2 = sli;

			// обрабатываем макро		
			// сканируем уже полученные параметры
			this->scanMacros( sli2, this->directiveLexemes, *scrMacro );
			// анализируем параметры если есть
			this->parseMacros( *(*sli), *scrMacro );
			// выполняем макрос если есть
			this->executeMacros( *(*sli), *scrMacro, this->outputMacrosList );
			// перепрыгиваем на лексему следующую за макросом
			sli = sli2;

			// продолжаем обработку
			continue;
		}
		else 
		{
			// просто копируем лексему
			lex = new scriptLexeme ( *( *sli ) );
			lex->scaner = this;
			this->outputMacrosList.push_back( lex );
		}	
	}
	
	// оптимизируем разделители
	this->optimizeParsers( this->outputMacrosList );
	this->optimizeMacroUnaryMinusAndPlus( this->outputMacrosList.begin(), this->outputMacrosList.end(), this->outputMacrosList );	
				
	// выполняем директиву
	this->executeIfDirective();
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineDirective()
{

	std::list< scriptLexeme* >::iterator	sli, sli2;
	scriptMacros								*scrMacro;
	scriptRetCode							retCode;
	bool									redef = false;

	// получаем лексему следующую за define
	sli = this->directiveLexemes.begin();
	sli ++;	// define
	sli ++;	// имя макроса

	// если на самом деле больше не найдено лексем
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
	
	// если такой макрос есть необходимо добавить предупреждение
    if( scrMacro != NULL )
	{
		_stprintf_s( this->tempBuffer, TEXT( "'%s'" ), (*sli)->name.getCString() );
		this->addError( SE_SCANER_WARNING, SPPRV_MACRO_REDEFINITION, this->tempBuffer, *(*sli) );
				
		// очищаем макрос
		scrMacro->clear();
		redef = true;
	}
	else {
		scrMacro = new scriptMacros();
	}
	
	// теперь инициализируем макрос
	scrMacro->name			= (*sli)->name;
	scrMacro->line			= (*sli)->line;
	scrMacro->col			= (*sli)->col;
	scrMacro->fileIndex		= this->fInfo.fileIndex;
						
	// надо считать параметры
	retCode = this->parseDefineParams( *scrMacro, sli );
	
	// надо считать тело макроса
	retCode = this->parseDefineBody( *scrMacro, sli );

	// запоминаем макрос
	if( redef == false )
		this->parent->macros.push_back( scrMacro );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineParams( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli )
{
	std::list< scriptLexeme* >::iterator	sli2; 
	scriptLexeme							*lex;
	
	// очищаем список параметров
	clearLexList( scrMacro.params );
	
	sli2 = sli;
	sli2 ++;	// вероятнее всего скобка
	
	if( sli2 == this->directiveLexemes.end() )
	{
		sli ++;
		return SRV_SUCCESS;
	}
	
	// если нет строки с параметрами
	if( (*sli2)->type != LT_PARSER || (*sli2)->parser != LPT_LEFT_BRACKET || !(*sli)->isParserNearId( *(*sli2) ) )
	{
		sli ++;
		return SRV_SUCCESS;
	}
	
	bool mustFoundId = true;
	
	// теперь проверяем параметры
	sli = sli2;
	for( sli ++; sli != this->directiveLexemes.end(); sli++ )
	{
		
		// если мы нашли )
		if( (*sli)->type == LT_PARSER && (*sli)->parser == LPT_RIGHT_BRACKET )
			break;
		
		// если нам сейчас надо найти параметр
		if( mustFoundId )
		{
			if( (*sli)->type != LT_ID && (*sli)->type != LT_RWORD )
			{
				this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR01, *(*sli) );
				continue;
			
			}
			else
			{
				// добавляем лексему
				lex = new scriptLexeme( *(*sli) );
				lex->scaner = this;
				scrMacro.params.push_back( lex );
			}
		}
		else // надо найти запятую
		{
			if( (*sli)->type != LT_PARSER || (*sli)->parser != LPT_COMMA )
			{
				this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR02, *(*sli) );
				continue;
				// SPPRV_PARAM_LIST_ERROR02;
			}
		}
	
		// меняем состояние нахождения
		mustFoundId = !mustFoundId;
	}

	// разбор параметров был прерван: нет завершающей скобки
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_MACRO_PARAM_LIST_ERROR03, *(*sli) ).code;
	}
	
	// пропускаем завершающую скобку
	sli ++;

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseDefineBody( scriptMacros &scrMacro, std::list< scriptLexeme* >::iterator &sli )
{
	
	scriptLexeme *lex;

	// очищаем тело макроса
	clearLexList( scrMacro.body );

	// смотрим а не закончился ли макрос
	if( sli == this->directiveLexemes.end() )
		return SRV_SUCCESS;

	// если не закончился
	// добавляем лексемы в список
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

	// получаем лексему следущую за ifdef
	std::list< scriptLexeme* >::iterator	sli;
	scriptMacros								*scrMacro;

	sli = this->directiveLexemes.begin();
	sli ++;	// ifdef
	sli ++;	// имя макроса

	// если на самом деле больше не найдено лексем
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
	
	// если макрос существует - то условие выполнено и компилить дальше можно
	if( scrMacro != NULL )
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );
	else // иначе обойти этот участок
		this->skipScanning.push( SST_SKIP_UNTIL_END );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseIfNdefDirective()
{

	// получаем лексему следущую за ifdef
	std::list< scriptLexeme* >::iterator	sli;
	scriptMacros							*scrMacro;

	sli = this->directiveLexemes.begin();
	sli ++;	// ifdef
	sli ++;	// имя макроса

	// если на самом деле больше не найдено лексем
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
	
	// если макрос не существует - то условие выполнено и компилить дальше можно
	if( scrMacro != NULL )
		this->skipScanning.push( SST_SKIP_UNTIL_END );
	else // иначе обойти этот участок
		this->skipScanning.push( SST_DO_NOT_SKIP_SCANNING );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseEndIfDirective()
{
	
	std::list< scriptLexeme* >::iterator	sli;
	
	// если за endif идет еще какая то лексема или endif использован сам по себе
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
	else // все нормально
		this->skipScanning.pop();
		
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::parseElseDirective()
{
	
	std::list< scriptLexeme* >::iterator	sli;
		
	// если за endif идет еще какая то лексема или endif использован сам по себе
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
	else // все нормально
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

	// получаем третью лексему
	sli = this->directiveLexemes.begin();
	sli++; // undef
	sli++; // macro name

	// если # undef
	if( sli == this->directiveLexemes.end() )
	{
		sli--;
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR01, *(*sli) ).code;
	}
	
	// если указано не имя а хрен знает что
	if( (*sli)->type != LT_ID && (*sli)->type != LT_RWORD )
		return this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR02, *(*sli) ).code;
	
	// получаем имя макроса
	if( (*sli)->type == LT_ID )
		name = (*sli)->name;
	else
		name = scriptRWordArray [ (*sli)->rwordType ];

	// если этого макроса нет
	if( ( scrMacro = this->macroExist( name ) ) == NULL )
	{
		// выдать предупрежде о том, что его здесь нет
		this->addError( SE_PREPROCESSOR_WARNING, SPPRV_UNDEFINED_MACRO, name.getCString(), *(*sli) );
		return SRV_SUCCESS;
	}

	// удалить макрос
	for( smi = this->parent->macros.begin(); smi != this->parent->macros.end(); smi++ )
	{
		if( *smi == scrMacro )
		{
			delete *smi;
			this->parent->macros.erase( smi );
			break;
		}
	}
	
	// если идут еще какие то лексемы
	if( this->directiveLexemes.size() > 3 )
		this->addError( SE_PREPROCESSOR_ERROR, SPPRV_UNDEF_DIRECTIVE_ERROR03, *this->directiveLexemes.front() );
	
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeErrorDirective()
{

	// здесь нам необходимо считать строку для ошибки	
	int	readSymbolCount = 0;
	
	// считываем оставшуюся часть 
	while( 1 )
	{
		
		// проверка на конец файла
		if( this->fInfo.filePos == this->fInfo.fileLength )
			break;
				
		// если получили конец строки
		if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0d )
		{
			this->fInfo.line ++;
			// если продолжение строки
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
		
		// если получили 0x0a
		if( this->fInfo.fp[ this->fInfo.filePos ] == 0x0a )
		{
			this->fInfo.filePos ++;
			this->fInfo.col		= 0;
			continue;
		}

		// все остальное добавить
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

	// получаем первую лексему
	sli1 = this->directiveLexemes.begin();
	// пропускаем символ #
	sli1 ++;

	// получаем вторую лексему
	sli2 = sli1;
	sli2 ++;

	// include ...
	if( sli2 == this->directiveLexemes.end() )
		return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR01, *(*sli1) ).code;
	
	// получаем третью лексему
	sli3 = sli2;
	sli3++;

	// если запись типа include "file.h"
	if( (*sli2)->type == LT_STRING )
	{
		// если еще что то идет после этого
		if( sli3 != this->directiveLexemes.end() )
			this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR02, *(*sli3) );
			
		return this->executeIncludeDirective( (*sli2)->name, false );
	}
	
	// если похоже на запись тип include < my file.h > т.е. < "my file.h" >
	if( (*sli2)->type == LT_PARSER && (*sli2)->parser == LPT_LESS_THAN )
	{
		// пытаемся получить первый идентификатор, затем '.', второй идентификатор и затем закрывающую скобку
		sli1 = sli2;
		sli1 ++;
		
		// если вместо первого идентификатора найден конец строки
		if( sli1 == this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR04, *(*sli2) ).code;
		// если не найдена строка 
		else if( (*sli1)->type != LT_STRING )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR05, *(*sli2) ).code;
				
		// наконец мы должны получить заврешающую треугольную скобку
		sli2 = sli1;
		sli2 ++;

		// ожидалась > но найден перевод строки
		if( sli2 == this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR06, *(*sli2) ).code;
		else if( (*sli2)->type != LT_PARSER || (*sli2)->parser != LPT_GREATER_THAN )// найдено что то другое
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR07, *(*sli2) ).code;
				
		// ожидаем конец строки
		sli3 = sli2;
		sli3 ++;

		// если конец строки не найден
		if( sli3 != this->directiveLexemes.end() )
			return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR08, *(*sli2) ).code;
			
		// теперь выполняем директиву
		return this->executeIncludeDirective( (*sli1)->name, true );

	}
	else // иначе - ошибка 
		return this->addError( SE_SCANER_FATAL_ERROR, SPPRV_INCLUDE_DIRECTIVE_ERROR03, *(*sli2) ).code;
			
	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::executeIncludeDirective( shString &str, bool inSysDir )
{
	
	// необходимо запустить следующий сканер и просканировать файл если возможно
	scriptRetCode		retCode;
		
	this->nextScaner = new scriptScaner();
	this->nextScaner->prevScaner	= this;
	this->nextScaner->parent		= this->parent;
	
	retCode = this->nextScaner->openIncludeFile( str, inSysDir );
	if( retCode == SRV_SUCCESS )
	{
		// сканируем следующий файл
		retCode = this->nextScaner->scan();
		// удаляем после завершения 
		delete this->nextScaner;
		this->nextScaner = NULL;
		// возвращаем его код		
		return retCode;
	}

	// если мы находимся здесь то произошла ошибка при открытии
	else
	{
		// записать ошибку и вернуться обратно
		this->addError( SE_PREPROCESSOR_FATAL_ERROR, retCode, *this->directiveLexemes.front() );
		
		// удаляем после завершения 
		delete this->nextScaner;
		this->nextScaner = NULL;
		return retCode;
	}

	return SRV_SUCCESS;
}

scriptRetCode scriptScaner::optimize( std::list< scriptLexeme* > &lexList )
{

	scriptRetCode	retCode;
	
	// оптимизируем разделители
	retCode = this->optimizeParsers( lexList );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// оптимизация строк
	retCode = this->optimizeStrings( lexList );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// оптимизация идентификаторов
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

	// считаем число косых черт перед символом
	for( index = dwLength - 1; index >= 0; index -- )
	{
		if( szString[ index ] == '\\' )
			escapeCount++;
		else
			break;
	}

	// если число нечетно - то управляющий escape последовательнось
	if( escapeCount & 0x01 )
		return true;
	 
	// завершающая ковычка
	return false;
}

scriptRetCode scriptScaner::formatString( TCHAR *szString, TCHAR *szString2, DWORD dwLength, scriptLexeme &lex )
{

	DWORD			ind1, ind2, charVal;
	bool			escapeStatement;
	TCHAR			buf[16];

	// сначала мы находимся не в escape последовательности
	escapeStatement = false;
	
	// цикл по числу символов в первой строке
	for( ind1 = 0, ind2 = 0; ind1 < dwLength; )
	{
		// если не escape символ
		if( escapeStatement == false )
		{
			if( szString[ ind1 ] != '\\' )
			{
				// просто записываем его в выходную строку
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
			escapeStatement = false;	// выходим из управляющего режима
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
			
			// восьмеричное число
			case '1': case '2':	case '3': case '4': case '5': case '6': case '7':
					
				// присваиваем первую цифру
				charVal = szString[ ind1 ] - 0x30;
				
				// проверяем вторую цифру
				if( szString[ ind1 + 1 ] < '0' || szString[ ind1 + 1 ] > '7' )
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
					break;
				}
				
				ind1 ++;	
				
				// записываем вторую цифру
				charVal = charVal * 8 + szString[ ind1 ] - 0x30;

				// проверяем третью цифру
				if( szString[ ind1 + 1 ] < '0' || szString[ ind1 + 1 ] > '7' ) 
				{
					szString2[ ind2 ] = ( TCHAR ) charVal;
					break;
				}
				
				ind1 ++;	
				charVal = charVal * 8 + szString[ ind1 ] - 0x30;
				if( charVal > 255 )
				{
					// число больше чем 255
					// составляем ошибку
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
				 
				// первое число
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
				// записываем неудачный элемент
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
	
	// если лексем меньше 3 - то не найдено
	if( this->directiveLexemes.size() < 1 )
		return false;

	// получаем последнюю лексему
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

	// если идентификатор уже есть в таблице
	if( this->parent->idExist( lex.name, tableIndex ) )
	{
		// то просто получить его индекс
		lex.index = tableIndex;
	}
	else 
	{
		// сначала добавить его в список идентификаторов и затем получить его индекс
		lex2 = new scriptLexeme( lex );
		lex2->scaner = this;
		lex.index = (int) this->parent->ids.size();
		
		this->parent->ids.push_back( lex2 );
	}

	// очистим имя идентификатора
	lex.name.clear();

	return ;
}

void scriptScaner::createStringReference( scriptLexeme &lex )
{

	int				tableIndex;
	scriptLexeme	*lex2;
	
	// если есть уже такая строка в таблице
	if( this->parent->stringExist( lex.name, tableIndex ) )
	{	
		// то просто получить ее индекс
		lex.index = tableIndex;
	}
	else 
	{
		// добавляем ее в таблицу строк и потом только получаем ее индекс
		lex2 = new scriptLexeme( lex );
		lex2->scaner = this;
		lex.index = (int) this->parent->constStrings.size();
		
		this->parent->constStrings.push_back( lex2 );
	}
	
	// очистим текст строки
	lex.name.clear();
				
	return ;
}

scriptRetCode scriptScaner::optimizeStrings( std::list< scriptLexeme* > &lexList )
{

	std::list< scriptLexeme* >::iterator	sli, sli2;
		
	// начинаем не с нулевого элемента а с первого
	sli = lexList.begin();
	sli ++;
	for( ; sli != lexList.end(); sli ++ )
	{
		
		// если не строка - продолжаем работу
		if( (*sli)->type != LT_STRING )
			continue;
		
		// предыдущая лексема		
		sli2 = sli;
		sli2 --;
	
		// если предыдущая лексема не строка - продолжаем работу
		if( (*sli2)->type != LT_STRING )
			continue;
		
		// присоединяем текущую лексему к предыдущей
		(*sli2)->name.concat( (*sli)->name );
		// удаляем текущую лексему
		delete *sli;
		// теперь текущей становится лексема за sli2
		lexList.erase( sli );
		sli = sli2;
	}

	// теперь мы заносим строки в таблицу строк
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
	
	// нельзя оптимизировать унарные опреаторы, как бы мне этого не хотелось
	// поскольку определить унарный оператор или нет очень геморно на стадии сканирования
	// например запись
	// -- - 123
	// может быть унарный минус 
	// если запись типа 
	// -- - 123
	// или бинарный минус - если запись типа 
	// id -- - 123
	// но главный аргумент - то что оптимизировать константы еще можно
	// а оптимизировать переменные нельзя
	// поэтому оптимизириуем на стадии лексического анализатора
	// для макросов оптимизируем на уровне сканирования макроса

	// а вот для макросов все просто, поскольку операции -- или ++ в выражении 
	// для if и elif быть не должно

	// оптимизируем унарные операторы - и + для констант
	for( sli1 = begin; sli1 != end; )
	{

		// первую лексему мы вообще просматривать не будем
		if( sli1 == begin )
		{
			sli1 ++;
			continue;
		}

		// предыдущая лексема
		sli2 = sli1;
		sli2 --;

		// вторую лексему также просматривать не будем
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
				// лексема со знаком минус
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
				// удаляем вторую лексему
				delete lex2;
				lexList.erase( sli2 );

				// переходим на одну лексему назад для повторной проверки
				sli1 = sli3;
			}
			else
				sli1 ++;
		}
		else // если ничего не нашли то просто двигаемся вперед
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