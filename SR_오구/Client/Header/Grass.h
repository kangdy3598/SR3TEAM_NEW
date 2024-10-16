#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CAnimator2;
class CTransform;
class CCollider;		

END		

class CGrass : public CGameObject
{
private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CGrass();	

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
	Engine::CAnimator2* m_pAnimatorCom2;	
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;


public:
	static CGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);

private:
	virtual void Free();

	IDirect3DTexture9* m_pTex;

	bool   m_bDie;
	bool m_bonce;
};
