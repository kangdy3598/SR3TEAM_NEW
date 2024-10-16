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

class CNoteStatue :public Engine::CGameObject
{
private:
	explicit CNoteStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNoteStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);
	virtual void SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }

public:
	void Play_Note();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

public:
	static CNoteStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);	
	void Set_NoteID(_int _iID) { m_iNoteID = _iID; }
	void Set_Group(CGameObject* _pObj) { m_pGroup = _pObj; }	;

private:
	_int m_iNoteID;
	_bool m_bIsActivate;
	_float m_fActiveTime;
	CGameObject* m_pGroup;

protected:
	CPlayer* m_CPlayer;

private:
	virtual void Free();
};
