#include "objDesc.h"

void clearObjList( std::list< scriptObjectDesc* > &sodl )
{

	std::list< scriptObjectDesc* >::iterator	sodi;

	for( sodi = sodl.begin(); sodi != sodl.end(); sodi ++ )
		delete *sodi;

	sodl.clear();
	
	return ;
}

// ����������� �� ���������
scriptObjectDesc::scriptObjectDesc( )
{
}

// ���������� - ������ �� ���� ������
scriptObjectDesc::~scriptObjectDesc()
{
}

