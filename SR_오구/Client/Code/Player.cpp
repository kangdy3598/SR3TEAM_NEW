#include "pch.h"
#include "Player.h"
#include "Define.h"
#include "Monster.h"
#include "InvenUI.h"//1003
#include "Export_System.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pCamera(nullptr)
    , m_ePlayerState(PLAYERSTATE::PLY_END)
    //, m_iPlayerDir(OBJ_DIRECTION::OBJDIR_FRONT)
    //, m_vPlayerCurrPos()
    //, m_vPlayerPrevPos()
    , m_bIsDiagonal(false)
    , m_bFixPlayerDir(false)
    , m_bSwingTrigger(false)
    , m_bIsInteracting(false)
    , m_objInteractionBox(nullptr)
    , m_objInteracting(nullptr)
    , m_fMoveSpeed(0.f)
    , m_fItemMoveSpeed(0.f)
    , m_iAttackPower(0)
    , m_iItemAttackPower(0)
    , m_bInvincible(false)
    , m_bPushTrigger(false)
    , m_bPassAble(true)
    , m_vColPlayerPos(0.f, 0.f, 0.f)
    , m_vColliderPos(0.f, 0.f, 0.f)
    , m_objLiftObject(nullptr)
    // 민지 초기화
    , m_iHonorScore(0.f) //1010 민지
    , m_iPlayerCoin(10000), m_bInven(false), m_bQuest(false), m_bStoreVisit(false)

    , m_bSmashEnd(false)

{
    ZeroMemory(&m_tPlayerHP, sizeof(PLAYERHP));
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    m_bDontDestroy = true;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    //민지 임시 코드
    m_tPlayerHP.iCurHP = 1;
    m_tPlayerHP.iMaxHP = 6;
    m_eTag = TAG_PLAYER;
    m_pTransformCom->m_vScale = { 10.f,18.f,8.f };
    m_pTransformCom->Set_Pos(200.f, 16.f, 500.f);

    m_pTexTransformCom->m_vScale = { 20.f,20.f,20.f };
    m_pTexTransformCom->Set_Pos(200.f, 16.f, 500.f);
    
    m_pStateControlCom->ChangeState(PlayerIdle::GetInstance(), this);

    D3DLIGHT9		tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));



    //m_pGraphicDev->SetLight(0, &tLightInfo);    
    //m_pGraphicDev->LightEnable(0, TRUE);    

   

    return S_OK;
}

void CPlayer::LateReady_GameObject()
{
    m_pStateControlCom->ChangeState(PlayerIdle::GetInstance(), this);
    m_bNextStage = false;

    Engine::CGameObject::LateReady_GameObject();

    m_pQuestUI = dynamic_cast<CQuestUI*>(Engine::Get_GameObject(L"Layer_UI", L"Quest_UI"));
    NULL_CHECK_RETURN(m_pQuestUI); // 아직 스테이지에 안만듬

    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_pQuickSlot = dynamic_cast<CQuickSlot*>(Engine::Get_GameObject(L"Layer_UI", L"QuickSlot_UI"));
    NULL_CHECK_RETURN(m_pQuickSlot);

    m_BuffArray[0] = dynamic_cast<CPowerUI*>(Engine::Get_GameObject(L"Layer_UI", L"Power_UI"));
    NULL_CHECK_RETURN(m_BuffArray[0]);

    m_BuffArray[1] = dynamic_cast<CSpeedUI*>(Engine::Get_GameObject(L"Layer_UI", L"Speed_UI"));
    NULL_CHECK_RETURN(m_BuffArray[1]);

    m_equipHat = CEquipHat::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(m_equipHat, );
    FAILED_CHECK_RETURN(Get_Layer(L"Layer_GameLogic")->Add_GameObject(L"EquipHat", m_equipHat), );
    m_equipHat->LateReady_GameObject();

}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE ||
        GetPlayerState() == PLAYERSTATE::PLY_PICKUP)
    {
        /*m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
        NULL_CHECK_RETURN(m_pInven, 0);*/


        if (m_BuffArray[0]->Get_BuffTime() < m_BuffArray[1]->Get_BuffTime())
        {
            m_BuffArray[0]->Set_YPos(90.f);
            m_BuffArray[1]->Set_YPos(175.f);
        }
        else
        {
            m_BuffArray[1]->Set_YPos(90.f);
            m_BuffArray[0]->Set_YPos(175.f);
        }
        Engine::CGameObject::Update_GameObject(fTimeDelta);
    }

    Key_Input(fTimeDelta);
    Add_RenderGroup(RENDER_ALPHA, this);

    return 0;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (Get_Layer(L"Layer_GameLogic")->GetGameState() == GAMESTATE_NONE ||
        GetPlayerState() == PLAYERSTATE::PLY_PICKUP)
    {
        SetPlayerDirection();

        m_pAnimationCom->Update_Component(fTimeDelta);

        DurationInvincible(fTimeDelta);

        Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    }

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);    
    m_pTexTransformCom->Set_Pos(vPos);
}

