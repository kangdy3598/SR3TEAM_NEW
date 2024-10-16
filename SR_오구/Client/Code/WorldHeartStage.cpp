#include "pch.h"
#include "WorldHeartStage.h"
#include "Export_Utility.h" 
#include "MoonForestStage.h"


CWorldHearStage::CWorldHearStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{


}

CWorldHearStage::~CWorldHearStage()
{



}

HRESULT CWorldHearStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	Engine::StopAll();	
	Engine::PlayBGM(L"BGM_3_JungleAreaField1.wav", 0.5f);

	CManagement::GetInstance()->m_imap_stage = 0;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void CWorldHearStage::LateReady_Scene()
{
	_vec3 pos(350.f, 30.f, 150.f);
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(pos);

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, player->GetInteractionBox());

	Engine::CScene::LateReady_Scene();

	player->GetCamera()->WalkTo2(pos, 10.f, _vec3(1900.f, 180.f, 1700.f));
}

_int CWorldHearStage::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);


	if (GetAsyncKeyState('B') & 0x8000)
	{
		Engine::CScene* pStage3 = CJungleForestStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage3, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
		//pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

		return 0;
	}


	return iExit;
}

void CWorldHearStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CWorldHearStage::Render_Scene()
{
}


void CWorldHearStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();

	DWORD bytesRead = 1;
	HANDLE hFile = CreateFile(L"../Map/WorldHeartStageReal2.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CWorldHearStage::Ready_LightInfo()
{
	return S_OK;
}

HRESULT CWorldHearStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CWorldHearStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{

	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

#pragma region Main
	pGameObject = CWorldHeartMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WorldHeartMap", pGameObject), E_FAIL);

	pGameObject = CWhiteBird::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WhiteBird", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

#pragma endregion

#pragma region MinJi
	pGameObject = CBranch::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_vec3 ItemPos = { 300.f, 20.f, 700.f };
	dynamic_cast<CBranch*>(pGameObject)->Set_DropItem(ItemPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_Branch", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);


	pGameObject = CMouse::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mouse", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


	pGameObject = CGod::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CGod*>(pGameObject)->Call_God();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"God", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CGodEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GodEffect", pGameObject), E_FAIL);

	pGameObject = CB52Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CB52Bomber", pGameObject), E_FAIL);
	static_cast<CB52Bomber*>(pGameObject)->Pre_CreateShell();
	//static_cast<CB52Bomber*>(pGameObject)->Set_Player(PlayerObj);
#pragma endregion

#pragma region Crystal Puzzle
	_float fOffsetX = 1000;
	_float fOffsetZ = 850;
/*
#pragma region Catapult Puzzle

	pGameObject = CCatapult::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_00", pGameObject), E_FAIL);
	static_cast<CCatapult*>(pGameObject)->Init_Pos(fOffsetX + (30.f * 5), fOffsetZ + (30.f * 3));
	static_cast<CCatapult*>(pGameObject)->SetPlayer(static_cast<CPlayer*>(PlayerObj));
	pGameObject->SetObjectKey(L"Player");	

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

	pGameObject = CRock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_Rock_32", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 9), 16.f, fOffsetZ + (30.f * 13));

	pGameObject = CBreakableStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_BreakableStone_03", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 12));

#pragma endregion
*/

#pragma endregion

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CWorldHearStage::Ready_Layer_UI(const _tchar* pLayerTag)
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
	dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L" 세계의 심장 ");
	dynamic_cast<CMapName*>(pGameObject)->CallName();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CWorldHearStage* CWorldHearStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWorldHearStage* pStage2 = new CWorldHearStage(pGraphicDev);

	return pStage2;
}

void CWorldHearStage::Free()
{
	Engine::CScene::Free();
}
