#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

//class CRcTex;
//class CTexture;
class CAnimator2;
class CAnimation2;
class CTransform;

END
class CAddHP :public CGameObject
{
private:
	explicit CAddHP(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAddHP();

public:
	//_float	  Get_BuffTime() { return m_fBuffTime; }
	//void	  Update_SettingTime(const _float& _fTimeDelta);

	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	Call_AddHP() { m_bCall = true; };
private:
	HRESULT    Add_Component();

public:
	static CAddHP* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	//Engine::CRcTex* m_pBufferCom;
	//Engine::CTexture* m_pTextureCom;
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;

	//_float		m_fBuffTime;
	_bool		m_bCall;
private:
	virtual void Free();
	IDirect3DTexture9* m_pTex;
};

