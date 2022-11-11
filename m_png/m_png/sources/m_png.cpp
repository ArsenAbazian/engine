#include "libpng\png.h"

#include "m_png.h"

#include <stdlib.h>
#include <math.h>

static void error_func(png_structp png, png_const_charp data){  longjmp(png->jmpbuf, 1); }
static void warning_func(png_structp png, png_const_charp data){}


PNG::PNG()
{
     data              = new PNGDATA;
     data->output_file = 0;
     data->input_file  = 0;
     data->buf         = 0;
     data->cPal        = 0;
     data->pal         = 0;
	 data->interlace   = PNG_INTERLACE_NONE;
}


PNG::~PNG()
{
    closeRFile();
    closeWFile();
    freeMem();
    delete data;
}


void PNG::openRFile(char *name)
{
     data->status=0;
     data->input_file=fopen(name,"rb");
     if(!data->input_file) data->status=1;
}


void PNG::closeRFile()
{
     if(data->input_file) {fclose(data->input_file);data->input_file=0;}
}

bool PNG::saveToFile( char *name ) { 
	
	png_voidp user_error_ptr = NULL;
	png_structp png_ptr;
	png_infop info_ptr;
	int png_transforms = 0;

	this->openWFile( name );
	if( this->data->status != 0 )
		return false;

	png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,
										user_error_ptr, 
										error_func, 
										warning_func );

	if( png_ptr == NULL ) {
		this->closeRFile();
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		this->closeWFile();
		png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
		return false;
	}
	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		this->closeWFile();
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	png_init_io( png_ptr, this->data->output_file );

	png_set_IHDR( png_ptr, info_ptr, this->data->width, this->data->height, this->data->bpp * 8 / 3, this->typeToInternalType(),
		this->data->interlace, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );	

	png_color_8 sig_bit;
	
	sig_bit.gray = getComponentBpp();
	sig_bit.red = getComponentBpp();
	sig_bit.green = getComponentBpp();
	sig_bit.blue = getComponentBpp();
	sig_bit.alpha = getComponentBpp();
	
	png_set_sBIT( png_ptr, info_ptr, &sig_bit );
	
	png_set_gAMA( png_ptr, info_ptr, this->data->gamma );

	png_write_info(png_ptr, info_ptr);

	/* invert monochrome pixels */
	png_set_invert_mono(png_ptr);

	/* Shift the pixels up to a legal bit depth and fill in
	* as appropriate to correctly scale the image.
	*/
	png_set_shift(png_ptr, &sig_bit);

	/* pack pixels into bytes */
	png_set_packing(png_ptr);

	/* swap location of alpha bytes from ARGB to RGBA */
	png_set_swap_alpha(png_ptr);

	/* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
	* RGB (4 channels -> 3 channels). The second parameter is not used.
	*/
	//png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

	/* flip BGR pixels to RGB */
	png_set_bgr(png_ptr);

	/* swap bytes of 16-bit files to most significant byte first */
	//png_set_swap(png_ptr);

	/* swap bits of 1, 2, 4 bit packed pixel formats */
	//png_set_packswap(png_ptr);

	/* turn on interlace handling if you are not using png_write_image() */
	/*
	if (interlacing)
		number_passes = png_set_interlace_handling(png_ptr);
	else
		number_passes = 1;
	*/

	png_bytep *row_pointers = new png_bytep[ this->data->height ];
	for( int i = 0; i < this->data->height; i++ ) { 
		row_pointers[ i ] = ((unsigned char*)this->data->buf) + i * this->data->width * this->data->bpp;
	}   

	png_write_image( png_ptr, row_pointers );

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	this->closeWFile();

	delete[] row_pointers;

	return true;
}

int PNG::getComponentBpp( int type ) { 

	switch( type )
	{
	case PNG_T_UNK: return -1;
	case PNG_T_R8G8B8: return 8;
	case PNG_T_A8R8G8B8: return 8;
	case PNG_T_R16G16B16: return 16;
	case PNG_T_A16R16G16B16: return 16;
	case PNG_T_GRY8: return 8;  
	case PNG_T_GRY16: return 16;
	case PNG_T_A8GRY8: return 8;
	case PNG_T_A16GRY16: return 16;
	case PNG_T_BW1: return -1;
	case PNG_T_BW8: return -1;
	case PNG_T_PALETTED: return PNG_COLOR_MASK_PALETTE;
	}

	return 8;

}

