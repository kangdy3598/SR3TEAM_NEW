#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CAnimator2;
class CAnimation2;
class CTransform;
class CRcTex;

class CRhinoEffect : public Engine::CGameObject
{

private:
	explicit CRhinoEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRhinoEffect();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();


private:
	HRESULT  Add_Component();
	ULONGLONG dwtime;

private:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

public:
	static CRhinoEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	bool m_bEffect;
	IDirect3DTexture9* m_pTex;

public:
	wstring  name;

};

END