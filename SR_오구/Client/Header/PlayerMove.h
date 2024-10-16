#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerMove : public CState
{
public:
    static PlayerMove* GetInstance()
    {
        if (!m_instance)
            m_instance = new PlayerMove;

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
    static PlayerMove* m_instance;
    float m_fMoveSpeed;
    CGameObject* m_interactionObj = nullptr;
    float m_fDuration = 0;
private:
    void Key_Input(const _float& fTimeDelta);
};