#pragma once
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CMusicStatue :public Engine::CGameObject
{
private:
	explicit CMusicStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMusicStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);
	virtual void	SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }

public:	
	void Add_StoneBlock(CGameObject* _pObj) { m_vecStoneBlocks.push_back(_pObj); }	
	void Add_FirePit(CGameObject* _pObj) { m_vecFirePits.push_back(_pObj); }
	void Play_Music();
	void Match_Note(_int _iNote);
	void Reset_FirePit();
	void Clear_Puzzle();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

private:
	_bool m_bIsClear;
	_bool m_bIsActivate;
	_int m_iCurNote;
	_float m_fActiveTime;
	vector<_int> m_vecKeyOrder;
	vector<CGameObject*> m_vecFirePits;
	vector<CGameObject*> m_vecStoneBlocks;

protected:
	CPlayer* m_CPlayer;

public:
	static CMusicStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
};
