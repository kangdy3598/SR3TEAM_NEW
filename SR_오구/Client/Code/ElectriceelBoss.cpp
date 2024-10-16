#include "pch.h"
#include "ElectriceelBoss.h"
#include "Export_Utility.h"
#include "ElectricEffect.h"
#include "BossHPBar.h"
#include "ElectriceelBullet.h"
#include "WaterFall.h"
#include "Stone.h"
#include "HitEffect.h"
#include "VictoryUI.h"


CElectriceelBoss::CElectriceelBoss(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pComeTex(nullptr)
    , m_eCurState(Electriceel_STATE::OUT_12_MAGIC)
    , m_ePreState(Electriceel_STATE::OUT_12_MAGIC)
    , m_iCount(0)
    , m_iBulletCount(0)
    , m_iWaterFallCount(0)
{

    m_tInfo.pBossName = L"천둥 뱀장어";
    m_tInfo.iMaxHP = 5;
    m_tInfo.iCurHP = 5;
}

CElectriceelBoss::~CElectriceelBoss()
{
}

HRESULT CElectriceelBoss::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_vecTexture.resize(10);

    // 사운드 다 멈추기 
    StopAll();


    //12시,1시,3시,9시 방향  InOut 텍스처
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_InOut1.png", &m_vecTexture[0]);
    //11시 InOut 텍스처
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_InOut2.png", &m_vecTexture[1]);
    //12시,1시,3시,9시 Idle 텍스처
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_Idle1.png", &m_vecTexture[2]);
    //11시 Idle 텍스처
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_Idle2.png", &m_vecTexture[3]);

    //12시 MAGIC 텍스처 
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_Magic.png", &m_vecTexture[4]);
    
    //12시 1시 3시 3시 9시 SHOT 텍스처 
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_ProjectileShoot.png", &m_vecTexture[5]);
    //11시 SHOT 텍스처 
    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_HarborElectricEel_ProjectileShoot_Recover.png", &m_vecTexture[6]);



    m_pAnimatorCom->CreateAnimation(L"OUT_12", m_vecTexture[0], _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5); 
    m_pAnimatorCom->CreateAnimation(L"OUT_1",  m_vecTexture[0], _vec2(0.f, 512.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"OUT_3",  m_vecTexture[0], _vec2(0.f, 1024.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"OUT_9",  m_vecTexture[0], _vec2(0.f, 1536.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"OUT_11", m_vecTexture[1], _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5);
    
    m_pAnimatorCom->CreateAnimation(L"IN_12", m_vecTexture[0], _vec2(2560.f, 0.f), _vec2(512.f, 512.f), _vec2(-512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"IN_1", m_vecTexture[0], _vec2(2560.f, 512.f), _vec2(512.f, 512.f), _vec2(-512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"IN_3", m_vecTexture[0], _vec2(2560.f, 1024.f), _vec2(512.f, 512.f), _vec2(-512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"IN_9", m_vecTexture[0], _vec2(2560.f, 1536.f), _vec2(512.f, 512.f), _vec2(-512.f, 0.f), 0.2f, 5);
    m_pAnimatorCom->CreateAnimation(L"IN_11", m_vecTexture[1], _vec2(2560.f, 0.f), _vec2(512.f, 512.f), _vec2(-512.f, 0.f), 0.2f, 5);


    m_pAnimatorCom->CreateAnimation(L"IDLE_12", m_vecTexture[2], _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);
    m_pAnimatorCom->CreateAnimation(L"IDLE_1",  m_vecTexture[2], _vec2(0.f, 512.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);
    m_pAnimatorCom->CreateAnimation(L"IDLE_3",  m_vecTexture[2], _vec2(0.f, 1024.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);
    m_pAnimatorCom->CreateAnimation(L"IDLE_9",  m_vecTexture[2], _vec2(0.f, 1536.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);
    m_pAnimatorCom->CreateAnimation(L"IDLE_11", m_vecTexture[3], _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 3);


    m_pAnimatorCom->CreateAnimation(L"PROJECT_MAGIC_12", m_vecTexture[4], _vec2(0.f, 0.f),   _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.2f, 5);

    m_pAnimatorCom->CreateAnimation(L"PROJECT_SHOT_12", m_vecTexture[5], _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.25f, 3);
    m_pAnimatorCom->CreateAnimation(L"PROJECT_SHOT_1", m_vecTexture[5], _vec2(0.f, 512.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.25f, 3);
    m_pAnimatorCom->CreateAnimation(L"PROJECT_SHOT_3", m_vecTexture[5], _vec2(0.f, 1024.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.25f, 3);
    m_pAnimatorCom->CreateAnimation(L"PROJECT_SHOT_9", m_vecTexture[5], _vec2(0.f, 1536.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.25f, 3);
    m_pAnimatorCom->CreateAnimation(L"PROJECT_SHOT_11", m_vecTexture[6], _vec2(0.f, 1024.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.25f, 3);


    //m_pAnimatorCom->Play(L"InOut1", true);

    dwtime = GetTickCount64();  
    dwtimeShot = GetTickCount64();  
    m_pTransformCom->Set_Pos(200.f, 60.f, 500.f);
    m_pTransformCom->m_vScale = { 75.f, 100.f, 20.f };

    srand((unsigned int)time(NULL));

    m_pBossHPBar = dynamic_cast<CBossHPBar*>(CBossHPBar::Create(m_pGraphicDev));
    //NULL_CHECK_RETURN(m_pBossHPBar);
    if(m_pBossHPBar == nullptr)
    {
        MSG_BOX("보스피통 널포인트임");
    }
    m_pBossHPBar->Init_BossInfo(&m_tInfo);  

    return S_OK;
}



_int CElectriceelBoss::Update_GameObject(const _float& fTimeDelta)
{
   
    Add_RenderGroup(RENDER_ALPHA, this);          

    if (m_tInfo.iCurHP != 0)
    {

        Electriceel_STATE patterns[] =
        {
            Electriceel_STATE::OUT_9_WATERFALL,
            Electriceel_STATE::OUT_1_SHOT,
            //Electriceel_STATE::OUT_12,
            Electriceel_STATE::OUT_11_SHOT,
            Electriceel_STATE::OUT_3_WATERFALL,
            //Electriceel_STATE::IDLE_12,
            //Electriceel_STATE::IDLE_12,
            //Electriceel_STATE::PROJECT_SHOT_12,
            //Electriceel_STATE::IN_12,
            Electriceel_STATE::OUT_12_MAGIC,

        };

        std::random_device rd;
        std::mt19937 gen(rd());
        //std::uniform_int_distribution<> dis(0, 4);
        std::uniform_int_distribution<> dis(0, 4);
        if (dwtime + 3500 < GetTickCount64())
        {
            int randomIndex = dis(gen);
            m_eCurState = patterns[randomIndex];

            dwtime = GetTickCount64();
        }

        m_ePreState = m_eCurState;

        update_move();
        update_state();
        update_sound();

        m_pBossHPBar->Update_GameObject(fTimeDelta);


        map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
        //그니깐 레이어에서 삭제해줘야한다는것. 
        CLayer* pLayer = nullptr;

        for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
        {
            const _tchar* layerKey = iter->first;

            if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
            {
                pLayer = iter->second;
            }
        }
        map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();

        auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(L"Stone0"));

        if (iter == pMap.end())
        {
            Engine::CGameObject* pGameObject = nullptr;
            pGameObject = CStone::Create(m_pGraphicDev);
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            mapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Stone0", pGameObject);
            static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 21.f, 500.f);
            pGameObject->SetObjectKey(L"Stone0");
            pGameObject->SetObjectType(OBJ_TYPE::PUSH_ABLE);
        }

    }

    else
    {
        m_eCurState = Electriceel_STATE::IN_12_END;
    }

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CElectriceelBoss::update_move()
{
    if (m_eCurState == Electriceel_STATE::OUT_9_MAGIC || m_eCurState == Electriceel_STATE::OUT_9_WATERFALL)
    {
        m_pTransformCom->Set_Pos(200.f, 85.f, 500.f);
    }

    else if (m_eCurState == Electriceel_STATE::OUT_11_MAGIC || m_eCurState == Electriceel_STATE::OUT_11_SHOT)
    {
        m_pTransformCom->Set_Pos(350.f, 85.f, 650.f);
    }

    else if (m_eCurState == Electriceel_STATE::OUT_12_MAGIC || m_eCurState == Electriceel_STATE::OUT_12_SHOT)
    {
        m_pTransformCom->Set_Pos(500.f, 85.f, 700.f);
    }

    else if (m_eCurState == Electriceel_STATE::OUT_1_MAGIC || m_eCurState == Electriceel_STATE::OUT_1_SHOT)
    {
        m_pTransformCom->Set_Pos(650.f, 85.f, 650.f);
    }

    else if (m_eCurState == Electriceel_STATE::OUT_3_MAGIC || m_eCurState == Electriceel_STATE::OUT_3_WATERFALL)
    {
        m_pTransformCom->Set_Pos(700.f, 85.f, 500.f);
    }


}

void CElectriceelBoss::update_state()
{
}

void CElectriceelBoss::update_sound()
{
}


void CElectriceelBoss::update_animation()
{
    map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();   

#pragma region MAGIC_12
    if(m_eCurState == Electriceel_STATE::OUT_12_MAGIC)      
    {
        m_pAnimatorCom->Play(L"OUT_12", false);  
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IDLE_12_MAGIC;
            
        }
    }

    if(m_eCurState == Electriceel_STATE::IDLE_12_MAGIC)
    {
        m_pAnimatorCom->Play(L"IDLE_12", false);    
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0); 

            m_eCurState = Electriceel_STATE::PROJECT_MAGIC_12;
            
        }
    }
        
    if (m_eCurState == Electriceel_STATE::PROJECT_MAGIC_12)  
    {   
        m_pAnimatorCom->Play(L"PROJECT_MAGIC_12", false);
        
        //map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
          
        if(m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 0 && m_iCount ==0)
        {
             
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(7);  

            CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);  
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect); 
            pElectricEffect->name = Effect_Name->c_str();   
            dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
            
            m_iCount++;
        }

        else if (m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 1 && m_iCount == 1)
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(8);

            CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect);
            pElectricEffect->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
            
            m_iCount++;
        }

        else if (m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 2 && m_iCount == 2)
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(9);

            CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect);
            pElectricEffect->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
            
            m_iCount++;

        }

        else if (m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 3 && m_iCount == 3)
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(10);

            CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect);
            pElectricEffect->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
            
            m_iCount++;

        }

        else if (m_pAnimatorCom->GetAnimation()->GetCurrentFrm() == 4 && m_iCount == 4)
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(11);

            CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect);
            pElectricEffect->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
            
            m_iCount++;

        }

        
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {
             
           
            for (int i = 0; i < 6; i++) 
            {
                wstring* Effect_Name = new wstring;
                *Effect_Name = L"Electric_Effect"+ to_wstring(i);
            
                CElectricEffect* pElectricEffect = CElectricEffect::Create(m_pGraphicDev);  
                pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectricEffect);
                pElectricEffect->name = Effect_Name->c_str();
                dynamic_cast<CTransform*>(pElectricEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(rand() % 200 + 400.f, 1.f, rand() % 200 + 400.f);

                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectricEffect);
               
            }

            m_pAnimatorCom->GetAnimation()->SetFrame(0); 
            m_iCount = 0; 
            m_eCurState = Electriceel_STATE::IN_12;     
           
        }

    }

    
    if(m_eCurState == Electriceel_STATE::IN_12)   
    {
        m_pAnimatorCom->Play(L"IN_12", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::OUT_12_MAGIC;     
        }
    }
