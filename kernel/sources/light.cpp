#include "light.h"

BYTE	checkByte14;

WORD	checkWord2;
#define CHECK_WORD2

BYTE	checkByte39;
BYTE	checkByte1_34;

WORD	checkWord2_1;
#define CHECK_WORD2_1

BYTE	checkByte2;
BYTE	checkByte21;

BYTE	checkByte1_2;
BYTE	checkByte1_11;

std::ostream& operator<<(std::ostream &os,Light &l){
    os<<"Type = "<<l.Type<<std::endl;
    
    os<<"Position = "<<l.Position.x;
    os<<","<<l.Position.y;
    os<<","<<l.Position.z<<std::endl;

    os<<"Direction = "<<l.Direction.x;
    os<<","<<l.Direction.y;
    os<<","<<l.Direction.z<<std::endl;

    os<<"Range = "<<l.Range<<std::endl;
    os<<"Falloff = "<<l.Falloff<<std::endl;
    os<<"Attenuation0 = "<<l.Attenuation0<<std::endl;
    os<<"Attenuation1 = "<<l.Attenuation1<<std::endl;
    os<<"Attenuation2 = "<<l.Attenuation2<<std::endl;

    os<<"Theta = "<<l.Theta<<std::endl;
    os<<"Phi = "<<l.Phi<<std::endl;
    
    return os;
}