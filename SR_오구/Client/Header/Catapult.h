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

class CCatapult :public Engine::CGameObject
{
private:
	explicit CCatapult(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCatapult();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	void Init_Pos(_float _fX, _float _fZ);
	virtual void	SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;	
	Engine::CTransform* m_pTransformCom;
	Engine::CTransform* m_pTexTransformCom;
	Engine::CTransform* m_pBucketTransformCom;
	Engine::CTransform* m_pInTransformCom;
	Engine::CTexture* m_pInTextureCom;

	Engine::CCollider* m_pBoundBox;

public:
	static CCatapult* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool m_bIndicator;
	_bool m_bIsActivate;
	_float m_fPullDuration;
	_float m_fTime;
	_int m_iTargetID;
	_int m_iImageID;
	CGameObject* m_pStone;
	vector<IDirect3DTexture9*> m_vecTexture;

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