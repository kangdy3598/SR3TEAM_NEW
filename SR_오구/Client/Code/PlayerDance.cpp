#include "pch.h"
#include "PlayerDance.h"
#include "Player.h"

PlayerDance* PlayerDance::m_instance = nullptr;

void PlayerDance::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixPlayerDir(OBJDIR_FRONT);

    switch (m_iDanceType)
    {
    case 0: // 가만히 서 있을 때 나오는 동작
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_IDLEDANCE);
        m_pAnimationCom->SetTextureScale(0.8f);
        break;

    case 1: // e키 눌러 체력 회복 하는 동작
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_DANCE);

        m_fTickTime = 0.f;
        m_iPlayerMaxHP = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerHP().iMaxHP;
        break;
    }

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);
}

void PlayerDance::Update(const _float& fTimeDelta)
{
    switch (m_iDanceType)
    {
    case 0:
        if (Engine::GetAnyActionKeyDown())
            m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
        break;

    case 1:
        if (Engine::GetKeyDown(CONTROLKEY::PLY_DANCEKEY))
            m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

        m_fTickTime += fTimeDelta;
        if (m_fTickTime > 3.f)
        {
            m_fTickTime = 0.f;
            int iPlayerCurHP = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerHP().iCurHP;
            if (iPlayerCurHP < m_iPlayerMaxHP / 2)
                (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerCurHP(1);
        }
        break;
    default:
        break;
    }

}


void PlayerDance::Exit()
{
    switch (m_iDanceType)
    {
        // 가만히 서 있을 때 나오는 동작
    case 0:
        m_pAnimationCom->SetTextureScale(1.f);
        break;
    }

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);
}

