#include "pch.h"
#include "JungleForestStage.h"
#include "Export_Utility.h"



CJungleForestStage::CJungleForestStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{
}

CJungleForestStage::~CJungleForestStage()
{
}

HRESULT CJungleForestStage::Ready_Scene()
{
    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

    CManagement::GetInstance()->m_imap_stage = 0;

    Engine::StopAll();  
    Engine::PlayBGM(L"BGM_4_JungleAreaField2.wav", 0.5f);


    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}

void CJungleForestStage::LateReady_Scene()
{
    CPlayer* player = dynamic_cast<CPlayer*>(
        Get_GameObject(L"Layer_GameLogic", L"Player"));

    dynamic_cast<CTransform*>(
        player->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Set_Pos(1400.f, 30.f, 300.f);

    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

    Engine::CScene::LateReady_Scene();

    player->GetCamera()->WalkTo2(_vec3(750.f, 30.f, 400.f)
        , 5.f, _vec3(750.f, 30.f, 1900.f));
}

_int CJungleForestStage::Update_Scene(const _float& fTimeDelta)
{
    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);


    if (GetAsyncKeyState('M') & 0x8000)
    {
        Engine::CScene* pStage3 = CSunTempleStage::Create(m_pGraphicDev);  
        NULL_CHECK_RETURN(pStage3, -1);

        FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
        //pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

        return 0;
    }

    return iExit;
}

void CJungleForestStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CJungleForestStage::Render_Scene()
{
}

void CJungleForestStage::init()
{
    Engine::CLayer* pLayer = CLayer::Create();
    
    DWORD bytesRead = 1;
    HANDLE hFile = CreateFile(L"../Map/junglestageReal4.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        if (hFile != INVALID_HANDLE_VALUE) {
    
            Engine::CLayer* pLayer = CLayer::Create();
    
            DWORD bytesRead = 1;
    
            while (bytesRead > 0)
            {
                ObjectData* objData = new ObjectData;
    
                if (!ReadFile(hFile, objData, sizeof(ObjectData), &bytesRead, NULL))
                {
                    MSG_BOX("읽기 오류");
                };
    
                if (bytesRead == 0)
                {
                    break;
                }
    
    
                int a = 4;
    
                Engine::CGameObject* pGameObject = nullptr;
                pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                if (pGameObject == nullptr)
                {
                    MSG_BOX("CObject nullptr Error");
                }
    
                pGameObject->SetObjectKey(objData->objectName);
                pGameObject->SetTextureKey(objData->textureKey);
    
    
                map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
                pMapLayer[L"Layer_GameLogic"]->Add_GameObject(objData->objectName, pGameObject);
    
                CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", objData->objectName, L"Com_Transform"));
    
    
                pTransform->m_vScale = { objData->scale.x,objData->scale.y,objData->scale.z };
                pTransform->Rotation(ROT_X, objData->rotation.x * 3.14f / 180.f);
                pTransform->Rotation(ROT_Y, objData->rotation.y * 3.14f / 180.f);
                pTransform->Rotation(ROT_Z, objData->rotation.z * 3.14f / 180.f);
                pTransform->Set_Pos(objData->worldMatrix._41, objData->worldMatrix._42, objData->worldMatrix._43);
    
    
                pTransform->Rotation_x = objData->rotation.x;
                pTransform->Rotation_y = objData->rotation.y;
                pTransform->Rotation_z = objData->rotation.z;
   
    
                pTransform->Scale_x = objData->scale.x;
                pTransform->Scale_y = objData->scale.y;
                pTransform->Scale_z = objData->scale.z;
    
    
            }
            CloseHandle(hFile);
        }
    }
}

HRESULT CJungleForestStage::Ready_LightInfo()
{
    return S_OK;
}

