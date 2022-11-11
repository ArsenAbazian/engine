#include "stblock.h"
#include <stdio.h>

BYTE	checkByte16;
BYTE	checkByte1_26;

WORD	checkWord5;
#define CHECK_WORD5

WORD	checkWord5_1;
#define CHECK_WORD5_1
BYTE	checkByte1_37;

BYTE	checkByte1_22;

BYTE	checkByte5;
BYTE	checkByte36;
BYTE	checkByte1_28;
BYTE	checkByte25;

StateBlockInfo::StateBlockInfo(){
	ZeroMemory(rsenabled,sizeof(bool)*103);
	ZeroMemory(ssenabled,sizeof(bool)*13);
	ZeroMemory(tssenabled,sizeof(bool)*8*18);

	ZeroMemory(rstate_value,sizeof(StateValue)*103);
	ZeroMemory(sstate_value,sizeof(StateValue)*13);
	ZeroMemory(tsstate_value,sizeof(StateValue)*8*18);
	ZeroMemory(sel_tsstate,sizeof(DWORD)*8);
	ZeroMemory(tsenabled,sizeof(bool)*8);
	
	sel_rstate  = 0;
	sel_sstate  = 0;
	sel_stage	= 0;
	name		= NULL;
}

StateBlockInfo::~StateBlockInfo(){
	ZeroMemory(rsenabled,sizeof(bool)*103);
	ZeroMemory(ssenabled,sizeof(bool)*13);
	ZeroMemory(tssenabled,sizeof(bool)*8*18);

	ZeroMemory(rstate_value,sizeof(StateValue)*103);
	ZeroMemory(sel_tsstate,sizeof(DWORD)*8);
	ZeroMemory(sstate_value,sizeof(StateValue)*13);
	ZeroMemory(tsstate_value,sizeof(StateValue)*8*18);
	
	sel_rstate  = 0;
	sel_sstate  = 0;
	sel_stage	= 0;
	delete[] name;
	name		= NULL;
};

void StateBlockInfo::clear(){
	ZeroMemory(rsenabled,sizeof(bool)*103);
	ZeroMemory(ssenabled,sizeof(bool)*13);
	ZeroMemory(tssenabled,sizeof(bool)*8*18);

	ZeroMemory(rstate_value,sizeof(StateValue)*103);
	ZeroMemory(sel_tsstate,sizeof(DWORD)*8);
	ZeroMemory(sstate_value,sizeof(StateValue)*13);
	ZeroMemory(tsstate_value,sizeof(StateValue)*8*18);

	sel_rstate  = 0;
	sel_sstate  = 0;
	sel_stage	= 0;
	delete[] name;
	name		= NULL;
}

