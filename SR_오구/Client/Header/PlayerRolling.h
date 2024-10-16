#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerRolling : public CState
{
public:
    static PlayerRolling* GetInstance()
    {
        if (!m_instance)
            m_instance = new PlayerRolling;

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
    static PlayerRolling* m_instance;
    float m_fMoveSpeed;

private:
    void Key_Input(const _float& fTimeDelta);
};