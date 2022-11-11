#ifndef A_ANIMATION_INC
#define A_ANIMATION_INC

#include <windows.h>
#include <fstream>
#include "hstring.h"
#include "rcmanager.h"

#define ANIM_FRAME_UB				0xffffffff
#define ANIM_FRAME_LB				0

typedef enum {
	ANIM_PLAY_SIMPLE				= 1,
	ANIM_PLAY_CYCLE					= 2,
	ANIM_PLAY_FORWARDBACK			= 3
}AnimPlayType;

typedef enum {
	ANIM_STOP						= 1,
	ANIM_PLAY						= 2,
	ANIM_PAUSE						= 3
}AnimStateType;

typedef enum {
	ANIM_DIR_FORWARD				= 1,	
	ANIM_DIR_BACK					= 2	
}AnimDirType;


typedef struct {
	DWORD	frame;
	DWORD	dir;
	DWORD	cycle;
}AnimationPos;

class Animation{
	
	DWORD				dwDeltaTime;	// промежуток времени
	DWORD				dwLastTime;		// предыдущее время
	DWORD	   			dwUpperBound;	// верхний предел кадра
	
	AnimationPos		StartPos;		// начальная позиция анимации
	AnimationPos		EndPos;			// конечная позиция анимации
	AnimationPos		CurrPos;		// текущая позиция анимации
	
	bool    			bWillStop;		// кагда то должен остановиться
	bool				bRollBack;		// должен идти обратно
	AnimPlayType		PlayType;		// тип воспроизведения
	AnimStateType		State;			// состояние анимации
	
	DWORD   			dwDataSize;      	// размер буффера
	DWORD   			dwElemSize;      	// размер элемента 
	
	char    			*lpData;          	// какие то данные анимации
	char    			*lpCurrElem;   		// текущий элемент
	
	bool		IsIqualAnimPos(AnimationPos *pos1, AnimationPos *pos2){
					return (bool)( pos1->frame == pos2->frame &&
					 				pos1->dir == pos2->dir &&
					 				pos1->cycle == pos2->cycle);
				}
	
	void		CopyAnimPos(AnimationPos *dstPos, AnimationPos *srcPos){
					dstPos->frame = srcPos->frame;
					dstPos->dir   = srcPos->dir;
					dstPos->cycle = srcPos->cycle;
				}
	
public:
	Animation(){
		ZeroMemory(this,sizeof(Animation));
	}
	~Animation(){};

	void SetFramesBound(DWORD ub){
		dwUpperBound = ub;
	}
	
	DWORD GetFramesBound(){ return dwUpperBound; }
	
	void SetBeginTime(DWORD ltime){
		dwLastTime = ltime;
	}
		
	void SetTimeInterval(DWORD dtime){
		dwDeltaTime = dtime;
	}
	
	DWORD GetTimeInterval(){ return dwDeltaTime; }
	
	void SetStartPos(DWORD sf,DWORD sd,DWORD sc){
		
		StartPos.frame = sf;
		StartPos.dir   = sd;
		StartPos.cycle = sc;
		
				
		if(StartPos.frame >= dwUpperBound)StartPos.frame = dwUpperBound;		

		CopyAnimPos(&CurrPos,&StartPos);		
	
		
		lpCurrElem  = lpData + dwElemSize * StartPos.frame;
	}
	
	void GetStartPos(AnimationPos &stPos){
	
		stPos.frame = StartPos.frame;
		stPos.dir   = stPos.dir;
		stPos.cycle = StartPos.cycle;

	}
	
	void SetEndPos(DWORD ef,DWORD edir,DWORD ecyc){
		
		EndPos.frame = ef;
		EndPos.dir   = edir;
		EndPos.cycle = ecyc;
		
				
		if(EndPos.frame >= dwUpperBound)EndPos.frame = dwUpperBound;		
		
	}
	
	void GetEndPos(AnimationPos &endPos){
	
		endPos.frame = EndPos.frame;
		endPos.dir   = EndPos.dir;
		endPos.cycle = EndPos.cycle;

	}
	
	void GetCurrPos(AnimationPos &currPos){
	
		currPos.frame = CurrPos.frame;
		currPos.dir   = CurrPos.dir;
		currPos.cycle = CurrPos.cycle;

	}
	
	void WillStop(bool ws){
		bWillStop = ws;
	}	

	bool IsWillStop(){ return bWillStop; }
	bool IsRollBack(){ return bRollBack; } 
	
	void SetPlayType(AnimPlayType ptype){

		PlayType = ptype;

	}	
	
	AnimPlayType  GetPlayType(){ return PlayType; }
	AnimStateType GetState(){ return State; }
	
	void Play(){ 
		lpCurrElem = lpData + dwElemSize * CurrPos.frame;
		State = ANIM_PLAY; 
	}
	
	void Stop(){ State = ANIM_STOP; }
	void Pause(){ State = ANIM_PAUSE; }
	void Reset(){ 
		bRollBack = false;
		CopyAnimPos(&CurrPos, &StartPos); 
		lpCurrElem = lpData + dwElemSize * CurrPos.frame;
	}
	void Resume(DWORD dt){ 
		SetBeginTime(dt); 
		if(State == ANIM_PAUSE)
			Play(); 
	} 

	void Clear(){ 
		if(lpData)delete[] lpData;
		ZeroMemory(this,sizeof(Animation)); 
	}
	
	DWORD GetFrame(){ return CurrPos.frame; }
	char* GetElem(){ return lpCurrElem; }
	char* GetElem(DWORD ind){ return (lpData + ind * dwElemSize); }
	
	void Update(DWORD curr_time); 

	bool LoadFromFile(TCHAR *file);
		
	bool SaveToFile(TCHAR *file);
	
	void SetData(char *d,DWORD ds,DWORD es);
	void ChangeDir(AnimDirType d){
		CurrPos.dir = d;
	}
	
	void MirrorDir(){  
		if(CurrPos.dir == ANIM_DIR_FORWARD)CurrPos.dir = ANIM_DIR_BACK;
		else CurrPos.dir = ANIM_DIR_FORWARD;
	}
	
	void RollBack(){
		MirrorDir();
		bRollBack = ! bRollBack;
	}
	
	void Copy(const Animation &anim);
};

void pass_comment(std::ifstream &fin);

#endif
