#include "progDesc.h"
		
scriptProgramm::scriptProgramm()
{
	
	this->progVersionMajor	= 0;
	this->progVersionMinor	= 0;
	this->progBuild			= 0;

}

scriptProgramm::~scriptProgramm()
{
	this->clear();
}

void	scriptProgramm::clear()
{
	
	// сначала очищаем свойства
	this->progName.clear();		
	this->progDesc.clear();		
	this->progOrganization.clear();
	this->progVersionDesc.clear();
	this->progVersionMajor	= 0;
	this->progVersionMinor	= 0;
	this->progBuild			= 0;

	this->progDirectory.clear();	
		 
	this->includePath.clear();	
	this->libPath.clear();		
	this->libFiles.clear();		
	this->sourceFiles.clear();	
		 
	this->clearResults();
	
	return ;
}

void	scriptProgramm::clearResults()
{

	clearModuleList( this->moduleList );
	
	return ;
}

scriptRetCode	scriptProgramm::compileModule( shString &fileName )
{
	
	scriptModule	*module;
	shString		fullPathName;
	scriptRetCode	retCode;

	// полное им€ файла
	fullPathName	= this->progDirectory;
	fullPathName	+= TEXT( "\\" );
	fullPathName	+= fileName;

	// создаем объект модуль
	module			= new scriptModule( this );
	
	// инициализируем его
	retCode = module->initialize( this->moduleList.size() );
	if( retCode != SRV_SUCCESS )
	{
		delete module;
		return retCode;
	}

	// добавл€ем модуль в список модулей
	this->moduleList.push_back( module );
	
	// компиллируем модуль
	return module->compile( fullPathName );

}

// пока заглушка - а вообще будет создаватьс€ объект и загражтьс€ 
// бинарный код из файла
scriptRetCode	scriptProgramm::loadLibrary( shString &fileName )
{
	

	return SRV_SUCCESS;
}

scriptRetCode	scriptProgramm::compile()
{
	
	// очистка результатов предыдущей компилл€ции
	this->clearResults();
		
	// начинаем по списку компилить программу
	// дл€ каждого файла - будь до source файл 
	// или lib файл создаетс€ объект scriptModule
	// и он уже компилитс€
    
	// сначала обрабатываем source file
	std::list< shString >::iterator		srcIndex, libIndex;
	
	// компилим каждый модуль
	for( srcIndex = this->sourceFiles.begin(); srcIndex != this->sourceFiles.end(); srcIndex ++ )
		this->compileModule( *srcIndex );
	
	// теперь добавл€ем lib файлы
	for( libIndex = this->libFiles.begin(); libIndex != this->libFiles.end(); libIndex ++ )
		this->loadLibrary( *libIndex );

	return SRV_SUCCESS;
}