bool	StateBlockInfo::get_rs_values(DWORD	**values,char ***dbg_values,DWORD *count){

	switch(RenderStateFunc[sel_rstate]){
		case SB_DWORD: 
		case SB_FLOAT: 
		case SB_INT:
		case SB_COLOR:		
			values = NULL;
			dbg_values = NULL;
			count = 0;
		break;
		case SB_FLAGS: // here we mast analyse sel_rstate
		
		break;
		case SB_ZBUFFERTYPE:
			*values = (DWORD*)ZBufferType;
			*dbg_values = DBGZBEnable;
			*count = sizeof(ZBufferType)>>2;
		break;
		case SB_FILLMODE:
			*values = (DWORD*)FillMode;
			*dbg_values = DBGFMode;
			*count = sizeof(FillMode)>>2;
		break;
		case SB_SHADEMODE:
			*values = (DWORD*)ShadeMode;
			*dbg_values = DBGSMode;
			*count = sizeof(ShadeMode)>>2;
		break;
		case SB_BOOL:
			*values = (DWORD*)D3DBoolValue;
			*dbg_values = DBGTrueFalse;
			*count = sizeof(D3DBoolValue)>>2;
		break;
		case SB_BLEND:
			*values = (DWORD*)Blend;
			*dbg_values = DBGBlend;
			*count = sizeof(Blend)>>2;
		break;
		case SB_CULL:
			*values = (DWORD*)Cull;
			*dbg_values = DBGCull;
			*count = sizeof(Cull)>>2;
		break;
		case SB_CMPFUNC:
			*values = (DWORD*)CmpFunc;
			*dbg_values = DBGCmpFunc;
			*count = sizeof(CmpFunc)>>2;
		break;
		case SB_FOGMODE:
			*values = (DWORD*)FogMode;
			*dbg_values = DBGFogMode;
			*count = sizeof(FogMode)>>2;
		break;
		case SB_STENCILCAPS:
			*values = (DWORD*)StencilCaps;
			*dbg_values = DBGStencilCaps;
			*count = sizeof(StencilCaps)>>2;
		break;
		case SB_WRAP:
			*values = (DWORD*)Wrap;
			*dbg_values = DBGWrap;
			*count = sizeof(Wrap)>>2;
		break;
		case SB_MATCOLORSOURCE:
			*values = (DWORD*)MatColorSource;
			*dbg_values = DBGMatColSource;
			*count = sizeof(MatColorSource)>>2;
		break;
		case SB_VBELNDFLAGS:
			*values = (DWORD*)VBlendFlags;
			*dbg_values = DBGVBlendFlags;
			*count = sizeof(VBlendFlags)>>2;
		break;
		case SB_CLIPPLANES:
			*values = (DWORD*)ClipPlanes;
			*dbg_values = DBGClipPlanes;
			*count = sizeof(ClipPlanes)>>2;
		break;
		case SB_PATHEDGESTYLE:
			*values = (DWORD*)PatchEdgeStyle;
			*dbg_values = DBGPatchEdgeStyle;
			*count = sizeof(PatchEdgeStyle)>>2;
		break;
		case SB_MONITORTOKENS:
			*values = (DWORD*)MonitorTokens;
			*dbg_values = DBGMonitorTokens;
			*count = sizeof(MonitorTokens)>>2;
		break;
		case SB_COLORWRENABLE:
			*values = (DWORD*)ColWriteEnable;
			*dbg_values = DBGColWriteEnable;
			*count = sizeof(ColWriteEnable)>>2;
		break;
		case SB_BLENDOP:
			*values = (DWORD*)BlendOp;
			*dbg_values = DBGBlendOp;
			*count = sizeof(BlendOp)>>2;
		break;
		case SB_DEGREETYPE:
			*values = (DWORD*)DegreeType;
			*dbg_values = DBGDegreeType;
			*count = sizeof(DegreeType)>>2;
		break;
	}
	
	return true;
}	

bool	StateBlockInfo::get_ss_values(DWORD	**values,char ***dbg_values,DWORD *count){

	switch(SamplerStateFunc[sel_sstate]){
		case SB_SMRDWORD: 
		case SB_SMRFLOAT: 
		case SB_SMRCOLOR:		
			*values = NULL;
			*dbg_values = NULL;
			*count = 0;
		break;	
		case SB_TEXTUREADDRESS:
			*values = (DWORD*)TextureAddress;	
			*dbg_values = DBGTextAddr;
			*count = sizeof(TextureAddress) >> 2;
		break;
		case SB_TEXFILTERTYPE:
			*values = (DWORD*)TextureFilterType;	
			*dbg_values = DBGTextFilterType;
			*count = sizeof(TextureFilterType) >> 2;
		break;
	}
	
	return true;
}

