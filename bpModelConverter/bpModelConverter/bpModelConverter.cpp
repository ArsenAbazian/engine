// bpModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf( TEXT( "BoogiePerets Max model converter. (c) BoogiePerets 2007.\n" ) );
	if( argc < 3 ) { 
		_tprintf( TEXT( "usage: bpModelConverter.exe input_file output_file\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch(); 
		return 0;
	}
	
	shString fileName( argv[1] );
	a3dTextModelFile file;
	if( !file.loadFromFile( fileName ) ) {
		_tprintf( TEXT( "error: can't import file. parsing error.\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	_tprintf( TEXT( "file successfully converted.\n" ) );
	
	a3dModel model;

	if( !model.import( file ) ) { 
		_tprintf( TEXT( "a3dModel import error!\n" ) ); 
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	_tprintf( TEXT( "import model -> success.\n" ) );
	if( !model.saveToFile( shString( argv[2] ) ) ) { 
		_tprintf( TEXT( "a3dModel save error!\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	
	if( argc > 3 && shString( argv[3] ) == shString( TEXT( "debug" ) ) ) {
		
		if( !model.loadFromFile( shString( argv[2] ) ) ) { 
			_tprintf( TEXT( "a3dModel load error!\n" ) );
			_tprintf( TEXT( "press any key.\n" ) );
			_gettch();
			return 0;
		}
		
		shString testFile( argv[2] );
		
		shString testFile2 = testFile.extractDirectory();
		testFile2.concat( shString( TEXT("\\debug.amd") ) );

		if( !model.saveToFile( testFile2 ) ) { 
			_tprintf( TEXT( "a3dModel save error!\n" ) );
			_tprintf( TEXT( "press any key.\n" ) );
			_gettch();
			return 0;
		}

	}
	
	_tprintf( TEXT( "a3dModel saved!\n" ) );
	_tprintf( TEXT( "press any key.\n" ) );
	_gettch();
	
	return 0;
}

