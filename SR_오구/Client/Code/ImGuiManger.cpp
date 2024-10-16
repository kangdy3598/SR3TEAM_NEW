#include "pch.h"
#include "ImGuiManger.h"
#include "Define.h"
#include "iostream"

IMPLEMENT_SINGLETON(CImGuiManger)

CImGuiManger::CImGuiManger()
	:testbool(false)
	,inttest(10)
	,floattest(5.5f)
	,m_pTerrainTexture(nullptr)
	,m_bshowTileTextureWindow(false)
	,iTileX(0)
	,iTileY(0)
	,fVtxItv(0)
	,m_bTerrainOnOff(false)
	,m_bWireFrameModeOnOff(false)
	,m_pGraphicDev(nullptr)
	,m_pMapTex(nullptr)
	,m_vecPickPos(0.f,0.f,0.f)
	,m_bImageButtonClick(false)
	,m_pCurTerrainTexture(nullptr)
	,m_bshowObjectTextrueWindow(false)
	,m_bshowInstalledObjectList(false)
{

}


CImGuiManger::~CImGuiManger()
{
}

void CImGuiManger::init()
{
	//초기화 하기 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

	m_pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();


    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

	//ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("../TTF/Pretendard-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());


	ImGui::SetNextWindowSize(ImVec2(400, 400)); // Imgui 창 크기 
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	

	
	m_vecTerrainTexture.resize(14);
	


	for(int i=1; i<m_vecTerrainTexture.size()+1; i++)
	{

		char filePath[256];
		sprintf_s(filePath, "../Bin/Resource/Texture/Map/Tile%d.png", i-1);

		HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecTerrainTexture[i-1]);
		if (FAILED(hr))
		{
			MSG_BOX("Terrain Texture load failed");
		}
	}

	
	// 오브잭트 툴 작업 
	CImgui_ObjectTool::GetInstance()->init();

	// 오브젝트 및 맵 세이브 작업
	//CImgui_ObjectTool::GetInstance()->Save();

}

void CImGuiManger::update()
{

	// 업데이트 

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	if (ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar))
	{

		// 오브젝트 및 맵 세이브 작업
		if (ImGui::BeginMenuBar())
		{
			// 저장 , 불러오기. 
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem(u8"저장"))
				{
					CImgui_ObjectTool::GetInstance()->Save();
				}

				else if (ImGui::MenuItem(u8"불러오기"))
				{
					CImgui_ObjectTool::GetInstance()->Read();
				}
				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		//Tile MenuBar
		if (ImGui::BeginMenuBar())
		{
			//내용..
			if (ImGui::BeginMenu("Tile"))
			{
				if (ImGui::MenuItem("Tile Texture List"))
				{
					m_bshowTileTextureWindow = true;
				}

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		//Terrain MenuBar
		if (ImGui::BeginMenuBar())
		{
			// Terrain 끄고 키기 
			if (ImGui::BeginMenu("Terrain"))
			{
				if (ImGui::MenuItem("On"))
				{
					m_bTerrainOnOff = true;
				}

				else if(ImGui::MenuItem("Off"))
				{
					m_bTerrainOnOff = false; 
				}


				else if(ImGui::MenuItem("WireFrameOn"))
				{
					m_bWireFrameModeOnOff = true;
				}

				else if (ImGui::MenuItem("WireFrameOff"))
				{
					m_bWireFrameModeOnOff = false;
				}

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		// 오브젝트 툴 업데이트하기 
		CImgui_ObjectTool::GetInstance()->update();	
		

		// 이 밑에다가 내용들을 추가
		

		ImGui::Checkbox("Test Bool", &testbool);
		if (ImGui::Button("Click me!"))
		{
			testbool = true;
		}
		ImGui::SliderInt("Chose Int", &inttest, 1, 25);
		ImGui::SliderFloat("Chose Float", &floattest, 0.1f, 15.5f);


	}ImGui::End();


	// imgui 타일 맵 형태
	if (m_bshowTileTextureWindow)	
	{
		ShowTileMenuWindow();	
	}


}

void CImGuiManger::Render()
{
	ImGui::Render();	
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());	
	ImGui::EndFrame();	
}

void CImGuiManger::Release()
{
	// 릴리즈 
}

void CImGuiManger::ShowTileMenuWindow()
{
	// Tile Texture List 창 띄우기
	if (m_bshowTileTextureWindow)
	{
		// 새로운 창 생성
		ImGui::Begin("Tile Texture List", &m_bshowTileTextureWindow);
		// 여기에 창의 내용을 추가
		// 
		//현재 창 사이즈 가져오기 
		ImVec2 CurrentWindowSize = ImGui::GetWindowSize();


		ImVec2 imageSize(100, 100);  // 출력할 이미지 크기

		
		for (int i = 1; i < m_vecTerrainTexture.size() + 1; i++)
		{

			char label[16];
			sprintf_s(label, "Tile%d", i);
			// 이미지 출력
			//ImGui::Image((void*)m_vecTexture[i - 1], imageSize);
			if (ImGui::ImageButton(label, (void*)m_vecTerrainTexture[i - 1], imageSize)) 
			{
				// ImageButton이 클릭된 경우 호출할 함수
				OnTileImageButtonClick(i);  // 예시: 클릭된 타일의 인덱스를 함수로 전달
			};

			// 현재 창에서 남은 가로 공간 확인
			//ImVec2 availableSpace = ImGui::GetContentRegionAvail();

			// 남은 공간이 이미지 크기보다 크다면 같은 줄에 이미지 출력
			if (i % (int)(CurrentWindowSize.x/100) != 0)
			{
				ImGui::SameLine();  // 같은 줄에 출력
			}

			else if (i % (int)(CurrentWindowSize.x/100) == 0)
			{
				// 공간이 부족하면 다음 줄로 이동
				ImGui::NewLine();
			}
		}

		ImGui::NewLine();
		//ImGui::Text("Tile Texture List Content Here");

		if (!ImGui::CollapsingHeader(u8"타일 생성"))
		{	
			
			ImGui::Text(u8"생성할 타일의 행과 열의 수 설정");
			ImGui::NewLine();


			ImGui::PushItemWidth(100.0f);  // 너비를 100픽셀로 설정
			
			ImGui::Text("Tile X");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputInt("##Tile X", &iTileX);
			ImGui::SameLine(170.0f, 0.0f);
			ImGui::Text("Tile Y");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputInt("##Tile Y", &iTileY);
			ImGui::NewLine();
			ImGui::Text(u8"점 간격");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputFloat("##VtxItv" ,&fVtxItv);

			ImGui::NewLine();	
			if(ImGui::Button(u8"지형생성"))
			{
				
			}
			//ImGui::PopStyleColor(3);
			ImGui::PopItemWidth();  // 설정된 너비를 원래대로 복구
		}


		

		ImGui::End();
	}

}


void CImGuiManger::OnTileImageButtonClick(int tileIndex)
{
	// 여기서 타일 이미지값 가져옴 
	// 여기서 버퍼 가지고 와서 해당 마우스좌표에 랜더링되게 해야함. 

	//m_pGraphicDev->SetTexture(0, m_vecTexture[tileIndex]);
	m_bImageButtonClick = true; 
	m_pCurTerrainTexture = m_vecTerrainTexture[tileIndex - 1];

}