HRESULT CJungleForestStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CJungleForestStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    //여기 수정해야하고
    pGameObject = CJungleForestMap::Create(m_pGraphicDev);  
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"JungleForestMap", pGameObject), E_FAIL);

    pGameObject = CDochi::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dochi", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CHochi::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hochi", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CCheerRabbit::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbit", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CBardCat::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BardCat", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CB52Bomber::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);
    static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();
    //static_cast<CB52Bomber*>(pGameObject)->Set_Player(PlayerObj);

#pragma region Crystal Puzzle
    _float fOffsetX = 700;
    _float fOffsetZ = 600;

    //Engine::CGameObject* pCGameObject = nullptr;
    //pCGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pCGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pCGameObject), E_FAIL);
    //CCrystalPuzzle* pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pCGameObject);

    //pGameObject = CCrystal::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 18.f, fOffsetZ);
    //pCrystalPuzzle->Add_Crystal(pGameObject);

    //pGameObject = CPressBlock::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);	
    //static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX, fOffsetZ - 35.f);
    //pCrystalPuzzle->Add_PressBlock(pGameObject);

    //pGameObject = CCrystal::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 35.f, 18.f, fOffsetZ);
    //static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
    //pCrystalPuzzle->Add_Crystal(pGameObject);

    //pGameObject = CPressBlock::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);	
    //static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 4, fOffsetX + 35.f, fOffsetZ - 35.f);
    //pCrystalPuzzle->Add_PressBlock(pGameObject);

    //pGameObject = CCrystal::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 70.f, 18.f, fOffsetZ);
    //pCrystalPuzzle->Add_Crystal(pGameObject);

    //pGameObject = CPressBlock::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
    //static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + 70.f, fOffsetZ - 35.f);
    //pCrystalPuzzle->Add_PressBlock(pGameObject);

    //pGameObject = CBreakableStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_00", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 35.f, 18.f, fOffsetZ);

    //pGameObject = CBreakableStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_01", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 105.f, 18.f, fOffsetZ);

    //pGameObject = CBreakableStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_02", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 70.f, 18.f, fOffsetZ);

    //pGameObject = CBreakableStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_03", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 140.f, 18.f, fOffsetZ);

#pragma endregion

#pragma region Monkey Statue
   /* fOffsetX = 650;
    fOffsetZ = 650;*/

    //pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_00", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f,fOffsetZ);
    //CMonkeyStatue* pMS = static_cast<CMonkeyStatue*>(pGameObject);

    //pGameObject = CStoneBlock::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_00", pGameObject), E_FAIL);
    //static_cast<CStoneBlock*>(pGameObject)->Init(100.f + fOffsetX, 20.f + fOffsetZ, true);
    //pMS->Add_StoneBlock(pGameObject);

    //pGameObject = CStoneBlock::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_01", pGameObject), E_FAIL);	
    //static_cast<CStoneBlock*>(pGameObject)->Init(150.f + fOffsetX, 20.f + fOffsetZ, false);
    //pMS->Add_StoneBlock(pGameObject);
#pragma endregion

#pragma region Hole
    //pGameObject = CStoneHole::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(45.f + fOffsetX, 0.05f, fOffsetZ - 200.5f);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(45.f + fOffsetX, 0.05f, fOffsetZ - 200.5f);

    //pGameObject = CStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_01", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ - 200.5f);


#pragma endregion


#pragma region Scale Puzzle
    fOffsetX = 750;
    fOffsetZ = 900;

    //pGameObject = CScale::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CScale_00", pGameObject), E_FAIL);
    //static_cast<CScale*>(pGameObject)->Init_Position(170.f + fOffsetX, 150.f + fOffsetZ);

    //pGameObject = CWeightComparator::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WeightComparator_00", pGameObject), E_FAIL);	
    //static_cast<CWeightComparator*>(pGameObject)->Init(170.f + fOffsetX, 30.f + fOffsetZ);

#pragma endregion


