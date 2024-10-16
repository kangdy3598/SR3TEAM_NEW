#pragma once
#include "GameObject.h"

BEGIN(Engine)


class CAnimator2;
class CTransform;

END

class CHitEffect : public Engine::CGameObject
{

private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CHitEffect();	

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


public:
	static CHitEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
	IDirect3DTexture9* m_pTexture;
	bool m_bEffect;
	int m_iCount;


public:
	virtual void OnCollision(CGameObject* _pOther);
	wstring  name;

};

