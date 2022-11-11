#include <stdio.h>
#include "n3dsfile.h"

//#define ADEBUG

#ifdef ADEBUG
std::ofstream fout;
#endif


WORD	checkWord7;
#define CHECK_WORD7

BYTE	checkByte27;
BYTE	checkByte1_35;

WORD	checkWord7_1;
#define CHECK_WORD7_1

BYTE	checkByte7;
BYTE	checkByte34;

BYTE	checkByte1_14;

word GetChunkCount(std::ifstream &is,word ChunkId,dword Pos){
    ChunkHdr3DS block;
    long        pos = is.tellg();
    word        Count = 0;
    
    while((dword)is.tellg()<Pos){
        is>>block;
        if(block.Desc == ChunkId)Count++;
        is.seekg(block.Lenght-6,std::ios::cur);
    }
    
    is.seekg(pos,std::ios::beg);
    return Count;
}

std::ifstream& operator>>(std::ifstream &is,ChunkHdr3DS &ch){
    is.read((char*)&ch.Desc,sizeof(word));
    is.read((char*)&ch.Lenght,sizeof(dword));
    return is;
}

std::ifstream& operator>>(std::ifstream &is,Color3DS &col){
    
    ChunkHdr3DS hdr;
    float       fcol;
    
    is.seekg(-6,std::ios::cur);
    is>>hdr;
    
    if(hdr.Desc == COL_RGB || hdr.Desc == COL_RGBG){
        is.read((char*)&col.red,sizeof(byte));
        is.read((char*)&col.green,sizeof(byte));
        is.read((char*)&col.blue,sizeof(byte));
    }
    else if(hdr.Desc == COL_FRGB || hdr.Desc == COL_FRGBG){
        is.read((char*)&fcol,sizeof(float));
        col.red = (byte)(fcol*255.0f);
        is.read((char*)&fcol,sizeof(float));
        col.green = (byte)(fcol*255.0f);
        is.read((char*)&fcol,sizeof(float));
        col.blue = (byte)(fcol*255.0f);
        col.alpha = 0;
    }
        
    col.alpha = 0;
    
    return is;
}

std::ifstream& operator>>(std::ifstream &is,Vector3DS &vec){
    
    is.read((char*)&vec.x,sizeof(float));
    is.read((char*)&vec.y,sizeof(float));
    is.read((char*)&vec.z,sizeof(float));
    
    return is;
}

std::ifstream& operator>>(std::ifstream &is,TextCoor3DS &tcoor){

    is.read((char*)&tcoor.u,sizeof(float));
    is.read((char*)&tcoor.v,sizeof(float));
    
    return is;
}

std::ifstream& operator>>(std::ifstream &is,Face3DS &face){
    
    is.read((char*)&face.Va_indx,sizeof(word));
    is.read((char*)&face.Vb_indx,sizeof(word));
    is.read((char*)&face.Vc_indx,sizeof(word));

    return is;
}

std::ifstream& operator>>(std::ifstream &is,Basis3DS &bas){
    
    is>>bas.x_axis;
    is>>bas.y_axis;
    is>>bas.z_axis;
    is>>bas.point;
    
    return is;
}

std::ifstream& operator>>(std::ifstream &is,Light3DS &light){
    ChunkHdr3DS hdr;
    long pos,len,i;
    
    pos = is.tellg();
    is.seekg(-6,std::ios::cur);
    is>>hdr;
    len = hdr.Lenght;
    
    is>>light.Pos;
        
#ifdef ADEBUG
    fout<<"  position:       ";
    fout<<light.Pos<<std::endl;
#endif
    
    light.Type = DIRECTIONAL;
            
    for(i=0;i<(len-12);i++){
        is>>hdr;
        
        if(hdr.Desc == LIT_SPOT){
            light.Type = SPOTLIGHT;
            is>>light.Target;
            is.read((char*)&light.HotSpot,sizeof(float));
            is.read((char*)&light.FallOff,sizeof(float));
#ifdef ADEBUG
            fout<<"  target:         ";
            fout<<light.Target<<std::endl;
            fout<<"  hot spot:       ";
            fout<<light.HotSpot<<std::endl;
            fout<<"  fall off:       ";
            fout<<light.FallOff;
#endif
        }
        else if(hdr.Desc == COL_FRGB  || 
                hdr.Desc == COL_RGB   ||
                hdr.Desc == COL_FRGBG ||
                hdr.Desc == COL_RGBG){ 
            is>>light.Color;
#ifdef ADEBUG
            fout<<"  color:          ";
            fout<<light.Color<<std::endl;
#endif            
        }
        else {
            is.seekg(hdr.Lenght-6,std::ios::cur);
        }
    }
        
    is.seekg(pos,std::ios::beg);
    is.seekg(len,std::ios::cur);

#ifdef ADEBUG
    fout<<"  -------- end light --------"<<std::endl;
#endif   
    return is;
}