#pragma endregion

#pragma region OUT_1_SHOT
    if (m_eCurState == Electriceel_STATE::OUT_1_SHOT)
    {
        m_pAnimatorCom->Play(L"OUT_1", false); 
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::IDLE_1_SHOT; 

        }
    }

    if (m_eCurState == Electriceel_STATE::IDLE_1_SHOT)
    {
        m_pAnimatorCom->Play(L"IDLE_1", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::PROJECT_SHOT_1;
            m_iBulletCount = 0; 

        }
    }

    if (m_eCurState == Electriceel_STATE::PROJECT_SHOT_1)   
    {
        if (dwtimeShot + 150 < GetTickCount64())
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(m_iBulletCount);

            CElectriceelBullet* pElectriceelBullet = CElectriceelBullet::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectriceelBullet);
            pElectriceelBullet->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectriceelBullet->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(650.f, 15.f, 650.f);
            pElectriceelBullet->SetBulletDir(_vec3(0.f - 0.15f * m_iBulletCount, 0.f, -1.f + 0.15f * m_iBulletCount));
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectriceelBullet);
            m_iBulletCount++;
            Play_Sound(L"SFX_530_ElectricEel_Shoot.wav", SOUND_ElectricEel, 1.f);
            dwtimeShot = GetTickCount64();
        }
        m_pAnimatorCom->Play(L"PROJECT_SHOT_1", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IN_1;
            m_iBulletCount = 0; 

        }
    }

    if (m_eCurState == Electriceel_STATE::IN_1)
    {
        m_pAnimatorCom->Play(L"IN_1", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::OUT_1_SHOT;    

        }
    }




