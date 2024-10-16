#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerDance : public CState
{
public:

	static PlayerDance* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerDance;

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

	void SetDanceType(int _iType) { m_iDanceType = _iType; }

private:
	static PlayerDance* m_instance;
	int m_iDanceType = 0;

	float m_fTickTime = 0.f;
	int m_iPlayerMaxHP = 0;
};