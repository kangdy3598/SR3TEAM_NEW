#include "pch.h"
#include "Rhino.h"
#include "RhinoEffect.h"
#include "VictoryUI.h"

CRhino::CRhino(LPDIRECT3DDEVICE9 pGraphicDev)
    :CArenaMonster(pGraphicDev)
    , m_fTime(0), m_fSpeed(2.f), m_bStoneAppear(false)
    , m_fNextEffectTime(0), m_iEffectCount(0), m_iStoneCnt(6), m_activation(true)
{
    m_tInfo.pBossName = L"거대 사막뿔소";
    m_tInfo.iMaxHP = 600;
    m_tInfo.iCurHP = 600;
}

CRhino::~CRhino()
{
}

HRESULT CRhino::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    SetObjectType(OBJ_TYPE::HURT_ABLE);

    m_eState = RHINO_APPEAR;

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStomp.png", &m_pTex);

    m_pAnimatorCom->CreateAnimation(L"RhinoStompFront", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLB", m_pTex, _vec2(1024.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLeft", m_pTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLT", m_pTex, _vec2(1024.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompBack", m_pTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRT", m_pTex, _vec2(1024.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRight", m_pTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRB", m_pTex, _vec2(1024.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->Play(L"RhinoStompFront", true);
    //m_pRunTex

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStun.png", &m_pStunTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunLB", m_pStunTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunRB", m_pStunTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunLT", m_pStunTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunRT", m_pStunTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoRun.png", &m_pRunTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunFront", m_pRunTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLB", m_pRunTex, _vec2(768.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLeft", m_pRunTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLT", m_pRunTex, _vec2(768.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunBack", m_pRunTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRT", m_pRunTex, _vec2(768.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRight", m_pRunTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRB", m_pRunTex, _vec2(768.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoJump.png", &m_pAppearTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoLand", m_pAppearTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.1f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoJump", m_pAppearTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.1f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoSky", m_pAppearTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 2.0f, 0);

    m_pAnimatorCom->Play(L"RhinoLand", true); // 등장씬~

    return S_OK;
}

void CRhino::LateReady_GameObject()
{
    if (!m_bCreate)
        return;

    CArenaMonster::LateReady_GameObject();

    m_pBossHPBar = dynamic_cast<CBossHPBar*>(CBossHPBar::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pBossHPBar);
    m_pBossHPBar->Init_BossInfo(&m_tInfo);

    for (size_t i = 0; i < m_iStoneCnt; i++)
    {
        _tchar StoneName[32] = L"RhinoStone";
        _tchar StoneNum[32];

        swprintf(StoneNum, 32, L"%d", i + 1);

        wcscat_s(StoneName, 32, StoneNum);

        m_pRhinoStone[i] = dynamic_cast<CRhinoStone*>(Engine::Get_GameObject(L"Layer_GameLogic", StoneName));
        NULL_CHECK_RETURN(m_pRhinoStone[i]);
    }

    for (size_t i = 0; i < m_iStoneCnt; i++)
    {
        m_pRhinoStone[i]->Set_EffectNumber(i);
    }
}

_int CRhino::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCreate)
        return 0;

    if (m_tInfo.iCurHP <= 0)
    {
        m_activation = false;
        m_bCreate = false;
        m_pPlayer->Set_HonorScore(100);
        m_pPlayer->SetPlayerPos(_vec3(500.f, 20.f, 800.f));

        CVictoryUI* pUI = dynamic_cast<CVictoryUI*>(Engine::Get_GameObject(L"Layer_UI", L"Victory_UI"));
        NULL_CHECK_RETURN(pUI, 0);
        pUI->CallVictory();

        for (size_t i = 0; i < m_iStoneCnt; i++)
        {
            m_pRhinoStone[i]->Set_ActiveNone();
        }
        return 0;
    }

    Add_RenderGroup(RENDER_ALPHA, this);

    m_pBossHPBar->Update_GameObject(fTimeDelta);

    RhinoState_Update(fTimeDelta); // 상태 업데이트 + 애니

    //for (size_t i = 0; i < m_iStoneCnt; i++)
    //{
    //    m_pRhinoStone[i]->Update_GameObject(fTimeDelta);
    //}

    _int iExit = CArenaMonster::Update_GameObject(fTimeDelta);

    return iExit;
}

void CRhino::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bCreate)
        return;

    if (m_tInfo.iCurHP <= 0)
    {
        return;
    }
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    //for (size_t i = 0; i < m_iStoneCnt; i++)
    //{
    //    m_pRhinoStone[i]->LateUpdate_GameObject(fTimeDelta);
    //}
}

