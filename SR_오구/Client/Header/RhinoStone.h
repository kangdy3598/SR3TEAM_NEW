#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTriCol;
class CAnimator2;
class CAnimation2;

END
class CPlayer;
class CRhino;

class CRhinoStone : public CGameObject
{
	// temp
private:
	_uint m_iIndex;
	static _uint m_iStaticID;

public:
	explicit CRhinoStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRhinoStone();

public:
	enum StoneState { STONE_TARGET, STONE_APPEAR, STONE_SURVIVE, STONE_BROKEN, STONE_DEAD };

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollision(CGameObject* _pOther);

	StoneState		Get_State() { return	m_eState; }
	void			Set_PosAble() { m_bPosSet = false; }
	void			Set_EffectNumber(_int _Num) { m_iNum = _Num; }
	void			Set_Appear() { m_eState = STONE_TARGET; m_fTime = 0; }
	void			Set_Dead()
	{
		m_eState = STONE_BROKEN;
		m_fTime = 0;
		Engine::Play_Sound(L"SFX_332_StoneSpike_Ready.wav", SOUND_STONE, 2.f);
	}
	_bool 			Get_Dead() { return m_eState == STONE_DEAD ? true : false; }
	void			Set_ActiveNone() { m_bDead = true; }
private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pColliderCom;

	StoneState  m_eState;
	_float		m_fTime;

	_bool		m_bPosSet;
	_bool		m_bRotation;

	_bool		m_bDead;
	_int		m_iNum;
public:
	static CRhinoStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CRhino* m_pRhino;
	CPlayer* m_pPlayer;

private:
	virtual void Free();

	IDirect3DTexture9* m_pStoneTex;

};

