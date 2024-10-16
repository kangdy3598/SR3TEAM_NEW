#pragma once
#include "State.h"
#include "GameObject.h"
#include "StateController.h"

BEGIN(Engine)
//class CGameObject;
//class CStateController;
class ENGINE_DLL CStateManager : public CState
{
public:
	CStateManager(){}
	virtual ~CStateManager(){}

public:
	CGameObject* m_CGameObject = nullptr;
	CStateController* m_CStateController = nullptr;

public:
	void SetGameObject(CGameObject* _ob) { m_CGameObject = _ob; }
	void SetController()
	{
		m_CStateController = dynamic_cast<CStateController*>(
			m_CGameObject->Get_Component(ID_DYNAMIC, L"Com_State"));
	}

public:
	virtual void Enter() {}
	virtual void Update(float _fTickTime) {}
	virtual void Exit() {}
	virtual void Free() {}
};
END
