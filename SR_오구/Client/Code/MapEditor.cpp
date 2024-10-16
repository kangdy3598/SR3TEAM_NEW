#include "pch.h"
#include "MapEditor.h"
#include "Export_Utility.h"
#include "ImGuiManger.h"
#include "ResMgr.h"



CMapEditor::CMapEditor(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}

CMapEditor::~CMapEditor()
{
}




HRESULT CMapEditor::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);	 // 버퍼 추가 .	

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	m_pGraphicDev->SetLight(0, &tLightInfo);
	m_pGraphicDev->LightEnable(0, TRUE);


	return S_OK;
}

_int CMapEditor::Update_Scene(const _float& fTimeDelta)
{

	// 아 imgui가 view행렬을 항등행렬로 초기화해서 반드시 update_scene보다 앞에서 해야함

	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	CImGuiManger::GetInstance()->update();
	

	
	return iExit; 
}

void CMapEditor::LateUpdate_Scene(const _float& fTimeDelta)
{
	//CImGuiManger::GetInstance()->Render();

	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CMapEditor::Render_Scene()
{
	//화면이 바뀔때 render Scene되고 그다음에 lateupdate 그리고 update가 진행되고있음 
	CImGuiManger::GetInstance()->Render();
}

HRESULT CMapEditor::Ready_Prototype()
{


	//ETC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_State", Engine::CStateController::Create(m_pGraphicDev)), E_FAIL);

	//Buffer 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TestMapTex", Engine::CStartMap::Create(m_pGraphicDev)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.f,1.f)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WorldHeartMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.f,2.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonForestMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.5f,1.5f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JungleMonkeyTownMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.5f,2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonTempleMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArenaMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.0f, 1.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SunTempleMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 2.0f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TownMapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 2.0f, 1.0f)), E_FAIL);	

	//Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player1.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/Tutorial_Map.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WorldHeartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/WorldHeart.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonForestMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/MoonForestMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JungleMonkeyTownMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/JungleMonkeyTownMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MoonTempleMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/MoonTempleMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArenaMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/ArenaMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SunTempleMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/SunTempleMap.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TownMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/TownMap.png", TEX_NORMAL)), E_FAIL);	

	CResMgr::GetInstance()->init();


	return S_OK;
}

HRESULT CMapEditor::Ready_Layer_Environment(const _tchar* pLayerTag)
{

	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	_vec3 Eye = { 0.f, 10.f, -10.f };
	_vec3 At = { 0.f, 0.f, 1.f };
	_vec3 Up = { 0.f, 1.f, 0.f };	

	Engine::CGameObject* pGameObject = nullptr;
		
	pGameObject = CMapCamera::Create(m_pGraphicDev, &Eye, &At, &Up);	

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapCamera", pGameObject), E_FAIL);


	// 첫번째 맵 
	//pGameObject = CMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);	
	//pGameObject->SetObjectKey(L"Map");	
	//pGameObject->SetTextureKey(L"Map");


	//세계 심장 맵
	//pGameObject = CWorldHeartMap::Create(m_pGraphicDev);		
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);		
	//pGameObject->SetObjectKey(L"Map");		
	//pGameObject->SetTextureKey(L"Map");	


	// 정글 포레스트 맵 
	//pGameObject = CJungleForestMap::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);
	//pGameObject->SetObjectKey(L"Map");
	//pGameObject->SetTextureKey(L"Map");



	//달숲 맵
	//pGameObject = CMoonForestMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);	
	//pGameObject->SetObjectKey(L"Map");	
	//pGameObject->SetTextureKey(L"Map");	


	//달숲 신전맵
	//pGameObject = CMoonTempleMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);	
	//pGameObject->SetObjectKey(L"Map");	
	//pGameObject->SetTextureKey(L"Map");	


	//아레나 맵
	//pGameObject = CArenaMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);	
	//pGameObject->SetObjectKey(L"Map");	
	//pGameObject->SetTextureKey(L"Map");	

	//태양 신전 맵 
	pGameObject = CSunTempleMap::Create(m_pGraphicDev);		
	NULL_CHECK_RETURN(pGameObject, E_FAIL);		
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);		
	pGameObject->SetObjectKey(L"Map");		
	pGameObject->SetTextureKey(L"Map");	

	// 항구 맵 
	//pGameObject = CTown::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);	
	//pGameObject->SetObjectKey(L"Map");	
	//pGameObject->SetTextureKey(L"Map");	



		
	m_mapLayer.insert({ pLayerTag, pLayer });		
		
	return S_OK;
}

HRESULT CMapEditor::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();	
	NULL_CHECK_RETURN(pLayer, E_FAIL);	

	//Engine::CGameObject* pGameObject = nullptr;

	/*pGameObject = CMap::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);
	pGameObject->SetObjectKey(L"Map");
	pGameObject->SetTextureKey(L"Map");*/
	


	m_mapLayer.insert({ pLayerTag, pLayer });	

	return S_OK;
}

HRESULT CMapEditor::Ready_Layer_UI(const _tchar* pLayerTag)
{
	return S_OK;
}

CMapEditor* CMapEditor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapEditor* pMapEditor = new CMapEditor(pGraphicDev);

	//if(FAILED(pMapEditor->Ready_Scene()))
	//{
	//	Safe_Release(pMapEditor);
	//	MSG_BOX("MapEditor Create Failed");
	//	return nullptr;
	//}

	return pMapEditor;
}

void CMapEditor::Free()
{
	Engine::CScene::Free();
}
