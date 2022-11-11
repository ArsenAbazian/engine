 /***********************************************************
  *
  *   Program:    M_PNG
  *   Created:    10.09.2001 12:34
  *   Author:
  *   Requested:
  *   D-R&Build:  WATCOM v11.0 + VC6.0(SP4) Win32(W9x/ME/Win32s/NT/W2000/XP), WinAMP 2.76, ME 8.0c
  *   Comments:
  *   Copyrights: copyright (c) AI Corp. 2001. All rights reserved.
  *
  *   Build:      10.09.2001 12:34
  *
  ************************************************************
  *
  * History:
  *
  * $Log:$
  *
  ************************************************************/

#ifndef _H_M_PNG_INCLUDED_
#define _H_M_PNG_INCLUDED_

#include <stdio.h>

// PNGDATA::type
#define PNG_T_UNK           0  //
#define PNG_T_R8G8B8       10  //24   R/W
#define PNG_T_A8R8G8B8     11  //32   R/W
#define PNG_T_R16G16B16    12  //48   R/W
#define PNG_T_A16R16G16B16 13  //64   R/W
#define PNG_T_GRY8         20  //     R/W
#define PNG_T_GRY16        21  //     R/W
#define PNG_T_A8GRY8       22  //     R/W
#define PNG_T_A16GRY16     23  //     R/W
#define PNG_T_BW1          30  //B&W  R/W
#define PNG_T_BW8          31  //B&W   /
#define PNG_T_PALETTED     40  //      /


typedef struct{
 char b;
 char g;
 char r;
}sPalette;

typedef struct{
   void   *buf;
   int    width;
   int    height;
   int    bpp;
   int    type;
   int	  interlace;
   FILE  *output_file;
   FILE  *input_file;

   sPalette  *pal;
   int   cPal;

   float gamma;
   float aspect;

   int    status;
}PNGDATA;

class PNG{
	int		typeToInternalType( int type );	
	int		typeToInternalType( ) { return this->typeToInternalType( this->data->type ); }
	int		getComponentBpp() { return this->getComponentBpp( this->data->type ); }
	int		getComponentBpp(int type);
	public:
		PNGDATA *data;
        
		PNG();
        void openRFile(char *name);
        void closeRFile();
        void openWFile(char *name);
        void closeWFile();
        void allocateMem();
        void freeMem();
        void load();
        void getInfo();
		void getData();
		~PNG();

		bool saveToFile( char *name );
};


#endif

