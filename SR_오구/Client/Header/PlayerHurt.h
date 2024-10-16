#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerHurt : public CState
{
public:

	static PlayerHurt* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerHurt;

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
	static PlayerHurt* m_instance;
	void KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir);
private:
	float m_fHurtDuration = 0.f;
	_bool m_bKnockBackTrigger = false;
	_vec3 m_vKnockBackDir;
};