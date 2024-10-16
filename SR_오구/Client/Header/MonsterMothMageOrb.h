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

class CMonsterMothMageOrb : public CMonster
{
private:
	explicit CMonsterMothMageOrb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterMothMageOrb();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();

	virtual HRESULT	Add_Component();

public:
	void  SetMothMage(CMonster* _monster) { m_pMothMage = _monster; }
	void  SetTextureCom(CTexture* _textureCom)
	{ 
		m_pTextureCom = _textureCom;
		m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", m_pTextureCom });
	}
private:
	CMonster* m_pMothMage = nullptr;
private:

private:

public:
	static CMonsterMothMageOrb* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

