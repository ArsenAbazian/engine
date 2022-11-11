#include "moduleDesc.h"
#include "progDesc.h"

// очистить список модулей
void clearModuleList( std::list< scriptModule* > &ml )
{

	std::list< scriptModule* >::iterator	mli;

	for( mli = ml.begin(); mli != ml.end(); mli ++ )
		delete *mli;
	
	return ;
}

// конструктор
scriptModule::scriptModule( scriptProgramm *prg )
{

	this->programm			= prg;

}

scriptRetCode	scriptModule::initialize( int ind )
{
	
	scriptRetCode	retCode;

	// запоминаем индекс модуля
	this->index		= ind;
	
	// инициализация сканера
	retCode = this->scaner.initialize( this->programm->includePath );
	if( retCode != SRV_SUCCESS )
		return retCode;
	
	// устанавливаем модуль
	parser.setModule( this );
	
	// создаем лог файл
	parser.initializeLog( shString( TEXT( "parser.log" ) ) );
	
	return SRV_SUCCESS;
}

scriptRetCode	scriptModule::compile( shString fileName )
{

	scriptRetCode	retCode;

	// открываем файл для сканирования
	retCode = this->scaner.openFile( fileName.getCString() );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// сканируем файл
	retCode = this->scaner.scan();

	// оптимизируем файл
	retCode = this->scaner.optimize();

	// производим синтаксический анализ файла
	retCode = parser.parse( this->scaner );

	return SRV_SUCCESS;
}

void scriptModule::clear()
{

	// сначала очищаем список объектов

	
	return ;
}