#include "pch.h"
#include "PlayerMove.h"
#include "Player.h"

PlayerMove* PlayerMove::m_instance = nullptr;

void PlayerMove::Enter()
{
    if (!m_pStateController)
        SetComponent();

    dynamic_cast<CPlayer*>(m_CGameObject)->SetPlayerState(PLAYERSTATE::PLY_MOVE);

    m_fMoveSpeed = 50.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);
    float fItemMoveSpeed = (dynamic_cast<CPlayer*>(m_CGameObject))->GetItemMoveSpeed();
    m_fMoveSpeed += fItemMoveSpeed;

    m_fDuration = 0.7f;

}

void PlayerMove::Update(const _float& fTimeDelta)
{
    m_fDuration += fTimeDelta;
    if (m_fDuration > 1.f)
    {
        m_fDuration = 0.f;
        Engine::Play_Sound(L"SFX_4_Walk_Ground_1.wav", SOUND_EFFECT, 0.3);
    }

    if (!Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_DASHKEY))
        m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(CONTROLKEY::PLY_SWINGKEY))
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(CONTROLKEY::PLY_SMASHKEY))
        m_pStateController->ChangeState(PlayerSmash::GetInstance(), m_CGameObject);

    if (Engine::GetKeyDown(CONTROLKEY::PLY_ROLLKEY))
        m_pStateController->ChangeState(PlayerRolling::GetInstance(), m_CGameObject);

    m_interactionObj = (dynamic_cast<CPlayer*>(m_CGameObject))->GetInteractingObj();
    if (m_interactionObj && m_interactionObj->IncludingType(OBJ_TYPE::PUSH_ABLE))
        m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);

    Key_Input(fTimeDelta);
}

void PlayerMove::Exit()
{

}

void PlayerMove::Key_Input(const _float& fTimeDelta)
{
    _vec3  vLook;
    _vec3  vRight;
    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

    _vec3 colPos = (dynamic_cast<CPlayer*>(m_CGameObject))->GetColliderPos();
    _vec3 colPlayerPos = (dynamic_cast<CPlayer*>(m_CGameObject))->GetColPlayerPos();
    _vec3 colDir = colPos - colPlayerPos; // 플레이어에서 충돌체로 가는 벡터
    D3DXVec3Normalize(&colDir, &colDir);  // 충돌 했을 때 충돌체 방향

    if (Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() ||
            (abs(colDir.x) > 0.71 || colDir.z < 0.69))
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vLook, &vLook), fTimeDelta, m_fMoveSpeed);
        }
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() ||
            (abs(colDir.x) > 0.71 || colDir.z > -0.69))
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
        }
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() ||
            (abs(colDir.z) > 0.71 || colDir.x > -0.69))
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
        }

    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() ||
            (abs(colDir.z) > 0.71 || colDir.x < 0.69))
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vRight, &vRight), fTimeDelta, m_fMoveSpeed);
        }
    }
}