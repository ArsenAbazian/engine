/*********************************************************************
*	
*		class RCManager	- simple resourse manager 
*		created by Abazyan Arsen
*		
*		Tula 12.03.2004		22:52
*		e-mail: ars_s@uic.tula.ru
*		
*		information:
*			- support only files with 50 000 identifiers, parsers, etc.
*			- file must be in ASCII win-1251 codetable
**********************************************************************/

#ifndef A_RCMANAGER_INC
#define A_RCMANAGER_INC

#include "hstring.h"
#include <list>

#define RULE_MAX_LEN	10


// return code for RCManager::Load()
typedef enum {
	RC_ERROROPENFILE				= 0,
	RC_OK							= 1,
	RC_ERRORALLOCATEMEM				= 2,
	RC_ERROROPENLOGFILE				= 3,
	RC_ERRORSCANFILE				= 4,
	RC_INVALIDCFGFILE				= 5,
	RC_ERRORPARSEFILE				= 6
} RC_RETURN_CODE;

typedef enum{
	RC_IDENTIFIER					= 0,
	RC_FLOAT						= 1,
	RC_DWORD						= 2,
	RC_STRING						= 3,
	RC_PARSER						= 4, 
	RC_INT							= 5
} RC_ITEM_TYPE;



// token struct
typedef struct _rc_token{	
	DWORD			type;		// token type
	DWORD			row_num;
	union{
		shString	*id;		// identifier
		shString	*strval;	// string value
		float		fval;		// float value
		DWORD		dval;		// dword, int value
		BYTE		parser;		// parser
	};
}rc_token;


typedef enum {	
	CLASS_BAD_SYMBOL	= 0, 
	CLASS_DIGIT			= 1, 
	CLASS_LETTER		= 2, 
	CLASS_PARSER		= 3
}RC_SCAN_CLASS;

// this is ASCII character table
// supports win 1251 codetable
// 0 - bad symbol
// 1 - digit
// 2 - letter
// 3 - parser
static char ascii_win1251_code_table[256]={
//  0 1 2 3 4 5 6 7 8 9 a b c d e f 
	0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0, //0
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //1
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, //2
	1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3, //3
	3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //4
	2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,2, //5
	3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //6
	2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,0, //7
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //8 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //9
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //a
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //b
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //c
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //d
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //e
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  //f
};

// return symbol class
#define symb_to_class(x) *(ascii_win1251_code_table+x)

typedef enum {	
	S0				= 0,	// default begin value
	S_ID_RWORD		= 1,	// id or reserved word value
	S_NUMVALUE		= 2,	// float or dword value
	S_STRING		= 3,	// string state
	S_COMMENT_LINE	= 4,	// line comment state
	S_COMMENT_BLOCK	= 5,	// block comment 
	S_ERROR			= 6		// error state
}RC_SCAN_STATE;

typedef struct{
	DWORD	Val;
	BYTE	Type;
}DwordValue;

typedef struct{
	
	std::list<shString*>	HStrings;
	std::list<shString>		HStringNames;

	std::list<DwordValue>	DwordValues;
	std::list<shString>		DwordValueNames;

	std::list<float>		FloatValues;
	std::list<shString>		FloatValueNames;
	
}CCategoryValues;

// this is RCManager identifier
static	DWORD RCIdCount = 0;

class	RCManager{
	
	HWND								hWnd;		
	bool								bRCDebug;			// if true - create debug info
	DWORD								dwRCId;				// RC manager identifier
	shString							FileName;			//
	DWORD								dwScanLogCount;		// log file count
	DWORD								dwParseLogCount;	// log file count
		
	std::list< shString>				Categories;
	std::list<CCategoryValues*>			CValues;			

	CCategoryValues						*LastAccessedValues;
	shString							*LastAccessedCategory;
	shString							*LastAccessedName;
protected:

	RC_RETURN_CODE						ScanFile(TCHAR *filename, rc_token **table, DWORD *tb_size);	// scan file	
	RC_RETURN_CODE						ParseTable(rc_token **table, DWORD tb_size);				// parse table
	void*								GetCategory(shString &CategoryName, bool enable_msg = false);
	void*								GetCategory(TCHAR* CategoryName, bool enable_msg = false);
public:
	RCManager();						
	RCManager(HWND hwnd, bool bdbg);	
	~RCManager(){ Clear(); };						
	
	void					Initialize(HWND hwnd, bool bdbg);
	
	RC_RETURN_CODE			Load(TCHAR *filename); // load resourse set from file	
	RC_RETURN_CODE			Save(TCHAR *filename); // save file
	
	void*					GetElementByName(shString &CategoryName, RC_ITEM_TYPE ItemType, shString &ItemName, bool enable_msg = false);
	void*					GetElementByName(TCHAR *CategoryName, RC_ITEM_TYPE ItemType, TCHAR* ItemName,bool enable_msg = false);
	void*					GetElementByName(shString &CategoryName, RC_ITEM_TYPE ItemType, TCHAR* ItemName,bool enable_msg = false);

	void*					GetElementByNameEx(shString &CategoryName, RC_ITEM_TYPE ItemType, shString &ItemName, shString **pItemName,bool enable_msg = false);
	void*					GetElementByNameEx(TCHAR *CategoryName, RC_ITEM_TYPE ItemType, TCHAR* ItemName, shString **pItemName,bool enable_msg = false);

	void					AddCategory(TCHAR *CatName, bool enable_msg = false);
	void					AddCategory(shString &CatName, bool enable_msg = false);
	
	bool					SelectCategory(shString &CategoryName, bool enable_msg = false) {
								if( GetCategory(CategoryName, enable_msg) == NULL)return false;
								return true;
							}
	bool					SelectCategory(TCHAR* CategoryName, bool enable_msg = false){
								if( GetCategory(CategoryName, enable_msg) == NULL)return false;
								return true;
							}


	void					AddValue(shString &ValName, DWORD Val, BYTE Type, bool enable_msg = false);
	void					AddValue(shString &ValName, float Val, bool enable_msg = false);
	void					AddValue(shString &ValName, TCHAR *Str, bool enable_msg = false);
	void					AddValue(shString &ValName, shString &Str, bool enable_msg = false);
	
	void					AddValue(TCHAR *ValName, DWORD Val, BYTE Type, bool enable_msg = false);
	void					AddValue(TCHAR *ValName, float Val, bool enable_msg = false);
	void					AddValue(TCHAR *ValName, TCHAR *Str, bool enable_msg = false);
	void					AddValue(TCHAR *ValName, shString &Str, bool enable_msg = false);

	void					Clear();

};

#endif