int PNG::typeToInternalType( int type ) { 

	switch( type )
	{
	case PNG_T_UNK: return -1;
	case PNG_T_R8G8B8: return PNG_COLOR_TYPE_RGB;
	case PNG_T_A8R8G8B8: return PNG_COLOR_TYPE_RGB_ALPHA;
	case PNG_T_R16G16B16: return PNG_COLOR_TYPE_RGB;
	case PNG_T_A16R16G16B16: return PNG_COLOR_TYPE_RGB_ALPHA;
	case PNG_T_GRY8: return PNG_COLOR_TYPE_GRAY;  
	case PNG_T_GRY16: return PNG_COLOR_TYPE_GRAY;
	case PNG_T_A8GRY8: return PNG_COLOR_TYPE_GRAY_ALPHA;
	case PNG_T_A16GRY16: return PNG_COLOR_TYPE_GRAY_ALPHA;
	case PNG_T_BW1: return -1;
	case PNG_T_BW8: return -1;
	case PNG_T_PALETTED: return PNG_COLOR_MASK_PALETTE;
	}
	
	return -1;
}

void PNG::openWFile(char *name)
{
     data->status=0;
     data->output_file=fopen(name,"wb");
     if(!data->output_file) data->status=1;
}


void PNG::closeWFile()
{
     if(data->output_file) {fclose(data->output_file);data->output_file=0;}
}


void PNG::allocateMem()
{
     if(data->buf) freeMem();
     if(data->bpp)
     data->buf=(void*)new char[data->width*data->height*data->bpp];
     else
     data->buf=(void*)new char[((int)ceil(data->width/8.0f))*data->height];
}


void PNG::freeMem()
{
   if(data->pal)
   {
      delete[] data->pal;
      data->pal=0;
   }
   if(data->buf)
   {
      delete[]data->buf;
      data->buf=0;
   } //if
}

void PNG::load()
{
  long int p;
     p=ftell(data->input_file);
     getInfo();
     fseek(data->input_file,p,SEEK_SET);
     if(data->status==1) return;
     allocateMem();
     if(data->status==1) return;
     getData();
}

void PNG::getInfo()
{
   unsigned char magic_numbers[8];
   long int      p;
   int           bpp;
   png_struct    *png;
   png_info      *info;

    data->status = 0;

    p = ftell(data->input_file);

    // grab the first 8 bytes for testing
    if (fread(magic_numbers, 1, 8, data->input_file) != 8)
    {
        fseek(data->input_file, p, SEEK_SET);
        data->status = 1;
        return;
    } else fseek(data->input_file, p, SEEK_SET);

	// Make sure we're a png file
    if (!png_check_sig(magic_numbers, 8))
    {
        data->status = 1;
        return;
	}

    png = png_create_read_struct (PNG_LIBPNG_VER_STRING, (void *) this, error_func, warning_func);
    if(!png)
    {
        data->status = 1;
        return;
    }
    if (setjmp(png->jmpbuf))
    {
        png_destroy_read_struct (&png, &info, NULL);
        data->status = 1;
        return;
    }
    info = png_create_info_struct(png);

    png_init_io(png, data->input_file);
	png_read_info(png, info);

	// fill in the information.
    data->width  = (int)info->width;
    data->height = (int)info->height;
    if (info->valid & PNG_INFO_gAMA) data->gamma = info->gamma; else data->gamma = 1.0f;

    if (info->valid & PNG_INFO_pHYs) data->aspect = (float)info->x_pixels_per_unit / (float)info->y_pixels_per_unit; else data->aspect = 1.0f;

   int pngtype = PNG_T_UNK;

    if (info->bit_depth == 1)
    {
        pngtype = PNG_T_BW1;
        bpp     = 0;
    }
    else
    {
        switch(info->color_type)
        {
           case PNG_COLOR_TYPE_PALETTE: pngtype = PNG_T_PALETTED; bpp = 1; break;
           case PNG_COLOR_TYPE_RGB_ALPHA:
               switch(info->bit_depth)
               {
                  case 2:   // Not allowed
                  case 4:   break;
                  case 8:   pngtype = PNG_T_A8R8G8B8;     bpp = 4;       break;
                  case 16:  pngtype = PNG_T_A16R16G16B16; bpp = 8;       break;
               }
               break;

           case PNG_COLOR_TYPE_RGB:
               switch(info->bit_depth)
               {
                  case 2:   // Not allowed
                  case 4:   break;
                  case 8:   pngtype = PNG_T_R8G8B8;    bpp = 3;       break;
                  case 16:  pngtype = PNG_T_R16G16B16; bpp = 6;       break;
               }
               break;

           case PNG_COLOR_TYPE_GRAY_ALPHA:
               switch(info->bit_depth)
               {
                  case 2:   // call it 8 since there's no smaller
                  case 4:                                              break;
                  case 8:   pngtype = PNG_T_A8GRY8;   bpp = 1+1;       break;
                  case 16:  pngtype = PNG_T_A16GRY16; bpp = 2+2;       break;
               }
               break;

           case PNG_COLOR_TYPE_GRAY:
               switch(info->bit_depth)
               {
                  case 2:   // call it 8 since there's no smaller
                  case 4:                                         break;
                  case 8:   pngtype = PNG_T_GRY8;  bpp = 1;       break;
                  case 16:  pngtype = PNG_T_GRY16; bpp = 2;       break;
               }
               break;

        }  //switch
    }  //elseif


    if (pngtype == PNG_T_UNK) data->status = 1;

    data->type = pngtype;
    data->bpp  = bpp;

    png_destroy_read_struct (&png, &info, NULL);

    return;
}


