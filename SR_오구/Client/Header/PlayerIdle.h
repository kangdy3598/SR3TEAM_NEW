#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerIdle : public CState
{
public:

	static PlayerIdle* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerIdle;

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
	static PlayerIdle* m_instance;
	float m_fIdleDuration = 0.f;
	float m_fDanceKeyDownTimer = 0.f;
	CGameObject* m_interactionObj = nullptr;
};