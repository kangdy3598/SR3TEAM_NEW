#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "Export_Utility.h"
#include "Monster.h"
#include "MonsterMothMageOrb.h"
#include "MonsterMothMageBullet.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CMonsterMothMage : public CMonster
{
private:
	explicit CMonsterMothMage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterMothMage();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();

	virtual HRESULT	Add_Component();
	virtual	void	OnCollision(CGameObject* _pOther);

	int testNum = 0;
public:

private:
	_int m_iImageID;
	CMonsterHPUI* m_HPBar;
	IDirect3DTexture9* m_pTexture;

	CMonsterMothMageOrb* m_pMothMageOrb = nullptr;
	int num = 0;
	_bool m_bIsAttack = false;

	int bulletCount = 0;
	_bool m_bIsShot = false;
private:
	void  SetIdleAnimation();
	void  SetAttackAnimation();

	_bool CheckPlayerDistance();
	void  AttackPlayer();

private:
	list<CMonsterMothMageBullet*> m_listBullet;
public:
	static CMonsterMothMage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

