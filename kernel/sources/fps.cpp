#include "fps.h"

void FPS::Update(){
    DWORD cur_time;
    
    fps_count++;
    
    cur_time   = timeGetTime();
    delta_time+= cur_time-last_time; 
    last_time  = cur_time;
    
    if(delta_time>=max_time){
        ave_fps   = 1000*fps_count/delta_time;
        fps_count = 0;
        delta_time = 0;
    }
    return;
}
