#include "pch.h"
#include "SunTempleStage.h"
#include "Export_Utility.h"

CSunTempleStage::CSunTempleStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{
}

CSunTempleStage::~CSunTempleStage()
{
}

HRESULT CSunTempleStage::Ready_Scene()
{
	StopAll();	
	PlayVideo(g_hWnd, L"../Bin/Resource/Video/CutScene_22_SunTempleEntrance.wmv");	

    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	CManagement::GetInstance()->m_imap_stage = 2;	
    //m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    return S_OK;
}

void CSunTempleStage::LateReady_Scene()
{
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(1030.f, 30.f, 180.f);

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

	Engine::CScene::LateReady_Scene();
}

_int CSunTempleStage::Update_Scene(const _float& fTimeDelta)
{
	if (m_bVideoPlaying == false)
	{
		Engine::PlayBGM(L"BGM_8_JungleDungeon.wav", 0.5f);
		m_bVideoPlaying = true;
	}

    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);


	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));
	if(player->IsNextSceneOn())
    //if (GetAsyncKeyState('B') & 0x8000)
    {
		

        Engine::CScene* pStage3 = CMoonForestStage::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pStage3, -1);

        FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
        //pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

        return 0;
    }

    return iExit;
}

void CSunTempleStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CSunTempleStage::Render_Scene()
{
}

void CSunTempleStage::init()
{

    Engine::CLayer* pLayer = CLayer::Create();

    DWORD bytesRead = 1;
    HANDLE hFile = CreateFile(L"../Map/SunTempleReal6.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CSunTempleStage::Ready_LightInfo()
{
    return S_OK;
}

HRESULT CSunTempleStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{

    Engine::CLayer* pLayer = CLayer::Create();  
    NULL_CHECK_RETURN(pLayer, E_FAIL);  

    m_mapLayer.insert({ pLayerTag, pLayer });   

    return S_OK;
}

HRESULT CSunTempleStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{

    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

   
    pGameObject = CSunTempleMap::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL); 
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SunTempleMap", pGameObject), E_FAIL);		

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");

	_float fOffsetX = 1030;
	_float fOffsetZ = 500;
	
	//숭숭이
	pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExpressMonkey", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CExpressTicket::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_vec3 ItemPos = { 1030.f, 30.f, 1800.f }; // 이거만 수정 하면 될거같은디 
	dynamic_cast<CExpressTicket*>(pGameObject)->Set_DropItem(ItemPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_ExpressTicket", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

	pGameObject = CB52Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);
	static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();
	//static_cast<CB52Bomber*>(pGameObject)->Set_Player(PlayerObj);


#pragma region Bug Statue Puzzle
	pGameObject = CBugStatuePuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatuePuzzle_00", pGameObject), E_FAIL);
	CBugStatuePuzzle* pBugStatuePuzzle = static_cast<CBugStatuePuzzle*>(pGameObject);

#pragma endregion

#pragma region Combination Puzzle

	pGameObject = CCombinationPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CCombinationPuzzle_00", pGameObject), E_FAIL);
	CCombinationPuzzle* pCombination = static_cast<CCombinationPuzzle*>(pGameObject);

	pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 100.f, 20.f, fOffsetZ - 40.f);
	static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	pCombination->Add_Statue(pGameObject);

	pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 20.f, fOffsetZ);
	static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	pCombination->Add_Statue(pGameObject);

	pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 100.f, 20.f, fOffsetZ - 40.f);
	static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	pCombination->Add_Statue(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 30.f, fOffsetZ + 190.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	pCombination->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX, fOffsetZ + 190.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	pCombination->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_StoneBlock_02", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 30.f, fOffsetZ + 190.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	pCombination->Add_StoneBlock(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 4), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + 190.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 4), 16.f, fOffsetZ + 220.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + 220.f);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 15.f, fOffsetZ + 350.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	pBugStatuePuzzle->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 15.f, fOffsetZ + 350.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	pBugStatuePuzzle->Add_StoneBlock(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 45.f, 16.f, fOffsetZ + 350.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 45.f, 16.f, fOffsetZ + 350.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 75.f, 16.f, fOffsetZ + 350.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Com_Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 75.f, 16.f, fOffsetZ + 350.f);

#pragma endregion

#pragma region Crystal type 0 Puzzle
	fOffsetX = 300;
	fOffsetZ = 300;

	pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f, fOffsetZ);
	CMonkeyStatue* pMS = static_cast<CMonkeyStatue*>(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX, fOffsetZ + (30.f * 4.f), true);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3.f), fOffsetZ + (30.f * 6.f), false);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ);

	pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 22.f, fOffsetZ);
	pMS = static_cast<CMonkeyStatue*>(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_02", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 2), fOffsetZ + (30.f), true);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_03", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX, fOffsetZ + (30.f), false);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 5.f), 0.2f, fOffsetZ);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5.f), 0.2f, fOffsetZ);

	pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7.f), 22.f, fOffsetZ);
	pMS = static_cast<CMonkeyStatue*>(pGameObject);


	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_04", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 5.f), fOffsetZ + (30.f * 2.f), false);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_05", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX, fOffsetZ + (30.f * 2.f), true);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 4.f));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 7.f), 0.2f, fOffsetZ + (30.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7.f), 0.2f, fOffsetZ + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 2.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f));

	pGameObject = CBreakableStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BreakableStone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 4.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_14", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_17", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_18", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f, fOffsetZ + (30.f * 8.f));
	pMS = static_cast<CMonkeyStatue*>(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_06", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 2), fOffsetZ + (30.f * 8.f), false);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_07", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 8.f), true);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5.f), 16.f, fOffsetZ + (30.f * 6.f));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 7.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 7.f));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 3), 0.2f, fOffsetZ + (30.f * 4.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 0.2f, fOffsetZ + (30.f * 4.f));

	pGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pGameObject), E_FAIL);
	CCrystalPuzzle* pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6.f), 18.f, fOffsetZ + (30.f * 3.f));
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 6), fOffsetZ + (30.f * 4));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 18.f, fOffsetZ + (30.f * 3));
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 7), fOffsetZ + (30.f * 4));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle_01", pGameObject), E_FAIL);
	pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6.f), 18.f, fOffsetZ + (30.f * 6.f));
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ + (30.f * 6));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 18.f, fOffsetZ + (30.f * 7));
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_03", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ + (30.f * 7));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 20.f, fOffsetZ + (30.f * 9));
	static_cast<CBugStatue*>(pGameObject)->Set_Group(pBugStatuePuzzle);
	static_cast<CBugStatue*>(pGameObject)->Set_ImageID(2);
	pBugStatuePuzzle->Add_Statue(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_20", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.F));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_21", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_22", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ - (30.F));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_23", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_24", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_25", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_26", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 5));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_27", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_28", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_29", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ);

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_30", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_31", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_32", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_33", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_34", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_35", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_36", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_37", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_38", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_39", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_40", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_41", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_42", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_43", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_44", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_45", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_46", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_47", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 10));



