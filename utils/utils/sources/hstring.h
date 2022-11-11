/*********************************************************************
*	
*		class shString	- simple hash string created by Abazyan Arsen
*		Tula 12.03.2004		20:47
*		e-mail: ars_s@uic.tula.ru
*		
*
**********************************************************************/

#ifndef A_HSTRING_INC
#define A_HSTRING_INC

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "serialization.h"

class	shString : public a3dSerializableObject
{
	
	TCHAR				*szString;						// character string
	unsigned long		dwHashCode;						// string hash code
	unsigned long		dwLength;						// lenght of the string, without '\0'

public:
	shString();											// default constructor
	~shString();										// destructor
	shString( const shString &src );					// copy initialization constructor
	shString( const TCHAR *str );						// initialization from string constant 
	shString& operator= ( const shString &src );		// assing initialization
	shString& operator= ( const TCHAR*);				
	shString& operator+= ( const shString &src );
	shString& operator+= ( const TCHAR* src );
	bool operator== ( const shString &src );			// boolean == opeartor
	bool operator== ( const TCHAR *str );			
	bool operator!= ( const shString &src ){ return ! operator==( src ); }
	bool operator!= ( const TCHAR *str ){ return ! operator==( str ); }
	
	shString&		concat( const shString &hstr );
	
	unsigned long	createHashCode( const TCHAR *str );	// creates hash code of string
	unsigned long	createHashCode( const TCHAR *str, unsigned long strsize );	
	unsigned long	getHashCode(){						// get hash code 
						return dwHashCode; 
					}	
	
	unsigned long	getLength(){						// get string lenght
						return dwLength; 
					}		
	
	TCHAR*		getCString(){						// get string of characters
						return szString; 
					}	
	
	bool			isEqual( const shString &src );		// same as == operator
	bool			isEqual( const TCHAR *str );		
	void			clear();							// clear string
	void			copy( const shString& src );		// copy one string from another
	void			initialize( const TCHAR *str );	 
	void			initialize( const TCHAR *str, int len);

	int				findFromLeft( TCHAR symb );			// ����� ������ ������ � ������ �����
	int				findFromRight( TCHAR symb );		// ����� ������ ������ � ������� �����
	shString&		deleteSubString( int beginPos, int count );	// ������� �� ������ ���������
	shString&		deleteLeft( int toPos );
	shString&		deleteRight( int fromPos ); 

	shString&		getModuleDirectory();				// �������� � ���� ������ ���������� ���������
	shString&		getModuleFileName();				// �������� � ���� ������ ��� ����� ���������
	shString&		getModuleFullPathName();			// �������� � ���� ������ ������ ���� ���������
	shString&		extractFileName();					// �������� ��� ����� �� ������� ����
	shString&		extractDirectory();					// �������� ��� ���������� �� ������� ����
	
	shString&		toLowerCase();						// ������������� ��� � ������� ��������
	shString&		toUpperCase();						// ������������� ��� � �������� ��������
	shString&		cutSpacesLeft();					// ������� ������� ��������� � ������ �����
	shString&		cutSpacesRight();					// ������� ������� ��������� � ������ ������
	shString&		cutSpacesFromTheEnds();				// ������� ������� ��������� � ������� ����� � ������
	bool			loadFile(shString &fileName);		// ��������� ���� ����� �� ����� � ������
	bool			saveToFile(shString &fileName);		// ��������� ��� ������ � ����
	shString&		removeSymbol( TCHAR symb );
	shString&		appendPath( shString &path );		// � ����� ����� �������� ����
	bool			endsWith( shString &str );			// ������������ �������
	int				hasSubstr( shString &str );			// �������� ���������
	bool			hasSubstr( shString &str, int beginPos ); // �������� ��������� ������� � �������
	void			copyToChar( char *buffer );
	void			copyFromChar( char *buffer );
	void			removePath( shString &fileName, shString &dataDir, shString &shortDataDir );
	void			replacePath( shString &fileName, shString &dataDir, shString &shortDataDir );
	bool			readString( TCHAR *text, int stringNumber );

	int				calcStringSize( bool asChar );
	int				calcSizeCore() { return calcStringSize( true ); }
	int				getHeaderCode() { return MAKE_HEADER( 's', 't', 'r', ' ' ); }
	bool			saveToMemoryCore( a3dMemoryFile &file );
	bool			loadFromMemoryCore( a3dMemoryFile &file );
};

size_t strToWCS(char *str, TCHAR *wcs);
size_t wcsToStr(TCHAR *wcs, char *str);

#endif