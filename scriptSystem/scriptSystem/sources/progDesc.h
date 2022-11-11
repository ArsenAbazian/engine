#ifndef __PROGRAMM_DESC_INC
#define __PROGRAMM_DESC_INC

#include "scaner.h"
#include "syntparser2.h"
#include "moduleDesc.h"

// �������� ���������
class scriptProgramm
{

public:
	// �������� ���������
	shString					progName;				// ��� ����� ���������
	shString					progDesc;				// �������� ���������
	shString					progOrganization;		// ��� �����������
	shString					progVersionDesc;		// �������� ������
	DWORD						progVersionMajor;		// ������ ��������� ������� �����
	DWORD						progVersionMinor;		// ������ ��������� ������� �����
	DWORD						progBuild;				// ������� ��� ����������� ���������

	shString					progDirectory;			// ���������� ���������
	
	std::list< shString	>		includePath;			// ���� � ������ include
	std::list< shString >		libPath;				// ���� � ������ lib ������
	std::list< shString >		libFiles;				// �������������� lib �����
	std::list< shString >		sourceFiles;			// ����� ��������

	std::list< scriptModule* >	moduleList;				// ������ �������

public:
	
	scriptProgramm();									// �����������
	~scriptProgramm();									// ����������

	// ��������� �������� ���������
	void					clear();
	// �������� ���������� ���������� �����������
	void					clearResults();

	// ������������� ���� ������
	scriptRetCode			compileModule( shString &fileName );
	// ������������� ���������
	scriptRetCode			compile();
	
	// ��������� ���� ����������
	scriptRetCode			loadLibrary( shString &fileName );
};

#endif