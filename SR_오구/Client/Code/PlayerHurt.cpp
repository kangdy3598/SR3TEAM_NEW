#include "pch.h"
#include "PlayerHurt.h"
#include "Player.h"

PlayerHurt* PlayerHurt::m_instance = nullptr;

void PlayerHurt::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->DisableDiagonal();
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_HURT);

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerCurHP(-1);
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetInvincible();

    m_fHurtDuration = 0.f;
    m_vKnockBackDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();
    m_vKnockBackDir *= -1;
    m_bKnockBackTrigger = true;
}

void PlayerHurt::Update(const _float& fTimeDelta)
{
    m_fHurtDuration += fTimeDelta;

    if (m_fHurtDuration >= 0.1f &&
        dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }

    KnockBack(fTimeDelta, m_vKnockBackDir);
}


void PlayerHurt::Exit()
{
    m_fHurtDuration = 0.f;
}

void PlayerHurt::KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir)
{
    if (!m_bKnockBackTrigger)
        return;

    static float speed = 60.f;
    static float tickTime = 0.f;

    if (speed <= 10)
    {
        speed = 60.f;
        tickTime = 0.f;
        m_bKnockBackTrigger = false;
        return;
    }

    tickTime += fTimeDelta;
    if (tickTime >= 0.5f)
    {
        tickTime = 0.f;
        speed *= 0.1f;
    }

    m_pTransformCom->Move_Pos(&m_vKnockBackDir, fTimeDelta, speed);

    CGameObject* interactionBox = dynamic_cast<CPlayer*>(m_CGameObject)->GetInteractionBox();
    dynamic_cast<CTransform*>(interactionBox->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Move_Pos(&m_vKnockBackDir, fTimeDelta, speed);
}