std::ifstream& operator>>(std::ifstream &is,FMatList3DS &fml){
    int		i;
    char	c;

    TCHAR	buf[512];
	
	for(i=0;;i++){
        is.read((char*)&c,1);
        buf[i] = (TCHAR)c;
		if(c == '\0')break;
    }
    
    if(i){
		fml.Name.initialize(buf);
    }
    
#ifdef ADEBUG
	fout<<"  name:        "<<fml.Name.getCString()<<std::endl;
#endif
    
    is.read((char*)&fml.FCount,2);
    if(fml.FCount)fml.F_index = new word[fml.FCount];

#ifdef ADEBUG
    fout<<"  face count:  "<<fml.FCount<<std::endl;
#endif    
    for(i=0;i<fml.FCount;i++){
        is.read((char*)&fml.F_index[i],2);
#ifdef ADEBUG
        fout<<"  "<<i<<" "<<fml.F_index[i]<<std::endl;
#endif        
    }
#ifdef ADEBUG    
    fout<<"  -------- end material ref--------"<<std::endl;
#endif            
    return is;
}

std::ifstream& operator>>(std::ifstream &is,Object3DS &obj){
    ChunkHdr3DS hdr;
    long pos,len,fpos,i,j,
          l_ind  = 0,
          ml_ind = 0;    
    
    word  flags;
    
    is.seekg(-6,std::ios::cur);
    pos = is.tellg();
    is>>hdr;
    len = hdr.Lenght;
	
	char	c;

	TCHAR	buf[512];
	
	for(i=0;i<len;i++){
        is.read((char*)&c,1);
		buf[i] = (TCHAR)c;
        if(buf[i] == '\0')break;
    }
    
    if(i){
        obj.Name.initialize(buf);
    }

#ifdef ADEBUG
    fout<<"  name:          "<<obj.Name.getCString()<<std::endl;
#endif    
    
    obj.LCount  = GetChunkCount(is,OBJ_LIGHT,pos+len);
    
#ifdef ADEBUG
    fout.flags(std::ios::dec);
    fout<<"  light count:   "<<obj.LCount<<std::endl;
#endif    
    
    if(obj.LCount)obj.Light = new Light3DS[obj.LCount];
        
    for(;is.tellg()<(pos+len);){
        is>>hdr;
    
        if(hdr.Desc == OBJ_LIGHT){        
#ifdef ADEBUG
            fout<<"  -------- light --------"<<std::endl;
#endif            
            is>>obj.Light[l_ind];
            l_ind++;
        }
        else if(hdr.Desc == TRI_MATLIST){
#ifdef ADEBUG
            fout<<"  -------- material ref--------"<<std::endl;
#endif            
            is>>obj.MList[ml_ind];
            ml_ind++;
        }
        else if(hdr.Desc == TRI_VERTEXL){
            is.read((char*)&obj.VCount,2);
            if(obj.VCount)obj.Vert = new Vertex3DS[obj.VCount];

#ifdef ADEBUG
            fout<<"  vert count:    "<<obj.VCount<<std::endl;
            fout<<"  space coors: ->"<<std::endl;
#endif            
            for(j=0;j<obj.VCount;j++){
                is>>obj.Vert[j].SCoor;
#ifdef ADEBUG            
                fout<<"  "<<j<<" ";
                fout<<obj.Vert[j].SCoor<<std::endl;
#endif                        
            }
        }
        else if(hdr.Desc == TRI_FACEL1){
            fpos = is.tellg();
            is.read((char*)&obj.FCount,2);
#ifdef ADEBUG
            fout<<"  face count:    "<<obj.FCount<<std::endl;
            fout<<"  vert ref:    ->"<<std::endl;
#endif            
            if(obj.FCount)obj.Face = new Face3DS[obj.FCount];
            for(j=0;j<obj.FCount;j++){
                is>>obj.Face[j];    
                is.read((char*)&flags,2);
#ifdef ADEBUG
                fout<<"  "<<j<<" ";
                fout<<obj.Face[j]<<std::endl;
#endif
            }
            obj.MLCount = GetChunkCount(is,TRI_MATLIST,fpos+hdr.Lenght-6);
            if(obj.MLCount)
                obj.MList = new FMatList3DS[obj.MLCount];
#ifdef ADEBUG
			fout<<"  material reference: "<<obj.MLCount<<std::endl;
#endif			
		}
        else if(hdr.Desc == TRI_MAPCOOR){
            is.read((char*)&flags,2);
#ifdef ADEBUG
            fout.flags(std::ios::dec);
            fout<<"  texture coor:  "<<flags<<std::endl;
#endif            
            for(j=0;j<flags;j++){
                is>>obj.Vert[j].TCoor;
#ifdef ADEBUG
                fout<<"  "<<j<<" ";
                fout<<obj.Vert[j].TCoor<<std::endl;
#endif                
            }
        }
        else if(hdr.Desc == TRI_LOCAL){
            is>>obj.Basis;
#ifdef ADEBUG
            fout.flags(std::ios::dec);
            fout<<"  basis:         "<<std::endl;
            fout<<"  x ";
            fout<<obj.Basis.x_axis<<std::endl;
            fout<<"  y ";
            fout<<obj.Basis.y_axis<<std::endl;
            fout<<"  z ";
            fout<<obj.Basis.z_axis<<std::endl;
            fout<<"  o ";
            fout<<obj.Basis.point<<std::endl;
#endif                            
        }
        else if(hdr.Desc == OBJ_TRIMESH)continue;
        else {
            is.seekg(hdr.Lenght-6,std::ios::cur);
        }
    }
    
    is.seekg(pos,std::ios::beg);
    is.seekg(len,std::ios::cur);

#ifdef ADEBUG
    fout<<"  -------- end object --------"<<std::endl;
#endif

    return is;
}

