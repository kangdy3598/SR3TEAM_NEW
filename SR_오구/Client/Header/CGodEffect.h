#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include "God.h"
BEGIN(Engine)

//class CRcTex;
//class CTexture;
class CAnimator2;
class CAnimation2;
class CTransform;

END
class CGodEffect :public CGameObject
{
private:
	explicit CGodEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGodEffect();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	Call_GodEffect() { m_bCall = true; };
private:
	HRESULT    Add_Component();

public:
	static CGodEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;

	_bool		m_bCall;
private:
	virtual void Free();

	IDirect3DTexture9* m_pTex;
	CGod*		m_pGod;


};

