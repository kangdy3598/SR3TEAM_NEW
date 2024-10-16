#pragma once
#include "Engine_Define.h"
#include "GraphicDev.h"


class CResMgr
{
	DECLARE_SINGLETON(CResMgr)

private:
	explicit CResMgr();
	virtual ~CResMgr();


public:
	void init();
	void update();
	void Render();
	void Release();

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}


private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	vector<IDirect3DTexture9*> m_vecTexture;
	vector<IDirect3DTexture9*> m_vecEffectTexture;
	



public:
	vector<IDirect3DTexture9*>& GetTexture() { return m_vecTexture; }
	vector<IDirect3DTexture9*>& GetEffectTexture() { return  m_vecEffectTexture; }
};

