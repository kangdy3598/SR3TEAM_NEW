#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerDash : public CState
{
public:

	static PlayerDash* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerDash;

		return m_instance;
	}
	static void	DestroyInstance()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	virtual void Enter();
	virtual void Update(const _float& fTimeDelta);
	virtual void Exit();
	virtual void Free() {}

private:
	static PlayerDash* m_instance;
	float m_fMoveSpeed;
	float m_fMoveDuration;
	float m_fDuration;
	bool m_bZoomOutTrigger;

private:
	void Key_Input(const _float& fTimeDelta);
	void CameraZoomOut(const _float& fTimeDelta);
};