#pragma endregion

#pragma region OUT_11_SHOT
    if (m_eCurState == Electriceel_STATE::OUT_11_SHOT)
    {
        m_pAnimatorCom->Play(L"OUT_11", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IDLE_11_SHOT;

        }
    }

    if (m_eCurState == Electriceel_STATE::IDLE_11_SHOT)
    {
        m_pAnimatorCom->Play(L"IDLE_11", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::PROJECT_SHOT_11;
        }
    }

    if (m_eCurState == Electriceel_STATE::PROJECT_SHOT_11)
    {
        if (dwtimeShot + 150 < GetTickCount64())
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(m_iBulletCount);

            CElectriceelBullet* pElectriceelBullet = CElectriceelBullet::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pElectriceelBullet);
            pElectriceelBullet->name = Effect_Name->c_str();
            dynamic_cast<CTransform*>(pElectriceelBullet->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 15.f, 650.f);
            pElectriceelBullet->SetBulletDir(_vec3(0.f + 0.15f * m_iBulletCount, 0.f, -1.f + 0.15f * m_iBulletCount));
            CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pElectriceelBullet);
            m_iBulletCount++;
            Play_Sound(L"SFX_530_ElectricEel_Shoot.wav", SOUND_EFFECT, 1.f);    
            dwtimeShot = GetTickCount64();
        }

        m_pAnimatorCom->Play(L"PROJECT_SHOT_11", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IN_11;
            m_iBulletCount = 0;

        }
    }

    if (m_eCurState == Electriceel_STATE::IN_11)    
    {
        m_pAnimatorCom->Play(L"IN_11", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::OUT_11_SHOT;   

        }
    }
