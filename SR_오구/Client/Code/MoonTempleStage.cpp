#include "pch.h"
#include "MoonTempleStage.h"

CMoonTempleStage::CMoonTempleStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{

}

CMoonTempleStage::~CMoonTempleStage()
{

}

HRESULT CMoonTempleStage::Ready_Scene()
{
	StopAll();
	PlayVideo(g_hWnd, L"../Bin/Resource/Video/CutScene_23_MoonTempleEntrance.wmv");

	FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	CManagement::GetInstance()->m_imap_stage = 2;
	// 
	//D3DLIGHT9		tLightInfo; 
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9)); 
	//
	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL; 
	//
	//tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };    
	//tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };   
	//tLightInfo.Ambient = { 0.5f, 0.5f, 0.5f, 0.2f };    
	//tLightInfo.Direction = { 1.f, -1.f, 1.f };
	//    
	//m_pGraphicDev->SetLight(0, &tLightInfo);    
	//m_pGraphicDev->LightEnable(0, TRUE);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

void CMoonTempleStage::LateReady_Scene()
{
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(1000.f, 16.f, 150.f);

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

	Engine::CScene::LateReady_Scene();
}

_int CMoonTempleStage::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (m_bVideoPlaying == false)
	{

		Engine::PlayBGM(L"BGM_11_MoonForestFieldLight_Haze.wav", 0.5f);
		m_bVideoPlaying = true;
	}

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

void CMoonTempleStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CMoonTempleStage::Render_Scene()
{
}

void CMoonTempleStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();

	DWORD bytesRead = 1;
	HANDLE hFile = CreateFile(L"../Map/MoonTempleReal7.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CMoonTempleStage::Ready_LightInfo()
{
	return S_OK;
}

HRESULT CMoonTempleStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMoonTempleStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	pGameObject = CMoonTempleMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonForestMap", pGameObject), E_FAIL);

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");

	float fOffsetX = 500.f;
	float fOffsetZ = 500.f;

	pGameObject = CNue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Nue", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CHeartFruit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CHeartFruit*>(pGameObject)->Set_Pos(_vec3{ 1300.f, 20.f, 1400.f });
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HeartFruit", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);


	pGameObject = CB52Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);
	static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();
	static_cast<CB52Bomber*>(pGameObject)->Set_Player(PlayerObj);

	fOffsetX = 1000.f;
	fOffsetZ = 875.f;

