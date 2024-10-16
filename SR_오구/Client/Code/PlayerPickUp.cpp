#include "pch.h"
#include "PlayerPickUp.h"
#include "Player.h"

PlayerPickUp* PlayerPickUp::m_instance = nullptr;

void PlayerPickUp::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixPlayerDir(OBJDIR_FRONT);
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_PICKUP);
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);

    Engine::Play_Sound(L"SFX_147_OguItemAdd.wav", SOUND_EFFECT, 0.7);
}

void PlayerPickUp::Update(const _float& fTimeDelta)
{
    if (m_pAnimationCom->IsAnimationEnd())
    {
        m_pAnimationCom->SetAnimationPause(true);

    }

    if (Engine::GetKeyDown(CONTROLKEY::PLY_PICKUPKEY))
    {
        Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_NONE);
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }
}


void PlayerPickUp::Exit()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);
    m_pAnimationCom->SetAnimationPause(false);
}

