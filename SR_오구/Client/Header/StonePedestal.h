#pragma once
#include "GameObject.h"
#include "Stone.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CStonePedestal :public Engine::CGameObject
{
private:
	explicit CStonePedestal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStonePedestal();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:	
	void Set_Group(CGameObject* _pObj) { m_pGroup = _pObj; }
	CGameObject* Get_Stone() { return m_pCollided; }
	_int Get_StoneID() { return m_pCollided == nullptr ? -1 : static_cast<CStone*>(m_pCollided)->Get_StoneID(); }
	void Init(_float _fX, _float _fZ);	

private:
	HRESULT    Add_Component();	
		
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTransform* m_pTexTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CStonePedestal* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_float m_fPullDuration;
	CGameObject* m_pCollided;
	CGameObject* m_pGroup;
	vector<IDirect3DTexture9*> m_vecTexture;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};