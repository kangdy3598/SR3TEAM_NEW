#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CFirePit :public Engine::CGameObject
{
private:
	explicit CFirePit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirePit();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Set_ImageID(_int _iID) { m_iImageID = _iID; }
	void Set_Ignite(_bool _bIgnite) { m_bIsIgnite = _bIgnite; };
	void Init_Pos(_float _fX, _float _fZ);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;	
	Engine::CTransform* m_pFireTransformCom;

private:
	HRESULT    Add_Component();

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	_bool m_bIsIgnite;
	_int m_iImageID;
	_int m_iTargetID;
	_float m_fTime;

public:
	static CFirePit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};
