#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CAnimator2;	
class CAnimation2;
class CTransform;

END

class CTestObject : public CGameObject
{
private:
	explicit CTestObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestObject();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();

private:
	HRESULT  Add_Component();
	void     Key_Input(const _float& fTimeDelta);

private:
	Engine::CAnimator2* m_pAnimatorCom;	
	Engine::CTransform* m_pTransformCom;
	

public:
	static CTestObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

	IDirect3DTexture9* m_pTex;

};