void CRhino::Render_GameObject()
{
    if (m_tInfo.iCurHP <= 0)
    {
        return;
    }

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    LPDIRECT3DTEXTURE9 pCurrentTex = nullptr;

    switch (m_eState)
    {
    case RHINO_LAND:
    case RHINO_APPEAR:
        if (m_ePrevState == m_eState)
        {
            pCurrentTex = m_pAppearTex;
            break;
        }
        pCurrentTex = m_pAppearTex;
        Engine::Play_Sound(L"SFX_495_RhinoSmash_Land.wav", SOUND_RHINO, 1.f);
        dynamic_cast<CDynamicCamera*>(m_pPlayer->GetCamera()
            )->ShakeMove(0.3f, 0.2f, 25.f);
        break;
    case RHINO_JUMP:
        if (m_ePrevState == m_eState)
        {
            pCurrentTex = m_pAppearTex;
            break;
        }
        pCurrentTex = m_pAppearTex;
        Engine::Play_Sound(L"SFX_494_RhinoSmash_Jump.wav", SOUND_RHINO, 1.f);
        break;
    case RHINO_SKY:
        pCurrentTex = m_pAppearTex;
        break;
    case RHINO_STOMPING:
        if (m_ePrevState == m_eState)
        {
            pCurrentTex = m_pTex;
            break;
        }
        pCurrentTex = m_pTex;
        break;
    case RHINO_RUNNING:
        if (m_ePrevState == m_eState)
        {
            pCurrentTex = m_pRunTex;
            break;
        }
        pCurrentTex = m_pRunTex;
        Engine::Play_Sound(L"SFX_254_Rhino_Rush.wav", SOUND_RHINO, 1.f);
        break;
    case RHINO_STUN:
        if (m_ePrevState == m_eState)
        {
            pCurrentTex = m_pStunTex;
            break;
        }
        pCurrentTex = m_pStunTex;
        Engine::Play_Sound(L"SFX_255_Rhino_Crush.wav", SOUND_RHINO, 1.f);
        break;
    }

    if (pCurrentTex)
        m_pGraphicDev->SetTexture(0, pCurrentTex);

    m_pAnimatorCom->render();
    //m_pColliderCom->Render_Buffer();

    m_ePrevState = m_eState;

}

void CRhino::OnCollision(CGameObject* _pOther)
{
    if (!m_bCreate)
        return;

    if (_pOther->GetObjectKey() == L"Player")
    {
        m_pPlayer->SetPlayerCurHP(-1);
        m_pPlayer->IsInvincible();
    }

    return;
}

