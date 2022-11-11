#include "hstring.h"
#include "memoryFile.h"

// default constructor
shString::shString()
{
	// clear all params
	this->szString = NULL;
	this->dwLength = 0;
	this->dwHashCode = 0;
}

// destructor
shString::~shString()
{
	this->clear();
}

// copy initialization constructor
// example shString a = b;
shString::shString( const shString &src )
{

	// we must set szString to NULL 
	// so string will be not destroed uninitializing
	this->szString = NULL;
	copy( src );		

}

// clear string
void shString::clear()
{

	if( szString )delete[] szString;			// delete string 
	this->szString = NULL;
	this->dwLength = 0;
	this->dwHashCode = 0;
	
	return ;
}

// initialization string constant 
void shString::initialize( const TCHAR *str )
{
	
	clear();	// clear previous string
	if( str == NULL )return;
	
	if( str )
	{	// protect from NULL ;)

		this->dwLength = (unsigned long)_tcslen(str);
		this->szString = new TCHAR[this->dwLength + 1];	// +1 for '\0' 
		memcpy(this->szString,str,sizeof(TCHAR) * (dwLength+1));
		this->dwHashCode = createHashCode(str,dwLength);

	}
}

// initialization string constant 
void shString::initialize( const TCHAR *str, int strLen )
{
	clear();	// clear previous string
	if( str == NULL )return;

	this->dwLength = strLen;
	this->szString = new TCHAR[strLen + 1];	// +1 for '\0' 
	memcpy(this->szString,str,sizeof(TCHAR) * (strLen));
	this->szString[ strLen ] = 0;
	this->dwHashCode = createHashCode(str,dwLength);
}

// copy constructor
shString::shString(const TCHAR *str)
{
	
	// we must set szString to NULL 
	// so string will be not destroed uninitializing

	this->szString = NULL;
	initialize(str);
	
}

// assing initialization 
shString& shString::operator= ( const shString &src )
{
	
	this->copy(src);	// copy

	return *this;
}

// assign initialization from 
shString& shString::operator= ( const TCHAR* str )
{
	
	initialize(str);
	
	return *this;
}


bool shString::isEqual( const shString &src )
{

	unsigned long i;
	
	if( this->dwHashCode == src.dwHashCode &&
		this->dwLength == src.dwLength )
	{	// now we must compare characters
		
		for( i=0; i<this->dwLength; i++ ) 
			if( this->szString[ i ] != src.szString[ i ] )return false;
		
		// all characters are equal
		return true;
	}
	
	return false;
}

bool shString::isEqual( const TCHAR *str )
{

	unsigned long	tmpHash = 0;
	unsigned long	tmpLength = 0;
	unsigned long	i;
	
	if(str)
	{	// protect from NULL ;)	
		tmpLength = ( unsigned long )_tcslen( str );
		tmpHash = createHashCode( str );
	}
	
	if( this->dwHashCode == tmpHash &&
		this->dwLength == tmpLength )
	{	// now we must compare characters

		for( i=0; i<this->dwLength; i++ )
			if( this->szString[ i ] != str[ i ] )return false;

		// all characters are equal
		return true;
	}

	return false;
}

// creates hash code of string
unsigned long shString::createHashCode( const TCHAR *str, unsigned long strsize )
{
	
	unsigned long	Size4,
					Size,
					HashCode,
					i,k;

	unsigned long	*szString4;
	BYTE			*bstr;

	if( str == NULL ) return 0;
	
	strsize *= sizeof( TCHAR );			// convert wchars to bytes
	
	Size4	= strsize >> 2;				// >> 2 - is division by 4	
	Size	= strsize & 0x03;			// остаток от деления на 4 ;)))
	
	szString4 = ( unsigned long * )str;	// we will process 4 bytes of char
	HashCode  = 0;
	
	if( Size4 )
	{
		for( i=0; i< Size4; i++ )
		{
			HashCode ^= szString4[ i ];	// hash is simple xor ;)
		}
	}

	if( Size )
	{ // process other bytes
		bstr = (BYTE*)str;
		for( k=8, i= strsize - Size; i< strsize; i++ )
		{
			HashCode ^= (( unsigned long )bstr[ i ] << k );	// xor with other chars
			k += 8;
		}
	}

	return HashCode;
}

