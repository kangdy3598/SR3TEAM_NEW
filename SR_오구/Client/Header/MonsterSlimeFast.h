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

class CMonsterSlimeFast : public CMonster
{
private:
	explicit CMonsterSlimeFast(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterSlimeFast();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();

	virtual HRESULT	Add_Component();
	virtual	void	OnCollision(CGameObject* _pOther);
	
public:
	
private:
	_int m_iImageID;
	CMonsterHPUI* m_HPBar;
	IDirect3DTexture9* m_pTexture;

	int num = 0;
private:
	
private:

public:
	static CMonsterSlimeFast* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