bool	StateBlockInfo::get_tss_values(DWORD **values,char ***dbg_values,DWORD *count){

	switch(TextureStageStateFunc[sel_tsstate[sel_stage]]){
		case SB_TEXDWORD:
		case SB_TEXFLOAT:
		case SB_TEXCOLOR:
			*values = NULL;	
			*dbg_values = NULL;
			*count = 0;
		break;
		case SB_TEXTUREOP:
			*values = (DWORD*)TextureOp;	
			*dbg_values = DBGTextureOp;
			*count = sizeof(TextureOp) >> 2;
		break;
		case SB_TEXTUREARG:
			*values = (DWORD*)TextureArg;	
			*dbg_values = DBGTextureArg;
			*count = sizeof(TextureArg)>>2;
			break;
		case SB_TEXCOORINDEX:
			*values = (DWORD*)TextCoorIndex;	
			*dbg_values = DBGTextCoorIndex;
			*count = sizeof(TextCoorIndex) >> 2;
		break;
		case SB_TEXTRANSFLAGS:
			*values = (DWORD*)TextureTransFlags;	
			*dbg_values = DBGTextureTransFlags;
			*count = sizeof(TextureTransFlags) >> 2;
		break;
	}
	
	return true;
}


bool	StateBlockInfo::get_rs_values(DWORD rstate,DWORD	**values,char ***dbg_values,DWORD *count){

	switch(RenderStateFunc[rstate]){
		case SB_DWORD: 
		case SB_FLOAT: 
		case SB_INT:
		case SB_COLOR:		
			values = NULL;
			dbg_values = NULL;
			count = 0;
			break;
		case SB_FLAGS: // here we mast analyse sel_rstate

			break;
		case SB_ZBUFFERTYPE:
			*values = (DWORD*)ZBufferType;
			*dbg_values = DBGZBEnable;
			*count = sizeof(ZBufferType)>>2;
			break;
		case SB_FILLMODE:
			*values = (DWORD*)FillMode;
			*dbg_values = DBGFMode;
			*count = sizeof(FillMode)>>2;
			break;
		case SB_SHADEMODE:
			*values = (DWORD*)ShadeMode;
			*dbg_values = DBGSMode;
			*count = sizeof(ShadeMode)>>2;
			break;
		case SB_BOOL:
			*values = (DWORD*)D3DBoolValue;
			*dbg_values = DBGTrueFalse;
			*count = sizeof(D3DBoolValue)>>2;
			break;
		case SB_BLEND:
			*values = (DWORD*)Blend;
			*dbg_values = DBGBlend;
			*count = sizeof(Blend)>>2;
			break;
		case SB_CULL:
			*values = (DWORD*)Cull;
			*dbg_values = DBGCull;
			*count = sizeof(Cull)>>2;
			break;
		case SB_CMPFUNC:
			*values = (DWORD*)CmpFunc;
			*dbg_values = DBGCmpFunc;
			*count = sizeof(CmpFunc)>>2;
			break;
		case SB_FOGMODE:
			*values = (DWORD*)FogMode;
			*dbg_values = DBGFogMode;
			*count = sizeof(FogMode)>>2;
			break;
		case SB_STENCILCAPS:
			*values = (DWORD*)StencilCaps;
			*dbg_values = DBGStencilCaps;
			*count = sizeof(StencilCaps)>>2;
			break;
		case SB_WRAP:
			*values = (DWORD*)Wrap;
			*dbg_values = DBGWrap;
			*count = sizeof(Wrap)>>2;
			break;
		case SB_MATCOLORSOURCE:
			*values = (DWORD*)MatColorSource;
			*dbg_values = DBGMatColSource;
			*count = sizeof(MatColorSource)>>2;
			break;
		case SB_VBELNDFLAGS:
			*values = (DWORD*)VBlendFlags;
			*dbg_values = DBGVBlendFlags;
			*count = sizeof(VBlendFlags)>>2;
			break;
		case SB_CLIPPLANES:
			*values = (DWORD*)ClipPlanes;
			*dbg_values = DBGClipPlanes;
			*count = sizeof(ClipPlanes)>>2;
			break;
		case SB_PATHEDGESTYLE:
			*values = (DWORD*)PatchEdgeStyle;
			*dbg_values = DBGPatchEdgeStyle;
			*count = sizeof(PatchEdgeStyle)>>2;
			break;
		case SB_MONITORTOKENS:
			*values = (DWORD*)MonitorTokens;
			*dbg_values = DBGMonitorTokens;
			*count = sizeof(MonitorTokens)>>2;
			break;
		case SB_COLORWRENABLE:
			*values = (DWORD*)ColWriteEnable;
			*dbg_values = DBGColWriteEnable;
			*count = sizeof(ColWriteEnable)>>2;
			break;
		case SB_BLENDOP:
			*values = (DWORD*)BlendOp;
			*dbg_values = DBGBlendOp;
			*count = sizeof(BlendOp)>>2;
			break;
		case SB_DEGREETYPE:
			*values = (DWORD*)DegreeType;
			*dbg_values = DBGDegreeType;
			*count = sizeof(DegreeType)>>2;
			break;
	}

	return true;
}	

