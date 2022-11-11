#include "anim.h"
#include <fstream>


void Animation::Update(DWORD curr_time){
	
	long interval;
	
	if(dwLastTime == 0){ 
		dwLastTime = curr_time;
		return ;
	}
	
	if(State == ANIM_STOP || State == ANIM_PAUSE)return;
		
	interval = curr_time - dwLastTime - dwDeltaTime;
		
	if(interval > 0)dwLastTime = curr_time;
	while(interval > 0){
			
		if(bWillStop){ // if animation have to stop
			
			if(bRollBack){
				if(CurrPos.frame == StartPos.frame &&
					CurrPos.dir == StartPos.dir){
					CurrPos.cycle--;
				
					if(CurrPos.cycle == StartPos.cycle){
						dwLastTime = curr_time;
						Stop();
						bRollBack = false;				
						return;
					}
				}
			}
			
			else if(CurrPos.frame == EndPos.frame &&
				CurrPos.dir == EndPos.dir){
				CurrPos.cycle++;
				
				if(CurrPos.cycle == EndPos.cycle){
					dwLastTime = curr_time;
					Stop();
					return;
				}
			}
		}
	
		if(CurrPos.frame == EndPos.frame){
			if(CurrPos.dir == ANIM_DIR_FORWARD){
				
				if(PlayType == ANIM_PLAY_SIMPLE){
					dwLastTime = curr_time;
					return;
				}
				
				if(PlayType == ANIM_PLAY_CYCLE){
					
					CurrPos.frame = StartPos.frame;
					lpCurrElem  = lpData + StartPos.frame * dwElemSize;
					
					CurrPos.cycle ++;
				}
				
				if(PlayType == ANIM_PLAY_FORWARDBACK){
											
						CurrPos.dir = ANIM_DIR_BACK;
						CurrPos.frame--;
						lpCurrElem -= dwElemSize;
				}	
			}
			else {
				
				CurrPos.frame--;
				lpCurrElem -= dwElemSize;
			}
		}
	
		else if(CurrPos.frame == StartPos.frame){
			if(CurrPos.dir == ANIM_DIR_BACK){
				if(PlayType == ANIM_PLAY_SIMPLE){
					dwLastTime = curr_time;
					return ;
				}
				if(PlayType == ANIM_PLAY_CYCLE){
					CurrPos.frame = EndPos.frame;
					lpCurrElem = lpData + EndPos.frame * dwElemSize;
				
					CurrPos.cycle --;
				
				}
				if(PlayType == ANIM_PLAY_FORWARDBACK){
					
					CurrPos.dir = ANIM_DIR_FORWARD;
					CurrPos.frame++;
					lpCurrElem += dwElemSize;
				}
			}
			else {
				CurrPos.frame++;
				lpCurrElem += dwElemSize;
			}
		}
				
		else if(CurrPos.frame == 0 && CurrPos.dir == ANIM_DIR_FORWARD){
			CurrPos.frame = dwUpperBound;
			lpCurrElem = lpData + dwDataSize;
		}
		
		else if(CurrPos.frame == dwUpperBound && CurrPos.dir == ANIM_DIR_BACK){
			CurrPos.frame = 0;
			lpCurrElem = lpData;
		}
		
		else if(CurrPos.dir == ANIM_DIR_BACK){
			CurrPos.frame--;
			lpCurrElem -= dwElemSize;
		}
		
		else if(CurrPos.dir == ANIM_DIR_FORWARD){
			CurrPos.frame++;
			lpCurrElem += dwElemSize;
		}
	
		interval -= dwDeltaTime;
	}
}