std::ifstream& operator>>(std::ifstream &is,Material3DS &mat){
    ChunkHdr3DS hdr;

    long i,pos,len;
    short sperc;
    
    is.seekg(-6,std::ios::cur);
    pos = is.tellg();
    is>>hdr;
    len = hdr.Lenght;

    is>>hdr;
   
	char	c;

	TCHAR buf[512];
	
	for(i=0;i<len;i++){
        is.read(&c,1);
		buf[i] = (TCHAR)c;
        if(buf[i] == '\0')break;
    }
    
    if(i){
		mat.Name.initialize(buf);
    }
#ifdef ADEBUG
    fout<<"  name:          "<<mat.Name.getCString()<<std::endl;
#endif    

    for(;is.tellg()<(pos+len);){
        is>>hdr;
        
        if(hdr.Desc == MAT_AMBCOL){
            is>>hdr;
            is>>mat.Ambient;
#ifdef ADEBUG
            fout<<"  ambient:       ";
            fout<<mat.Ambient<<std::endl;
#endif            
        }    
        else if(hdr.Desc == MAT_DIFCOL){
            is>>hdr;
            is>>mat.Diffuse;
#ifdef ADEBUG
            fout<<"  diffuse:       ";
            fout<<mat.Diffuse<<std::endl;
#endif                    
        }
        else if(hdr.Desc == MAT_SPCCOL){
            is>>hdr;
            is>>mat.Specular;
#ifdef ADEBUG
            fout<<"  specular:      ";
            fout<<mat.Specular<<std::endl;
#endif                    
        }
        else if(hdr.Desc == MAT_SHININESS){
            is>>hdr;
            if(hdr.Desc == FLOAT_PERCENT){
                is.read((char*)&mat.Shininess,4);
            }
            else {
                is.read((char*)&sperc,2);
                mat.Shininess = sperc/100.0f;
            }
#ifdef ADEBUG
            fout.flags(std::ios::dec);
            fout<<"  Shininess:     "<<mat.Shininess<<std::endl;
#endif            
        }
        else if(hdr.Desc == MAT_SHIN2PCN){
            is>>hdr;
            if(hdr.Desc == FLOAT_PERCENT){
                is.read((char*)&mat.ShinStrenght,4);
            }
            else {
                is.read((char*)&sperc,2);
                mat.ShinStrenght = sperc/100.0f;
            }
#ifdef ADEBUG
            fout.flags(std::ios::dec);
            fout<<"  ShinStrenght:  "<<mat.ShinStrenght<<std::endl;
#endif      
        }
        else if(hdr.Desc == MAT_TRANSPARENCY){
            is>>hdr;
            if(hdr.Desc == FLOAT_PERCENT){
                is.read((char*)&mat.Transparency,4);
            }
            else {
                is.read((char*)&sperc,2);
                mat.Transparency = sperc/100.0f;
            }
#ifdef ADEBUG
            fout.flags(std::ios::dec);
            fout<<"  Transparency:  "<<mat.Transparency<<std::endl;
#endif      
        }
        else if(hdr.Desc == MAT_TEX1){
#ifdef	ADEBUG
			fout<<"      -------------- found texture1 chunk ----------------"<<std::endl;
#endif            
			is>>mat.Texture1;
#ifdef	ADEBUG
			fout<<"      -------------- end texture1 chunk ----------------"<<std::endl;
#endif            			
		}
		else if(hdr.Desc == MAT_BUMP){
#ifdef	ADEBUG
			fout<<"      -------------- found bump chunk ----------------"<<std::endl;
#endif            
			is>>mat.Bump;
#ifdef	ADEBUG
			fout<<"      -------------- end bump chunk ----------------"<<std::endl;
#endif            			
		}
		else if(hdr.Desc == MAT_OPACITY){
#ifdef	ADEBUG
			fout<<"      -------------- found opacity chunk ----------------"<<std::endl;
#endif            
			is>>mat.Opacity;
#ifdef	ADEBUG
			fout<<"      -------------- end opacity chunk ----------------"<<std::endl;
#endif            			
		}
		else if(hdr.Desc == MAT_REFLECTION){
#ifdef	ADEBUG
			fout<<"      -------------- found reflection chunk ----------------"<<std::endl;
#endif            
			is>>mat.Reflection;
#ifdef	ADEBUG
			fout<<"      -------------- end reflection chunk ----------------"<<std::endl;
#endif            			
		}
        else {
            is.seekg(hdr.Lenght-6,std::ios::cur);
        }
    }
    
    is.seekg(pos,std::ios::beg);
    is.seekg(len,std::ios::cur);

#ifdef ADEBUG
    fout<<"  -------- end material --------"<<std::endl;
#endif        

    return is;
}

