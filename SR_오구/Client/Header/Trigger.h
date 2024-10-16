#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCollider;

END

class CTrigger :public Engine::CGameObject
{
private:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrigger();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollisionEnter(CGameObject* _pOther);
private:
	HRESULT    Add_Component();

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:	
	CGameObject* m_pGroup;

public:
	static CTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_Group(CGameObject* _pObj) { m_pGroup = _pObj; }
	void Initialize(_float _fX, _float _fZ, _vec3 _vScale);

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};