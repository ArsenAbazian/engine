#ifndef		LIGHT_INC
#define		LIGHT_INC

#include <d3d9.h>
#include <fstream>
#include "vecmat.h"

class Light{
    LPDIRECT3DDEVICE9   lpDevice;
    DWORD               Index;
public:
    D3DLIGHTTYPE        Type;
    D3DCOLORVALUE       Diffuse;
    D3DCOLORVALUE       Specular;
    D3DCOLORVALUE       Ambient;
    D3DVECTOR           Position;
    D3DVECTOR           Direction;
    float               Range;
    float               Falloff;
    float               Attenuation0;
    float               Attenuation1;
    float               Attenuation2;
    float               Theta;
    float               Phi;
    
    Light(){ ZeroMemory(this,sizeof(Light)); }
   ~Light(){};
    
    void Pos(float x,float y,float z){
        Position.x = x;
        Position.y = y;
        Position.z = z;
    }
    void Pos(Vector3 pos){
        Position.x = pos[0];
        Position.y = pos[1];
        Position.z = pos[2];
    }
    
    void Dir(float x,float y,float z){
        Direction.x = x;
        Direction.y = y;
        Direction.z = z;
        normalizeVector((float*)&Direction);
    }
    void Dir(Vector3 dir){
        Direction.x = dir[0];
        Direction.y = dir[1];
        Direction.z = dir[2];
        normalizeVector((float*)&Direction);
    }
    
    void Diff(float a,float r,float g,float b){
        Diffuse.a = a;
        Diffuse.r = r;
        Diffuse.g = g;
        Diffuse.b = b;
    }
    void Amb(float a,float r,float g,float b){
        Ambient.a = a;
        Ambient.r = r;
        Ambient.g = g;
        Ambient.b = b;
    }
    void Spec(float a,float r,float g,float b){
        Specular.a = a;
        Specular.r = r;
        Specular.g = g;
        Specular.b = b;
    }
            
    void Attenuations(float a0,float a1,float a2){
        Attenuation0 = a0;
        Attenuation1 = a1;
        Attenuation2 = a2;
    }
    
    void Device(LPDIRECT3DDEVICE9 lpd3dd9){
        lpDevice = lpd3dd9;
    }
    HRESULT  Set(DWORD index){
        Index = index;
        return lpDevice->SetLight(Index,(D3DLIGHT9*)&Type);
    }
    HRESULT  Set(){
        return lpDevice->SetLight(Index,(D3DLIGHT9*)&Type);
    }
    HRESULT Enable(){
        return lpDevice->LightEnable(Index,TRUE);
    }
    HRESULT Disable(){
        return lpDevice->LightEnable(Index,FALSE);
    }

    void Move(float dx,float dy,float dz){
        Position.x+= dx;
        Position.y+= dy;
        Position.z+= dz;
    }
    void Move(Vector3 dv){
        Position.x+= dv[0];
        Position.y+= dv[1];
        Position.z+= dv[2];
    }
    void RotateAroundAxis(Vector3 point,Vector3 axis,float rad){
         rotateVectorAroundAxis((float*)&Position,axis,point,(float*)&Position,rad);
    }
    void RotateAroundNormAxis(Vector3 point,Vector3 naxis,float rad){
        rotateVectorAroundNormAxis((float*)&Position,naxis,point,(float*)&Position,rad);
    }
};

std::ostream& operator<<(std::ostream &os,Light &light);

#endif