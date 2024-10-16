#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerPickUp : public CState
{
public:

	static PlayerPickUp* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerPickUp;

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
	static PlayerPickUp* m_instance;
};