#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

//0913
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMonster;
class CMonsterHPUI : public Engine::CGameObject
{
private:
	explicit CMonsterHPUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterHPUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void SetMonster(CMonster* _pMonster) 
	{ 
		m_pMonster = _pMonster;
	}
private:
	HRESULT    Add_Component();
	void UpdateHPGauge();

private:
	enum DEFAULT_UI { HPFRAME, HPBAR, HPBACK, UI_END };

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[UI_END];
	Engine::CTransform* m_pTransformCom[UI_END];

	// HP
	_bool m_bRenderHP;
	float m_HPBarGaugeScaleX;

private:
	CPlayer* m_pPlayer;
	CMonster* m_pMonster;

public:
	static CMonsterHPUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