// creates hash code of string
unsigned long shString::createHashCode( const TCHAR *str )
{
	
    unsigned long StrSize;
	
	if(str == NULL)return 0;
	
	StrSize = ( unsigned long )_tcslen( str );	
	
	return this->createHashCode( str,StrSize );
}

// copy one string from another
void shString::copy( const shString& src )
{

	clear();	// clear previous string

	// copy src params
	this->dwLength		= src.dwLength;
	this->dwHashCode	= src.dwHashCode;

	if( src.szString )
	{ // if src string not empty

		this->szString = new TCHAR[ this->dwLength + 1 ];	// + 1 for '\0'
		memcpy( this->szString, src.szString, sizeof( TCHAR ) * ( this->dwLength + 1 ) );	// copy str characters

	}
}

// boolean == opeartor
bool shString::operator== ( const shString &src )
{

	return this->isEqual(src);
}


bool shString::operator== ( const TCHAR *str )
{

	return this->isEqual(str);
}


shString& shString::concat( const shString &hstr )
{
	
	TCHAR *mybuf = new TCHAR [8192];
		
	_stprintf_s( mybuf, 8192, TEXT("%s"), getCString() );
	_stprintf_s( &mybuf[dwLength], 8192 - dwLength,TEXT("%s"), hstr.szString ); // correct this later
		
	initialize( mybuf );
	delete[] mybuf;
	
	return *this;
}

shString& shString::operator+= ( const shString &src )
{
	return concat( src );
}

shString& shString::operator+= ( const TCHAR *src )
{
	shString hsrc( src );
	return concat( src );
}

int shString::findFromLeft( TCHAR symb )
{

	int	symbPos;
	
	for( symbPos = 0; symbPos < (int)this->dwLength; symbPos++ )
	{
		if( this->szString[ symbPos ] == symb )
			return symbPos;
	}

	return -1;
}

int shString::findFromRight( TCHAR symb )
{

	int	symbPos;
	
	for( symbPos = this->dwLength; symbPos >= 0; symbPos-- )
	{
		if( this->szString[ symbPos ] == symb )
			return symbPos;
	}

	return -1;
}

shString& shString::deleteSubString( int beginPos, int count )
{
	
	int		pos = 0;

	if( count == 0 )
		return *this;

	if( beginPos >= (int)this->dwLength )
		return *this;

	TCHAR *mybuf = new TCHAR [8192];
	// если возможно - копируем начало
	if( beginPos != 0 )
	{
		memcpy( mybuf, this->szString, sizeof( TCHAR ) * beginPos );
		pos = beginPos;
	}	

	// копируем оставшуюся часть	
	if( beginPos + count < (int)this->dwLength )
	{
		memcpy( &mybuf[ pos ], &szString[ beginPos + count ], sizeof( TCHAR ) * ( this->dwLength - beginPos - count ) );
		pos += this->dwLength - beginPos - count;
	}
	
	mybuf[ pos ] = 0;

	initialize( mybuf );
	delete[] mybuf;

	return *this;
}

shString& shString::deleteLeft( int toPos )
{
	return this->deleteSubString( 0, toPos ); 
}

shString& shString::deleteRight( int fromPos )
{
	return this->deleteSubString( fromPos, this->dwLength - fromPos );
}

shString& shString::getModuleFullPathName()
{
	TCHAR *mybuf = new TCHAR [8192];
	
	GetModuleFileName( GetModuleHandle( NULL ), mybuf, 8192 );
	
	initialize( mybuf );
	delete[] mybuf;

	return *this;
}

shString& shString::extractDirectory()
{
	int			fileNamePos;
	
	fileNamePos = this->findFromRight( '\\' );
	if( fileNamePos != -1 )
		return this->deleteRight( fileNamePos );		
	else {
		initialize( TEXT("") );
		return *this;
	}

	return *this;
}

shString& shString::extractFileName()
{
	int fileNamePos;
	
	fileNamePos = this->findFromRight( '\\' );
	if( fileNamePos != -1 )
		return this->deleteLeft( fileNamePos + 1 );
	else {
		initialize( TEXT("") );
		return *this;
	}

	return *this;
}

shString& shString::getModuleDirectory()
{
	// получаем полное имя файла
	this->getModuleFullPathName();

	return this->extractDirectory();

}

shString& shString::getModuleFileName()
{
	// получаем полное имя файла
	this->getModuleFullPathName();
	
	return this->extractFileName();
}

