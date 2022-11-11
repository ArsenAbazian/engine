#ifndef __PREPROCESSOR_INC
#define __PREPROCESSOR_INC

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <list>

#include "hstring.h"
#include "lexeme.h"
#include "scrmacro.h"
#include "screrror.h"

#define SPP_FILE_MAX_COUNT	1024	// этот макрос показывает размера массива дескрипторов файлов 

class scriptPreProcessor
{
private:
	// конфигурация
	std::list< shString >					includeDirList;					// директоии для включаемых файлов
	shString								programmPath;
	// файловая информация
	fileInfo								files[ SPP_FILE_MAX_COUNT ];	// файлы и имена
	shString								outFileName;					// имя файла выхода
	FILE									*fpOut;							// файл выхода
	int										fileCount;						// число открытых файлов
	
	// макросы
	std::list< scriptMacros >				macros;							// список макросов
		
	// события
	std::list< scriptError* >				errorList;						// список ошибок и предупреждений	
	std::list< scriptError* >				warningList;						

	// временный буффер
	TCHAR									tempBuffer[ 2048 ];				// 2 килобайта под строковый буффер

public:
	scriptPreProcessor();													// конструктор
	~scriptPreProcessor();													// деструктор
	
	scriptRetCode	initialize( std::list< shString > &incDir, shString &progPath );	// инициализация - библиотеки заголовочных файлов плюс директория программы
	scriptRetCode	clear();												// очистка информации прероцессора
	scriptRetCode	openInputFile( TCHAR *fileName, bool inSystem );		// открыть файл для чтения
	scriptRetCode	closeInputFile();
	scriptRetCode	openOutputFile( TCHAR *fileName );						// открыть файл для записи
	scriptRetCode	preProcess();											// преобразовать ее

	scriptRetCode	getFullPathName( shString &directory, shString &fileName, shString &fullPathName );
	bool			isFileOpened( shString &fileName );						// смотрит открыт ли уже такой файл
	void			runUntilEOL( fileInfo &fInfo );							// пропускаем символы до конца строки или файла

	bool			isMacros( fileInfo &fInfo, scriptMacros &mac );			// смотрит, есть ли в списке макросов, макрос с таким именем

};

#endif