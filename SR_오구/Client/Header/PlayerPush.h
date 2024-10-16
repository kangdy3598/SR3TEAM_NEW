#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerPush : public CState
{
public:
    static PlayerPush* GetInstance()
    {
        if (!m_instance)
            m_instance = new PlayerPush;

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
    static PlayerPush* m_instance;
    float m_fMoveSpeed;
    CGameObject* colObj;

private:
    void Key_Input(const _float& fTimeDelta);
};