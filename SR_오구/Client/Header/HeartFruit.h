#pragma once
#include "GameObject.h"

#include "PickUpButton.h"
#include "AddHP.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CTriCol;
class CCollider;

END
class CPlayer;

class CHeartFruit : public CGameObject
{
private:
	explicit CHeartFruit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHeartFruit();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	  void		OnCollision(CGameObject* _pOther);

	void		Set_Pos(_vec3 _vPos) 
	{
		m_pTransformCom->m_vInfo[INFO_POS] = _vPos;
	}
private:
	HRESULT    Add_Component();

	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pPriceTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pColliderCom;

	CPickUpButton*  m_pPickUpButton;
	CPlayer*		m_pPlayer;
	CAddHP*		m_pEffect;
	_bool m_bDead;
public:
	static CHeartFruit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