#pragma endregion

#pragma region OUT_3_SHOT


    if (m_eCurState == Electriceel_STATE::OUT_3_WATERFALL)  
    {
        m_pAnimatorCom->Play(L"OUT_3", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::IDLE_3_WATERFALL;  

        }
    }

    if (m_eCurState == Electriceel_STATE::IDLE_3_WATERFALL) 
    {
        m_pAnimatorCom->Play(L"IDLE_3", false); 
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::PROJECT_WATERFALL_3;   
        }
    }


    if (m_eCurState == Electriceel_STATE::PROJECT_WATERFALL_3)  
    {
       
        m_pAnimatorCom->Play(L"PROJECT_SHOT_3", false);
        if (dwtimeShot + 300 < GetTickCount64())
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(m_iWaterFallCount);  

            _vec3 patterns[] =
            {
                {-1.f,0.f,-1.f},// 이때는 1시  방향 위치 시작
                {1.f,0.f,-1.f}, // 이때는 11시 방향 위치 시작
                {0.f,0.f,-1.f}, // 이때는 12시 방향 위치 시작
                {1.f,0.f,1.f},  // 이때는 7시  방향 위치 시작
                {0.f,0.f,1.f},  // 이때는 6시  방향 위치 시작
                {-1.f,0.f,1.f}, // 이때는 5시  방향 위치 시작

            };
            std::random_device rd;  
            std::mt19937 gen(rd()); 
            std::uniform_int_distribution<> dis(0, 5);  

            int randomIndex = dis(gen);
            _vec3 vDir = patterns[randomIndex]; 

            CWaterFall* pWateFall = CWaterFall::Create(m_pGraphicDev);  
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pWateFall);   
            pWateFall->name = Effect_Name->c_str(); 

            if(randomIndex == 0) // 1시 방향
            {
                
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(650.f, 41.f, 650.f);
                pWateFall->SetWaterFallDir(patterns[0]);
                pWateFall->LateReady_GameObject();

                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 1) // 11시 방향
            {
               
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 42.f, 650.f);
                pWateFall->SetWaterFallDir(patterns[1]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 2) // 12시 방향
            {
                
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 43.f, 700.f);
                pWateFall->SetWaterFallDir(patterns[2]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 3) // 7시 방향
            {
                
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 44.f, 350.f);
                pWateFall->SetWaterFallDir(patterns[3]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 4) // 6시 방향
            {
                
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 45.f, 300.f);
                pWateFall->SetWaterFallDir(patterns[4]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 5) // 6시 방향
            {
                
                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(650.f, 46.f, 350.f);
                pWateFall->SetWaterFallDir(patterns[5]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            m_iWaterFallCount++;       
            //if (randomIndex == 3) // 시 방향
            //{
            //    dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 100.f, 700.f);
            //    pWateFall->SetWaterFallDir(patterns[3]);
            //    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            //}
            dwtimeShot = GetTickCount64();
        }

        
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::IN_3;  
            m_iWaterFallCount = 0;  
        }
    }

    if (m_eCurState == Electriceel_STATE::IN_3) 
    {
        m_pAnimatorCom->Play(L"IN_3", false);   
        if (m_pAnimatorCom->GetAnimation()->IsFinish()) 
        {   
            m_pAnimatorCom->GetAnimation()->SetFrame(0);    
            m_eCurState = Electriceel_STATE::OUT_3_WATERFALL;   
        }
    }





