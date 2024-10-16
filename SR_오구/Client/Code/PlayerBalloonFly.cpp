#include "pch.h"
#include "PlayerBalloonFly.h"
#include "Player.h"

PlayerBalloonFly* PlayerBalloonFly::m_instance = nullptr;

void PlayerBalloonFly::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixPlayerDir(OBJDIR_FRONT);
    if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerState() !=
        PLAYERSTATE::PLY_BALLOONFLYUP)
    {
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_BALLOONFLYUP);
    }
    else
    {
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_BALLOONFLYDOWN);
        _vec3 playerPos;
        m_pTransformCom->Get_Info(INFO_POS, &playerPos);
        playerPos.y = 100;
        m_pTransformCom->Set_Pos(playerPos);
    }
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);
    m_pAnimationCom->SetTextureScale(1.5f);
    m_pAnimationCom->SetTexturePos(-0.15f, 0.15f);
}

void PlayerBalloonFly::Update(const _float& fTimeDelta)
{
    _vec3 up(0.f, 1.f, 0.f);
    _vec3 down(0.f, -1.f, 0.f);

    if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerState() ==
        PLAYERSTATE::PLY_BALLOONFLYUP)
    {
        if (m_pAnimationCom->GetCurrentFrame() >= 13 ||
            m_pAnimationCom->IsAnimationEnd())
        {
            m_pTransformCom->Move_Pos(&up, fTimeDelta, 30);
        }

        if (m_pAnimationCom->IsAnimationEnd())
        {
            m_pAnimationCom->SetAnimationPause(true);
            _vec3 playerPos;
            m_pTransformCom->Get_Info(INFO_POS, &playerPos);
            if (playerPos.y >= 100)
                (dynamic_cast<CPlayer*>(m_CGameObject))->SetNextSceneOn();
        }
    }

    else
    {
        _vec3 playerPos;
        m_pTransformCom->Get_Info(INFO_POS, &playerPos);
        if (playerPos.y <= 30)
        {
            m_pAnimationCom->SetAnimationPause(false);
            if (m_pAnimationCom->GetCurrentFrame() > 1 &&
                m_pAnimationCom->IsAnimationEnd())
            {
                m_pAnimationCom->SetTextureScale(1.f);
                m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
            }
        }

        else
        {
            m_pTransformCom->Move_Pos(&down, fTimeDelta, 30);
        }
    }

}


void PlayerBalloonFly::Exit()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);

}

