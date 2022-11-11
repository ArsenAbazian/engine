#ifndef __SCRIPT_TYPE_DESC
#define __SCRIPT_TYPE_DESC

#include <windows.h>

// описание встроенных типов
#define ST_NONE			0
#define ST_VOID			1
#define ST_BOOL			2
#define ST_CHAR			3
#define ST_SHORT		4
#define ST_INT			5
#define ST_FLOAT		6
#define ST_STRING		7
#define ST_COM_OBJECT	8
#define ST_USERDEFINED	9

#define STF_NONE		0x00000000
#define STF_STATIC		0x00000001
#define STF_EXTERN		0x00000002
#define STF_CONST		0x00000004
#define STF_INTERNAL	0x00000008
#define STF_CONST2		0x00000010

#define STF_FUNCTION	0x00000020
#define STF_METHOD		0x00000040
#define STF_OPERATOR	0x00000080
#define STF_PROPERTY	0x00000100
#define STF_EVENT		0x00000200

class scriptTypeDesc
{
public:
	DWORD	flags;			// флаги типа 
	DWORD	objectType;		// тип объекта: булевый символьный или др
	DWORD	refCount;		// число звездочек перед объектом ( порядок указателя )

	scriptTypeDesc();		// конструктор по умолчанию
	~scriptTypeDesc();		// деструктор

	scriptTypeDesc& operator= ( scriptTypeDesc &dsk );

	void	clear();
};

#endif