#pragma endregion

#pragma region OUT_9_SHOT
    if (m_eCurState == Electriceel_STATE::OUT_9_WATERFALL)  
    {
        m_pAnimatorCom->Play(L"OUT_9", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IDLE_9_WATERFALL;  

        }
    }

    if (m_eCurState == Electriceel_STATE::IDLE_9_WATERFALL)
    {
        m_pAnimatorCom->Play(L"IDLE_9", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::PROJECT_WATERFALL_9;
        }
    }


    if (m_eCurState == Electriceel_STATE::PROJECT_WATERFALL_9)
    {
        m_pAnimatorCom->Play(L"PROJECT_SHOT_9", false);
        if (dwtimeShot + 300 < GetTickCount64())
        {
            wstring* Effect_Name = new wstring;
            *Effect_Name = L"Electric_Effect" + to_wstring(m_iWaterFallCount);

            _vec3 patterns[] =
            {
                {-1.f,0.f,-1.f},// 이때는 1시  방향 위치 시작
                {1.f,0.f,-1.f}, // 이때는 11시 방향 위치 시작
                {0.f,0.f,-1.f}, // 이때는 12시 방향 위치 시작
                {1.f,0.f,1.f},  // 이때는 7시  방향 위치 시작
                {0.f,0.f,1.f},  // 이때는 6시  방향 위치 시작
                {-1.f,0.f,1.f}, // 이때는 5시  방향 위치 시작

            };
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 5);

            int randomIndex = dis(gen);
            _vec3 vDir = patterns[randomIndex];

            CWaterFall* pWateFall = CWaterFall::Create(m_pGraphicDev);
            pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pWateFall);
            pWateFall->name = Effect_Name->c_str();

            if (randomIndex == 0) // 1시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(650.f, 41.f, 650.f);
                pWateFall->SetWaterFallDir(patterns[0]);
                pWateFall->LateReady_GameObject();

                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 1) // 11시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 42.f, 650.f);
                pWateFall->SetWaterFallDir(patterns[1]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 2) // 12시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 43.f, 700.f);
                pWateFall->SetWaterFallDir(patterns[2]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 3) // 7시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 44.f, 350.f);
                pWateFall->SetWaterFallDir(patterns[3]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 4) // 6시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 45.f, 300.f);
                pWateFall->SetWaterFallDir(patterns[4]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            if (randomIndex == 5) // 6시 방향
            {

                dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(650.f, 46.f, 350.f);
                pWateFall->SetWaterFallDir(patterns[5]);
                pWateFall->LateReady_GameObject();
                CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            }

            m_iWaterFallCount++;
            //if (randomIndex == 3) // 시 방향
            //{
            //    dynamic_cast<CTransform*>(pWateFall->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(500.f, 100.f, 700.f);
            //    pWateFall->SetWaterFallDir(patterns[3]);
            //    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pWateFall);
            //}
            dwtimeShot = GetTickCount64();
        }


        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::IN_9;
            m_iWaterFallCount = 0;
        }
    }

    if (m_eCurState == Electriceel_STATE::IN_9)
    {
        m_pAnimatorCom->Play(L"IN_9", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_pAnimatorCom->GetAnimation()->SetFrame(0);
            m_eCurState = Electriceel_STATE::OUT_9_WATERFALL;   
        }
    }
