#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END


class CPlayerInteractionBox : public Engine::CGameObject
{
private:
	explicit CPlayerInteractionBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerInteractionBox();

public:

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	void			SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }
	CPlayer*		GetPlayer() { return m_CPlayer; }
public:

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

	CPlayer* m_CPlayer;

public:
	static CPlayerInteractionBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

private:
	virtual void Free();

};

