#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerLiftMove : public CState
{
public:

	static PlayerLiftMove* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerLiftMove;

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
	static PlayerLiftMove* m_instance;
	float m_fMoveSpeed;

	CGameObject* colObj;

private:
	void Key_Input(const _float& fTimeDelta);

public:
	void SetColObj(CGameObject* _obj) { colObj = _obj; }
};