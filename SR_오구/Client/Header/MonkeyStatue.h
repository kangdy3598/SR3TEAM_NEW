#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CMonkeyStatue :public Engine::CGameObject
{
private:
	explicit CMonkeyStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonkeyStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual void LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);
	
public:
	void Active_StoneBlock();
	void Add_StoneBlock(CGameObject* pSb) { m_vecStoneBlocks.push_back(pSb); }	

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

public:
	static CMonkeyStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);				

private:
	_bool m_bIs;
	_bool m_bIsActivate;
	_float m_fActiveTime;	
	vector<CGameObject*> m_vecStoneBlocks;	

protected:
	CPlayer* m_CPlayer;

private:
	virtual void Free();
};