std::ifstream& operator>>(std::ifstream &is,Map3DS &mp){
	ChunkHdr3DS hdr;

    long pos,len;
	DWORD i;
    
    is.seekg(-6,std::ios::cur);
    pos = is.tellg();
    is>>hdr;
    len = hdr.Lenght;

    char	c;
	TCHAR	*mpName;

	for(;is.tellg()<(pos+len);){
		is>>hdr;	
	
		if(hdr.Desc == MAT_MAPFILE){
			mpName = new TCHAR[hdr.Lenght-6];
			for(i=0;i<hdr.Lenght-6;i++){
				is.read((char*)&c,1);
				mpName[i] = (TCHAR)c;
			}
			mp.Name.initialize(mpName);
			delete[] mpName;
#ifdef ADEBUG			
			fout<<"        name:     "<<mp.Name.getCString()<<std::endl;
#endif		
		}
		else if(hdr.Desc == MAT_MAP_USCALE){
			is.read((char*)&mp.uscale,4);			
#ifdef ADEBUG
			fout<<"        uscale:   "<<mp.uscale<<std::endl;
#endif			
		}
		
		else if(hdr.Desc == MAT_MAP_VSCALE){
			is.read((char*)&mp.vscale,4);			
#ifdef ADEBUG
			fout<<"        vscale:   "<<mp.vscale<<std::endl;
#endif			
		}
		else if(hdr.Desc == MAT_MAP_UOFFSET){
			is.read((char*)&mp.uoffset,4);			
#ifdef ADEBUG
			fout<<"        uoffset:  "<<mp.uoffset<<std::endl;
#endif			
		}
		else if(hdr.Desc == MAT_MAP_VOFFSET){
			is.read((char*)&mp.voffset,4);			
#ifdef ADEBUG
			fout<<"        voffset:  "<<mp.voffset<<std::endl;
#endif			
		}
		else
		{
			is.seekg(hdr.Lenght-6,std::ios::cur);		
		}
	}
	
	is.seekg(pos,std::ios::beg);
    is.seekg(len,std::ios::cur);
	return is;
}

std::ifstream& operator>>(std::ifstream &is, KeyFrameObjectInfo3DS &oi3ds)
{
	ChunkHdr3DS hdr;

    long pos,len, pos2;
    
    TCHAR	buf[512];
	char	cbuf[512];
	
	is.seekg(-6,std::ios::cur);
    pos = is.tellg();
    is>>hdr;
    len = hdr.Lenght;

    for(;is.tellg()<(pos+len);){
		is>>hdr;
		if( hdr.Desc == 0xb010 )
		{
			pos2 = is.tellg();
			is.read( cbuf, hdr.Lenght - 6 );
			strToWCS(cbuf, buf);
			is.seekg( pos2, std::ios::beg );
			is.seekg( hdr.Lenght - 6, std::ios::cur );
			oi3ds.name.initialize( buf );
#ifdef ADEBUG
			fout<<"object name = '"<<oi3ds.name.getCString()<<"'"<<std::endl;
#endif
		}
		else if( hdr.Desc == 0xb013 )
		{
			is.read( (char*)&oi3ds.pivot.x, 4 );
			is.read( (char*)&oi3ds.pivot.y, 4 );
			is.read( (char*)&oi3ds.pivot.z, 4 );
#ifdef ADEBUG
			fout<<"object pivot = { "<<oi3ds.pivot.x<<", "<<oi3ds.pivot.y<<", "<<oi3ds.pivot.z<<" }"<<std::endl;
#endif		
		}
		else if( hdr.Desc == 0xb021 )
		{
			is.read( (char*)&oi3ds.rotTag.flags, 2 );
			is.read( (char*)oi3ds.rotTag.unknown, 2 * 4 );
			is.read( (char*)&oi3ds.rotTag.keys, 2 );
			is.read( (char*)&oi3ds.rotTag.unknown2, 2 );
			oi3ds.rotTag.rotation = NULL;
			if(oi3ds.rotTag.keys) { 
				oi3ds.rotTag.rotation = new Rotation3DS[oi3ds.rotTag.keys];
				is.read( (char*)oi3ds.rotTag.rotation, sizeof(Rotation3DS) * oi3ds.rotTag.keys);
			}
#ifdef ADEBUG
			fout<<"object rotation tag:"<<std::endl;
			fout<<"    flags -> "<<oi3ds.rotTag.flags<<std::endl;
			fout<<"    count -> "<<oi3ds.rotTag.keys<<std::endl;
			for(int i = 0; i < oi3ds.rotTag.keys; i ++) { 
				fout<<"    "<<i<<"  frame = "<<oi3ds.rotTag.rotation[i].frame_number<<"  rot axis { "<<oi3ds.rotTag.rotation[i].axis_x<<", "<<oi3ds.rotTag.rotation[i].axis_y<<", "<<oi3ds.rotTag.rotation[i].axis_z<<"  "<<oi3ds.rotTag.rotation[i].rotation<<std::endl;
			} 
#endif		
		}
		else 
		{
#ifdef ADEBUG
			fout.flags(std::ios::hex);
			fout<<"found chunk "<<hdr.Desc;
			fout<<" at pos "<<is.tellg()<<" ";
			fout.flags(std::ios::dec);
			fout<<" lenght = "<<hdr.Lenght<<std::endl;
#endif
			is.seekg(hdr.Lenght-6,std::ios::cur);						
		}
	}
	
	is.seekg(pos,std::ios::beg);
	is.seekg(len,std::ios::cur);
	return is;
}

