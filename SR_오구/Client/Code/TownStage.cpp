#include "pch.h"
#include "TownStage.h"
#include "WorldHeartStage.h"

CTownStage::CTownStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{
}

CTownStage::~CTownStage()
{
}

HRESULT CTownStage::Ready_Scene()
{

    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	CManagement::GetInstance()->m_imap_stage = 0; 

	StopAll();
	Engine::PlayBGM(L"BGM_33_HarborTown.wav", 0.5f);

    return S_OK;
}

void CTownStage::LateReady_Scene()
{
	_vec3 pos(965.f, 16.f, 120.f);
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(pos);

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

	Engine::CScene::LateReady_Scene();

	player->GetCamera()->WalkTo2(pos, 5.f, _vec3(965.f, 30.f, 820.f));

}

_int CTownStage::Update_Scene(const _float& fTimeDelta)
{
    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);


	if (GetAsyncKeyState('B') & 0x8000)
	{
		Engine::CScene* pStage3 = CElectriceelBossStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage3, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
		//pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

		return 0;
	}

	if (GetAsyncKeyState('N') & 0x8000)
	{
		Engine::CScene* pStage3 = CArenaStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage3, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
		//pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

		return 0;
	}

	if (GetAsyncKeyState('H') & 0x8000)
	{
		Engine::CScene* pStage2 = CWorldHearStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

    return iExit;
}

void CTownStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CTownStage::Render_Scene()
{
}

void CTownStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();

	DWORD bytesRead = 1;
	HANDLE hFile = CreateFile(L"../Map/harborReal.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CTownStage::Ready_LightInfo()
{
	return S_OK;
}

HRESULT CTownStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTownStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CTown::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TownMap", pGameObject), E_FAIL);

	//1001
	pGameObject = CSeaLion::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_SeaLion", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CHarborcat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_HarborCat", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CHarborMeer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_HarborMeerCat", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CPanda::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Panda", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);
	m_mapLayer.insert({ pLayerTag, pLayer });
	
	//숭숭이
	pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExpressMonkey1", pGameObject), E_FAIL);
	dynamic_cast<CExpressMonkey*>(pGameObject)->Set_Pos(_vec3{ 1100.f, 30.f, 451.f });
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


	return S_OK;
}

HRESULT CTownStage::Ready_Layer_UI(const _tchar* pLayerTag)
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
	dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"항구");
	dynamic_cast<CMapName*>(pGameObject)->CallName();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CTownStage* CTownStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTownStage* pTownStage = new CTownStage(pGraphicDev);

	return pTownStage;
}

void CTownStage::Free()
{
	Engine::CScene::Free();
}
