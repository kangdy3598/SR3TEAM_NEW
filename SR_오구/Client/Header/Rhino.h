#pragma once
#include "ArenaMonster.h"
#include "BossHPBar.h"
#include "RhinoStone.h"
#include "Export_System.h"

class CRhino : public CArenaMonster
{
private:
	explicit CRhino(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRhino();

public:

	enum RhinoState {
		RHINO_APPEAR,
		RHINO_IDLE,
		RHINO_STOMPING,
		RHINO_RUNNING,
		RHINO_JUMP,
		RHINO_SKY,
		RHINO_LAND,
		RHINO_STUN
	};

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollision(CGameObject* _pOther);


	void			Stomping_Ani();
	void			Running_Ani();
	void			Stun_Ani();

	void			RhinoState_Update(_float fTimeDelta);

	_bool			GetActivation() { return m_activation; }
	void			Set_Time() { m_fTime = 0.f; }
	void			Set_Stun() { m_eState = RHINO_STUN; }
	void			Set_CurHP(_int _iSetHP) { m_tInfo.iCurHP += _iSetHP; }
private:
	HRESULT			Add_Component();

private:
	_int		m_iStoneCnt;

	_vec3		m_vLookDir;

	_float		m_fSpeed;

	_float		m_fTime;

	RhinoState  m_ePrevState;
	RhinoState  m_eState;

	_float		m_fNextEffectTime;
	_float		m_iEffectCount;

	_bool		m_bStoneAppear;
	CBossHPBar* m_pBossHPBar;
	CBossHPBar::BOSS_INFO m_tInfo;

	CRhinoStone* m_pRhinoStone[10];
	_bool m_activation;
public:
	static CRhino* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	IDirect3DTexture9* m_pAppearTex;
	IDirect3DTexture9* m_pRunTex;
	IDirect3DTexture9* m_pStunTex;
};

