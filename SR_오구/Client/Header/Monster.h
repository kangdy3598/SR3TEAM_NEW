#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "MonsterHPUI.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

struct MONSTER_HP
{
	_int	iCurHP;
	_int	iMaxHP;
};

class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();

	virtual	void	OnCollision(CGameObject* _pOther);

public:
	virtual void	SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }
	virtual void	GetLayer(CLayer* _layer) { m_pLayer = _layer; }
	virtual MONSTER_HP	GetMonsterHP() { return m_tMonsterHP; }
	virtual _bool	GetActivation() { return m_activation; }
protected:
	virtual HRESULT	Add_Component();
	virtual void	KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir);

	virtual void	SetInvincible(_bool value = true) { m_bInvincible = value; }
	virtual bool	IsInvincible() { return m_bInvincible; }
	virtual void	DurationInvincible(const _float& fTimeDelta);


	virtual void		SetMonsterCurHP(_int _SetHP)
	{
		m_tMonsterHP.iCurHP += _SetHP;
		if (m_tMonsterHP.iCurHP > m_tMonsterHP.iMaxHP)
			m_tMonsterHP.iCurHP = m_tMonsterHP.iMaxHP;

		if (m_tMonsterHP.iCurHP < 0)
			m_tMonsterHP.iCurHP = 0;
	}
	void MoveToPlayer(const _float& fTimeDelta);

private:
	IDirect3DTexture9* m_Texture;
	_int m_iImageID;


private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}
protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CAnimation* m_pAnimationCom;
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CCollider* m_pBoundBox;
	Engine::CTexture* m_pTextureCom;
	Engine::CLayer* m_pLayer;

	MONSTER_HP m_tMonsterHP;

	CPlayer* m_CPlayer;
	_vec3 m_vToPlayerDir;
	_bool m_bKnockBackTrigger;
	_vec3 m_vKnockBackDir;

	_bool m_bInvincible;
	float m_fMoveSpeed;
	int m_iDirIndex;
	bool m_activation;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