void PNG::getData()
{
   unsigned char magic_numbers[8];
   png_struct    *png;
   png_info      *info;
   png_bytep     *row_pointers;
   long int      p;
   int           pngtype;
	int			i;
	png_uint_32			ix, iy;

    data->status = 0;

    p = ftell(data->input_file);

	// grab the first 8 bytes for testing
    if (fread(magic_numbers, 1, 8, data->input_file) != 8)
    {
        fseek(data->input_file, p, SEEK_SET);
        data->status = 1;
        return;
    } else fseek(data->input_file, p, SEEK_SET);

	// Make sure we're a png file
    if (!png_check_sig(magic_numbers, 8))
    {
        data->status = 1;
        return;
	}

    png = png_create_read_struct (PNG_LIBPNG_VER_STRING, (void *) this, error_func, warning_func);
    if(!png)
    {
        data->status = 1;
        return;
    }
    if (setjmp(png->jmpbuf))
    {
        png_destroy_read_struct (&png, &info, NULL);
        data->status = 1;
        return;
    }
    info = png_create_info_struct(png);

    png_init_io(png, data->input_file);
	png_read_info(png, info);

    /* expand grayscale images to the full 8 bits */
	/* expand images with transparency to full alpha channels */
	/* I'm going to ignore lineart and just expand it to 8 bits */
	if ((info->color_type == PNG_COLOR_TYPE_PALETTE && info->bit_depth < 8) ||
		(info->color_type == PNG_COLOR_TYPE_GRAY && info->bit_depth < 8) ||
		(info->valid & PNG_INFO_tRNS))
		png_set_expand(png);

	int number_passes = 1;

    if (info->interlace_type)   number_passes = png_set_interlace_handling(png);

    if (info->bit_depth == 16)  png_set_swap(png);

	png_read_update_info(png, info);

    pngtype = PNG_T_UNK;

    if (info->bit_depth == 1)
    {
        pngtype = PNG_T_BW1;
    }
    else
    {
        switch(info->color_type)
        {
           case PNG_COLOR_TYPE_PALETTE:   pngtype = PNG_T_PALETTED; break;
           case PNG_COLOR_TYPE_RGB_ALPHA:
               switch(info->bit_depth)
               {
                  case 2:   // Not allowed
                  case 4:   break;
                  case 8:   pngtype = PNG_T_A8R8G8B8;                break;
                  case 16:  pngtype = PNG_T_A16R16G16B16;            break;
               }
               break;

           case PNG_COLOR_TYPE_RGB:
               switch(info->bit_depth)
               {
                  case 2:   // Not allowed
                  case 4:   break;
                  case 8:   pngtype = PNG_T_R8G8B8;                break;
                  case 16:  pngtype = PNG_T_R16G16B16;             break;
               }
               break;

           case PNG_COLOR_TYPE_GRAY_ALPHA:
               switch(info->bit_depth)
               {
                  case 2:   // call it 8 since there's no smaller
                  case 4:                                         break;
                  case 8:   pngtype = PNG_T_A8GRY8;               break;
                  case 16:  pngtype = PNG_T_A16GRY16;             break;
               }
               break;

           case PNG_COLOR_TYPE_GRAY:
               switch(info->bit_depth)
               {
                  case 2:   // call it 8 since there's no smaller
                  case 4:                                         break;
                  case 8:   pngtype = PNG_T_GRY8;                 break;
                  case 16:  pngtype = PNG_T_GRY16;                break;
               }
               break;

        }  //switch
    }  //elseif

    if (pngtype == PNG_T_UNK)
    {
        png_destroy_read_struct (&png, &info, NULL);
        data->status = 1;
        return;
	}

//?    if (info->channels == 2 || info->channels == 4)  fbi->SetFlags(MAP_HAS_ALPHA);

    row_pointers = (png_bytep *)malloc(info->height * sizeof(png_bytep));

    for (png_uint_32 i = 0; i < info->height; i++) row_pointers[i] = (png_bytep)malloc(info->rowbytes);

	// now read the image
	png_read_image(png, row_pointers);

    if((pngtype==PNG_T_BW1)&&(data->type==PNG_T_BW8)) pngtype=PNG_T_BW8;

    switch(pngtype)
    {

       case PNG_T_BW1:
       {
          char *b1;
            b1 = (char*)data->buf;
            for ( iy = 0; iy < info->height; iy++)
            for ( ix = 0; ix < info->rowbytes; ix++)
              b1[iy*info->rowbytes+ix] = row_pointers[iy][ix];
            break;
       }


       case PNG_T_BW8:
       {
          char *b8;
            b8 = (char*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            for (png_uint_32 ix = 0; ix < info->width; ix++)
            {
                png_uint_32 abyte = ix / 8;
                png_uint_32 abit = ix % 8;
                unsigned char tbyte = row_pointers[iy][abyte];
                unsigned char c = tbyte & (0x80 >> abit);
                b8[iy*info->width+ix] = c ? 0xff : 0;
            }  //for
            break;
       }

       case PNG_T_PALETTED:
       {
          if (info->bit_depth == 8)
          {
             char *b8;
               b8 = (char*)data->buf;
               for (png_uint_32 iy = 0; iy < info->height; iy++)
               for (png_uint_32 ix = 0; ix < info->width;  ix++)
                b8[iy*info->width+ix] = row_pointers[iy][ix];
          }
          else
          {
             char *b8;
               b8 = (char*)data->buf;

               unsigned char *pixels = (unsigned char *)calloc(info->width, sizeof(unsigned char));
               for (png_uint_32 iy = 0; iy < info->height; iy++)
               {
                 // now fill a row of pixels
                  unsigned char *inbyte = row_pointers[iy];
                  for (ix = 0; ix < info->width; inbyte++)
                  {
                     switch(info->bit_depth)
                     {
                        case 2:
                          pixels[ix] = (*inbyte & 0xc0) >> 6;
                          ix++; if (ix >= info->width) break;
                          pixels[ix] = (*inbyte & 0x30) >> 4;
                          ix++; if (ix >= info->width) break;
                          pixels[ix] = (*inbyte & 0x0c) >> 2;
                          ix++; if (ix >= info->width) break;
                          pixels[ix] = *inbyte & 0x03;
                          ix++;
                          break;
                        case 4:
                          pixels[ix] = (*inbyte & 0xf0) >> 4;
                          ix++; if (ix >= info->width) break;
                          pixels[ix] = *inbyte & 0x0f;
                          ix++;
                          break;
                     }  //switch
                 }  //for
                 for(ix=0;ix<info->width;ix++)
                  b8[iy*info->width+ix] = pixels[ix];
               }  //for
               free(pixels);
          }  //if

          // Now the palette
          data->cPal = png->num_palette;
          data->pal  = new sPalette[data->cPal];
          for(int i = 0; i < png->num_palette; i++)
          {
             data->pal[i].r = (char)png->palette[i].red;
             data->pal[i].g = (char)png->palette[i].green;
             data->pal[i].b = (char)png->palette[i].blue;
          }  //for
       }
       break;

       case PNG_T_A8R8G8B8:
       {
          long *b32;
          long  r,g,b,a;

            b32 = (long*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            for (png_uint_32 ix = 0, x = 0; ix < info->rowbytes; x++)
            {
                r = (long)row_pointers[iy][ix++];
                g = (long)row_pointers[iy][ix++];
                b = (long)row_pointers[iy][ix++];
                if (info->channels == 4) a = (long)row_pointers[iy][ix++]; else a = 0;
                b32[iy*info->width+x]= ((a<<24)|(r<<16)|(g<<8)|b);
            }  //for
       }
       break;


       case PNG_T_R8G8B8:
       {
          char *b24;
          long  r,g,b;

            b24 = (char*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            for (png_uint_32 ix = 0, x = 0; ix < info->rowbytes; x++)
            {
                r = (long)row_pointers[iy][ix++];
                g = (long)row_pointers[iy][ix++];
                b = (long)row_pointers[iy][ix++];
                b24[(iy*info->width+x)*3+0]= (char)b;
                b24[(iy*info->width+x)*3+1]= (char)g;
                b24[(iy*info->width+x)*3+2]= (char)r;
            }  //for
       }
       break;


       case PNG_T_A16R16G16B16:
       {
          short *b64;
          short  r,g,b,a;
          unsigned short *row;
            b64 = (short*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            {
               row = (unsigned short *) row_pointers[iy];
               for (png_uint_32 ix = 0; ix < info->width; ix++)
               {
                  r = *row++;
                  g = *row++;
                  b = *row++;
                  if (info->channels == 4) a = *row++; else a = 0;
                  b64[(iy*info->width+ix)*4+0] = b;
                  b64[(iy*info->width+ix)*4+1] = g;
                  b64[(iy*info->width+ix)*4+2] = r;
                  b64[(iy*info->width+ix)*4+3] = a;
               }  //for
            }  //for
       }
       break;


       case PNG_T_R16G16B16:
       {
          short *b64;
          short  r,g,b;
          unsigned short *row;
            b64 = (short*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            {
               row = (unsigned short *) row_pointers[iy];
               for (png_uint_32 ix = 0; ix < info->width; ix++)
               {
                  r = *row++;
                  g = *row++;
                  b = *row++;
                  b64[(iy*info->width+ix)*3+0] = b;
                  b64[(iy*info->width+ix)*3+1] = g;
                  b64[(iy*info->width+ix)*3+2] = r;
               }  //for
            }  //for
       }
       break;


       case PNG_T_GRY8:
       {
          char *b8;
            b8 = (char*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            for (png_uint_32 ix = 0; ix < info->width;  ix++)
               b8[iy*info->width+ix] = (char)row_pointers[iy][ix];
		}
		break;


       case PNG_T_A8GRY8:
       {
          short *b88;
          short  a;
          char  *row;
            b88 = (short*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height;   iy++)
            {
               row = (char*)row_pointers[iy];
               for (png_uint_32 ix = 0; ix < info->width; ix++ )
               {
                   b88[iy*info->width+ix] = *row++;
                   if(info->channels == 2) a = *row++; else a = 0;
                   b88[iy*info->width+ix]|= (a<<8);
               }  //for
            }  //for
       }
       break;


       case PNG_T_GRY16:
       {
          short *b16;
          short *row;
            b16 = (short*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            {
               row = (short*)row_pointers[iy];
               for (png_uint_32 ix = 0; ix < info->width;  ix++)
                b16[iy*info->width+ix] = row[ix];
            }  //for
       }
       break;


       case PNG_T_A16GRY16:
       {
          long  *b1616;
          long  a;
          long  c;
          short *row;
            b1616 = (long*)data->buf;
            for (png_uint_32 iy = 0; iy < info->height; iy++)
            {
               row = (short*)row_pointers[iy];
               for (png_uint_32 ix = 0; ix < info->width; ix++)
               {
                  c = *row++;
                  if (info->channels == 2) a = *row++; else  a = 0;
                  b1616[iy*info->width+ix] = ((a<<16)|c);
               }  //for
            }  //for
       }
       break;


    }  //switch

	png_read_end(png, info);

    for (i = 0; i < info->height; i++) free(row_pointers[i]);

	free(row_pointers);

    png_destroy_read_struct (&png, &info, NULL);

    return;
}