std::ifstream& operator>>(std::ifstream &is, KeyFrame3DS &k3ds )
{
	ChunkHdr3DS hdr;

    long pos,len, frmIndex = 0;
    
	is.seekg(-6,std::ios::cur);
    pos = is.tellg();
    is>>hdr;
    len = hdr.Lenght;
    	
	for(;is.tellg()<(pos+len);){
		is>>hdr;	
		
		if( hdr.Desc == 0xb002 )
		{
			is>>k3ds.info[frmIndex];
			frmIndex ++;
		}
		else 
		{
#ifdef ADEBUG
			fout.flags(std::ios::hex);
			fout<<"found chunk "<<hdr.Desc;
			fout<<" at pos "<<is.tellg()<<" ";
			fout.flags(std::ios::dec);
			fout<<" lenght = "<<hdr.Lenght<<std::endl;
#endif
			is.seekg(hdr.Lenght-6,std::ios::cur);						
		}
	}
	
	is.seekg(pos,std::ios::beg);
    is.seekg(len,std::ios::cur);
	return is;
}



bool File3DS::Load(TCHAR *FileName){
    std::ifstream    fin;
    ChunkHdr3DS hdr;
    dword       ver,
                fpos,
                obj_indx = 0,
                mat_indx = 0,
				frm_index = 0;
    
	char  cbuf[512];
	
#ifdef	ADEBUG	
	TCHAR buf[512];
	int	  i;
	
	_stprintf(buf,TEXT("%s"),FileName);
    for(i =0;;i++){
        if(buf[i] == '.')break;
    }
    _stprintf(&buf[i],TEXT(".log"));
    
	wcsToStr(buf, cbuf);
	
	fout.open(cbuf);
#endif

    wcsToStr(FileName, cbuf);
    fin.open(cbuf,std::ios::binary);
#ifdef ADEBUG
        fout<<"parsing file "<<FileName<<std::endl;
        fout<<"---------------------------"<<std::endl;
#endif           
    
    if(!fin.is_open()){
#ifdef ADEBUG
        fout<<"error: cannot open "<<FileName<<std::endl;
#endif
        return false;
    }
    
	fin.seekg( 0, std::ios::end );
	int length = fin.tellg();
	fin.seekg( 0, std::ios::beg );

    fin>>hdr;
    if(hdr.Desc!=MAIN3DS){
#ifdef ADEBUG
        fout.flags(std::ios::hex);
        fout<<"error: this is not 3ds file -> "<<hdr.Desc<<std::endl;
#endif        
        return false;
    }
    
    while(fin.tellg() < length - 1){
        fin>>hdr;
        switch(hdr.Desc){
            
            case VERSION:
                fin.read((char*)&ver,4);
                if(ver<3)return false;
                
#ifdef ADEBUG
                fout.flags(std::ios::dec);
                fout<<"file version:      "<<ver<<std::endl;
#endif
            break;
            
            case EDIT3DS:
                fpos = fin.tellg();
                ObjCount = GetChunkCount(fin,EDIT_OBJECT,fpos+hdr.Lenght-6);
                if(ObjCount)Object   = new Object3DS[ObjCount];
				//if(ObjCount)this->keyFrame.info = new KeyFrameObjectInfo3DS[ObjCount];
				                
                MatCount = GetChunkCount(fin,EDIT_MATERIAL,fpos+hdr.Lenght-6);
                if(MatCount)Material = new Material3DS[MatCount];
#ifdef ADEBUG
                fout.flags(std::ios::dec);
                fout<<"object count:      "<<ObjCount<<std::endl;
                fout<<"material count:    "<<MatCount<<std::endl;
#endif
            break;
        
            case EDIT_OBJECT:
#ifdef ADEBUG
                fout<<"  -------- object "<<obj_indx<<" --------"<<std::endl;
                
#endif                
                fin>>Object[obj_indx];
                obj_indx++;
            break;

            case EDIT_MATERIAL:
#ifdef ADEBUG
                fout<<"  -------- material --------"<<std::endl;
#endif                                
                fin>>Material[mat_indx];
                mat_indx++;
            break;
			case 0xb000:
#ifdef ADEBUG
				fout.flags(std::ios::hex);
				fout<<"found keyframe "<<hdr.Desc;
				fout<<" at pos "<<fin.tellg()<<" ";
				fout.flags(std::ios::dec);
				fout<<" lenght = "<<hdr.Lenght<<std::endl;
#endif
				fpos = fin.tellg();
				keyFrameCount = GetChunkCount( fin, 0xb002,fpos+hdr.Lenght-6 );
				this->keyFrame.info = new KeyFrameObjectInfo3DS[ keyFrameCount ];
				fin>>this->keyFrame;
			break;
            default:
            	fin.seekg(hdr.Lenght-6,std::ios::cur);
			break;
		}
    }
    
#ifdef	ADEBUG
	fout.close();
#endif	
	
	return true;
}

