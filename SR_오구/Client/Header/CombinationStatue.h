#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CCombinationStatue :public Engine::CGameObject
{
private:
	explicit CCombinationStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCombinationStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);
	virtual void	SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }

public:
	void Set_Group(CGameObject* _pGroup) { m_pGroup = _pGroup; }
	_int Get_ImageID() { return m_iImageID; }

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

public:
	static CCombinationStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	CGameObject* m_pGroup;
	_float m_fActiveTime;
	_bool m_bIsActivate;

protected:
	CPlayer* m_CPlayer;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};