shString& shString::toLowerCase()
{

	DWORD sIndex;
	
	for( sIndex = 0; sIndex < this->dwLength; sIndex++ )
	{	
		if( isupper( this->szString[ sIndex ] ) )
			this->szString[ sIndex ] = _tolower( this->szString[ sIndex ] );
	}
	
	this->dwHashCode = createHashCode( szString, this->dwLength );

	return *this;
}

shString& shString::toUpperCase()
{

	DWORD sIndex;
	
	for( sIndex = 0; sIndex < this->dwLength; sIndex++ )
	{	
		if( islower( this->szString[ sIndex ] ) )
			this->szString[ sIndex ] = _toupper( this->szString[ sIndex ] );
	}
	
	this->dwHashCode = createHashCode( szString, this->dwLength );

	return *this;
}

shString& shString::cutSpacesLeft()
{

	DWORD	index;
	
	if( this->dwLength == 0 )
		return *this;
	
	TCHAR	*mybuf;
	for( index = 0; index < this->dwLength; index++ )
	{
		if( this->szString[ index ] != 0x20 && 
			this->szString[ index ] != 0x09 )
		break;
	}
	if( index == 0 )
		return *this;

	// временный буффер
	mybuf = new TCHAR[8192];
	// копируем строку
	 _tcscpy_s( mybuf, 8192, &this->szString[ index ] );
	// инициализация
	this->initialize( mybuf );

	delete[] mybuf;
	
	return *this;
}

shString& shString::cutSpacesRight()
{

	int		index;
	
	if( this->dwLength == 0 )
		return *this;
	
	TCHAR	*mybuf;
	for( index = (int)this->dwLength-1; index >= 0; index-- )
	{
		if( this->szString[ index ] != 0x20 && 
			this->szString[ index ] != 0x09 )
		break;
	}
	if( index == this->dwLength - 1 )
		return *this;

	this->szString[ index + 1 ] = 0;
	
	// временный буффер
	mybuf = new TCHAR[8192];
	// копируем строку
	_tcscpy_s( mybuf, 8192, this->szString );
	// инициализация
	this->initialize( mybuf );

	delete[] mybuf;
	
	return *this;
}

shString& shString::cutSpacesFromTheEnds() { 
	
	this->cutSpacesLeft();
	this->cutSpacesRight();

	return *this;
}

bool shString::loadFile( shString &fileName ) { 
	
	FILE *fp;

	_tfopen_s(&fp, fileName.getCString(), TEXT("rt"));
	if(fp == NULL) return false;
	fseek(fp, 0, SEEK_END);
	int fileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE *lpBuffer = new BYTE[fileLen+1];
	TCHAR *lpTBuffer = new TCHAR[fileLen+1];
	fread(lpBuffer, sizeof(BYTE), fileLen, fp);
	fclose(fp);
	lpBuffer[fileLen] = 0;
	strToWCS((char*)lpBuffer, lpTBuffer);
	delete[] lpBuffer;
	this->initialize(lpTBuffer);
	delete[] lpTBuffer;

	return true;
}

bool shString::saveToFile( shString &fileName ) { 
	
	FILE *fp;

	_tfopen_s(&fp, fileName.getCString(), TEXT("wt"));
	if(fp == NULL) return false;

	BYTE *lpBuffer = new BYTE[this->dwLength + 1];
	wcsToStr(this->szString, (char*)lpBuffer);
	fwrite(lpBuffer, sizeof(BYTE), this->dwLength, fp);
	fclose(fp);
	delete[] lpBuffer;

	return true;
}

shString& shString::removeSymbol( TCHAR symb )
{
	DWORD		index, index2;
	
	if( this->dwLength == 0 )
		return *this;
	
	TCHAR	*mybuf;

	// временный буффер
	mybuf = new TCHAR[8192];
	
	// фильтрация
	for( index = 0, index2 = 0; index < this->dwLength; index ++ )
	{
		if( this->szString[index] != symb )
		{
			mybuf[ index2 ] = this->szString[ index ];
			index2 ++;
		}
	}
	
	mybuf[ index2 ] = 0;

	this->initialize( mybuf );
	delete[] mybuf;
	
	return *this;
}

shString& shString::appendPath( shString &path ) { 
	
	// если уже с путем то ничего не делать
	if( this->szString[1] == ':' ) return *this;
	shString str = path;
	
	// если между именем и путем нет слеша то добавить его
	if( str.szString[str.dwLength - 1] != '\\' && this->szString[ this->dwLength - 1] != '\\') str.concat( TEXT( "\\" ) );
	str.concat( *this );
	
	*this = str;
	return *this;
}

