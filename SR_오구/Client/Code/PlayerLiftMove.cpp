#include "pch.h"
#include "PlayerLiftMove.h"
#include "Player.h"

PlayerLiftMove* PlayerLiftMove::m_instance = nullptr;

void PlayerLiftMove::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
        PLAYERSTATE::PLY_LIFTMOVE);

    m_fMoveSpeed = 50.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);
}

void PlayerLiftMove::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {
        m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
    }

    Key_Input(fTimeDelta);

}


void PlayerLiftMove::Exit()
{
}

void PlayerLiftMove::Key_Input(const _float& fTimeDelta)
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

    _vec3 colObjPos;
    m_pTransformCom->Get_Info(INFO_POS, &colObjPos);
    colObjPos.y += 20;

    dynamic_cast<CTransform*>(
        colObj->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Set_Pos(colObjPos);
}