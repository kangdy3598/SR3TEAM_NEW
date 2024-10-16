#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerBalloonFly : public CState
{
public:

	static PlayerBalloonFly* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerBalloonFly;

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
	static PlayerBalloonFly* m_instance;
	bool m_bTrigger = false;
};