void File3DS::Close(){
    if(Object)delete[] Object;
    if(Material)delete[] Material;
	if(keyFrame.info)
		delete[] keyFrame.info;
	Object   = NULL;
	Material = NULL;
	real_obj_count = -1;
}

std::ofstream& operator<<(std::ofstream &os,ChunkHdr3DS &hdr){
    os<<"desc: "<<hdr.Desc;
    os<<", size: "<<hdr.Lenght;
    
    return os;
}

std::ofstream& operator<<(std::ofstream &os,Color3DS &col){
    os<<"( "<<((int)col.red)<<", ";
    os<<((int)col.green)<<", ";
    os<<((int)col.blue)<<", ";
	os<<((int)col.alpha)<<" )";
    
    return os;
}

std::ofstream& operator<<(std::ofstream &os,Vector3DS &vec){
    os<<"{ "<<vec.x<<", ";
    os<<vec.y<<", ";
    os<<vec.z<<" }";

    return os;
}

std::ofstream& operator<<(std::ofstream &os,TextCoor3DS &tcoor){
    os<<"{ "<<tcoor.u<<", ";
    os<<tcoor.v<<" }";

    return os;
}

std::ofstream& operator<<(std::ofstream &os,Face3DS &face){
    os<<"{ "<<face.Va_indx<<", ";
    os<<face.Vb_indx<<", ";
    os<<face.Vc_indx<<" }";

    return os;
}

std::ofstream& operator<<(std::ofstream &os,Basis3DS &basis){
    os<<"x ";
    os<<basis.x_axis<<std::endl;
    
    os<<"y ";
    os<<basis.y_axis<<std::endl;
    os<<"z ";
    os<<basis.z_axis<<std::endl;
    os<<"o ";
    os<<basis.point<<std::endl;
    
    return os;
}

void File3DS::mirrorVector(Vector3DS &v) { 
	v.x *= -1.0f;
	v.y *= -1.0f;
	v.z *= -1.0f;
}

void File3DS::basisToVectors(Basis3DS &b, Vector3 x, Vector3 y, Vector3 z) {
	
	x[0] = b.x_axis.x;
	x[1] = b.x_axis.y;
	x[2] = b.x_axis.z;

	y[0] = b.y_axis.x;
	y[1] = b.y_axis.y;
	y[2] = b.y_axis.z;

	z[0] = b.z_axis.x;
	z[1] = b.z_axis.y;
	z[2] = b.z_axis.z;
}