#pragma endregion

	fOffsetX = 880;
	fOffsetZ = 1150;
#pragma region Catapult Puzzle

	pGameObject = CCatapult::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_00", pGameObject), E_FAIL);
	static_cast<CCatapult*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 3));
	static_cast<CCatapult*>(pGameObject)->SetPlayer(static_cast<CPlayer*>(PlayerObj));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Stone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 15.f, fOffsetZ + (30.f));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Stone_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 15.f, fOffsetZ + (30.f * 3));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Stone_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 15.f, fOffsetZ + (30.f * 5));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Stone_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 15.f, fOffsetZ + (30.f));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Stone_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 15.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CBreakableStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_BreakableStone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CBreakableStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_BreakableStone_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_14", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CBreakableStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_BreakableStone_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_17", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_18", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_20", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_21", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_22", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_23", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_24", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_25", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_26", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_27", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_28", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_29", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_30", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_31", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 13));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_32", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_33", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_34", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_35", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_36", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 12));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_37", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 12));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_38", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 12));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_39", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_40", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_41", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_42", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_43", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 9));


#pragma endregion


	fOffsetX = 1590;
	fOffsetZ = 250;
#pragma region Scale Puzzle


	pGameObject = CScale::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CScale_00", pGameObject), E_FAIL);
	static_cast<CScale*>(pGameObject)->Init_Position(170.f + fOffsetX, 150.f + fOffsetZ);

	pGameObject = CWeightComparator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WeightComparator_00", pGameObject), E_FAIL);
	static_cast<CWeightComparator*>(pGameObject)->Init(170.f + fOffsetX, 30.f + fOffsetZ);
	CWeightComparator* PWeightComparator = static_cast<CWeightComparator*>(pGameObject);

	fOffsetX = 1575;
	fOffsetZ = 250;

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 6), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	PWeightComparator->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(5);
	PWeightComparator->Add_StoneBlock(pGameObject);

	pGameObject = CBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 20.f, fOffsetZ + (30.f * 12));
	static_cast<CBugStatue*>(pGameObject)->Set_Group(pBugStatuePuzzle);
	static_cast<CBugStatue*>(pGameObject)->Set_ImageID(2);
	pBugStatuePuzzle->Add_Statue(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 13), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Scale_Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 10));


#pragma endregion

	fOffsetX = 960;
	fOffsetZ = 1720;
#pragma region Crytal Puzzle 1

	pGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle1", pGameObject), E_FAIL);
	pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 18.f, fOffsetZ + (30.f * 3.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_04", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX, fOffsetZ + (30.f));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 18.f, fOffsetZ + (30.f * 2.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(2);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_05", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX, fOffsetZ);
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 18.f, fOffsetZ + (30.f * 3.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(1);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_06", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f), fOffsetZ + (30.f));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 18.f, fOffsetZ + (30.f * 2.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_07", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f), fOffsetZ);
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 18.f, fOffsetZ + (30.f * 3.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(2);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_08", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 2), fOffsetZ + (30.f));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 18.f, fOffsetZ + (30.f * 2.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(1);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_09", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 2), fOffsetZ);
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 18.f, fOffsetZ + (30.f * 3.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_10", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 3), fOffsetZ + (30.f));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 18.f, fOffsetZ + (30.f * 2.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(2);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_11", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 3), fOffsetZ);
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 18.f, fOffsetZ + (30.f * 3.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(1);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_12", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ + (30.f));
	pCrystalPuzzle->Add_PressBlock(pGameObject);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 18.f, fOffsetZ + (30.f * 2.f));
	static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	pCrystalPuzzle->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_13", pGameObject), E_FAIL);
	static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ);
	pCrystalPuzzle->Add_PressBlock(pGameObject);


#pragma endregion

    m_mapLayer.insert({ pLayerTag, pLayer });   

    return S_OK;
}

HRESULT CSunTempleStage::Ready_Layer_UI(const _tchar* pLayerTag)
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
	dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"태양의 신전");
	dynamic_cast<CMapName*>(pGameObject)->Set_Time(15.f);
	dynamic_cast<CMapName*>(pGameObject)->CallName();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

CSunTempleStage* CSunTempleStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSunTempleStage* pSunTempleStage = new CSunTempleStage(pGraphicDev);    

    return pSunTempleStage; 
}

void CSunTempleStage::Free()
{
    Engine::CScene::Free(); 
}
