#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBossHPBar : public Engine::CGameObject
{
private:
	explicit CBossHPBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossHPBar();

public:
	struct BOSS_INFO
	{
		const _tchar* pBossName;
		_int		  iMaxHP;
		_int		  iCurHP;
	};

	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  Init_BossInfo(BOSS_INFO* _pInfo) { m_pInfo = _pInfo; } 
	// 실시간으로 hp반영 해야하니까 포인터로

private:
	HRESULT    Add_Component();

private:
	enum BOSSHP_UI { HPFRAME, HPBAR, HPBACK, HP_END };

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[HP_END];
	Engine::CTransform* m_pTransformCom[HP_END];


	BOSS_INFO*	m_pInfo;

public:
	static CBossHPBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

