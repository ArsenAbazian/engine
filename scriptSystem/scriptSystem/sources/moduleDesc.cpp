#include "moduleDesc.h"
#include "progDesc.h"

// �������� ������ �������
void clearModuleList( std::list< scriptModule* > &ml )
{

	std::list< scriptModule* >::iterator	mli;

	for( mli = ml.begin(); mli != ml.end(); mli ++ )
		delete *mli;
	
	return ;
}

// �����������
scriptModule::scriptModule( scriptProgramm *prg )
{

	this->programm			= prg;

}

scriptRetCode	scriptModule::initialize( int ind )
{
	
	scriptRetCode	retCode;

	// ���������� ������ ������
	this->index		= ind;
	
	// ������������� �������
	retCode = this->scaner.initialize( this->programm->includePath );
	if( retCode != SRV_SUCCESS )
		return retCode;
	
	// ������������� ������
	parser.setModule( this );
	
	// ������� ��� ����
	parser.initializeLog( shString( TEXT( "parser.log" ) ) );
	
	return SRV_SUCCESS;
}

scriptRetCode	scriptModule::compile( shString fileName )
{

	scriptRetCode	retCode;

	// ��������� ���� ��� ������������
	retCode = this->scaner.openFile( fileName.getCString() );
	if( retCode != SRV_SUCCESS )
		return retCode;

	// ��������� ����
	retCode = this->scaner.scan();

	// ������������ ����
	retCode = this->scaner.optimize();

	// ���������� �������������� ������ �����
	retCode = parser.parse( this->scaner );

	return SRV_SUCCESS;
}

void scriptModule::clear()
{

	// ������� ������� ������ ��������

	
	return ;
}