bool shString::hasSubstr( shString &str, int beginPos ) { 

	int strLen = str.getLength();
	int myLen = this->getLength();
	
	if( beginPos + strLen > myLen ) return false;
	for( int i = beginPos; i < beginPos + strLen && i < myLen; i++ ) { 
		if( str.szString[i - beginPos] != this->szString[i] ) return false;
	}
	
	return true;
}

void shString::removePath( shString &fileName, shString &dataDir, shString &shortDataDir ) { 
	int subStrPos = fileName.hasSubstr( shortDataDir );
	if( subStrPos != -1 ) { 
		fileName.deleteLeft( subStrPos + shortDataDir.getLength() + 1 );
	}
}

void shString::replacePath( shString &fileName, shString &dataDir, shString &shortDataDir ) { 

	int subStrPos = fileName.hasSubstr( shortDataDir );
	if( subStrPos != -1 ) { 
		fileName.deleteLeft( subStrPos + shortDataDir.getLength() + 1 );
		fileName.appendPath( dataDir );
	}
	
	return ;
}

int shString::hasSubstr( shString &str ) { 

	int strLen = str.getLength();
	int myLen = this->getLength();

	for( int i = 0; i < myLen - strLen; i++ ) { 
		if( this->hasSubstr( str, i ) ) return i;
	} 
	
	return -1;
}

bool shString::endsWith( shString &str ) { 
	
	int strLen = str.getLength();
	int myLen = this->getLength();

	for( int i = 0; i < strLen; i ++ ) { 
		if( this->szString[ myLen - 1 - i ] != str.szString[ strLen - 1 - i ] ) return false;
	}

	return true;
}

bool shString::readString( TCHAR *text, int stringNumber ) { 

	int len = (int)_tcslen( text );
	TCHAR *begin;
	int endQuoteIndex, quoteCount = 0;
	
	int i;
	for( i = 0; i < len; i++ ) { 
		if( text[i] == '"' ) { 
			quoteCount++;
			if( quoteCount == stringNumber * 2 + 1 )
				begin = &text[i + 1];
			else if( quoteCount == stringNumber * 2 + 2 ) {
				endQuoteIndex = i;
				break;
			}
		}
	} 
	
	if( i == len ) 
		return false;

	text[endQuoteIndex] = 0;
	this->initialize( begin );
	text[endQuoteIndex] = '"';

	return true;
}

void shString::copyToChar( char *buffer ) { 
	
	wcsToStr( this->szString, buffer );
	return;
}

void shString::copyFromChar( char *buffer ) {
	
	if( sizeof( TCHAR ) == 1 ) { 
		this->initialize( (TCHAR*) buffer );
		return ;
	}

	TCHAR *tbuf = new TCHAR[ strlen( buffer) + 1 ];
	strToWCS( buffer, tbuf );
	this->initialize( tbuf );
	delete[] tbuf;

	return;
}

int shString::calcStringSize( bool asChar ) { 

	if( this->szString == NULL ) return sizeof( unsigned long ); 
	return sizeof( unsigned long ) + ( this->dwLength + 1 ) * ( asChar? sizeof(char) : sizeof( TCHAR ) );
} 

bool shString::saveToMemoryCore( a3dMemoryFile &file ) { 
	
	if( !file.writeString( *this, true ) ) return false;
	return true;
}

bool shString::loadFromMemoryCore( a3dMemoryFile &file ) {
	
	if( !file.readString( *this, true ) ) return false;
	return true;
}

size_t strToWCS( char *str, TCHAR *wcs )
{

	size_t len,i;
	
	if( str == NULL ) { 
		wcs[0] = 0;
		return 0;
	}
	len = strlen( str );
	
	for( i=0; i<=len; i++ ){
		wcs[i] = (TCHAR)str[ i ];
	}

	return len;
}

size_t wcsToStr( TCHAR *wcs, char *str )
{

	size_t len,i;
	
	if( wcs == NULL ) { 
		str[0] = 0;
		return 0;
	}
	len = _tcslen( wcs );
	
	for( i=0; i<=len; i++ ){
		str[ i ] = (char)wcs[ i ];
	}

	return len;
}