void CRhino::Stomping_Ani()
{
    //몬스터가 플레이어를 바라보는 방향 벡터가 있어야 함
    _vec3 vPlayerPos = m_pPlayer->GetPlayerPos();
    _vec3 vRhinoPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_vLookDir = vPlayerPos - vRhinoPos;
    D3DXVec3Normalize(&m_vLookDir, &m_vLookDir); // 정규화

    float fMarginOfError = 0.1f;

    if (fabs(m_vLookDir.x) < fMarginOfError && m_vLookDir.z > 0)
    {
        // Back
        m_pAnimatorCom->Play(L"RhinoStompBack", true);
    }
    else if (fabs(m_vLookDir.x) < fMarginOfError && m_vLookDir.z < 0)
    {
        // Front
        m_pAnimatorCom->Play(L"RhinoStompFront", true);
    }
    else if (m_vLookDir.x > 0 && fabs(m_vLookDir.z) < fMarginOfError)
    {
        // Right
        m_pAnimatorCom->Play(L"RhinoStompRight", true);
    }
    else if (m_vLookDir.x < 0 && fabs(m_vLookDir.z) < fMarginOfError)
    {
        // Left
        m_pAnimatorCom->Play(L"RhinoStompLeft", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        // RT
        m_pAnimatorCom->Play(L"RhinoStompRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        // LT
        m_pAnimatorCom->Play(L"RhinoStompLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        // RB
        m_pAnimatorCom->Play(L"RhinoStompRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        // LB
        m_pAnimatorCom->Play(L"RhinoStompLB", true);
    }
}

void CRhino::RhinoState_Update(_float fTimeDelta)
{
    map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();

    m_fTime += fTimeDelta;

    switch (m_eState)
    {
    case RHINO_APPEAR: // 1초 등장
        m_pAnimatorCom->Play(L"RhinoLand", false); // 착지

        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_eState = RHINO_STOMPING; // 발구르기
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);

            //이펙1
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Rhino_Effect" + to_wstring(30);

            CRhinoEffect* pRhinoEffect = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pRhinoEffect);
            pRhinoEffect->name = Effect_Name->c_str();

            _vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x += 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name1 = new wstring;
            *Effect_Name1 = L"Rhino_Effect" + to_wstring(31);

            CRhinoEffect* pRhinoEffect1 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name1).c_str(), pRhinoEffect1);
            pRhinoEffect1->name = Effect_Name1->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x -= 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect1->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name2 = new wstring;
            *Effect_Name2 = L"Rhino_Effect" + to_wstring(32);

            CRhinoEffect* pRhinoEffect2 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name2).c_str(), pRhinoEffect2);
            pRhinoEffect2->name = Effect_Name2->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect2->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);
        }
        break;

    case RHINO_STOMPING: // 3초 발구르기
        m_pTransformCom->m_vInfo[INFO_POS].y = 50.f;
        Stomping_Ani();

        // 0.5초마다 발동하기 위한 변수
        //if (m_fTime >= m_fNextEffectTime && m_iEffectCount < 6)
        if (m_fTime >= m_fNextEffectTime && m_iEffectCount < 12)
        {
            Engine::Play_Sound(L"SFX_253_Rhino_RushReady.wav", SOUND_RHINO, 1.f); // 발구르는 소리

            // 이펙트를 발생시키는 부분
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Rhino_Effect" + to_wstring(35 + m_iEffectCount);

            CRhinoEffect* pRhinoEffect = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pRhinoEffect);
            pRhinoEffect->name = Effect_Name->c_str();

            _vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            // 타이머 및 카운트 업데이트
            m_iEffectCount++;
            m_fNextEffectTime += 0.25f;  // 다음 이펙트 타이밍을 0.5초 후로 설정
            //m_fNextEffectTime += 0.5f;  // 다음 이펙트 타이밍을 0.5초 후로 설정
        }

        if (m_fTime >= 3.0f) // 3초 지나면
        {
            m_eState = RHINO_RUNNING; // 들이박기
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);

            // 상태 초기화
            m_iEffectCount = 0; // 발동 횟수 초기화
            m_fNextEffectTime = 0.25f;  // 다음 상태에서도 0.5초 간격 유지
            //m_fNextEffectTime = 0.5f;  // 다음 상태에서도 0.5초 간격 유지
        }
        break;

    case RHINO_RUNNING: // 들이박기 3초
        m_pTransformCom->m_vInfo[INFO_POS].y = 55.f;
        Running_Ani();

        //if (m_fTime >= m_fNextEffectTime && m_iEffectCount < 5)
        if (m_fTime >= m_fNextEffectTime && m_iEffectCount < 10)
        {
            // 이펙트를 발생시키는 부분
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Rhino_Effect" + to_wstring(40 + m_iEffectCount);

            CRhinoEffect* pRhinoEffect = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pRhinoEffect);
            pRhinoEffect->name = Effect_Name->c_str();

            _vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 50.f;
            dynamic_cast<CTransform*>(pRhinoEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            // 타이머 및 카운트 업데이트
            m_iEffectCount++;
            m_fNextEffectTime += 0.25f;  // 다음 이펙트 타이밍을 0.5초 후로 설정
            //m_fNextEffectTime += 0.5f;  // 다음 이펙트 타이밍을 0.5초 후로 설정
        }

        if (m_fTime >= 3.0f && m_pRhinoStone[m_iStoneCnt - 1]->Get_Dead()) // 마지막 돌이 부서졌는지 확인
            //if (m_fTime >= 3.0f && m_pRhinoStone[9]->Get_Dead()) // 마지막 돌이 부서졌는지 확인
        {
            m_eState = RHINO_JUMP; // 부서졌다면 점프로 넘어가기
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            // 상태 초기화
            m_iEffectCount = 0; // 발동 횟수 초기화
            //m_fNextEffectTime = 0.5f;  // 다음 상태에서도 0.5초 간격 유지
            m_fNextEffectTime = 0.25f;  // 다음 상태에서도 0.5초 간격 유지

        }
        else if (m_fTime >= 3.0f && !m_pRhinoStone[m_iStoneCnt - 1]->Get_Dead())
            //else if (m_fTime >= 3.0f && !m_pRhinoStone[9]->Get_Dead())
        {
            m_eState = RHINO_STOMPING; // 아니면 다시 발구르러 가기
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            // 상태 초기화
            m_iEffectCount = 0; // 발동 횟수 초기화
            m_fNextEffectTime = 0.25f;  // 다음 상태에서도 0.5초 간격 유지
            //m_fNextEffectTime = 0.5f;  // 다음 상태에서도 0.5초 간격 유지
        }
        break;

    case RHINO_JUMP: // 리노 점프해서 
        m_pAnimatorCom->Play(L"RhinoJump", false); // 점프(1회만 출력 / false)
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_fTime = 0.f;
            m_eState = RHINO_SKY; // 착지
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            //이펙1
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Rhino_Effect" + to_wstring(30);

            CRhinoEffect* pRhinoEffect = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pRhinoEffect);
            pRhinoEffect->name = Effect_Name->c_str();

            _vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x += 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name1 = new wstring;
            *Effect_Name1 = L"Rhino_Effect" + to_wstring(31);

            CRhinoEffect* pRhinoEffect1 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name1).c_str(), pRhinoEffect1);
            pRhinoEffect1->name = Effect_Name1->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x -= 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect1->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name2 = new wstring;
            *Effect_Name2 = L"Rhino_Effect" + to_wstring(32);

            CRhinoEffect* pRhinoEffect2 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name2).c_str(), pRhinoEffect2);
            pRhinoEffect2->name = Effect_Name2->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect2->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);
        }

        if (m_bStoneAppear) // 이미 생성했으면. 
            break;

        for (size_t i = 0; i < m_iStoneCnt; i++) // 스톤 타겟 잡는 애니 + 돌부서짐 + 하나만 남기 업데이트 
        {
            m_pRhinoStone[i]->Set_PosAble();
            m_pRhinoStone[i]->Set_Appear();
        }
        m_bStoneAppear = true;

        break;

    case RHINO_SKY:
        m_pAnimatorCom->Play(L"RhinoSky", false); // 충돌 브로큰 할거
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_eState = RHINO_LAND; // 착지
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_bStoneAppear = false; // 새 생성 없던 일로ㅋ
        }
        break;
    case RHINO_LAND:
        m_pAnimatorCom->Play(L"RhinoLand", false); // 점프

        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_eState = RHINO_STOMPING; // 초기 자세로 대충 얘가 idle인듯
            m_fTime = 0.0f;
            m_pAnimatorCom->GetAnimation()->SetFrame(0);

            //이펙1
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Rhino_Effect" + to_wstring(30);

            CRhinoEffect* pRhinoEffect = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pRhinoEffect);
            pRhinoEffect->name = Effect_Name->c_str();

            _vec3 vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x += 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name1 = new wstring;
            *Effect_Name1 = L"Rhino_Effect" + to_wstring(31);

            CRhinoEffect* pRhinoEffect1 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name1).c_str(), pRhinoEffect1);
            pRhinoEffect1->name = Effect_Name1->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.x -= 30.f;
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect1->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);

            //이펙2
            wstring* Effect_Name2 = new wstring;
            *Effect_Name2 = L"Rhino_Effect" + to_wstring(32);

            CRhinoEffect* pRhinoEffect2 = CRhinoEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name2).c_str(), pRhinoEffect2);
            pRhinoEffect2->name = Effect_Name2->c_str();

            vEffectPos = m_pTransformCom->m_vInfo[INFO_POS];
            vEffectPos.y -= 10.f;
            vEffectPos.z -= 1.f;
            dynamic_cast<CTransform*>(pRhinoEffect2->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vEffectPos);
        }
        break;

    case RHINO_STUN:
        Stun_Ani();
        if (m_fTime >= 3.0f)
        {
            m_eState = RHINO_STOMPING;
            m_fTime = 0.0f;
        }

    }
}

void CRhino::Running_Ani()
{
    m_pTransformCom->m_vInfo[INFO_POS] += m_vLookDir * m_fSpeed;

    // 달리기 애니메이션 재생
    if (fabs(m_vLookDir.x) < 0.1f && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunBack", true);
    }
    else if (fabs(m_vLookDir.x) < 0.1f && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunFront", true);
    }
    else if (m_vLookDir.x > 0 && fabs(m_vLookDir.z) < 0.1f)
    {
        m_pAnimatorCom->Play(L"RhinoRunRight", true);
    }
    else if (m_vLookDir.x < 0 && fabs(m_vLookDir.z) < 0.1f)
    {
        m_pAnimatorCom->Play(L"RhinoRunLeft", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunLB", true);
    }
}

void CRhino::Stun_Ani()
{
    if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunLB", true);
    }
}


HRESULT CRhino::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 65.f, 65.f, 10.f };
    m_pTransformCom->Set_Pos(500.f, 50.f, 600.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    return S_OK;
}

CRhino* CRhino::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRhino* pMonster = new CRhino(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Rhino Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CRhino::Free()
{
    Engine::CGameObject::Free();
}
