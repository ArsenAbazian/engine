// bpSceneConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf( TEXT( "BoogiePerets Max scene converter. (c) BoogiePerets 2007.\n" ) );
	if( argc < 3 ) { 
		_tprintf( TEXT( "usage: bpSceneConverter.exe input_file output_file\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	
	shString fileName( argv[1] );
	a3dTextSceneFile file;
	if( !file.loadFromFile( fileName ) ) {
		_tprintf( TEXT( "error: can't import file. parsing error.\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	_tprintf( TEXT( "file successfully converted.\n" ) );
	
	a3dScene scene;

	if( !scene.import( file ) ) { 
		_tprintf( TEXT( "a3dScene import error!\n" ) ); 
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	if( !scene.saveToFile( shString( argv[2] ) ) ) { 
		_tprintf( TEXT( "a3dScene save error!\n" ) );
		_tprintf( TEXT( "press any key.\n" ) );
		_gettch();
		return 0;
	}
	
	if( argc > 3 && shString( argv[3] ) == shString( TEXT( "debug" ) ) ) {
		
		if( !scene.loadFromFile( shString( argv[2] ) ) ) { 
			_tprintf( TEXT( "a3dScene load error!\n" ) );
			_tprintf( TEXT( "press any key.\n" ) );
			_gettch();
			return 0;
		}
		
		shString testFile( argv[2] );
		
		shString testFile2 = testFile.extractDirectory();
		testFile2.concat( shString( TEXT("\\debug.asc") ) );

		if( !scene.saveToFile( testFile2 ) ) { 
			_tprintf( TEXT( "a3dScene save error!\n" ) );
			_tprintf( TEXT( "press any key.\n" ) );
			_gettch();
			return 0;
		}
	}
	
	_tprintf( TEXT( "a3dScene saved!\n" ) );
	_tprintf( TEXT( "press any key.\n" ) );
	_gettch();
	
	return 0;
}

