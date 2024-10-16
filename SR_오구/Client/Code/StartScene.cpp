#include "pch.h"
#include "StartScene.h"
#include "Export_Utility.h"
#include "Logo.h"
#include "MapEditor.h"


CStartScene::CStartScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
	
	
{
}

CStartScene::~CStartScene()
{
}

HRESULT CStartScene::Ready_Scene()
{

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);	 // 버퍼 추가 .

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::Sound_Initialize();
	Engine::Play_Sound(L"BGM_19_TutorialAmbience.wav", SOUND_BGM, 1.f);	

	return S_OK;
}

_int CStartScene::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CStartScene::LateUpdate_Scene(const _float& fTimeDelta)
{

	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStartScene::Render_Scene()
{


	// Game Start 
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_pLogo = CLogo::Create(m_pGraphicDev);
		if (m_pLogo == nullptr)
		{
			MSG_BOX("m_pLogo NullPtr");
		}
		Set_Scene(m_pLogo);
	}

	// MapTool start
	else if (GetAsyncKeyState('G') & 0x8000)
	{
		m_pMapEditor = CMapEditor::Create(m_pGraphicDev);
		if (m_pMapEditor == nullptr)
		{
			MSG_BOX("m_pMapEditor NullPtr");
		}
		Set_Scene(m_pMapEditor);

	}

}

HRESULT CStartScene::Ready_Prototype()
{

	//buffer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex_Test", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	
	//Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StartBackGroundTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/StartScene.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

HRESULT CStartScene::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CStartScene::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CStartScene::Ready_Layer_UI(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();	
	NULL_CHECK_RETURN(pLayer, E_FAIL);	

	Engine::CGameObject* pGameObject = nullptr;	

	pGameObject = CStartBackGround::Create(m_pGraphicDev);		
	NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HPBar", pGameObject), E_FAIL);	
	m_mapLayer.insert({ pLayerTag,pLayer });	
	
	
	return S_OK;
}

CStartScene* CStartScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartScene* pStartScene = new CStartScene(pGraphicDev);

	//if (FAILED(pStartScene->Ready_Scene()))
	//{
	//	Safe_Release(pStartScene);
	//	MSG_BOX("MapEditor Create Failed");
	//	return nullptr;
	//}

	return pStartScene;
}

void CStartScene::Free()
{
	Engine::CScene::Free();
}