#pragma region Pipe Game

	pGameObject = CPipeBoard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PipeBoard", pGameObject), E_FAIL);
	CPipeBoard* pPipeBoard = static_cast<CPipeBoard*>(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 180.f, fOffsetZ + 210, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 0);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 210.f, fOffsetZ + 210, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 0);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 2), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 3), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f * 5));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ + 210 + (30.f * 6));


	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 2), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 3), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_14", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_17", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_18", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f * 5));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ + 210 + (30.f * 6));

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_02", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 15.f, fOffsetZ + 430, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 1);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_03", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 15.f, fOffsetZ + 430, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 1);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_40", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 15.f - (30.f), 16.f, fOffsetZ + 430);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_41", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 15.f + (30.f), 16.f, fOffsetZ + 430);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_42", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 15.f - (30.f * 2), 16.f, fOffsetZ + 430);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_43", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 15.f + (30.f * 2), 16.f, fOffsetZ + 430);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_04", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 180.f, fOffsetZ + 210, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 2);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_05", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 210.f, fOffsetZ + 210, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 2);


	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_06", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 180.f, fOffsetZ - 250.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 3);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_07", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX - 210.f, fOffsetZ - 250.f, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 3);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_20", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_21", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 2), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_22", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 3), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_23", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_24", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ - 250.f - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_25", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ - 250.f - (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_26", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ - 250.f - (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_27", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 180.f + (30.f), 16.f, fOffsetZ - 250.f - (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_28", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 4), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_29", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - 210.f - (30.f * 4), 16.f, fOffsetZ - 250 + 30.f);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_08", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 180.f, fOffsetZ - 250, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 4);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_StoneBlock_09", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 210.f, fOffsetZ - 250, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pPipeBoard->Push_StoneBlock(pGameObject, 4);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_30", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_31", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 2), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_32", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 3), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_33", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_34", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ - 250.f - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_35", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ - 250.f - (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_36", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ - 250.f - (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_37", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 180.f - (30.f), 16.f, fOffsetZ - 250.f - (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_38", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 4), 16.f, fOffsetZ - 250.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_Rock_39", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 210.f + (30.f * 4), 16.f, fOffsetZ - 250 + 30.f);

#pragma endregion

#pragma region Plant Puzzle
	fOffsetX = 15.f;
	fOffsetZ = 292.f;

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_08", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 2.f));

	pGameObject = CLightFlower::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_LightFlower_00", pGameObject), E_FAIL);
	CLightFlower* pLightFlower = static_cast<CLightFlower*>(pGameObject);
	pLightFlower->Init_Pos(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 3.f));

	pGameObject = CFlowerGlow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_FlowerGlow_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 15.f, fOffsetZ + (30.f * 3.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 4), 0.1f, fOffsetZ + (30.f * 3.f));
	pLightFlower->Set_Glow(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_StonePedestal_00", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 2));
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pLightFlower);
	pLightFlower->Set_Pedestal(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_09", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 6.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_14", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 6.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 7.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 7.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_17", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_18", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_20", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_00", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f * 7), fOffsetZ + (30.f * 2.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_01", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f * 10), fOffsetZ + (30.f * 2.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_21", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_22", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 5.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_23", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ + (30.f * 4.f));

	pGameObject = CLightFlower::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_LightFlower_01", pGameObject), E_FAIL);
	pLightFlower = static_cast<CLightFlower*>(pGameObject);
	pLightFlower->Init_Pos(fOffsetX + (30.f * 12), fOffsetZ + (30.f * 3.f));

	pGameObject = CFlowerGlow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_FlowerGlow_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 15.f, fOffsetZ + (30.f * 3.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 12), 0.1f, fOffsetZ + (30.f * 3.f));
	pLightFlower->Set_Glow(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_StonePedestal_01", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX + (30.f * 13), fOffsetZ + (30.f * 4.f));
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pLightFlower);
	pLightFlower->Set_Pedestal(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_24", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 13), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_25", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 14), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_26", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 15), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_27", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 16), 16.f, fOffsetZ + (30.f * 3.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_28", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 7.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_29", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 7.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_30", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 8.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_02", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(true, fOffsetX + (30.f * 13), fOffsetZ + (30.f * 8.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_03", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(true, fOffsetX + (30.f * 15), fOffsetZ + (30.f * 8.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_04", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(true, fOffsetX + (30.f * 13), fOffsetZ + (30.f * 12.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_05", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(true, fOffsetX + (30.f * 15), fOffsetZ + (30.f * 12.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_06", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f * 12), fOffsetZ + (30.f * 13.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_07", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f * 12), fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_31", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 10.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_32", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 14.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_33", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_34", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_35", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_36", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CLightFlower::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_LightFlower_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 30.f, fOffsetZ + (30.f * 15.f));
	pLightFlower = static_cast<CLightFlower*>(pGameObject);
	pLightFlower->Init_Pos(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 15.f));

	pGameObject = CFlowerGlow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_FlowerGlow_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 15.f, fOffsetZ + (30.f * 15.f));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 5), 0.1f, fOffsetZ + (30.f * 15.f));
	pLightFlower->Set_Glow(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_StonePedestal_02", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 14.f));
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pLightFlower);
	pLightFlower->Set_Pedestal(pGameObject);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_37", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_38", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 14.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_39", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 13.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_40", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 12.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_41", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 11.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_42", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 10.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_08", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f), fOffsetZ + (30.f * 14.f));

	pGameObject = CFenceThorn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Fence_09", pGameObject), E_FAIL);
	static_cast<CFenceThorn*>(pGameObject)->Init(false, fOffsetX + (30.f), fOffsetZ + (30.f * 16.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_44", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 13.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_45", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 13.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_46", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 14.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_47", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 15.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_48", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 16.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_49", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_50", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_51", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 1), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_52", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_53", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_54", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_55", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 9.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_56", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 11.f));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Stone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 15.f, fOffsetZ + (30.f * 6));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 12), 0.2f, fOffsetZ + (30.f * 6));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 0.2f, fOffsetZ + (30.f * 6));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 2));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 2));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 3));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 3));

	pGameObject = CStoneHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 4));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 4));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Stone_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 15.f, fOffsetZ + (30.f * 5));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Stone_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 15.f, fOffsetZ + (30.f * 2));

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Stone_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 15.f, fOffsetZ + (30.f * 9));

	pGameObject = CShootingPlant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ShootingPlant_00", pGameObject), E_FAIL);
	static_cast<CShootingPlant*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 8), fOffsetZ + (30.f * 2));

	pGameObject = CShootingPlant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ShootingPlant_01", pGameObject), E_FAIL);
	static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(1);
	static_cast<CShootingPlant*>(pGameObject)->Set_Dir(true);
	static_cast<CShootingPlant*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 9), fOffsetZ + (30.f * 2));

	pGameObject = CShootingPlant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ShootingPlant_02", pGameObject), E_FAIL);
	static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(4);
	static_cast<CShootingPlant*>(pGameObject)->Set_Dir(true);
	static_cast<CShootingPlant*>(pGameObject)->Init_Pos(fOffsetX, fOffsetZ + (30.f * 15));

	pGameObject = CShootingPlant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ShootingPlant_03", pGameObject), E_FAIL);
	static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(4);
	static_cast<CShootingPlant*>(pGameObject)->Set_Dir(true);
	static_cast<CShootingPlant*>(pGameObject)->Init_Pos(fOffsetX, fOffsetZ + (30.f * 14));

	pGameObject = CShootingPlant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ShootingPlant_04", pGameObject), E_FAIL);
	static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(4);
	static_cast<CShootingPlant*>(pGameObject)->Set_Dir(true);
	static_cast<CShootingPlant*>(pGameObject)->Init_Pos(fOffsetX, fOffsetZ + (30.f * 16));

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ColorStone_00", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 15), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ColorStone_01", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_ColorStone_02", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(0);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 6));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_57", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ - (30.f * 2.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_58", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_59", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + 30.f);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_60", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ);

	pGameObject = CBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_BugStatue_01", pGameObject), E_FAIL);
	static_cast<CBugStatue*>(pGameObject)->Set_Group(pPipeBoard);
	static_cast<CBugStatue*>(pGameObject)->SetPlayer(static_cast<CPlayer*>(PlayerObj));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 20.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_61", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 4), 16.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_62", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 5));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_63", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ + (30.f * 5));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_64", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_65", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_66", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_67", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_68", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_69", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_70", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_71", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_72", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_73", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_74", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_75", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ);


	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_76", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_77", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 13), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_78", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 14), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_79", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 15), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_80", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 16), 16.f, fOffsetZ);	

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_81", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_82", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_83", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_84", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_85", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_86", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_87", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_88", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_89", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_90", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 13), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_91", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 14), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_92", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 15), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_93", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 16), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_94", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_95", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_96", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_97", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_98", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_99", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_100", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_101", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_102", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 12), 16.f, fOffsetZ + (30.f) * 18);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_103", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 13), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_104", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 14), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_105", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 15), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_106", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 16), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_107", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_108", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_109", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_110", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_111", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_112", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_113", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_114", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 18));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_115", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 17));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_116", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 16));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_117", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 15));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_118", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 14));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_119", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_120", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 12));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_121", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 11));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_122", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonLeft_Rock_123", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f), 16.f, fOffsetZ + (30.f * 18));
#pragma endregion

