#pragma once
#include "GameObject.h"	

BEGIN(Engine)

class CTexture;
class CAnimator2;
class CTransform;
class CCollider;
class CRcTex;

class CWaterFall : public Engine::CGameObject
{
private:
	explicit CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterFall();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();
	virtual  void     LateReady_GameObject();	

private:
	HRESULT  Add_Component();
	HRESULT  SetUp_Material();
	void     Key_Input(const _float& fTimeDelta);

private:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;	
	Engine::CRcTex* m_pBufferCom;	
	Engine::CTexture* m_pTextureCom;	


	IDirect3DTexture9* m_pTexture;	
	bool m_bEffect;	
	int m_iCount;	

	_vec3 m_vDir;	

	ULONGLONG m_dwtime;
	

public:
	static CWaterFall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	wstring  name;

	void SetWaterFallDir(_vec3 _vDir) { m_vDir = _vDir; }

private:
	virtual void Free();
};
END
