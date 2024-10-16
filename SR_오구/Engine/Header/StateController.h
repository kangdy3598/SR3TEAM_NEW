#pragma once

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
class CState;
class ENGINE_DLL CStateController :public CComponent
{

private:
	explicit CStateController();
	explicit CStateController(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStateController(const CStateController& rhs);
	virtual ~CStateController();


public:
	HRESULT					Ready_State();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

	CState* GetState() { return m_pState; }
	void ChangeState(CState* _nextState, CGameObject* _ob);

public:

private:
	CState* m_pState;

public:
	static CStateController* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();


};

END