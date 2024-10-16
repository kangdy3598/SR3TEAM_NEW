#include "pch.h"
#include "PlayerRolling.h"
#include "Player.h"

PlayerRolling* PlayerRolling::m_instance = nullptr;

void PlayerRolling::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_ROLLING);



    m_fMoveSpeed = 150.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetInvincible();
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);
}

void PlayerRolling::Update(const _float& fTimeDelta)
{
    if (dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

    Key_Input(fTimeDelta);
}

void PlayerRolling::Exit()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);
}

void PlayerRolling::Key_Input(const _float& fTimeDelta)
{
    

    _vec3  vLook;
    _vec3  vRight;
    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

    _vec3 colPos = (dynamic_cast<CPlayer*>(m_CGameObject))->GetColliderPos();
    _vec3 colPlayerPos = (dynamic_cast<CPlayer*>(m_CGameObject))->GetColPlayerPos();
    _vec3 colDir = colPos - colPlayerPos; // 플레이어에서 충돌체로 가는 벡터
    D3DXVec3Normalize(&colDir, &colDir);  // 충돌 했을 때 충돌체 방향

    if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() ||
        (abs(colDir.x) > 0.71 || colDir.z < 0.69))
    {
        _vec3  vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vPlayerDir, &vPlayerDir), fTimeDelta, m_fMoveSpeed);
    }
}