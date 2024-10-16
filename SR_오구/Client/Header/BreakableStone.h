#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CBreakableStone :public Engine::CGameObject
{
private:
	explicit CBreakableStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBreakableStone();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollisionEnter(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CBreakableStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	_int m_iTargetID;
	_float m_fTime;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};