#pragma endregion

    if (m_eCurState == Electriceel_STATE::IN_12_END)
    {
        m_pAnimatorCom->Play(L"IN_12", false);
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {

        }
    }
}

void CElectriceelBoss::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    update_animation();

}

void CElectriceelBoss::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    switch (m_eCurState)    
    {
    case Electriceel_STATE::OUT_9_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::OUT_11_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[1]);
        break;
    case Electriceel_STATE::OUT_12_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        Play_Sound(L"SFX_531_ElectricEel_DiveOut.wav", SOUND_ElectricEel, 1.f);
        break;
    case Electriceel_STATE::OUT_1_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::OUT_3_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::OUT_9_WATERFALL:
        Play_Sound(L"SFX_531_ElectricEel_DiveOut.wav", SOUND_ElectricEel, 1.f);
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::OUT_11_SHOT:
        Play_Sound(L"SFX_531_ElectricEel_DiveOut.wav", SOUND_ElectricEel, 1.f); 
        m_pGraphicDev->SetTexture(0, m_vecTexture[1]);
        break;
    case Electriceel_STATE::OUT_1_SHOT:
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        Play_Sound(L"SFX_531_ElectricEel_DiveOut.wav", SOUND_ElectricEel, 1.f); 
        break;
    case Electriceel_STATE::OUT_3_WATERFALL:
        Play_Sound(L"SFX_531_ElectricEel_DiveOut.wav", SOUND_ElectricEel, 1.f); 
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::IDLE_9_WATERFALL:   
        
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);
        break;
    case Electriceel_STATE::IDLE_1_SHOT:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);
        break;
    case Electriceel_STATE::IDLE_3_WATERFALL:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);
        break;
    case Electriceel_STATE::IDLE_11_SHOT:
        m_pGraphicDev->SetTexture(0, m_vecTexture[3]);
        break;
    case Electriceel_STATE::IDLE_12_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);
        break;
    case Electriceel_STATE::IDLE_1_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);
        break;
    case Electriceel_STATE::IDLE_3_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);  
        break;
    case Electriceel_STATE::IDLE_9_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[2]);  
        break;
    case Electriceel_STATE::IDLE_11_MAGIC:
        m_pGraphicDev->SetTexture(0, m_vecTexture[3]);
        break;

    case Electriceel_STATE::IN_12:
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);  
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::IN_12_END:
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);  
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;
    case Electriceel_STATE::IN_1:
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;

    case Electriceel_STATE::IN_3:
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;

    case Electriceel_STATE::IN_9:   
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);
        m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
        break;

    case Electriceel_STATE::IN_11:
        //Play_Sound(L"SFX_532_ElectricEel_DiveIn.wav", SOUND_ElectricEel, 1.f);
        m_pGraphicDev->SetTexture(0, m_vecTexture[1]);
        break;
        
    case Electriceel_STATE::PROJECT_MAGIC_12:
        m_pGraphicDev->SetTexture(0, m_vecTexture[4]);
        break;
    
    case Electriceel_STATE::PROJECT_WATERFALL_3:    
        m_pGraphicDev->SetTexture(0, m_vecTexture[5]);  
        break;

    case Electriceel_STATE::PROJECT_WATERFALL_9:
        m_pGraphicDev->SetTexture(0, m_vecTexture[5]);
        break;
    case Electriceel_STATE::PROJECT_SHOT_1:
        m_pGraphicDev->SetTexture(0, m_vecTexture[5]);
        break;

    case Electriceel_STATE::PROJECT_SHOT_11:
        m_pGraphicDev->SetTexture(0, m_vecTexture[6]);
        break;

    case Electriceel_STATE::ATTACK:
        break;
    default:
        break;
    }

    m_pAnimatorCom->render();


    //맵툴 작성시 끄기 
    ////m_pBoundBox->Render_Buffer();   
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CElectriceelBoss::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);  
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });        

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);  
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });    

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));    
    NULL_CHECK_RETURN(pComponent, E_FAIL);  
    m_pBoundBox->SetGameObjectPtr(this);    
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent }); 

    return S_OK;
}

