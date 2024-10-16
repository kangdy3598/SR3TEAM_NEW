#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "StateController.h"
#include "Transform.h"
#include "Animation.h"

BEGIN(Engine)
class CGameObject;
class CAnimation;

class ENGINE_DLL CState : public CBase
{
public:
	CState() {}
	virtual ~CState() {}

public:
	CStateController* m_pStateController = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CAnimation* m_pAnimationCom = nullptr;
	CGameObject* m_CGameObject = nullptr;

public:

public:
	virtual void Enter() = 0;
	virtual void Update(const _float& fTimeDelta) = 0;
	virtual void Exit() = 0;

	virtual void SetComponent()
	{
		m_pStateController = dynamic_cast<CStateController*>(
			m_CGameObject->Get_Component(ID_DYNAMIC, L"Com_State"));

		m_pTransformCom = dynamic_cast<CTransform*>(
			m_CGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));

		m_pAnimationCom = (CAnimation*)
			m_CGameObject->Get_Component(ID_DYNAMIC, L"Com_Animation");

	}
};

END