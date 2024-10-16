#include "pch.h"
#include "../Header/MainApp.h"
#include "ImGuiManger.h"



CMainApp::CMainApp()
{

}

CMainApp::~CMainApp()
{

}

HRESULT CMainApp::Ready_MainApp()
{
	//여기가 init 함수 느낌임
	// 
	// 
	// 장치 초기화 한 주소값 얻어오기 -> 레퍼런스 카운트 증가함 
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);


	// 여기서 이제 각 신들의 맵 테그를 지어주고 신에 관련된 레이어에 그리고 레이어에 속한 물체에 대한 정보 저장
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);


	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{

	Engine::Update_InputDev();
	
	m_pManagementClass->Update_Scene(fTimeDelta);



	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	//_ulong	Mousemove(0);
	//
	//if (Mousemove = Engine::Get_DIMouseMove(DIMS_Z))
	//{
	//	int	a = 0;
	//}

	

	m_pManagementClass->LateUpdate_Scene(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));


	// imgui ui 옵션값에 따른 연동
	//if(CImGuiManger::GetInstance()->GetTerrainOnOFF())
	//{
		m_pManagementClass->Render_Scene(m_pGraphicDev);
	//}
	
	//if(CImGuiManger::GetInstance()->GetWireFrameOnOFF())
	//{
		//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//}

	//if(!CImGuiManger::GetInstance()->GetWireFrameOnOFF())
	//{
		//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//}


	// ImGui 렌더링
	//CImGuiManger::GetInstance()->Render();
	
	

	Engine::Render_End();	
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene* pScene = nullptr; 
	//각 종 신들 저장 하는 init 부분 
	pScene = CStartScene::Create(pGraphicDev);	
	NULL_CHECK_RETURN(pScene, E_FAIL);	

	// 신 매니저 느낌인듯 
	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);	
	(*ppManagement)->AddRef(); //매니저 싱글톤 주소 참조했으니 스택증가.	

	//Engine::Set_Scene(pScene) // 이렇게 해도됨 위에 2줄 빼고 
	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL); // 이게 현재 신 정해주는거 전 씬은 없애주고	

	
	
	// ================================================================================
	//pScene = CLogo::Create(pGraphicDev);
	//NULL_CHECK_RETURN(pScene, E_FAIL);

	//// 신 매니저 느낌인듯 
	//FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	//(*ppManagement)->AddRef(); //매니저 싱글톤 주소 참조했으니 스택증가.

	////Engine::Set_Scene(pScene) // 이렇게 해도됨 위에 2줄 빼고 
	//FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL); // 이게 현재 신 정해주는거 전 씬은 없애주고

	return S_OK;
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	//  Default로 true 값으로 되어 있는 것들 
	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 깊이 값을 기록은 하되 정렬을 수행할지 말지 여부를 묻는 옵션
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z버퍼에 깊이 값을 기록할 지 말 지 결정

	// 폰트 추가 

	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Default", L"궁서", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Myungjo", L"명조", 20, 20, FW_NORMAL), E_FAIL);
	
	//0922
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_OguBold14", L"카페24 써라운드", 14, 14, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_OguBold22", L"카페24 써라운드", 22, 22, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_OguBold24", L"카페24 써라운드", 24, 24, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_OguBold36", L"카페24 써라운드", 36, 36, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_OguBold48", L"카페24 써라운드", 48, 48, FW_NORMAL), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Ogu10", L"카페24 써라운드 에어", 10, 12, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Ogu14", L"카페24 써라운드 에어", 14, 18, FW_LIGHT), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Ogu22", L"카페24 써라운드 에어", 18, 24, FW_LIGHT), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*ppGraphicDev, L"Font_Ogu24", L"카페24 써라운드 에어", 20, 26, FW_THIN), E_FAIL);



	// dinput 추가
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);		


	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if(FAILED(pMainApp->Ready_MainApp()))
	{
		Safe_Release(pMainApp);
		return nullptr;
	}


	return pMainApp;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);


	//m_pDeviceClass->DestroyInstance();
	Engine::Release_Utility();
	Engine::Release_System();
}



// 로고 작업하기 와서 