#pragma region Music Room

	fOffsetX = 1580.f;
	fOffsetZ = 180.f;

	pGameObject = CMusicStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MusicStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 15.f, fOffsetZ + (30.f * 4));
	CMusicStatue* pMusicStatue = static_cast<CMusicStatue*>(pGameObject);

	fOffsetZ = 300.f;

	pGameObject = CBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_BugStatue_02", pGameObject), E_FAIL);
	static_cast<CBugStatue*>(pGameObject)->Set_Group(pPipeBoard);
	static_cast<CBugStatue*>(pGameObject)->SetPlayer(static_cast<CPlayer*>(PlayerObj));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 20.f, fOffsetZ + (30.f * 8));

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 6), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_01", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 8), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_02", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 9), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_03", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_04", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 7), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_05", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3), fOffsetZ + (30.f * 6), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_06", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_07", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_08", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 6), fOffsetZ + (30.f * 710), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_09", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 10), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_10", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 9), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_11", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 8), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_12", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 7), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_13", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 7), fOffsetZ + (30.f * 6), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_14", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 6), fOffsetZ + (30.f * 6), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_15", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 6), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Music_StoneBlock_16", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 6), true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(3);
	pMusicStatue->Add_StoneBlock(pGameObject);

	fOffsetZ = 150.f;

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 20.f, fOffsetZ + (30.f * 2));
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(0);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 20.f, fOffsetZ + (30.f));
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(1);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 20.f, fOffsetZ);
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(2);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 20.f, fOffsetZ);
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(3);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 20.f, fOffsetZ + (30.f));
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(4);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CNoteStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 20.f, fOffsetZ + (30.f * 2));
	static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(5);
	static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_00", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX, fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_01", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 2), fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_02", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_03", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 6), fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_04", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 8), fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

	pGameObject = CFirePit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_05", pGameObject), E_FAIL);
	static_cast<CFirePit*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 10), fOffsetZ + (30.f * 6));
	pMusicStatue->Add_FirePit(pGameObject);

