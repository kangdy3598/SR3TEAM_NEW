#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;	
class CTransform;	
class CCollider;	
class CRcTex;

class CElectriceelBullet : public Engine::CGameObject
{
private:
	explicit CElectriceelBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CElectriceelBullet();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();


private:
	HRESULT  Add_Component();
	ULONGLONG dwtime;

private:

	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;


public:
	static CElectriceelBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
	IDirect3DTexture9* m_pTexture;
	bool m_bEffect;
	int m_iCount;

	_vec3 m_vDir;

	ULONGLONG m_dwtime;

public:
	virtual void OnCollision(CGameObject* _pOther);
	wstring  name;

	void SetBulletDir(_vec3 _vDir) { m_vDir = _vDir; }
	
	


};

END