void CPlayer::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    /*if (!m_bInvincible)
        m_pBoundBox->Render_Buffer();*/

    ////조명작업
    D3DLIGHT9 tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9)); // 구조체 초기화
    
    tLightInfo.Type = D3DLIGHT_SPOT; // 점 조명 타입 설정  
    
    // 조명 색상 설정
    tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f }; // 확산 색상 (흰색)
    tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f }; // 반사 색상 (흰색)
    tLightInfo.Ambient = { 1.0f, 1.0f, 1.0f, 1.f }; // 주위 색상 (어두운 회색)
    
    
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos); 
    
    // 조명 위치 설정
    tLightInfo.Position = { vPos.x, vPos.y, vPos.z }; // 조명 위치 (x, y, z)
    
    //아래 방향
    
    //_vec3 vDir = { GetPlayerDirVector2().x, 0.0f, GetPlayerDirVector2().z };  
    _vec3 vDir = { 0.f,-1.f,0.f };  
    tLightInfo.Direction = vDir;    
    
    
    
    // 조명 범위 및 감쇠 설정
    tLightInfo.Range = 100.0f; // 조명이 도달할 수 있는 최대 거리
    tLightInfo.Falloff = 1.0f; // 감쇠 비율
    tLightInfo.Attenuation0 = 1.0f; // 고정 감쇠 계수
    tLightInfo.Attenuation1 = 0.005f; // 거리 감쇠 계수
    tLightInfo.Attenuation2 = 0.0f; // 거리 제곱 감쇠 계수
    tLightInfo.Theta = D3DXToRadian(150.0f);
    tLightInfo.Phi = D3DXToRadian(180.0f);
    
    // 조명 정보 설정
    m_pGraphicDev->SetLight(0, &tLightInfo); // 조명 인덱스 0에 조명 정보 설정  
    m_pGraphicDev->LightEnable(0, TRUE); // 조명 인덱스 0의 조명 활성화    
    //
    ////테스트


    if (m_bIsDiagonal)
        m_pTextureCom->Set_Texture(m_ePlayerState + 1);
    else
        m_pTextureCom->Set_Texture(m_ePlayerState);

    //Print_PlayerState();

    FAILED_CHECK_RETURN(SetUp_Material(), );

    m_pAnimationCom->Render_Buffer();
    m_equipHat->Render_GameObject();

    //9월 25일 충돌관련
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    if (!m_bInvincible)
        m_pBoundBox->Render_Buffer();



    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
        
}

void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{
    m_objInteracting = _pOther;

    /*if (_pOther->IncludingType(OBJ_TYPE::PUSH_ABLE) &&
        m_objLiftObject != _pOther)
    {
        if (!m_bIsDiagonal)
            m_bPushTrigger = true;
    }*/

    if (_pOther->IncludingType(OBJ_TYPE::NOTPASS_ABLE) &&
        m_objLiftObject != _pOther)
    {
        m_bPassAble = false;
        m_pTransformCom->Get_Info(INFO_POS, &m_vColPlayerPos);
        dynamic_cast<CTransform*>(
            _pOther->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Get_Info(INFO_POS, &m_vColliderPos);
    }

    
    if (_pOther->IncludingType(OBJ_TYPE::HURT_ABLE))
    {
        
        if (m_bInvincible || !dynamic_cast<CGameObject*>(_pOther)->GetActivation())
            return; 

        m_pStateControlCom->ChangeState(PlayerHurt::GetInstance(), this);
        //SetPlayerCurHP(-1);
    }

    if (_pOther->IncludingType(OBJ_TYPE::HURT_ABLE2))
    {
    //
    //    m_pStateControlCom->ChangeState(PlayerHurt::GetInstance(), this);
    //    SetPlayerCurHP(-1);
    }
}

void CPlayer::OnCollisionExit(CGameObject* _pOther)
{
    if (_pOther->IncludingType(OBJ_TYPE::NOTPASS_ABLE))
    {
        m_bPassAble = true;
        m_vColPlayerPos.x = 0.f;
        m_vColPlayerPos.y = 0.f;    
        m_vColPlayerPos.z = 0.f;

        m_vColliderPos.x = 0.f;
        m_vColliderPos.y = 0.f;
        m_vColliderPos.z = 0.f;
    }

    if (_pOther->IncludingType(OBJ_TYPE::PUSH_ABLE))
        m_bPushTrigger = false;
}

void CPlayer::SetPlayerDirection()
{
    if (m_bFixPlayerDir)
        return;

    _vec3 num;
    num.x = 0; num.y = 0; num.z = 0;

    if (Engine::GetKeyPress(CONTROLKEY::PLY_UPKEY))
    {
        num.z -= OBJ_DIRECTION::OBJDIR_FRONT;
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_DOWNKEY))
    {
        num.z += OBJ_DIRECTION::OBJDIR_FRONT;
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_LEFTKEY))
    {
        num.x -= OBJ_DIRECTION::OBJDIR_FRONT;
    }

    if (Engine::GetKeyPress(CONTROLKEY::PLY_RIGHTKEY))
    {
        num.x += OBJ_DIRECTION::OBJDIR_FRONT;
    }

    ////////////////////////////////////////////////////////////////////////
    if (num == m_vPlayerDir)
        return;
    // 방향이 이전과 다르면 애니메이션 갱신
    m_vPlayerDir = num;
    m_pAnimationCom->SetAnimDir(m_ePlayerState, m_vPlayerDir, m_bIsDiagonal);
}

