#include "objDesc.h"

void clearObjList( std::list< scriptObjectDesc* > &sodl )
{

	std::list< scriptObjectDesc* >::iterator	sodi;

	for( sodi = sodl.begin(); sodi != sodl.end(); sodi ++ )
		delete *sodi;

	sodl.clear();
	
	return ;
}

// конструктор по умолчанию
scriptObjectDesc::scriptObjectDesc( )
{
}

// деструктор - ничего не надо делать
scriptObjectDesc::~scriptObjectDesc()
{
}

