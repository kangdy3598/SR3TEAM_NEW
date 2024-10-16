#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerSmash : public CState
{
public:
    static PlayerSmash* GetInstance()
    {
        if (!m_instance)
            m_instance = new PlayerSmash;

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

    void SetStateCount(int _iType) { m_iStateCount = _iType; }
private:
    static PlayerSmash* m_instance;
    float m_fDuration = 0;
    int m_iSmashLevel = 0;

private:
    int m_iStateCount = 0;
};