#include "pch.h"
#include "Stage.h"
#include "Export_Utility.h"
#include "WorldHeartStage.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);	
	FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	StopAll();		
	Engine::PlayBGM(L"BGM_1_CentralArea.wav", 0.5f);	

	//Play_Sound(L"BGM_19_TutorialAmbience.wav", SOUND_EFFECT, 1.f);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(110.f, 16.f, 200.f);

	return S_OK;

}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (GetAsyncKeyState('M') & 0x8000)
	{
		Engine::CScene* pStage2 = CWorldHearStage::Create(m_pGraphicDev);	
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

	if (GetAsyncKeyState('8') & 0x8000)
	{
		Engine::CScene* pStage3 = CTownStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage3, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
		//pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

		return 0;
	}

	if (GetAsyncKeyState('9') & 0x8000)
	{
		Engine::CScene* pStage2 = CArenaStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

	if (GetAsyncKeyState('7') & 0x8000)
	{
		Engine::CScene* pStage2 = CMoonTempleStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

	return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{


}


HRESULT CStage::Ready_LightInfo()
{

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f,1.f,1.f,1.f };
	tLightInfo.Specular = { 1.f,1.f,1.f,1.f };
	tLightInfo.Ambient = { 1.f,1.f,1.f,1.f };
	tLightInfo.Direction = { 1.F, -1.f, 1.f };

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr; 

	_vec3 Eye = { 0.f, 0.f, 0.f };
	_vec3 At  = {0.f, 1.f, 1.f };
	_vec3 Up = { 0.f, 1.f, 0.f };

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &Eye, &At, &Up);	

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);
	// 
	//여기다가 맵 오브젝트들 넣기 파일 읽어오는기능 
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	pGameObject = CMap::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);		
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);	
	 
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);
	pGameObject->SetObjectKey(L"Player");

	pGameObject = CPlayerInteractionBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerInteractionBox", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);
	pGameObject->SetObjectKey(L"PlayerInteractionBox");

	pGameObject = CSwingEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SwingEffect", pGameObject), E_FAIL);

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");
	CGameObject* InteractionObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"PlayerInteractionBox");
	dynamic_cast<CPlayerInteractionBox*>(InteractionObj)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));
	CGameObject* SwingEffect = pLayer->Get_GameObject(L"Layer_GameLogic", L"SwingEffect");
	dynamic_cast<CSwingEffect*>(SwingEffect)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));

	//0926
	//pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_ExMonkey", pGameObject), E_FAIL);
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


	//1002
	pGameObject = CSheep::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Sheep", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CPig::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Pig", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CCow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Cow", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	//1003
	/*pGameObject = CBranch::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_vec3 ItemPos = { 300.f, 20.f, 700.f };
	dynamic_cast<CBranch*>(pGameObject)->Set_DropItem(ItemPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_Branch", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);*/

	CScene* pScene = CManagement::GetInstance()->GetCurScenePtr();

	////1010
	//pGameObject = CGrass::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass1", pGameObject), E_FAIL);
	//dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 20.f, 180.f);
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NATURAL, pGameObject);	


	////1010	
	//pGameObject = CGrass2::Create(m_pGraphicDev);		
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass2", pGameObject), E_FAIL);	
	//dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(320.f, 20.f, 180.f);
	//CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NATURAL, pGameObject);
	

	pGameObject = CB52Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);	
	static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDefaultUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Default_UI", pGameObject), E_FAIL);

	pGameObject = CInvenUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inven_UI", pGameObject), E_FAIL);

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuickSlot_UI", pGameObject), E_FAIL);

	pGameObject = CQuestUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quest_UI", pGameObject), E_FAIL);

	pGameObject = CPowerUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Power_UI", pGameObject), E_FAIL);

	pGameObject = CSpeedUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Speed_UI", pGameObject), E_FAIL);

	pGameObject = CStoreUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Store_UI", pGameObject), E_FAIL);

	pGameObject = CAddHP::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AddHP_UI", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pStage = new CStage(pGraphicDev);

	//if (FAILED(pStage->Ready_Scene()))
	//{
	//	Safe_Release(pStage);
	//	MSG_BOX("Stage Create Failed");
	//	return nullptr;
	//}
	return pStage;
}

void CStage::Free()
{
	Engine::CScene::Free();
}


void CStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();	
	
	DWORD bytesRead = 1; 
	HANDLE hFile = CreateFile(L"../Map/realmap10.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		while (bytesRead > 0)
		{
			//wstring ObjectName = L"";
			//WCHAR* buffer = new WCHAR[256];
			WCHAR buffer[256] = { 0 };	
			bytesRead = 100;

			//오브젝트키 (오브젝트 이름)
			if (!ReadFile(hFile, buffer, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}
			
			wstring* pObjectName = new wstring;
			*pObjectName = buffer;
		
			



			//wstring TextureName = L"";
			//WCHAR* buffer2 = new WCHAR[256];
			WCHAR buffer2[256] = { 0 };	
			bytesRead = 100;

			//텍스처키 (텍스처 이름)
			if (!ReadFile(hFile, buffer2, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}

			wstring* pTextureWstring = new wstring;
			*pTextureWstring = buffer2;

			


			int a = 4;


			D3DXMATRIX worldmatrix;
			bytesRead = 0;

			if (!ReadFile(hFile, worldmatrix, sizeof(D3DXMATRIX), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ WORLDMAXTIRX");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Rotation_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Rotation_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Sclae_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Sclae_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}


			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			if (pGameObject == nullptr)
			{
				MSG_BOX("CObject nullptr Error");
			}

			

			pGameObject->SetTextureKey((*pTextureWstring).c_str());

			//  지금 신이 변경이 안돼서 startScene을 가져와서 문제 발생 



			map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*pObjectName).c_str(), pGameObject);

			CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

			// 왜 못찾는거지?;;; 아 시발 아직도 신이 안바뀐거였음  이거 찾는 기준이 현재신기준이였음.
			CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", (*pObjectName).c_str(), L"Com_Transform"));
			//pTransform->ForGetWorldMaxtrix() = worldmatrix;

			////회전값만 이제 넣어주면 됨 ( 크기 -> 자전 -> 이동 ) 순서로  아 시발 병신같이 월드매트릭스를 넣어네
			// 월드매트릭스에 넣기 전의 크기값을 넣어줘야하네 
			pTransform->m_vScale = { Sclae_vec3.x,Sclae_vec3.y,Sclae_vec3.z };
			pTransform->Rotation(ROT_X, Rotation_vec3.x * 3.14f / 180.f);
			pTransform->Rotation(ROT_Y, Rotation_vec3.y * 3.14f / 180.f);
			pTransform->Rotation(ROT_Z, Rotation_vec3.z * 3.14f / 180.f);
			pTransform->Set_Pos(worldmatrix._41, worldmatrix._42, worldmatrix._43);

			//pTransform->m_vAngle = vec3;
			//D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
		}
		CloseHandle(hFile);
	}
	

	//9월25일 충돌관련
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::OBJECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::NPC);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PUZZLE, GROUP_TYPE::PUZZLE);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER_EFFECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::OBJECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::NATURAL);


}