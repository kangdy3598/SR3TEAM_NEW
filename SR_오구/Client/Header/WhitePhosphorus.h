#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CWhitePhosphorus :public Engine::CGameObject
{

private:
	explicit CWhitePhosphorus(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWhitePhosphorus();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:	
	void Set_Velocity(_vec3 _vVel) { m_vVelocity = _vVel; }
	void Init_Pos(_float _fX, _float _fZ);

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CWhitePhosphorus* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Launch();

private:
	_float m_fDuration;
	_float m_fTime;
	_bool m_bIsLaunched;
	_vec3 m_vVelocity;
	_int m_iImageID;	
	vector<IDirect3DTexture9*> m_vecTexture;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

public:
	virtual void Free();
};