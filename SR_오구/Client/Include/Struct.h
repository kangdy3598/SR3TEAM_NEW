#pragma once
#include "Types.h"
#include <string>

using namespace std; 

struct Vertex
{
	float _x, _y, _z;
	DWORD _color;
};



struct ObjectData
{
    WCHAR objectName[256];
    WCHAR textureKey[256];
    D3DXMATRIX worldMatrix; 
    D3DXVECTOR3 rotation;   
    D3DXVECTOR3 scale;  
};