void File3DS::correct_for_dx(){

	DWORD i,j, frm;
	Matrix3	G,Gmin1;
	Vector3	tmp;
	float	fScaleX, fScaleY, fScaleZ;
	
	// переводим вершины из глобального базиса в локальный базис
	for( i = 0; i < this->ObjCount; i ++ )
	{
		fScaleX = normalizeVector( (float*)&Object[i].Basis.x_axis );

		G[0][0] = Object[i].Basis.x_axis.x;
		G[1][0] = Object[i].Basis.x_axis.y;
		G[2][0] = Object[i].Basis.x_axis.z;
				
		fScaleY = normalizeVector( (float*)&Object[i].Basis.y_axis );

		G[0][1] = Object[i].Basis.y_axis.x;
		G[1][1] = Object[i].Basis.y_axis.y;
		G[2][1] = Object[i].Basis.y_axis.z;
		
		fScaleZ = normalizeVector( (float*)&Object[i].Basis.z_axis );

		G[0][2] = Object[i].Basis.z_axis.x;
		G[1][2] = Object[i].Basis.z_axis.y;
		G[2][2] = Object[i].Basis.z_axis.z;
		
		invertMatrix3(G,Gmin1);

		// находим pivot и отражения
		for( j = 0; j < this->keyFrameCount; j ++ )
		{
			if( this->keyFrame.info[j].name == Object[i].Name )
			{
				frm = j;
				break;
			}
		}

		for( j = 0; j < Object[i].VCount; j++ )
		{
			Object[i].Vert[j].SCoor.x -= Object[i].Basis.point.x;
			Object[i].Vert[j].SCoor.y -= Object[i].Basis.point.y;
			Object[i].Vert[j].SCoor.z -= Object[i].Basis.point.z;
		
			mulMatrix3ToVector3( Gmin1, (float*)&( Object[i].Vert[j].SCoor ), tmp );
			copyVector3( tmp, (float*)&( Object[i].Vert[j].SCoor ) );
		
			Object[i].Vert[j].SCoor.x -= this->keyFrame.info[frm].pivot.x;
			Object[i].Vert[j].SCoor.y -= this->keyFrame.info[frm].pivot.y;
			Object[i].Vert[j].SCoor.z -= this->keyFrame.info[frm].pivot.z;
		}

		// теперь смотрим есть ли отражения 
		// по идее это в самом начале
		/*
		if(this->keyFrame.info[frm].rotTag.keys > 0) { 
			tmp[0] = this->keyFrame.info[frm].rotTag.rotation[0].axis_x;
			tmp[1] = this->keyFrame.info[frm].rotTag.rotation[0].axis_y;
			tmp[2] = this->keyFrame.info[frm].rotTag.rotation[0].axis_z;

			setVector3(x, 1.0f, 0.0f, 0.0f);
			setVector3(y, 0.0f, 1.0f, 0.0f);
			setVector3(z, 0.0f, 0.0f, 1.0f);
			
			this->basisToVectors(Object[i].Basis, x, y, z);
			RotateVectorAroundNormVector1(x, tmp, res, this->keyFrame.info[frm].rotTag.rotation[0].rotation);
			Object[i].Basis.x_axis.x = res[0];
			Object[i].Basis.x_axis.y = res[1];
			Object[i].Basis.x_axis.z = res[2];
			RotateVectorAroundNormVector1(y, tmp, res, this->keyFrame.info[frm].rotTag.rotation[0].rotation);
			Object[i].Basis.y_axis.x = res[0];
			Object[i].Basis.y_axis.y = res[1];
			Object[i].Basis.y_axis.z = res[2];
			RotateVectorAroundNormVector1(z, tmp, res, this->keyFrame.info[frm].rotTag.rotation[0].rotation);
			Object[i].Basis.z_axis.x = res[0];
			Object[i].Basis.z_axis.y = res[1];
			Object[i].Basis.z_axis.z = res[2];
			
			//if(this->keyFrame.info[frm].rotTag.rotation[0].axis_x < 0.0f) this->mirrorVector(Object[i].Basis.x_axis);
			//if(this->keyFrame.info[frm].rotTag.rotation[0].axis_y < 0.0f) this->mirrorVector(Object[i].Basis.y_axis);
			//if(this->keyFrame.info[frm].rotTag.rotation[0].axis_z < 0.0f) this->mirrorVector(Object[i].Basis.z_axis);
		}*/

	//}

	// эта часть кода меняет местами y и z
	//for( i = 0; i < this->ObjCount; i ++ )
	//{
		// переведем сначала вершины в базис
			
		// меняем местами y и z координаты базиса
		tmp[0] = Object[i].Basis.x_axis.y;
		Object[i].Basis.x_axis.y = Object[i].Basis.x_axis.z;
		Object[i].Basis.x_axis.z = tmp[0];

		tmp[1] = Object[i].Basis.y_axis.y;
		tmp[2] = Object[i].Basis.y_axis.z;
		tmp[0] = Object[i].Basis.y_axis.x;
		Object[i].Basis.y_axis.x = Object[i].Basis.z_axis.x;
		Object[i].Basis.y_axis.y = Object[i].Basis.z_axis.z;
		Object[i].Basis.y_axis.z = Object[i].Basis.z_axis.y;

		Object[i].Basis.z_axis.y = tmp[2];
		Object[i].Basis.z_axis.z = tmp[1];
		Object[i].Basis.z_axis.x = tmp[0];
		
		tmp[0] = Object[i].Basis.point.y;
		Object[i].Basis.point.y = Object[i].Basis.point.z;
		Object[i].Basis.point.z = tmp[0];

		// теперь разбираемся с вершинами
		for( j = 0; j < this->Object[i].VCount; j ++ )
		{
			//this->Object[i].Vert[j].SCoor.x *= fScaleX;
			//this->Object[i].Vert[j].SCoor.y *= fScaleY;
			//this->Object[i].Vert[j].SCoor.z *= fScaleZ;
			
			tmp[0] = this->Object[i].Vert[j].SCoor.y; 
			this->Object[i].Vert[j].SCoor.y = this->Object[i].Vert[j].SCoor.z;
			this->Object[i].Vert[j].SCoor.z = tmp[0];
		}
	}
}

void File3DS::offset( float x, float y, float z, DWORD obj_ind )
{

	int i;
	
	Vector3	offset;

	offset[0] = x;
	offset[1] = y;
	offset[2] = z;

	for(i=0;i<Object[obj_ind].VCount;i++)
		addVector3( (float*)&(Object[obj_ind].Vert[i].SCoor), offset, (float*)&(Object[obj_ind].Vert[i].SCoor) );	
	
	return ;

}

void File3DS::offset( float x, float y, float z )
{

	int i;

	for( i=0; i<ObjCount; i++ )
		offset( x, y, z, i );

	return ;

}

void File3DS::circle_deform_dx( float radius, int axis ) // 0 - x, 1 - y, 2 - z
{
	
	int i;

	for( i=0; i<ObjCount; i++ )
		circle_deform_dx( radius, axis, i );

	return ;
}