bool	StateBlockInfo::get_ss_values(DWORD sstate,DWORD	**values,char ***dbg_values,DWORD *count){

	switch(SamplerStateFunc[sstate]){
		case SB_SMRDWORD: 
		case SB_SMRFLOAT: 
		case SB_SMRCOLOR:		
			*values = NULL;
			*dbg_values = NULL;
			*count = 0;
			break;	
		case SB_TEXTUREADDRESS:
			*values = (DWORD*)TextureAddress;	
			*dbg_values = DBGTextAddr;
			*count = sizeof(TextureAddress) >> 2;
			break;
		case SB_TEXFILTERTYPE:
			*values = (DWORD*)TextureFilterType;	
			*dbg_values = DBGTextFilterType;
			*count = sizeof(TextureFilterType) >> 2;
			break;
	}

	return true;
}

bool	StateBlockInfo::get_tss_values(DWORD tsstate,DWORD **values,char ***dbg_values,DWORD *count){

	switch(TextureStageStateFunc[tsstate]){
		case SB_TEXDWORD:
		case SB_TEXFLOAT:
		case SB_TEXCOLOR:
			*values = NULL;	
			*dbg_values = NULL;
			*count = 0;
			break;
		case SB_TEXTUREOP:
			*values = (DWORD*)TextureOp;	
			*dbg_values = DBGTextureOp;
			*count = sizeof(TextureOp) >> 2;
			break;
		case SB_TEXTUREARG:
			*values = (DWORD*)TextureArg;	
			*dbg_values = DBGTextureArg;
			*count = sizeof(TextureArg)>>2;
			break;
		case SB_TEXCOORINDEX:
			*values = (DWORD*)TextCoorIndex;	
			*dbg_values = DBGTextCoorIndex;
			*count = sizeof(TextCoorIndex) >> 2;
			break;
		case SB_TEXTRANSFLAGS:
			*values = (DWORD*)TextureTransFlags;	
			*dbg_values = DBGTextureTransFlags;
			*count = sizeof(TextureTransFlags) >> 2;
			break;
	}

	return true;
}


