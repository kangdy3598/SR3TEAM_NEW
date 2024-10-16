#include "pch.h"
#include "PlayerLift.h"
#include "Player.h"

PlayerLift* PlayerLift::m_instance = nullptr;

void PlayerLift::Enter()
{
    if (!m_pStateController)
        SetComponent();

    timeElapsed = 0.f;
    dynamic_cast<CPlayer*>(m_CGameObject)->SetPassAble(true);
    switch (m_iStateCount)
    {
    case 0: // 오브젝트를 들 때
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTSTART);

        colObj = dynamic_cast<CPlayer*>(m_CGameObject)->GetInteractingObj();
        colObjTransform = dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        colObjTransform->Get_Info(INFO_POS, &vColPos);

        (dynamic_cast<CPlayer*>(m_CGameObject))->SetLiftObj(colObj);
        break;
    case 1: // 오브젝트를 들고 있을 때
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTIDLE);
        break;
    case 2: // 오브젝트를 내릴 때
        m_pAnimationCom->SetAnimationPlaying();
        m_pTransformCom->Get_Info(INFO_POS, &vDownPos);

        _vec3  vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        if (vPlayerDir.z > 0)
            vDownPos.z += 30;
        else if (vPlayerDir.z < 0)
            vDownPos.z -= 30;
        else
        {
            if (vPlayerDir.x > 0)
                vDownPos.x += 30;
            else if (vPlayerDir.x < 0)
                vDownPos.x -= 30;
        }

        vDownPos.y -= 10;
        // colObj = dynamic_cast<CPlayer*>(m_CGameObject)->GetInteractingObj();
        colObjTransform = dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTEND);

        (dynamic_cast<CPlayer*>(m_CGameObject))->SetLiftObj(nullptr);
        break;
    }


}

void PlayerLift::Update(const _float& fTimeDelta)
{
    _vec3 vPlayerHeadPos;
    _vec3 currentPosition;

    switch (m_iStateCount)
    {
    case 0:
        if (m_pAnimationCom->IsAnimationEnd())
        {
            // 무조건 ChangeState보다 먼저 할 것
            // 그래야 ChangeState()안에서 PlayerLift 다음 행동으로 진입함
            m_iStateCount++;
            m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
            return;
        }

        m_pTransformCom->Get_Info(INFO_POS, &vPlayerHeadPos);
        vPlayerHeadPos.y += 20;

        currentPosition = vColPos;
        MoveAlongBezierCurve(fTimeDelta, currentPosition, vColPos, vPlayerHeadPos);

        colObjTransform->Set_Pos(currentPosition);

        break;
    case 1:
        if (Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY) ||
            Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY) ||
            Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY) ||
            Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
        {
            PlayerLiftMove::GetInstance()->SetColObj(colObj);
            m_pStateController->ChangeState(PlayerLiftMove::GetInstance(), m_CGameObject);

        }

        if (Engine::GetKeyDown(CONTROLKEY::PLY_LIFTKEY))
        {
            m_iStateCount++;
            m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);

        }


        break;
    case 2:
        if (m_pAnimationCom->IsAnimationEnd())
        {
            m_iStateCount = 0;
            m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

        }


        m_pTransformCom->Get_Info(INFO_POS, &vPlayerHeadPos);
        vPlayerHeadPos.y += 20;

        currentPosition = vPlayerHeadPos;
        MoveAlongBezierCurve(fTimeDelta, currentPosition, vPlayerHeadPos, vDownPos, false);

        colObjTransform->Set_Pos(currentPosition);

        break;
    }

}

void PlayerLift::Exit()
{
}

void PlayerLift::MoveAlongBezierCurve(float fTimeDelta, _vec3& currentPosition,
    const _vec3& A, const _vec3& B, _bool isLift)
{
    const float duration = 1.0f;

    timeElapsed += fTimeDelta;
    float t = timeElapsed / duration;
    if (t > 1.0f)
    {
        t = 1.0f;
    }


    _vec3 controlPoint1;

    if (isLift)
    {
        controlPoint1 = _vec3(A.x, B.y, (A.z + B.z) / 2.0f);
    }
    else
    {
        controlPoint1 = _vec3(B.x, A.y, (A.z + B.z) / 2.0f);
    }
    currentPosition = QuadraticBezier(A, controlPoint1, B, t);
}