void File3DS::circle_deform_dx( float radius, int axis, DWORD obj_ind )
{

	int		vertexIndex;
	 
	float	alpha, delta, length;

	// пускаем цикл по всем вершинам
	for( vertexIndex=0; vertexIndex<Object[obj_ind].VCount; vertexIndex++ )
	{
	
		// проверяем по какой оси искажать
		switch( axis )
		{
		
			case 0:	// радиус по оси х
				// получаем расстояние от центра окружности, до x координаты точки
				delta = radius - Object[obj_ind].Vert[vertexIndex].SCoor.x;	
				// вычисляем угол альфа
				// угол равен длине дуги / радиус
				length = Object[obj_ind].Vert[vertexIndex].SCoor.z;

				// вычисляем угол
				alpha = length / delta;
				
				// новые координаты
				Object[obj_ind].Vert[vertexIndex].SCoor.z = delta * sin( alpha );
				Object[obj_ind].Vert[vertexIndex].SCoor.x = radius - delta * cos( alpha );
								
				break;
			
			case 1: // радиус по оси y
				
				// получаем расстояние от центра окружности, до x координаты точки
				delta = radius - Object[obj_ind].Vert[vertexIndex].SCoor.y;	
				// вычисляем угол альфа
				// угол равен длине дуги / радиус
				length = Object[obj_ind].Vert[vertexIndex].SCoor.z;
				
				// вычисляем угол
				alpha = length / delta;
				
				// новые координаты
				Object[obj_ind].Vert[vertexIndex].SCoor.z = delta * sin( alpha );
				Object[obj_ind].Vert[vertexIndex].SCoor.y = radius - delta * cos( alpha );

				break;

			case 2: // радиус по оси z

				// получаем расстояние от центра окружности, до x координаты точки
				delta = radius - Object[obj_ind].Vert[vertexIndex].SCoor.z;	
				// вычисляем угол альфа
				// угол равен длине дуги / радиус
				length = Object[obj_ind].Vert[vertexIndex].SCoor.y;

				// вычисляем угол
				alpha = length / delta;
				
				// новые координаты
				Object[obj_ind].Vert[vertexIndex].SCoor.y = delta * sin( alpha );
				Object[obj_ind].Vert[vertexIndex].SCoor.z = radius - delta * cos( alpha );
				
				break;
		
		}
	
	}

	return ;
}

void File3DS::transform(Matrix4 mat,DWORD obj_ind){

	Matrix3		linOp;
	Vector3		offset,tmp;

	offset[0] = mat[3][0];
	offset[1] = mat[3][1];
	offset[2] = mat[3][2];

	DWORD i,j;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			linOp[i][j] = mat[i][j];

	for(i=0;i<Object[obj_ind].VCount;i++){
		subVector3((float*)&(Object[obj_ind].Vert[i].SCoor),offset,tmp);	
		mulMatrix3ToVector3(linOp,tmp,(float*)&(Object[obj_ind].Vert[i].SCoor));
	}
}


void File3DS::transform(Matrix4 mat){
	for(DWORD i=0;i<ObjCount;i++)
		transform(mat,i);
}

void File3DS::scale(float x,float y,float z){
	
	DWORD i,j;
	
	for(i=0;i<ObjCount;i++){
		for(j=0;j<Object[i].VCount;j++){
			Object[i].Vert[j].SCoor.x *= x;
			Object[i].Vert[j].SCoor.y *= y;
			Object[i].Vert[j].SCoor.z *= z;
		}		
	}

	return ;
}

void File3DS::apply_texture_path(TCHAR *path){
	
	long mat;
	Material3DS	*pmat;
	
	shString	sh1, sh2, sh3;

	sh1.initialize(path);
	sh2.initialize(TEXT("\\"));
	sh1.concat(sh2);
		
	for(mat = 0;mat<MatCount;mat++){
		pmat = &Material[mat];

		if(pmat->Texture1.Name.getLength() != 0){
			
			sh3.copy(sh1);
			sh3.concat(pmat->Texture1.Name);
			
			pmat->Texture1.Name.copy(sh3);
		}
	}
}

int	File3DS::get_real_obj_count(){

	int i;
	
	if(real_obj_count == -1){
		for(i=0;i<ObjCount;i++){
			if(Object[i].VCount > 0)real_obj_count++;
		}
		real_obj_count++;
		return real_obj_count;
	}
	else {
		return real_obj_count;
	}
}

int  File3DS::getRealObjectByIndex( int index ) { 
	
	int i, realIndex = 0;

	if (real_obj_count == 0) return 0;
	for( i = 0; i < real_obj_count; i ++ ) { 
		if(Object[i].VCount > 0){ 
			if( realIndex == index ) return i;
			realIndex ++;
		}
	}
	
	return 0;
}

Material3DS* File3DS::getMaterialByName( shString &name ) { 
	
	int i;

	for( i = 0; i < this->MatCount; i ++ ) { 
		if( this->Material[ i ].Name == name ) return &this->Material[ i ];
	}

	return NULL;
}