HRESULT CElectriceelBoss::SetUp_Material()
{
    return S_OK;
}

void CElectriceelBoss::Key_Input(const _float& fTimeDelta)
{
}

CElectriceelBoss* CElectriceelBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CElectriceelBoss* pElectriceelBoss = new CElectriceelBoss(pGraphicDev);

    if (FAILED(pElectriceelBoss->Ready_GameObject()))
    {
        Safe_Release(pElectriceelBoss);
        MSG_BOX("pObject Create Failed");
        return nullptr;
    }

    return pElectriceelBoss;
}

void CElectriceelBoss::Free()
{
    Engine::CGameObject::Free();
}

void CElectriceelBoss::OnCollisionEnter(CGameObject* _pOther)
{
    if(dynamic_cast<CStone*>(_pOther)->GetObjectKey() == L"Stone0")
    {
        Play_Sound(L"SFX_533_ElectricEel_Damage.wav", SOUND_ElectricEel_Collision, 1.f);  
        --m_tInfo.iCurHP;

        wstring* Effect_Name = new wstring; 
        *Effect_Name = L"Hit_Effect";

        map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();   

        CHitEffect* pHitEffect = CHitEffect::Create(m_pGraphicDev); 
        pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*Effect_Name).c_str(), pHitEffect);  
        pHitEffect->name = Effect_Name->c_str();    
        _vec3 vPos = { 0.f,0.f,0.f };   
        m_pTransformCom->Get_Info(INFO_POS, &vPos); 
        dynamic_cast<CTransform*>(pHitEffect->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos.x-1.f, vPos.y-10.f, vPos.z-1.f);
        CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER_EFFECT, pHitEffect);  
      

        if (m_tInfo.iCurHP == 0)
        {
            CVictoryUI* pVictoryUI = dynamic_cast<CVictoryUI*>(Engine::Get_GameObject(L"Layer_UI", L"Victory_UI"));
            //NULL_CHECK_RETURN(pFightUI);
            pVictoryUI->CallVictory();

            Engine::StopSound(SOUND_BGM);
            Play_Sound(L"BGM_30_BossClear.wav", SOUND_BGM, 0.3f);
        }
    }

}

void CElectriceelBoss::OnCollision(CGameObject* _pOther)
{
    int a = 4;
}
