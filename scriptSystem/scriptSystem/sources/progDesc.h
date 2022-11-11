#ifndef __PROGRAMM_DESC_INC
#define __PROGRAMM_DESC_INC

#include "scaner.h"
#include "syntparser2.h"
#include "moduleDesc.h"

// описание программы
class scriptProgramm
{

public:
	// свойства программы
	shString					progName;				// им€ файла программы
	shString					progDesc;				// описание программы
	shString					progOrganization;		// им€ организации
	shString					progVersionDesc;		// описание версии
	DWORD						progVersionMajor;		// верси€ программы старша€ часть
	DWORD						progVersionMinor;		// верси€ программы младша€ часть
	DWORD						progBuild;				// сколько раз компилилась программа

	shString					progDirectory;			// директори€ программы
	
	std::list< shString	>		includePath;			// пути к папкам include
	std::list< shString >		libPath;				// пути к папкам lib файлов
	std::list< shString >		libFiles;				// дополнительные lib файлы
	std::list< shString >		sourceFiles;			// файлы скриптов

	std::list< scriptModule* >	moduleList;				// список модулей

public:
	
	scriptProgramm();									// конструктор
	~scriptProgramm();									// деструктор

	// полностью очистить программу
	void					clear();
	// очистить результаты предыдущей компилл€ции
	void					clearResults();

	// компилировать один модуль
	scriptRetCode			compileModule( shString &fileName );
	// компилировать программу
	scriptRetCode			compile();
	
	// загрузить одну библиотеку
	scriptRetCode			loadLibrary( shString &fileName );
};

#endif