void StateBlockInfo::write_xml(char *filename){
	
	FILE		*fp;
	char		**dbg_value;
	DWORD		*value;
	DWORD		count;
	DWORD		flags;	
	bool		found;
	DWORD		i,j,k;

	fopen_s(&fp, filename,"w"); 
	// writting caption
	fprintf(fp,"<?xml version=\"1.0\" standalone=\"yes\"?>");	
	fprintf(fp,"<!-- This is an xml log file for StateBlockInfo class.\nCreated by Abazyan Arsen (ars_s) for 3D engine.\nAll rights reserved. Tula 2003.-->");
	
	fprintf(fp,"<StateBlockInfo>");
	fprintf(fp,"<name>%s</name>",name);
	
	found = false;
	
	for(i=0;i<103;i++){
		if(rsenabled[i]){
			found = true;
			break;
		}
	}
	if(found){
		fprintf(fp,"<RenderState>");
		for(i=0;i<103;i++){
			if(rsenabled[i]){
				fprintf(fp,"<%s>",DBGRenderStateType[i]);
				get_rs_values(i,&value,&dbg_value,&count);
				if( (i >=31 && i<=38) ||
					(i == 51) ||
					(i >=92 && i<=99)){
				
					flags = rstate_value[i].dvalue;
					for(j=0;j<count;j++){
						if(flags & 0x01){
							fprintf(fp,"%s",dbg_value[j]);
							flags>>=1;
							if(flags ==0)break;
							fprintf(fp," | ");
						}
					}
				}
				else if(RenderStateFunc[i] == SB_DWORD){
					fprintf(fp," 0x%8x ",rstate_value[i].dvalue);
				}
				else if(RenderStateFunc[i] == SB_FLOAT){
					fprintf(fp," %.5f ",rstate_value[i].fvalue);
				}
				else if(RenderStateFunc[i] == SB_INT){
					fprintf(fp," %d ",((int)rstate_value[i].dvalue));	
				}
				else if(RenderStateFunc[i] == SB_COLOR){
					fprintf(fp," ARGB(%d, %d, %d, %d) ",(rstate_value[i].color>>24),
													((rstate_value[i].color & 0x00ff0000)>>16),
													((rstate_value[i].color & 0x0000ff00)>>8),				
													(rstate_value[i].color & 0x000000ff));
					
				}
				else 
					fprintf(fp," %s ",dbg_value[rstate_value[i].dvalue]); 
				
				fprintf(fp,"</%s>",DBGRenderStateType[i]);
			}
		}
		fprintf(fp,"</RenderState>");
	}
	
	for(i=0;i<8;i++){
		found = false;
		if(tsenabled[i]){
			for(j=0;j<18;j++){
				if(tssenabled[i][j]){
					found = true;
					break;
				}
			}	
			if(found)break;
		}	
	}
	if(found){
		fprintf(fp,"<TextureStageState>");
		for(i=0;i<8;i++){
			found = false;
			for(j=0;j<18;j++){
				if(tssenabled[i][j]){
					found = true;
					break;
				}
			}
			if(found){
				fprintf(fp,"<Texture%d>",i);
				for(j=0;j<18;j++){
					if(tssenabled[i][j]){
						fprintf(fp,"<%s>",DBGTextureStageStateType[j]);
						get_tss_values(j,&value,&dbg_value,&count);
						if(i == 10){
							
							flags = rstate_value[i].dvalue;
							for(k=0;k<count;k++){
								if(flags & 0x01){
									fprintf(fp,"%s",dbg_value[k]);
									flags>>=1;
									if(flags ==0)break;
									fprintf(fp," | ");
								}
							}
						}
						else if(TextureStageStateFunc[j] == SB_TEXDWORD){
							fprintf(fp," 0x%8x ",tsstate_value[i][j].dvalue);
						}
						else if(TextureStageStateFunc[j] == SB_TEXFLOAT){
							fprintf(fp," %.5f ",tsstate_value[i][j].fvalue);
						}
						else if(TextureStageStateFunc[j] == SB_TEXCOLOR){
							fprintf(fp," ARGB(%d, %d, %d, %d) ",(tsstate_value[i][j].color>>24),
															((tsstate_value[i][j].color & 0x00ff0000)>>16),
															((tsstate_value[i][j].color & 0x0000ff00)>>8),				
															(tsstate_value[i][j].color & 0x000000ff));	
						}
						else 
							fprintf(fp," %s ",dbg_value[tsstate_value[i][j].dvalue]); 

						fprintf(fp,"</%s>",DBGTextureStageStateType[j]);
					}// if tssenabled	
				}
				fprintf(fp,"</Texture%d>",i);
			}
		}
		fprintf(fp,"</TextureStageState>");
	}
		
	found = false;
	for(i=0;i<13;i++){
		if(ssenabled[i]){
			found = true;
			break;
		}
	}
	if(found){
		fprintf(fp,"<SamplerState>");
		for(i=0;i<13;i++){
			if(ssenabled[i]){
				fprintf(fp,"<%s>",DBGSamplerStateType[i]);
				get_ss_values(i,&value,&dbg_value,&count);
				if(SamplerStateFunc[i] == SB_SMRDWORD){
					fprintf(fp," 0x%8x ",sstate_value[i].dvalue);	
				}
				else if(SamplerStateFunc[i] == SB_SMRFLOAT){
					fprintf(fp," %.5f ",sstate_value[i].fvalue);		
				}
				else if(SamplerStateFunc[i] == SB_SMRCOLOR){
					fprintf(fp," ARGB(%d, %d, %d, %d) ",(sstate_value[i].color>>24),
														((sstate_value[i].color & 0x00ff0000)>>16),
														((sstate_value[i].color & 0x0000ff00)>>8),				
														(sstate_value[i].color & 0x000000ff));

				}
				else 
					fprintf(fp," %s ",dbg_value[sstate_value[i].dvalue]); 

				fprintf(fp,"</%s>",DBGSamplerStateType[i]);
			}
		}
		fprintf(fp,"</SamplerState>");
	}
	
	fprintf(fp,"</StateBlockInfo>");
	fclose(fp);
	
	return ;
}

