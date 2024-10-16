#include "pch.h"
#include "PlayerIdle.h"
#include "Player.h"

PlayerIdle* PlayerIdle::m_instance = nullptr;

void PlayerIdle::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_IDLE);

    m_fIdleDuration = 0.f;
    m_fDanceKeyDownTimer = 0.f;

    m_interactionObj = (dynamic_cast<CPlayer*>(m_CGameObject))->GetInteractingObj();

}

void PlayerIdle::Update(const _float& fTimeDelta)
{
    // IdleDance
    m_fIdleDuration += fTimeDelta;
    if (m_fIdleDuration >= 3.f)
    {
        PlayerDance::GetInstance()->SetDanceType(0);
        m_pStateController->ChangeState(PlayerDance::GetInstance(), m_CGameObject);
    }

    // Move, Dash, Push
    if (Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY) ||
        Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY) ||
        Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY) ||
        Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {

        if (m_interactionObj && m_interactionObj->IncludingType(OBJ_TYPE::PUSH_ABLE))
            m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);

        else
        {
            if (Engine::GetKeyPress(CONTROLKEY::PLY_DASHKEY))
                m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);
            else
                m_pStateController->ChangeState(PlayerMove::GetInstance(), m_CGameObject);
        }
    }

    // Swing
    if (Engine::GetKeyPress(CONTROLKEY::PLY_SWINGKEY))
    {
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);
    }


    // Lift
    if (Engine::GetKeyDown(CONTROLKEY::PLY_LIFTKEY))
    {
        if (m_interactionObj && m_interactionObj->IncludingType(OBJ_TYPE::LIFT_ABLE))
            m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
    }

    // Dance
    if (Engine::GetKeyPress(CONTROLKEY::PLY_DANCEKEY))
    {
        m_fDanceKeyDownTimer += fTimeDelta;
        if (m_fDanceKeyDownTimer > 1.f)
        {
            PlayerDance::GetInstance()->SetDanceType(1);
            m_pStateController->ChangeState(PlayerDance::GetInstance(), m_CGameObject);
        }
    }

    // Smash
    if (Engine::GetKeyDown(CONTROLKEY::PLY_SMASHKEY))
    {
        m_pStateController->ChangeState(PlayerSmash::GetInstance(), m_CGameObject);
    }

    // Rolling
    if (Engine::GetKeyDown(CONTROLKEY::PLY_ROLLKEY))
    {
        m_pStateController->ChangeState(PlayerRolling::GetInstance(), m_CGameObject);
    }
}


void PlayerIdle::Exit()
{
    m_fIdleDuration = 0.f;
    m_fDanceKeyDownTimer = 0.f;
}

