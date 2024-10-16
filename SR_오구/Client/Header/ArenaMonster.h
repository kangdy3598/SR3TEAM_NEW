#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Export_Utility.h"

BEGIN(Engine)

//class CRcTex;
class CTransform;
class CCollider;
class CTriCol;
class CAnimator2;
class CAnimation2;

END



class CArenaMonster : public Engine::CGameObject
{
protected:
	explicit CArenaMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArenaMonster();

public:

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollision(CGameObject* _pOther) {}

	void			Set_Create() { m_bCreate = true; }
protected:
	HRESULT			Add_Component();

protected:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pColliderCom;

	Engine::CAnimator2* m_pEffectAnimatorCom;
	Engine::CTransform* m_pEffectTransformCom;

	CPlayer*		   m_pPlayer;

	_bool			   m_bCreate;
protected:
	virtual void Free();

	IDirect3DTexture9* m_pTex;
	IDirect3DTexture9* m_pEffectTex;
};