void StateBlockInfo::set_name(char *nm){
	
	if(nm == NULL){
		if(name)delete[] name;
		name = NULL;
		return ;
	}	
	
	size_t len = strlen(nm);
	
	if(len){
		if(name)delete[] name;
		name = NULL;
	}
	name = new char[len+1];
	memcpy(name,nm,len);
	name[len] = '\0';

	return ;
}

bool	StateBlockInfo::save(char *filename){

	FILE	*fp;
	fopen_s(&fp, filename,"wb");

	if(fp == NULL)return false;

	fwrite("ASTB1.0",1,7,fp);
		
	fwrite(name,1,strlen(name)+1,fp);
	
	fwrite(&sel_rstate,4,1,fp);
	fwrite(rsenabled,sizeof(bool),103,fp);
	fwrite(rstate_value,4,103,fp);

	fwrite(&sel_stage,4,1,fp);
	fwrite(sel_tsstate,4,8,fp);
	fwrite(tssenabled,4,8*18,fp);
	fwrite(tsstate_value,4,8*18,fp);

	fwrite(&sel_sstate,4,1,fp);
	fwrite(ssenabled,sizeof(bool),13,fp);
	fwrite(sstate_value,4,13,fp);	

	return true;
}

bool	StateBlockInfo::load(char *filename){

	FILE	*fp;
	fopen_s(&fp, filename,"rb");
	char	buf[512];
	DWORD   i;
	
	if(fp == NULL)return false;

	fread(buf,1,7,fp);
	if(buf[0] != 'A' ||
		buf[1] != 'S' ||
		buf[2] != 'T' ||
		buf[3] != 'B' ||
		buf[4] != '1' ||
		buf[5] != '.' ||
		buf[6] != '0'){
		return false;
	}
	
	for(i=0;;i++){
		fread(&buf[i],1,1,fp);
		if(buf[i] == '\0') break;
	}
	set_name(buf);
	
	fread(&sel_rstate,4,1,fp);
	fread(rsenabled,sizeof(bool),103,fp);
	fread(rstate_value,4,103,fp);

	fread(&sel_stage,4,1,fp);
	fread(sel_tsstate,4,8,fp);
	fread(tsenabled,4,8,fp);
	fread(tssenabled,4,8*18,fp);
	fread(tsstate_value,4,8*18,fp);

	fread(&sel_sstate,4,1,fp);
	fread(ssenabled,sizeof(bool),13,fp);
	fread(sstate_value,4,13,fp);	

	return true;
}