#pragma endregion

#pragma region Monster Room
	fOffsetX = 150.f;
	fOffsetZ = 1250.f;

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_04", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_05", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_06", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_07", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f * 4));

	pGameObject = CBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe_BugStatue_00", pGameObject), E_FAIL);
	static_cast<CBugStatue*>(pGameObject)->Set_Group(pPipeBoard);
	static_cast<CBugStatue*>(pGameObject)->SetPlayer(static_cast<CPlayer*>(PlayerObj));
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_10", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_11", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ);

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_12", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ - (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_13", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ - (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_14", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ - (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 3), 16.f, fOffsetZ - (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 4));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_17", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ + (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_18", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 2));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_19", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ + (30.f));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_20", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 8), 16.f, fOffsetZ - (30.f * 3));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_21", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ - (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_22", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ - (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_23", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ - (30.f * 6));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_24", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ - (30.f * 7));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_25", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ - (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_26", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ - (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_27", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 12), 16.f, fOffsetZ - (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_28", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ - (30.f * 10));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_29", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ - (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_30", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 10), 16.f, fOffsetZ - (30.f * 9));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_31", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 10), 16.f, fOffsetZ - (30.f * 8));

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rock_32", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ - (30.f * 7));


	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ - (30.f * 7));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 9), 16.f, fOffsetZ - (30.f * 7));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 2), 16.f, fOffsetZ - (30.f * 10));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 16.f, fOffsetZ + (30.f * 7));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_04", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ - (30.f * 2));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_05", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ - (30.f * 13));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_06", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 11), 16.f, fOffsetZ + (30.f * 6));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	//pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast_07", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX - (30.f * 5), 16.f, fOffsetZ - (30.f * 7));
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);


#pragma endregion


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMoonTempleStage::Ready_Layer_UI(const _tchar* pLayerTag)
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
	dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"달의 신전");
	dynamic_cast<CMapName*>(pGameObject)->Set_Time(14.f);
	dynamic_cast<CMapName*>(pGameObject)->CallName();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

	pGameObject = CAddHP::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AddHP_UI", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CMoonTempleStage* CMoonTempleStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoonTempleStage* pMoonTempleStage = new CMoonTempleStage(pGraphicDev);

	return pMoonTempleStage;
}

void CMoonTempleStage::Free()
{
	Engine::CScene::Free();
}
