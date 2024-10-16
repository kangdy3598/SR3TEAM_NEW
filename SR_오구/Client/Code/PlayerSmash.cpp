#include "pch.h"
#include "PlayerSmash.h"
#include "Player.h"

PlayerSmash* PlayerSmash::m_instance = nullptr;

void PlayerSmash::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_SMASH);

    m_pAnimationCom->SetTextureScale(1.5f);
    m_iStateCount = 0;
    m_fDuration = 0.f;
    m_iSmashLevel = 0;

    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);
}

void PlayerSmash::Update(const _float& fTimeDelta)
{
    m_fDuration += fTimeDelta;
    if (m_fDuration > 0.5f)
    {
        m_fDuration = 0.f;
        if (m_iSmashLevel < 3)
        {
            m_iSmashLevel++;
            TCHAR  szFileName[128] = L"";
            wsprintf(szFileName, L"SFX_%d_OguSmashCharge%d.wav",
                224 + m_iSmashLevel, m_iSmashLevel);
            Engine::Play_Sound(szFileName, SOUND_EFFECT, 1.f);
        }
    }
    if (m_iStateCount == 0 && m_pAnimationCom->GetCurrentFrame() >= 4)
    {
        m_iStateCount++;
        m_pAnimationCom->SetAnimationPause(true);
    }
    if (Engine::GetKeyUp(CONTROLKEY::PLY_SMASHKEY))
    {
        if (m_iStateCount == 0)
            m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
        else
        {
            m_pAnimationCom->SetAnimationPause(false);
            dynamic_cast<CDynamicCamera*>(
                dynamic_cast<CPlayer*>(m_CGameObject)->GetCamera()
                )->ShakeMove(0.3f);

            TCHAR  szFileName[128] = L"";
            wsprintf(szFileName, L"SFX_%d_OguSmashEffect%d.wav",
                221 + m_iSmashLevel, m_iSmashLevel);
            Engine::Play_Sound(szFileName, SOUND_EFFECT, 1.f);
        }
    }

    if (dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

    }

    if (m_pAnimationCom->GetCurrentFrame() == 5)
    {
        dynamic_cast<CPlayer*>(m_CGameObject)->m_bSmashEnd = true;
        //여기가 박치기가 마무리될때 들어오는 코드
        //_vec3 test = dynamic_cast<CPlayer*>(m_CGameObject)->GetPlayerDirVector();

    }

}

void PlayerSmash::Exit()
{
    m_pAnimationCom->SetTextureScale(1.f);
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);
}