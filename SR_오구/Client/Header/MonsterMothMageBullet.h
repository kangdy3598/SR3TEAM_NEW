#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "Export_Utility.h"
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CMonsterMothMageBullet : public CMonster
{
private:
	explicit CMonsterMothMageBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterMothMageBullet();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();

	virtual HRESULT	Add_Component();
	virtual	void	OnCollision(CGameObject* _pOther);

public:

	void SetShotDir(_vec3 _dir) { m_vShotDir = _dir; }
	void  SetTextureCom(CTexture* _textureCom)
	{
		m_pTextureCom = _textureCom;
		m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", m_pTextureCom });
	}

private:
	IDirect3DTexture9* m_pTexture;
	_vec3 m_vShotDir;

private:
	list<CMonsterMothMageBullet*> m_listBullet;

private:

public:
	static CMonsterMothMageBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