#pragma region Catapult
    fOffsetX = 500;
    fOffsetZ = 700;

    //pGameObject = CCatapult::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_00", pGameObject), E_FAIL);
    //static_cast<CCatapult*>(pGameObject)->Init_Pos(200.f + fOffsetX, 50.f + fOffsetZ);

    //pGameObject = CStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_02", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(250.f + fOffsetX, 15.f, 150.f + fOffsetZ);

    //pGameObject = CStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_03", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(150.f + fOffsetX, 15.f, 100.f + fOffsetZ);

    //pGameObject = CBreakableStone::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_04", pGameObject), E_FAIL);
    //static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f + fOffsetX, 14.f, 300.f + fOffsetZ);

#pragma endregion

#pragma region Puzzle

    fOffsetX = 752.5;
    fOffsetZ = 1118;

    pGameObject = CFrontDoor::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FrontDoor", pGameObject), E_FAIL);
    static_cast<CFrontDoor*>(pGameObject)->Init_Pos(fOffsetX, fOffsetZ);
    CGameObject* pFrontDoor = pGameObject;

    pGameObject = CTrigger::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Door_Trigger", pGameObject), E_FAIL);
    static_cast<CTrigger*>(pGameObject)->Initialize(fOffsetX, fOffsetZ - 200.f, { 200.f, 20.f, 20.f });
    static_cast<CTrigger*>(pGameObject)->Set_Group(pFrontDoor);

    fOffsetX = 625;
    fOffsetZ = 550;

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_00", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_01", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 30.f, 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_02", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 30.f, 16.f, fOffsetZ - 30.f);


    pGameObject = CStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_00", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ - 30.f);

    pGameObject = CStoneHole::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 2), 0.2f, fOffsetZ);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 0.2f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_03", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_04", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_05", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_06", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_07", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_08", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ);

    //Left	
    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_09", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + 30.f);

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_10", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 2));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_11", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 3));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_12", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_13", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 5));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_14", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_15", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 7));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_16", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_17", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_18", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f));

    pGameObject = CStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_01", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 2));

    pGameObject = CStoneHole::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_01", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 5), 0.2f, fOffsetZ + (30.f * 4));
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 0.2f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_19", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 3));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_20", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 3));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_21", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_22", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_23", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_24", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 30.f, 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_25", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_26", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_27", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_02", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 7));

    pGameObject = CStoneHole::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_02", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 2), 0.2f, fOffsetZ + (30.f * 7));
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 0.2f, fOffsetZ + (30.f * 7));

    pGameObject = CStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_03", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 3));

    pGameObject = CStoneHole::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_03", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 5), 0.2f, fOffsetZ + (30.f * 7));
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 0.2f, fOffsetZ + (30.f * 7));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_28", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_29", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 2));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_30", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 3));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_31", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 4));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_32", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 5));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_33", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_34", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_35", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_36", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 6));


    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_37", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_38", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_39", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_40", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_41", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_42", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_43", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 6));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_44", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * -1), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_45", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * -2), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_46", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * -3), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_47", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * -4), 16.f, fOffsetZ + (30.f * 8));

    pGameObject = CRock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_48", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * -5), 16.f, fOffsetZ + (30.f * 8));
#pragma endregion


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CJungleForestStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CDefaultUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Default_UI", pGameObject), E_FAIL);

    pGameObject = CPowerUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Power_UI", pGameObject), E_FAIL);

    pGameObject = CSpeedUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Speed_UI", pGameObject), E_FAIL);

    pGameObject = CStoreUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Store_UI", pGameObject), E_FAIL);

    pGameObject = CMapName::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);

    dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"정글 숲"); 
    dynamic_cast<CMapName*>(pGameObject)->CallName();       
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);
    
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

CJungleForestStage* CJungleForestStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CJungleForestStage* pJungleForestStage = new CJungleForestStage(pGraphicDev);

    return pJungleForestStage;  
}

void CJungleForestStage::Free()
{
    Engine::CScene::Free(); 
}
