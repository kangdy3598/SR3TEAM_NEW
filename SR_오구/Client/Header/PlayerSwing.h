#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerSwing : public CState
{
public:
    static PlayerSwing* GetInstance()
    {
        if (!m_instance)
            m_instance = new PlayerSwing;

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
    static PlayerSwing* m_instance;

private:
};