void CPlayer::ChangePickUpState()
{
    m_pStateControlCom->ChangeState(PlayerPickUp::GetInstance(), this);
}

void CPlayer::DurationInvincible(const _float& fTimeDelta)
{
    if (!m_bInvincible)
        return;

    static float fDurationTime = 0.f;
    fDurationTime += fTimeDelta;
    if (fDurationTime >= 3.f)
    {
        fDurationTime = 0.f;
        m_bInvincible = false;
    }
}

HRESULT CPlayer::SetUp_Material()
{
    D3DMATERIAL9 tMtrl; // 재질 구조체
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9)); // 구조체 초기화

    // 재질 색상 설정
    tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f }; // 확산 색상 (빨간색)
    tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f }; // 반사 색상 (흰색)
    tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.f }; // 주위 색상 (어두운 회색)
    tMtrl.Emissive = { 1.f, 1.f, 1.f, 1.f }; // 방출 색상 (검은색)
    tMtrl.Power = 0.f; // 반사 강도 설정

    // 재질 적용
    m_pGraphicDev->SetMaterial(&tMtrl); // 재질 설정

    return S_OK;
}

HRESULT CPlayer::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Proto_PlayerAnim"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

    pComponent = m_pStateControlCom = dynamic_cast<CStateController*>(Engine::Clone_Proto(L"Proto_State"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_State", pComponent });

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBoundBox->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    if (Engine::GetKeyDown(DIK_G))
        m_pStateControlCom->ChangeState(PlayerBalloonFly::GetInstance(), this);

    //1013
    if (Engine::GetKeyDown(DIK_I))
    {
        m_bInven ^= TRUE;
        if(m_bInven == FALSE)
            Engine::Play_Sound(L"SFX_68_UIBig_Close.wav", SOUND_EFFECT, 0.3);
        else if(m_bInven == TRUE)
            Engine::Play_Sound(L"SFX_67_UIBig_Open.wav", SOUND_EFFECT, 0.3);

        m_bQuest = false;
    }

    if (Engine::GetKeyDown(DIK_O))
    {
        m_bQuest ^= TRUE;
        if (m_bQuest == FALSE)
            Engine::Play_Sound(L"SFX_68_UIBig_Close.wav", SOUND_EFFECT, 0.3);
        else if (m_bQuest == TRUE)
            Engine::Play_Sound(L"SFX_67_UIBig_Open.wav", SOUND_EFFECT, 0.3);
        m_bInven = false;
    }

    if (m_bInven || m_bQuest || m_bIsInteracting)
    {
        Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_UIOPEN);
        return;
    }
    else
    {
        Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_NONE);
    }
    // 이 아래 추가 하삼 코드!!


    //0920 좆임시 아이템 추가
    if (Engine::GetKeyDown(DIK_L))
    {
        CItem* pItem = dynamic_cast<CExploreHat*>(CExploreHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPartyHat*>(CPartyHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMohican*>(CMohican::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CAppleHat*>(CAppleHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CEnkyHat*>(CEnkyHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPenguin*>(CPenguin::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPoopHat*>(CPoopHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallFruit*>(CSmallFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleFruit*>(CMiddleFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigFruit*>(CBigFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallHerb*>(CSmallHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleHerb*>(CMiddleHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigHerb*>(CBigHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallRoot*>(CSmallRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleRoot*>(CMiddleRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigRoot*>(CBigRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookSmallFish*>(CCookSmallFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookMiddleFish*>(CCookMiddleFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookBigFish*>(CCookBigFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);


        pItem = dynamic_cast<CBranch*>(CBranch::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CLeaf*>(CLeaf::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CNet*>(CNet::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CExpressTicket*>(CExpressTicket::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CWaterToken*>(CWaterToken::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);
    }

    for (int i = 0; i < 4; i++)
    {
        if (Engine::GetKeyDown(DIK_1 + i))
            m_pQuickSlot->Use_QuickItem(i);
    }

    if (Engine::GetKeyDown(DIK_SPACE))
        m_bInvincible ^= TRUE;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayer* pPlayer = new CPlayer(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }

    return pPlayer;
}

void CPlayer::Free()
{
    PlayerIdle::DestroyInstance();
    PlayerMove::DestroyInstance();
    PlayerDash::DestroyInstance();
    PlayerPush::DestroyInstance();
    PlayerLift::DestroyInstance();
    PlayerLiftMove::DestroyInstance();
    PlayerDance::DestroyInstance();
    PlayerSmash::DestroyInstance();
    PlayerRolling::DestroyInstance();
    PlayerHurt::DestroyInstance();

    Engine::CGameObject::Free();
}
