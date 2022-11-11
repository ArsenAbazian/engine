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
	
	// ������� ������� ��������
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

	// ������ ��� �����
	fullPathName	= this->progDirectory;
	fullPathName	+= TEXT( "\\" );
	fullPathName	+= fileName;

	// ������� ������ ������
	module			= new scriptModule( this );
	
	// �������������� ���
	retCode = module->initialize( this->moduleList.size() );
	if( retCode != SRV_SUCCESS )
	{
		delete module;
		return retCode;
	}

	// ��������� ������ � ������ �������
	this->moduleList.push_back( module );
	
	// ������������ ������
	return module->compile( fullPathName );

}

// ���� �������� - � ������ ����� ����������� ������ � ���������� 
// �������� ��� �� �����
scriptRetCode	scriptProgramm::loadLibrary( shString &fileName )
{
	

	return SRV_SUCCESS;
}

scriptRetCode	scriptProgramm::compile()
{
	
	// ������� ����������� ���������� �����������
	this->clearResults();
		
	// �������� �� ������ ��������� ���������
	// ��� ������� ����� - ���� �� source ���� 
	// ��� lib ���� ��������� ������ scriptModule
	// � �� ��� ����������
    
	// ������� ������������ source file
	std::list< shString >::iterator		srcIndex, libIndex;
	
	// �������� ������ ������
	for( srcIndex = this->sourceFiles.begin(); srcIndex != this->sourceFiles.end(); srcIndex ++ )
		this->compileModule( *srcIndex );
	
	// ������ ��������� lib �����
	for( libIndex = this->libFiles.begin(); libIndex != this->libFiles.end(); libIndex ++ )
		this->loadLibrary( *libIndex );

	return SRV_SUCCESS;
}