bool Animation::SaveToFile(TCHAR *filename){
	
	
	FILE *fp;
	_tfopen_s(&fp, filename,TEXT("wb"));
	
	if(fp == NULL)return false;
				
	// we open file in binary mode but write text info
	// so se must use \xd\xa instead \n
	fprintf(fp,"Animation\x0d\x0a");
	fprintf(fp,"type                = ARS_ANIMATION\x0d\x0a");
	fprintf(fp,"dwDeltaTime         = %d\x0d\x0a",dwDeltaTime);
	fprintf(fp,"dwUpperBound        = %d\x0d\x0a\x0d\x0a",dwUpperBound);
	
	// StartPos 
	fprintf(fp,"StartPos.frame      = %d\x0d\x0a",StartPos.frame);
	
	if(StartPos.dir == ANIM_DIR_FORWARD)
				    
		fprintf(fp,"StartPos.dir        = ANIM_DIR_FORWARD\x0d\x0a");
	else if(StartPos.dir == ANIM_DIR_BACK)
		fprintf(fp,"StartPos.dir        = ANIM_DIR_BACK\x0d\x0a");
	            
	fprintf(fp,"StartPos.cycle      = %d\x0d\x0a\x0d\x0a",StartPos.cycle);

	// EndPos   
	fprintf(fp,"EndPos.frame        = %d\x0d\x0a",EndPos.frame);
	if(EndPos.dir == ANIM_DIR_FORWARD)
		fprintf(fp,"EndPos.dir          = ANIM_DIR_FORWARD\x0d\x0a");
	else if(EndPos.dir == ANIM_DIR_BACK)
		fprintf(fp,"EndPos.dir          = ANIM_DIR_BACK\x0d\x0a");
			    
	fprintf(fp,"EndPos.cycle        = %d\x0d\x0a\x0d\x0a",EndPos.cycle);
	
	// CurrPos  
	fprintf(fp,"CurrPos.frame       = %d\x0d\x0a",CurrPos.frame);
	if(CurrPos.dir == ANIM_DIR_FORWARD)
		fprintf(fp,"CurrPos.dir         = ANIM_DIR_FORWARD\x0d\x0a");
	else if(CurrPos.dir == ANIM_DIR_BACK)
		fprintf(fp,"CurrPos.dir         = ANIM_DIR_BACK\x0d\x0a");
	            
	fprintf(fp,"CurrPos.cycle       = %d\x0d\x0a\x0d\x0a",CurrPos.cycle);
	
	if(bWillStop)   
		fprintf(fp,"bWillStop           = true\x0d\x0a");
	else 
		fprintf(fp,"bWillStop           = false\x0d\x0a");
	
	if(bRollBack)   
		fprintf(fp,"bRollBack           = true\x0d\x0a");
	else 
		fprintf(fp,"bRollBack           = false\x0d\x0a");
	
	switch(PlayType){
		case ANIM_PLAY_SIMPLE:
			fprintf(fp,"PlayType            = ANIM_PLAY_SIMPLE\x0d\x0a");	
		break;
		case ANIM_PLAY_CYCLE:
			fprintf(fp,"PlayType            = ANIM_PLAY_CYCLE\x0d\x0a");	
		break;
		case ANIM_PLAY_FORWARDBACK:
			fprintf(fp,"PlayType            = ANIM_PLAY_FORWARDBACK\x0d\x0a");	
		break;
	}
	
	switch(State){
		case ANIM_STOP:
			fprintf(fp,"State               = ANIM_STOP\x0d\x0a");	
		break;
		case ANIM_PLAY:
			fprintf(fp,"State               = ANIM_PLAY\x0d\x0a");	
		break;
		case ANIM_PAUSE:
			fprintf(fp,"State               = ANIM_PAUSE\x0d\x0a");	
		break;
		default: 
			fprintf(fp,"State               = ANIM_STOP\x0d\x0a");	
		break;
	}
	            
	fprintf(fp,"dwDataSize          = %d\x0d\x0a",dwDataSize);
	fprintf(fp,"dwElemSize          = %d\x0d\x0a",dwElemSize);

	fprintf(fp,"\x0d\x0alpData\x0d\x0a\x0d\x0a");
	
	fwrite(lpData,1, (dwUpperBound + 1) * dwElemSize,fp);
	
	fprintf(fp,"\x0d\x0a\x0d\x0a end Animation");
	
	fclose(fp);

	return true;
}


