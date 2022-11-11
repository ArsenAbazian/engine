#ifndef FPS_INC
#define FPS_INC

#include <windows.h>
#pragma comment(lib,"winmm.lib")

class FPS{
    float   fps_count;
    DWORD   last_time;
    DWORD   delta_time;
    DWORD   max_time;
    float   ave_fps;
public:
    FPS(){ ZeroMemory(this, sizeof(FPS)); }
   ~FPS(){}

    void    Period(DWORD max){ max_time = max; }
    void    Update();
    float   Average(){ return ave_fps; }
    void    Reset(){ fps_count = 0; 
                     delta_time = 0;
                     ave_fps = 0;
                     last_time = timeGetTime(); }
};

#endif