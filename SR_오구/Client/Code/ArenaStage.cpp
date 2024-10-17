#include "pch.h"
#include "ArenaStage.h"
#include "TownStage.h"

CArenaStage::CArenaStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{

}

CArenaStage::~CArenaStage()
{
}

HRESULT CArenaStage::Ready_Scene()
{
    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

    Engine::StopSound(SOUND_BGM);
    Engine::PlayBGM(L"BGM_55_HarborArena.wav", 0.7f);

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    

    return S_OK;
}

void CArenaStage::LateReady_Scene()
{
    CPlayer* player = dynamic_cast<CPlayer*>(
        Get_GameObject(L"Layer_GameLogic", L"Player"));

    dynamic_cast<CTransform*>(
        player->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Set_Pos(600.f, 16.f, 800.f);

    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

    Engine::CScene::LateReady_Scene();
}

_int CArenaStage::Update_Scene(const _float& fTimeDelta)
{
    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

    if (GetAsyncKeyState('M') & 0x8000)
    {
        Engine::CScene* pStage3 = CTownStage::Create(m_pGraphicDev);    
        NULL_CHECK_RETURN(pStage3, -1);

        FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
        //pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

        return 0;
    }


    return iExit;
}

void CArenaStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CArenaStage::Render_Scene()
{
}

void CArenaStage::init()
{
    Engine::CLayer* pLayer = CLayer::Create();

    DWORD bytesRead = 1;
    HANDLE hFile = CreateFile(L"../Map/ArenaRealMap.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CArenaStage::Ready_LightInfo()
{
    return S_OK;
}

HRESULT CArenaStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CArenaStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    //여기 수정해야하고
    pGameObject = CArenaMap::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AreanMap", pGameObject), E_FAIL);


    pGameObject = CB52Bomber::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);
    static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();

    //1006
    pGameObject = CRhino::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rhino", pGameObject), E_FAIL);
    pGameObject->SetObjectKey(L"Rhino");
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

    pGameObject = CMCRabbit::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPCRabbit", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CQuestLion::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lion", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CRhinoMaster::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoArm", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CKnightCat::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cat", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CKnightDog::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dog", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone1", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone2", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone3", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone4", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);


    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone5", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

    pGameObject = CRhinoStone::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RhinoStone6", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);



    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    _vec3 vNPCPos = { 850.f, 20.f, 500.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide1", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    vNPCPos = { 860.f, 20.f, 450.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide1", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    vNPCPos = { 880.f, 20.f, 450.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide2", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    vNPCPos = { 860.f, 20.f, 400.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide2", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    vNPCPos = { 860.f, 20.f, 800.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide3", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    vNPCPos = { 850.f, 20.f, 300.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide3", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Dir();
    vNPCPos = { 300.f, 20.f, 700.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide4", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Dir();
    vNPCPos = { 300.f, 20.f, 750.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide4", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Dir();
    vNPCPos = { 170.f, 20.f, 550.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide5", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Dir();
    vNPCPos = { 140.f, 20.f, 430.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide6", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CMoleSide::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Dir();
    vNPCPos = { 100.f, 20.f, 400.f };
    dynamic_cast<CMoleSide*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoleSide7", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Dir();
    vNPCPos = { 200.f, 20.f, 650.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide5", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

    pGameObject = CRabbitLeft::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Dir();
    vNPCPos = { 140.f, 20.f, 500.f };
    dynamic_cast<CRabbitLeft*>(pGameObject)->Set_Pos(vNPCPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CheerRabbitSide6", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CArenaStage::Ready_Layer_UI(const _tchar* pLayerTag)
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

    pGameObject = CFightUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fight_UI", pGameObject), E_FAIL);

    pGameObject = CVictoryUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Victory_UI", pGameObject), E_FAIL);

    pGameObject = CMapName::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"투기장");
    dynamic_cast<CMapName*>(pGameObject)->CallName();
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

CArenaStage* CArenaStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CArenaStage* pArenaStage = new CArenaStage(pGraphicDev);

    return pArenaStage;
}

void CArenaStage::Free()
{
    Engine::CScene::Free();
}