bool Animation::LoadFromFile(TCHAR *filename){
	
	Clear();
	
	FILE *fp;
	_tfopen_s(&fp, filename,TEXT("rb"));
	
	if(fp == NULL)return false;

	char		buf[256];
	TCHAR		tbuf[256];
	shString	hs1,hs2;
	
	// skip Animation
	fscanf_s(fp, "%s",buf);
	
	// skip type
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	
	// read ARS_ANIMATION
	fscanf_s(fp, "%s",buf);
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);
	hs2.initialize(TEXT("ARS_ANIMATION"));
	if(hs1 != hs2){
		fclose(fp);
		return false;
	}
	
	// skip dwDeltaTime
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read dwDeltaTime
	fscanf_s(fp, "%d",&dwDeltaTime);
    
	// skip dwUpperBound
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read dwUpperBound
	fscanf_s(fp, "%d",&dwUpperBound);

	// skip StartPos.frame
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read StartPos.frame
	fscanf_s(fp, "%d",&StartPos.frame);

	// skip StartPos.dir
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read StartPos.dir
	fscanf_s(fp, "%s",buf);

	// check DirType
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);

	if(hs1 == TEXT("ANIM_DIR_FORWARD"))
		StartPos.dir = ANIM_DIR_FORWARD;
	else if(hs1 == TEXT("ANIM_DIR_BACK"))
		StartPos.dir = ANIM_DIR_BACK;
	else {
		fclose(fp);
		return false;
	}
	
	// skip StartPos.cycle
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read StartPos.cycle
	fscanf_s(fp, "%d",&StartPos.cycle);
	
	
	// skip EndPos.frame
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read EndPos.frame
	fscanf_s(fp, "%d",&EndPos.frame);

	// skip EndPos.dir
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read EndPos.dir
	fscanf_s(fp, "%s",buf);

	// check DirType
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);

	if(hs1 == TEXT("ANIM_DIR_FORWARD"))
		EndPos.dir = ANIM_DIR_FORWARD;
	else if(hs1 == TEXT("ANIM_DIR_BACK"))
		EndPos.dir = ANIM_DIR_BACK;
	else {
		fclose(fp);
		return false;
	}
	
	// skip EndPos.cycle
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read EndPos.cycle
	fscanf_s(fp, "%d",&EndPos.cycle);
	
	
	// skip CurrPos.frame
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read CurrPos.frame
	fscanf_s(fp, "%d",&CurrPos.frame);

	// skip CurrPos.dir
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read CurrPos.dir
	fscanf_s(fp, "%s",buf);

	// check DirType
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);

	if(hs1 == TEXT("ANIM_DIR_FORWARD"))
		CurrPos.dir = ANIM_DIR_FORWARD;
	else if(hs1 == TEXT("ANIM_DIR_BACK"))
		CurrPos.dir = ANIM_DIR_BACK;
	else {
		fclose(fp);
		return false;
	}
	
	// skip CurrPos.cycle
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read CurrPos.cycle
	fscanf_s(fp, "%d",&CurrPos.cycle);
	
	// skip bWillStop
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read bWillStop
	fscanf_s(fp, "%s",buf);
	
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);
	if(hs1 == TEXT("true"))
		bWillStop = true;
	else if(hs1 == TEXT("false"))
		bWillStop = false;
	else {
		fclose(fp);
		return false;
	}

	// skip bRollBack
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read bRollBack
	fscanf_s(fp, "%s",buf);
	
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);
	if(hs1 == TEXT("true"))
		bRollBack = true;
	else if(hs1 == TEXT("false"))
		bRollBack = false;
	else {
		fclose(fp);
		return false;
	}
	
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);
	if(hs1 == TEXT("true"))
		bWillStop = true;
	else if(hs1 == TEXT("false"))
		bWillStop = false;
	else {
		fclose(fp);
		return false;
	}	
	
	// skip PlayType
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read PlayType
	fscanf_s(fp, "%s",buf);

	// check DirType
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);

	if(hs1 == TEXT("ANIM_PLAY_SIMPLE"))
		PlayType = ANIM_PLAY_SIMPLE;
	else if(hs1 == TEXT("ANIM_PLAY_FORWARDBACK"))
		PlayType = ANIM_PLAY_FORWARDBACK;
	else if(hs1 == TEXT("ANIM_PLAY_CYCLE"))
		PlayType = ANIM_PLAY_CYCLE;
	else {
		fclose(fp);
		return false;
	}

	// skip State
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read State
	fscanf_s(fp, "%s",buf);

	// check State
	strToWCS(buf, tbuf);
	hs1.initialize(tbuf);

	if(hs1 == TEXT("ANIM_PLAY"))
		State = ANIM_PLAY;
	else if(hs1 == TEXT("ANIM_STOP"))
		State = ANIM_STOP;
	else if(hs1 == TEXT("ANIM_PAUSE"))
		State = ANIM_PAUSE;
	else {
		fclose(fp);
		return false;
	}
	
	// skip dwDataSize
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read dwDataSize
	fscanf_s(fp, "%d",&dwDataSize);
    
	// skip dwElemSize
	fscanf_s(fp, "%s",buf);
	// skip = 
	fscanf_s(fp, "%s",buf);
	// read dwElemSize
	fscanf_s(fp, "%d",&dwElemSize);

	// skip Data and 2 CRLF
	fscanf_s(fp, "%s",buf);
    fseek(fp,4,SEEK_CUR);	
	
	lpData = new char [(dwUpperBound + 1)* dwElemSize];
	if(dwUpperBound + 1 != fread(lpData,dwElemSize,(dwUpperBound + 1),fp)){
		fclose(fp);
		return false;
	}
	
	lpCurrElem = lpData + CurrPos.frame * dwElemSize;
	
	fclose(fp);
	return true;
}


void Animation::SetData(char *d,DWORD ds,DWORD es){
	
	dwDataSize = ds;
	dwElemSize = es;
	
	lpData = new char[dwDataSize];
	memcpy(lpData,d,dwDataSize);
	
	lpCurrElem = lpData + CurrPos.frame * dwElemSize;

}

void Animation::Copy(const Animation &anim){

	Clear();
	
	dwDeltaTime  = anim.dwDeltaTime;
	dwLastTime   = anim.dwLastTime;
	dwUpperBound = anim.dwUpperBound;
	
	StartPos.frame = anim.StartPos.frame;
	StartPos.dir   = anim.StartPos.dir;
	StartPos.cycle = anim.StartPos.cycle;

	EndPos.frame = anim.EndPos.frame;
	EndPos.dir   = anim.EndPos.dir;
	EndPos.cycle = anim.EndPos.cycle;

	CurrPos.frame = anim.CurrPos.frame;
	CurrPos.dir   = anim.CurrPos.dir;
	CurrPos.cycle = anim.CurrPos.cycle;

	bWillStop     = anim.bWillStop;
	bRollBack     = anim.bRollBack;
	PlayType      = anim.PlayType;
	
	State		  = anim.State;
	dwDataSize    = anim.dwDataSize;
	dwElemSize    = anim.dwElemSize;

	lpData = new char[dwDataSize];
	memcpy(lpData,anim.lpData,dwDataSize);
	
	lpCurrElem = lpData + CurrPos.frame * dwElemSize;
}
