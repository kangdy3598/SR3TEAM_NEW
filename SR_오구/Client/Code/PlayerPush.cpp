#include "pch.h"
#include "PlayerPush.h"
#include "Player.h"

PlayerPush* PlayerPush::m_instance = nullptr;

void PlayerPush::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_PUSH);

    m_fMoveSpeed = 50.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);

    colObj = (dynamic_cast<CPlayer*>(m_CGameObject))->GetInteractingObj();
}

void PlayerPush::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY) &&
        !Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }


    if (!(dynamic_cast<CPlayer*>(m_CGameObject))->GetInteractingObj())
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

    // Swing
    if (Engine::GetKeyPress(CONTROLKEY::PLY_SWINGKEY))
    {
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);
    }

    Key_Input(fTimeDelta);
}


void PlayerPush::Exit()
{
}

void PlayerPush::Key_Input(const _float& fTimeDelta)
{
    _vec3  vLook;
    _vec3  vRight;
    _vec3  vPlayerDir;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    //vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector();


    if (Engine::GetKeyPress(::CONTROLKEY::PLY_UPKEY))
    {

        m_pTransformCom->Move_Pos(
            D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);

        if (colObj == nullptr)
            return;

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Move_Pos(
                D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);

        return;

    }


    if (Engine::GetKeyPress(::CONTROLKEY::PLY_DOWNKEY))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);

        if (colObj == nullptr)
            return;

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Move_Pos(
                D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
        return;
    }


    if (Engine::GetKeyPress(::CONTROLKEY::PLY_LEFTKEY))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);

        if (colObj == nullptr)
            return;

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Move_Pos(
                D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, -m_fMoveSpeed);

        return;
    }


    if (Engine::GetKeyPress(::CONTROLKEY::PLY_RIGHTKEY))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, m_fMoveSpeed);

        if (colObj == nullptr)
            return;

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Move_Pos(
                D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, m_fMoveSpeed);

        return;
    }
}