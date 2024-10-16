#include "pch.h"
#include "Imgui_ObjectTool.h"
#include "GraphicDev.h"
#include "Object.h"
#include "Define.h"
#include <sstream>
#include <commdlg.h>
#include <cwctype>
#include <cstring>


IMPLEMENT_SINGLETON(CImgui_ObjectTool);


CImgui_ObjectTool::CImgui_ObjectTool()
	: m_bShowObjectWindow(false)
	, m_bShowInstallObjectWindow(false)
	, m_pCurObjectTransform(nullptr)
	, m_sCurTextureData(L"")
	, m_sCurTextureKey(L"")
	, m_vecPickPos(0.f,0.f,0.f)
	, m_strCurObjectName(L"")
	, index(0)
	, deleteCount(0)
	, m_wCurReadTextureKey(L"Test")
{

}

CImgui_ObjectTool::~CImgui_ObjectTool()
{

}


wstring StringToTChar(const std::string& str)
{
	wstring wstr;
	wstr.assign(str.begin(), str.end());

	return wstr;
}


string WstringTostring(const std::wstring& wstr)
{
	string str;
	str.assign(wstr.begin(), wstr.end());

	return str;
}


std::wstring extractNumber(const std::wstring& input) {
	std::wstring result;
	for (wchar_t ch : input) {
		if (iswdigit(ch)) {
			result += ch; // 숫자이면 result에 추가
		}
	}
	return result;
}

std::wstring removeDigits(const std::wstring& input) {
	std::wstring result;
	for (wchar_t wch : input) {
		if (!std::iswdigit(wch)) {  // 숫자가 아닌 경우
			result += wch;
		}
	}
	return result;
}

const wchar_t* findInVector(const std::vector<const wchar_t*>& vec, const wchar_t* target) {
	for (auto it = vec.begin(); it != vec.end(); ++it) {
		if (wcscmp(*it, target) == 0) {  // 내용 비교
			return *it;  // 일치하는 요소 반환
		}
	}
	return nullptr;  // 일치하는 요소가 없는 경우
}

void CImgui_ObjectTool::init()
{
	m_vecObjectTexture.resize(100);
	
	

	for (int i = 0; i< m_vecObjectTexture.size(); i++)
	{
		char filePath[256];
		sprintf_s(filePath, "../Bin/Resource/Texture/Object/object%d.png", i);
	
		HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecObjectTexture[i]);
		if (FAILED(hr))
		{
			MSG_BOX("Object Texture load failed");
		}
	}
}

void CImgui_ObjectTool::update()
{

	if (ImGui::BeginMenuBar())
	{
		//내용..
		if (ImGui::BeginMenu("Object"))
		{
			if (ImGui::MenuItem("Object Texture List"))
			{
				m_bShowObjectWindow = true;
			}

			if (ImGui::MenuItem("Installed Objects List"))
			{
				m_bShowInstallObjectWindow = true;
			}

			ImGui::EndMenu();
		}
		// 
		//끝나면 End
		ImGui::EndMenuBar();
	}


	if(m_bShowObjectWindow)
	{
		ShowObjectWindow();	
		static bool wasLeftButtonDown = false;

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			// ImGui 창 밖에서의 마우스 입력 처리
			if (LONG mouseState = Engine::Get_DIMouseState(DIM_LB) & 0x80)
			{
				// 여기다가 조건식으로 오브젝트 창이 열렸을때만 반응하도록 하면 되겠다.
				// 레이까지 적용완료
				// 마우스 레이 코드 
				

				 //= Get_DIMouseState(DIM_LB);	

				if (mouseState & 0x80) // 마우스 왼쪽 버튼이 눌렸다면
				{
					// 이전 상태가 눌리지 않았을 때만 처리
					if (!wasLeftButtonDown)
					{
						// 클릭 이벤트 처리
						// 여기에서 원하는 작업 수행
						m_vecPickPos = PickingOnTerrain();
						InstallObject(m_sCurTextureKey,m_strCurObjectName,m_sCurTextureData);

						m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", m_vecObjectList.back(), L"Com_Transform"));
						if (m_pCurObjectTransform == nullptr)
						{
							MSG_BOX("m_pCurObjectTransform is nullptr");
							//m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Imgui_Object1", m_strCurObjectName, L"Com_Transform"));
							//if (m_pCurObjectTransform == nullptr)
							//{
							//	MSG_BOX("m_pCurObjectTransform is nullptr");
							//}
						}

						if (m_pCurObjectTransform != nullptr)
							m_pCurObjectTransform->Set_Pos(m_vecPickPos.x, 0, m_vecPickPos.z);

						
					}

					// 현재 상태를 업데이트
					wasLeftButtonDown = true;
					
				}
			}

			else
			{
				// 버튼이 눌리지 않으면 상태를 업데이트
				wasLeftButtonDown = false;
			}
		}
		else
		{
			// ImGui 창 안에서의 마우스 입력 처리
		}
	}

	if(m_bShowInstallObjectWindow)
	{
		ShowInstalledObjectWindow();

	}


	
}

void CImgui_ObjectTool::render()
{

}

void CImgui_ObjectTool::ShowObjectWindow()
{

	if (m_bShowObjectWindow)
	{
		ImGui::Begin("Object Texture List", &m_bShowObjectWindow);

		ImVec2 imageSize(100, 100);  // 출력할 이미지 크기

		if (ImGui::ImageButton("Wall", m_vecObjectTexture[0], imageSize))
		{

			//여기가 문제내 
			m_sCurTextureData  = L"Wall";
			m_sCurTextureKey   = L"Wall";
			m_strCurObjectName = L"Wall";

		}

		else if (ImGui::ImageButton("Super", m_vecObjectTexture[1], imageSize))	
		{

			//여기가 문제내 
			m_sCurTextureData = L"Super";
			m_sCurTextureKey = L"Super";
			m_strCurObjectName = L"Super";

		}

		if (ImGui::ImageButton("TelephonePole", m_vecObjectTexture[2], imageSize))
		{

			m_sCurTextureData = L"TelephonePole";
			m_sCurTextureKey = L"TelephonePole";
			m_strCurObjectName = L"TelephonePole";

		}

		if (ImGui::ImageButton("BusStation", m_vecObjectTexture[3], imageSize))
		{

			
			m_sCurTextureData = L"BusStation";
			m_sCurTextureKey = L"BusStation";
			m_strCurObjectName = L"BusStation";

		}

		if (ImGui::ImageButton("BreadStore", m_vecObjectTexture[4], imageSize))
		{

			m_sCurTextureData =  L"BreadStore";
			m_sCurTextureKey =   L"BreadStore";
			m_strCurObjectName = L"BreadStore";

		}

		if (ImGui::ImageButton("Telephone2Pole", m_vecObjectTexture[5], imageSize))
		{
			m_sCurTextureData =  L"Telephone2Pole";
			m_sCurTextureKey =   L"Telephone2Pole";
			m_strCurObjectName = L"Telephone2Pole";
		}

		if (ImGui::ImageButton("Bench", m_vecObjectTexture[6], imageSize))
		{
			m_sCurTextureData =  L"Bench";
			m_sCurTextureKey =   L"Bench";
			m_strCurObjectName = L"Bench";
		}


		if (ImGui::ImageButton("Carpet", m_vecObjectTexture[7], imageSize))
		{
			m_sCurTextureData =  L"Carpet";
			m_sCurTextureKey =   L"Carpet";
			m_strCurObjectName = L"Carpet";
		}

		if (ImGui::ImageButton("Ogu_House_Floor", m_vecObjectTexture[8], imageSize))
		{
			m_sCurTextureData =  L"Ogu_House_Floor";
			m_sCurTextureKey =   L"Ogu_House_Floor";
			m_strCurObjectName = L"Ogu_House_Floor";
		}

		if (ImGui::ImageButton("Ogu_House_Table", m_vecObjectTexture[9], imageSize))
		{
			m_sCurTextureData =  L"Ogu_House_Table";
			m_sCurTextureKey  =  L"Ogu_House_Table";
			m_strCurObjectName = L"Ogu_House_Table";
		}

		if (ImGui::ImageButton("Flower_Pot", m_vecObjectTexture[10], imageSize))
		{
			m_sCurTextureData = L"Flower_Pot";
			m_sCurTextureKey = L"Flower_Pot";
			m_strCurObjectName = L"Flower_Pot";
		}


		if (ImGui::ImageButton("Street_Lamp", m_vecObjectTexture[11], imageSize))
		{
			m_sCurTextureData = L"Street_Lamp";
			m_sCurTextureKey = L"Street_Lamp";
			m_strCurObjectName = L"Street_Lamp";
		}

		if (ImGui::ImageButton("No_Flower_Pot", m_vecObjectTexture[12], imageSize))
		{
			m_sCurTextureData = L"No_Flower_Pot";
			m_sCurTextureKey = L"No_Flower_Pot";
			m_strCurObjectName = L"No_Flower_Pot";
		}

		if (ImGui::ImageButton("One_Flower_Pot", m_vecObjectTexture[13], imageSize))
		{
			m_sCurTextureData = L"One_Flower_Pot";
			m_sCurTextureKey = L"One_Flower_Pot";
			m_strCurObjectName = L"One_Flower_Pot";
		}

		if (ImGui::ImageButton("One_No_Flower_Pot", m_vecObjectTexture[14], imageSize))
		{
			m_sCurTextureData = L"One_No_Flower_Pot";
			m_sCurTextureKey = L"One_No_Flower_Pot";
			m_strCurObjectName = L"One_No_Flower_Pot";
		}


		if (ImGui::ImageButton("Forest_Tree", m_vecObjectTexture[15], imageSize))
		{
			m_sCurTextureData = L"Forest_Tree";
			m_sCurTextureKey = L"Forest_Tree";
			m_strCurObjectName = L"Forest_Tree";
		}


		if (ImGui::ImageButton("Long_Wall", m_vecObjectTexture[16], imageSize))
		{
			m_sCurTextureData = L"Long_Wall";
			m_sCurTextureKey = L"Long_Wall";
			m_strCurObjectName = L"Long_Wall";
		}

		if (ImGui::ImageButton("Background_Wall", m_vecObjectTexture[17], imageSize))
		{
			m_sCurTextureData = L"Background_Wall";
			m_sCurTextureKey = L"Background_Wall";
			m_strCurObjectName = L"Background_Wall";
		}


		if (ImGui::ImageButton("Cart", m_vecObjectTexture[18], imageSize))
		{
			m_sCurTextureData = L"Cart";
			m_sCurTextureKey = L"Cart";
			m_strCurObjectName = L"Cart";
		}

		if (ImGui::ImageButton("Tutorial_Store2", m_vecObjectTexture[19], imageSize))
		{
			m_sCurTextureData = L"Tutorial_Store2";
			m_sCurTextureKey = L"Tutorial_Store2";
			m_strCurObjectName = L"Tutorial_Store2";
		}

		if (ImGui::ImageButton("One_Tree", m_vecObjectTexture[20], imageSize))
		{
			m_sCurTextureData = L"One_Tree";
			m_sCurTextureKey = L"One_Tree";
			m_strCurObjectName = L"One_Tree";
		}

		if (ImGui::ImageButton("Background_Sky", m_vecObjectTexture[21], imageSize))
		{
			m_sCurTextureData = L"Background_Sky";
			m_sCurTextureKey = L"Background_Sky";
			m_strCurObjectName = L"Background_Sky";
		}

		if (ImGui::ImageButton("Bucket", m_vecObjectTexture[22], imageSize))
		{
			m_sCurTextureData = L"Bucket";
			m_sCurTextureKey = L"Bucket";
			m_strCurObjectName = L"Bucket";
		}

		if (ImGui::ImageButton("Trash_Can", m_vecObjectTexture[23], imageSize))
		{
			m_sCurTextureData = L"Trash_Can";
			m_sCurTextureKey = L"Trash_Can";
			m_strCurObjectName = L"Trash_Can";
		}

		if (ImGui::ImageButton("bicycle", m_vecObjectTexture[24], imageSize))
		{
			m_sCurTextureData = L"bicycle";
			m_sCurTextureKey = L"bicycle";
			m_strCurObjectName = L"bicycle";
		}

		if (ImGui::ImageButton("Mini_Chair", m_vecObjectTexture[25], imageSize))
		{
			m_sCurTextureData = L"Mini_Chair";
			m_sCurTextureKey = L"Mini_Chair";
			m_strCurObjectName = L"Mini_Chair";
		}


		if (ImGui::ImageButton("Fence", m_vecObjectTexture[26], imageSize))
		{
			m_sCurTextureData = L"Fence";
			m_sCurTextureKey = L"Fence";
			m_strCurObjectName = L"Fence";
		}

		if (ImGui::ImageButton("Grass_collection", m_vecObjectTexture[27], imageSize))
		{
			m_sCurTextureData = L"Grass_collection";
			m_sCurTextureKey = L"Grass_collection";
			m_strCurObjectName = L"Grass_collection";

		};


		if (ImGui::ImageButton("Stone", m_vecObjectTexture[28], imageSize))
		{
			m_sCurTextureData =  L"Stone";
			m_sCurTextureKey =   L"Stone";
			m_strCurObjectName = L"Stone";
		};


		if (ImGui::ImageButton("Sky_Color_Tree", m_vecObjectTexture[29], imageSize))
		{
			m_sCurTextureData =  L"Sky_Color_Tree";
			m_sCurTextureKey =   L"Sky_Color_Tree";
			m_strCurObjectName = L"Sky_Color_Tree";
		};

		if (ImGui::ImageButton("Cave_Entrance", m_vecObjectTexture[30], imageSize))
		{
			m_sCurTextureData =  L"Cave_Entrance";
			m_sCurTextureKey =   L"Cave_Entrance";
			m_strCurObjectName = L"Cave_Entrance";
		};


		if (ImGui::ImageButton("Cave_Wall", m_vecObjectTexture[31], imageSize))
		{
			m_sCurTextureData =  L"Cave_Wall";
			m_sCurTextureKey =   L"Cave_Wall";
			m_strCurObjectName = L"Cave_Wall";
		};

		if (ImGui::ImageButton("Moon_Tree_one", m_vecObjectTexture[32], imageSize))
		{
			m_sCurTextureData =  L"Moon_Tree_one";
			m_sCurTextureKey =   L"Moon_Tree_one";
			m_strCurObjectName = L"Moon_Tree_one";
		};


		if (ImGui::ImageButton("Moon_Tree_two", m_vecObjectTexture[33], imageSize))
		{
			m_sCurTextureData =  L"Moon_Tree_two";
			m_sCurTextureKey =   L"Moon_Tree_two";
			m_strCurObjectName = L"Moon_Tree_two";
		};


		if (ImGui::ImageButton("Mushroom", m_vecObjectTexture[34], imageSize))
		{
			m_sCurTextureData =  L"Mushroom";
			m_sCurTextureKey =   L"Mushroom";
			m_strCurObjectName = L"Mushroom";
		};

		if (ImGui::ImageButton("Tree_bottom", m_vecObjectTexture[35], imageSize))
		{
			m_sCurTextureData =  L"Tree_bottom";
			m_sCurTextureKey =   L"Tree_bottom";
			m_strCurObjectName = L"Tree_bottom";
		};

		if (ImGui::ImageButton("Bug_statue_one", m_vecObjectTexture[36], imageSize))
		{
			m_sCurTextureData =  L"Bug_statue_one";
			m_sCurTextureKey =   L"Bug_statue_one";
			m_strCurObjectName = L"Bug_statue_one";
		};


		if (ImGui::ImageButton("Sign", m_vecObjectTexture[37], imageSize))
		{
			m_sCurTextureData =  L"Sign";
			m_sCurTextureKey =   L"Sign";
			m_strCurObjectName = L"Sign";
		};


		if (ImGui::ImageButton("Bug_statue_two", m_vecObjectTexture[38], imageSize))
		{
			m_sCurTextureData =  L"Bug_statue_two";
			m_sCurTextureKey =   L"Bug_statue_two";
			m_strCurObjectName = L"Bug_statue_two";
		};


		if (ImGui::ImageButton("Moon_Forest_Entrance", m_vecObjectTexture[39], imageSize))
		{
			m_sCurTextureData =  L"Moon_Forest_Entrance";
			m_sCurTextureKey =   L"Moon_Forest_Entrance";
			m_strCurObjectName = L"Moon_Forest_Entrance";
		};


		if (ImGui::ImageButton("Stone_statue", m_vecObjectTexture[40], imageSize))
		{
			m_sCurTextureData =  L"Stone_statue";
			m_sCurTextureKey =   L"Stone_statue";
			m_strCurObjectName = L"Stone_statue";
		};


		if (ImGui::ImageButton("Moon_Grass", m_vecObjectTexture[41], imageSize))
		{
			m_sCurTextureData =  L"Moon_Grass";
			m_sCurTextureKey =   L"Moon_Grass";
			m_strCurObjectName = L"Moon_Grass";
		};

		if (ImGui::ImageButton("Bug_statue_three", m_vecObjectTexture[42], imageSize))
		{
			m_sCurTextureData =  L"Bug_statue_three";
			m_sCurTextureKey =   L"Bug_statue_three";
			m_strCurObjectName = L"Bug_statue_three";
		};

		if (ImGui::ImageButton("Stone_statue_two", m_vecObjectTexture[43], imageSize))
		{
			m_sCurTextureData =  L"Stone_statue_two";
			m_sCurTextureKey =   L"Stone_statue_two";
			m_strCurObjectName = L"Stone_statue_two";
		};

		if (ImGui::ImageButton("Moon_Flag", m_vecObjectTexture[44], imageSize))
		{
			m_sCurTextureData =  L"Moon_Flag";
			m_sCurTextureKey =   L"Moon_Flag";
			m_strCurObjectName = L"Moon_Flag";
		};


		if (ImGui::ImageButton("Moon_Wall", m_vecObjectTexture[45], imageSize))
		{
			m_sCurTextureData =  L"Moon_Wall";
			m_sCurTextureKey =   L"Moon_Wall";
			m_strCurObjectName = L"Moon_Wall";
		};



		if (ImGui::ImageButton("Moon_Wall", m_vecObjectTexture[45], imageSize))
		{
			m_sCurTextureData = L"Moon_Wall";
			m_sCurTextureKey = L"Moon_Wall";
			m_strCurObjectName = L"Moon_Wall";
		};

		if (ImGui::ImageButton("Wood_Wall", m_vecObjectTexture[46], imageSize))
		{
			m_sCurTextureData =  L"Wood_Wall";
			m_sCurTextureKey =   L"Wood_Wall";
			m_strCurObjectName = L"Wood_Wall";
		};



		if (ImGui::ImageButton("Wood_Door", m_vecObjectTexture[47], imageSize))
		{
			m_sCurTextureData =  L"Wood_Door";
			m_sCurTextureKey =   L"Wood_Door";
			m_strCurObjectName = L"Wood_Door";
		};

		if (ImGui::ImageButton("Monkey_Flag_one", m_vecObjectTexture[48], imageSize))
		{
			m_sCurTextureData =  L"Monkey_Flag_one";
			m_sCurTextureKey =   L"Monkey_Flag_one";
			m_strCurObjectName = L"Monkey_Flag_one";
		};


		if (ImGui::ImageButton("Monkey_Flag_two", m_vecObjectTexture[49], imageSize))
		{
			m_sCurTextureData =  L"Monkey_Flag_two";
			m_sCurTextureKey =   L"Monkey_Flag_two";
			m_strCurObjectName = L"Monkey_Flag_two";
		};


		if (ImGui::ImageButton("Monkey_Flag_three", m_vecObjectTexture[50], imageSize))
		{
			m_sCurTextureData =  L"Monkey_Flag_three";
			m_sCurTextureKey =   L"Monkey_Flag_three";
			m_strCurObjectName = L"Monkey_Flag_three";
		};


		if (ImGui::ImageButton("Monkey_Flag_Four", m_vecObjectTexture[51], imageSize))
		{
			m_sCurTextureData =  L"Monkey_Flag_Four";
			m_sCurTextureKey =   L"Monkey_Flag_Four";
			m_strCurObjectName = L"Monkey_Flag_Four";
		};


		if (ImGui::ImageButton("Rock", m_vecObjectTexture[52], imageSize))
		{
			m_sCurTextureData =  L"Rock";
			m_sCurTextureKey =   L"Rock";
			m_strCurObjectName = L"Rock";
		};



		if (ImGui::ImageButton("Tree_Collection", m_vecObjectTexture[53], imageSize))
		{
			m_sCurTextureData =  L"Tree_Collection";
			m_sCurTextureKey =   L"Tree_Collection";
			m_strCurObjectName = L"Tree_Collection";
		};


		if (ImGui::ImageButton("Big_Tree", m_vecObjectTexture[54], imageSize))
		{
			m_sCurTextureData =  L"Big_Tree";
			m_sCurTextureKey =   L"Big_Tree";
			m_strCurObjectName = L"Big_Tree";
		};

		if (ImGui::ImageButton("Big_desk", m_vecObjectTexture[55], imageSize))
		{
			m_sCurTextureData =  L"Big_desk";
			m_sCurTextureKey =   L"Big_desk";
			m_strCurObjectName = L"Big_desk";
		};

		if (ImGui::ImageButton("Temple_Flower_one", m_vecObjectTexture[56], imageSize))
		{
			m_sCurTextureData =  L"Temple_Flower_one";
			m_sCurTextureKey =   L"Temple_Flower_one";
			m_strCurObjectName = L"Temple_Flower_one";
		};


		if (ImGui::ImageButton("Temple_Flower_two", m_vecObjectTexture[57], imageSize))
		{
			m_sCurTextureData = L"Temple_Flower_two";
			m_sCurTextureKey = L"Temple_Flower_two";
			m_strCurObjectName = L"Temple_Flower_two";
		};

		if (ImGui::ImageButton("Temple_Flower_three", m_vecObjectTexture[58], imageSize))
		{
			m_sCurTextureData =  L"Temple_Flower_three";
			m_sCurTextureKey =   L"Temple_Flower_three";
			m_strCurObjectName = L"Temple_Flower_three";
		};

		if (ImGui::ImageButton("Temple_Wall_one", m_vecObjectTexture[59], imageSize))
		{
			m_sCurTextureData =  L"Temple_Wall_one";
			m_sCurTextureKey =   L"Temple_Wall_one";
			m_strCurObjectName = L"Temple_Wall_one";
		};


		if (ImGui::ImageButton("Temple_Wall_two", m_vecObjectTexture[60], imageSize))
		{
			m_sCurTextureData =  L"Temple_Wall_two";
			m_sCurTextureKey =   L"Temple_Wall_two";
			m_strCurObjectName = L"Temple_Wall_two";
		};

		if (ImGui::ImageButton("Temple_statue", m_vecObjectTexture[61], imageSize))
		{
			m_sCurTextureData =  L"Temple_statue";
			m_sCurTextureKey =   L"Temple_statue";
			m_strCurObjectName = L"Temple_statue";
		};

		if (ImGui::ImageButton("black_cell", m_vecObjectTexture[62], imageSize))
		{
			m_sCurTextureData =  L"black_cell";
			m_sCurTextureKey =   L"black_cell";
			m_strCurObjectName = L"black_cell";
		};


		if (ImGui::ImageButton("Arena_Wall", m_vecObjectTexture[63], imageSize))
		{
			m_sCurTextureData =  L"Arena_Wall";
			m_sCurTextureKey =   L"Arena_Wall";
			m_strCurObjectName = L"Arena_Wall";
		};

		if (ImGui::ImageButton("blackBoard", m_vecObjectTexture[64], imageSize))
		{
			m_sCurTextureData =  L"blackBoard";
			m_sCurTextureKey =   L"blackBoard";
			m_strCurObjectName = L"blackBoard";
		};


		if (ImGui::ImageButton("ArenaMark", m_vecObjectTexture[65], imageSize))
		{
			m_sCurTextureData =  L"ArenaMark";
			m_sCurTextureKey =   L"ArenaMark";
			m_strCurObjectName = L"ArenaMark";
		};

		if (ImGui::ImageButton("Caffet_One", m_vecObjectTexture[66], imageSize))
		{
			m_sCurTextureData =  L"Caffet_One";
			m_sCurTextureKey =   L"Caffet_One";
			m_strCurObjectName = L"Caffet_One";
		};

		if (ImGui::ImageButton("Caffet_Two", m_vecObjectTexture[67], imageSize))
		{
			m_sCurTextureData =  L"Caffet_Two";
			m_sCurTextureKey =   L"Caffet_Two";
			m_strCurObjectName = L"Caffet_Two";
		};


		if (ImGui::ImageButton("Caffet_Three", m_vecObjectTexture[68], imageSize))
		{
			m_sCurTextureData =  L"Caffet_Three";
			m_sCurTextureKey =   L"Caffet_Three";
			m_strCurObjectName = L"Caffet_Three";
		};

		if (ImGui::ImageButton("Pillar", m_vecObjectTexture[69], imageSize))
		{
			m_sCurTextureData =  L"Pillar";
			m_sCurTextureKey =   L"Pillar";
			m_strCurObjectName = L"Pillar";
		};

		if (ImGui::ImageButton("SunTemple_Entrance", m_vecObjectTexture[70], imageSize))
		{
			m_sCurTextureData =  L"SunTemple_Entrance";
			m_sCurTextureKey =   L"SunTemple_Entrance";
			m_strCurObjectName = L"SunTemple_Entrance";
		};


		if (ImGui::ImageButton("Jungle_Wall", m_vecObjectTexture[71], imageSize))
		{
			m_sCurTextureData =  L"Jungle_Wall";
			m_sCurTextureKey =   L"Jungle_Wall";
			m_strCurObjectName = L"Jungle_Wall";
		};


		if (ImGui::ImageButton("Jungle_Tile", m_vecObjectTexture[72], imageSize))
		{
			m_sCurTextureData =  L"Jungle_Tile";
			m_sCurTextureKey =   L"Jungle_Tile";
			m_strCurObjectName = L"Jungle_Tile";
		};


		if (ImGui::ImageButton("Statue_3", m_vecObjectTexture[73], imageSize))
		{
			m_sCurTextureData =  L"Statue_3";
			m_sCurTextureKey =   L"Statue_3";
			m_strCurObjectName = L"Statue_3";
		};

		if (ImGui::ImageButton("Statue_5", m_vecObjectTexture[74], imageSize))
		{
			m_sCurTextureData =  L"Statue_5";
			m_sCurTextureKey =   L"Statue_5";
			m_strCurObjectName = L"Statue_5";
		};

		if (ImGui::ImageButton("Statue_7", m_vecObjectTexture[75], imageSize))
		{
			m_sCurTextureData = L"Statue_7";
			m_sCurTextureKey = L"Statue_7";
			m_strCurObjectName = L"Statue_7";
		};

		if (ImGui::ImageButton("Statue_11", m_vecObjectTexture[76], imageSize))
		{
			m_sCurTextureData = L"Statue_11";
			m_sCurTextureKey = L"Statue_11";
			m_strCurObjectName = L"Statue_11";
		};

		if (ImGui::ImageButton("Statue_9", m_vecObjectTexture[77], imageSize))
		{
			m_sCurTextureData = L"Statue_9";
			m_sCurTextureKey = L"Statue_9";
			m_strCurObjectName = L"Statue_9";
		};

		if (ImGui::ImageButton("Statue_1", m_vecObjectTexture[78], imageSize))
		{
			m_sCurTextureData = L"Statue_1";
			m_sCurTextureKey = L"Statue_1";
			m_strCurObjectName = L"Statue_1";
		};

		if (ImGui::ImageButton("MoonTempleTile", m_vecObjectTexture[79], imageSize))
		{
			m_sCurTextureData =  L"MoonTempleTile";
			m_sCurTextureKey =   L"MoonTempleTile";
			m_strCurObjectName = L"MoonTempleTile";
		};


		if (ImGui::ImageButton("House1", m_vecObjectTexture[80], imageSize))
		{
			m_sCurTextureData =  L"House1";
			m_sCurTextureKey =   L"House1";
			m_strCurObjectName = L"House1";
		};

		if (ImGui::ImageButton("House2", m_vecObjectTexture[81], imageSize))
		{
			m_sCurTextureData =  L"House2";
			m_sCurTextureKey =   L"House2";
			m_strCurObjectName = L"House2";
		};


		if (ImGui::ImageButton("House3", m_vecObjectTexture[82], imageSize))
		{
			m_sCurTextureData = L"House3";
			m_sCurTextureKey = L"House3";
			m_strCurObjectName = L"House3";
		};


		if (ImGui::ImageButton("WaterFall", m_vecObjectTexture[83], imageSize))
		{
			m_sCurTextureData =  L"WaterFall";
			m_sCurTextureKey =   L"WaterFall";
			m_strCurObjectName = L"WaterFall";
		};

		if (ImGui::ImageButton("Grass_Object", m_vecObjectTexture[84], imageSize))
		{
			m_sCurTextureData =  L"Grass_Object";
			m_sCurTextureKey =   L"Grass_Object";
			m_strCurObjectName = L"Grass_Object";
		};

		if (ImGui::ImageButton("Green_Pillar", m_vecObjectTexture[85], imageSize))
		{
			m_sCurTextureData =  L"Green_Pillar";
			m_sCurTextureKey =   L"Green_Pillar";
			m_strCurObjectName = L"Green_Pillar";
		};

		if (ImGui::ImageButton("harbor_Statue", m_vecObjectTexture[86], imageSize))
		{
			m_sCurTextureData  = L"harbor_Statue";
			m_sCurTextureKey   = L"harbor_Statue";
			m_strCurObjectName = L"harbor_Statue";
		};

		if (ImGui::ImageButton("harbor_Statue_2", m_vecObjectTexture[87], imageSize))
		{
			m_sCurTextureData  = L"harbor_Statue_2";
			m_sCurTextureKey   = L"harbor_Statue_2";
			m_strCurObjectName = L"harbor_Statue_2";
		};

		if (ImGui::ImageButton("House4", m_vecObjectTexture[88], imageSize))
		{
			m_sCurTextureData =  L"House4";
			m_sCurTextureKey =   L"House4";
			m_strCurObjectName = L"House4";
		};


		if (ImGui::ImageButton("Umbrella_one", m_vecObjectTexture[89], imageSize))
		{
			m_sCurTextureData =  L"Umbrella_one";
			m_sCurTextureKey =   L"Umbrella_one";
			m_strCurObjectName = L"Umbrella_one";
		};

		if (ImGui::ImageButton("Umbrella_two", m_vecObjectTexture[90], imageSize))
		{
			m_sCurTextureData =  L"Umbrella_two";
			m_sCurTextureKey =   L"Umbrella_two";
			m_strCurObjectName = L"Umbrella_two";
		};

		if (ImGui::ImageButton("Umbrella_three", m_vecObjectTexture[91], imageSize))
		{
			m_sCurTextureData =  L"Umbrella_three";
			m_sCurTextureKey =   L"Umbrella_three";
			m_strCurObjectName = L"Umbrella_three";
		};

		if (ImGui::ImageButton("Umbrella_Chair", m_vecObjectTexture[92], imageSize))
		{
			m_sCurTextureData =  L"Umbrella_Chair";
			m_sCurTextureKey =   L"Umbrella_Chair";
			m_strCurObjectName = L"Umbrella_Chair";
		};


		if (ImGui::ImageButton("Harbor_Wall", m_vecObjectTexture[93], imageSize))
		{
			m_sCurTextureData =  L"Harbor_Wall";
			m_sCurTextureKey =   L"Harbor_Wall";
			m_strCurObjectName = L"Harbor_Wall";
		};


		if (ImGui::ImageButton("Light_House", m_vecObjectTexture[94], imageSize))
		{
			m_sCurTextureData =  L"Light_House";
			m_sCurTextureKey =   L"Light_House";
			m_strCurObjectName = L"Light_House";
		};

		if (ImGui::ImageButton("Harbor_Wall_two", m_vecObjectTexture[95], imageSize))
		{
			m_sCurTextureData =  L"Harbor_Wall_two";
			m_sCurTextureKey =   L"Harbor_Wall_two";
			m_strCurObjectName = L"Harbor_Wall_two";
		};

		if (ImGui::ImageButton("Ship", m_vecObjectTexture[96], imageSize))
		{
			m_sCurTextureData =  L"Ship";
			m_sCurTextureKey =   L"Ship";
			m_strCurObjectName = L"Ship";
		};

		if (ImGui::ImageButton("Arena_Entrance", m_vecObjectTexture[97], imageSize))
		{
			m_sCurTextureData =  L"Arena_Entrance";
			m_sCurTextureKey =   L"Arena_Entrance";
			m_strCurObjectName = L"Arena_Entrance";
		};


		//일단 오브젝트 생성해보기 
		ImGui::End();

	}

}

void CImgui_ObjectTool::ShowInstalledObjectWindow()
{
	if (m_bShowInstallObjectWindow)
			{
				ImGui::Begin("Installed Obejct List", &m_bShowInstallObjectWindow);
		
				ImGui::BeginChild("LeftChild", ImVec2(200, 0), true); // 가로 200, 세로 자동(채움), 테두리 있음
				ImGui::Text("Installed Obejct");

				//여기서 지금 오브젝트거 다 가져오기
				map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
				//CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr()

				for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
				{
					const _tchar* layerKey = iter->first;

					if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
					{
						for (auto objectIter = iter->second->GetLayerGameObjectPtr().begin(); objectIter != iter->second->GetLayerGameObjectPtr().end(); ++objectIter)
						{
							//if(ImGui::Selectable())

							
							//if (ImGui::Selectable(objectIter->second->GetObjectKey().c_str()))
							if(ImGui::Selectable(WstringTostring(objectIter->second->GetObjectKey()).c_str()))
							{
								// 클릭 시 처리할 내용
								for(auto iter = m_vecObjectList.begin(); iter!= m_vecObjectList.end(); iter++)
								{
									if(wcscmp(*iter,objectIter->second->GetObjectKey().c_str())==0)
									{
										// 그럼 여기다가 찾은그걸 넣어주자 
										CurClickObject = *iter;	
									}
								}

								// 여기서 벡터의
								m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", CurClickObject, L"Com_Transform"));	

								if (m_pCurObjectTransform == nullptr)
								{
									MSG_BOX("m_pCurObjectTransform is nullptr");
								}
							}
						}
					}
				}
				ImGui::EndChild();

				ImGui::SameLine(); // 같은 줄에 다음 요소를 배치하기 위해 사용

				ImGui::BeginChild("RightChild", ImVec2(300, 0), true);
				ImGui::Text("Control Scale and Pos");

				if (!ImGui::CollapsingHeader("Setting"))	
				{
					if (m_pCurObjectTransform != nullptr)
					{
					
						if (m_pPreObjectTransform != m_pCurObjectTransform) {
							floatPosArrayTest[0] = m_pCurObjectTransform->Get_WorldMatrix()->_41;
							floatPosArrayTest[1] = m_pCurObjectTransform->Get_WorldMatrix()->_42;
							floatPosArrayTest[2] = m_pCurObjectTransform->Get_WorldMatrix()->_43;
							//m_pPreObjectTransform = m_pCurObjectTransform; // 이전 Transform 업데이트
						}
						ImGui::Text(u8"위치 좌표");
						ImGui::SameLine(50.0f, 0.0f);
						
						ImGui::SliderFloat3("##1", floatPosArrayTest, 0.f, 10000.f);

						//static	float floatPosArray[3] = { m_pCurObjectTransform->Get_WorldMatrix()->_41,m_pCurObjectTransform->Get_WorldMatrix()->_42, m_pCurObjectTransform->Get_WorldMatrix()->_43 };
						//ImGui::SliderFloat3("##1", floatPosArray, 0.f, 10000.f);
						
						m_pCurObjectTransform->Set_Pos(floatPosArrayTest[0], floatPosArrayTest[1], floatPosArrayTest[2]);

						// 현재 같은프레임이라 입력이 따로안됨
						ImGui::BeginChild("TestChild", ImVec2(200, 200), true);
						ImGui::Text(u8"회전");
						//if (m_pPreObjectTransform != m_pCurObjectTransform)
						//{
						//	
						//}m_pPreObjectTransform != m_pCurObjectTransform
						if (m_pPreObjectTransform != m_pCurObjectTransform)	
						{
							floatRotationArray[0] = m_pCurObjectTransform->Rotation_x;
							floatRotationArray[1] = m_pCurObjectTransform->Rotation_y;
							floatRotationArray[2] = m_pCurObjectTransform->Rotation_z;

							prevRotationArray[0] = m_pCurObjectTransform->Rotation_x;
							prevRotationArray[1] = m_pCurObjectTransform->Rotation_y;
							prevRotationArray[2] = m_pCurObjectTransform->Rotation_z;

							
						}
						//static float floatRotationArray[3] = { 0.f,0.f,0.f };
						//static float prevRotationArray[3] = { 0.f, 0.f, 0.f }; // 이전 회전값 저장용

						ImGui::SliderFloat3("##1", floatRotationArray, 0.f, 360.f);


						//현재 업데이트가 돌아서 계속 회전하게됨 이럴땐 어떻게 해줘야할지 생각하기 
						//m_pTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);


						// x축 회전
						if (floatRotationArray[0] != prevRotationArray[0])
						{
							m_pCurObjectTransform->Rotation(ROT_X, -prevRotationArray[0] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_X, floatRotationArray[0] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_x = floatRotationArray[0];
							prevRotationArray[0] = floatRotationArray[0];
						}

						// y축 회전	
						if (floatRotationArray[1] != prevRotationArray[1])
						{
							m_pCurObjectTransform->Rotation(ROT_Y, -prevRotationArray[1] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_Y, floatRotationArray[1] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_y = floatRotationArray[1];
							prevRotationArray[1] = floatRotationArray[1];
						}

						// z축 회전
						if (floatRotationArray[2] != prevRotationArray[2])
						{
							m_pCurObjectTransform->Rotation(ROT_Z, -prevRotationArray[2] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_Z, floatRotationArray[2] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_z = floatRotationArray[2];
							prevRotationArray[2] = floatRotationArray[2];
						}
						ImGui::EndChild();

						ImGui::BeginChild("Scale", ImVec2(200, 200), true);
						ImGui::Text(u8"크기");
						if (m_pPreObjectTransform != m_pCurObjectTransform)
						{
							floatScaleArray[0] = m_pCurObjectTransform->Scale_x;
							floatScaleArray[1] = m_pCurObjectTransform->Scale_y;
							floatScaleArray[2] = m_pCurObjectTransform->Scale_z;

							prevScaleArray[0] = m_pCurObjectTransform->Scale_x;
							prevScaleArray[1] = m_pCurObjectTransform->Scale_y;
							prevScaleArray[2] = m_pCurObjectTransform->Scale_z;

							m_pPreObjectTransform = m_pCurObjectTransform;
						}
						//static float floatScaleArray[3] = { 1.f,1.f,1.f };
						//static float prevScaleArray[3] = { 1.f, 1.f, 1.f }; // 이전 크기값 저장용




						ImGui::SliderFloat3("##1", floatScaleArray, 0.f, 100.f);

						// x축 크기
						if (floatScaleArray[0] != prevScaleArray[0])
						{
							m_pCurObjectTransform->m_vScale = { -prevScaleArray[0], floatScaleArray[1], floatScaleArray[2] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							
							m_pCurObjectTransform->Scale_x = floatScaleArray[0];
							prevScaleArray[0] = floatScaleArray[0];
						}

						// y축 크기
						if (floatScaleArray[1] != prevScaleArray[1])
						{
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], -prevScaleArray[1], floatScaleArray[2] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							m_pCurObjectTransform->Scale_y = floatScaleArray[1];
							prevScaleArray[1] = floatScaleArray[1];
						}

						// z축 크기 

						if (floatScaleArray[2] != prevScaleArray[2])
						{
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], -prevScaleArray[1] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							m_pCurObjectTransform->Scale_z = floatScaleArray[2];
							prevScaleArray[2] = floatScaleArray[2];
						}

						ImGui::EndChild();

					}

				
				}

				//삭제버튼 누를시 활성화
				if(ImGui::Button(u8"삭제", ImVec2(100.f, 0.f)))
				{

					map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
					
					auto iter = mapLayer.find(L"Layer_GameLogic");

					if(iter == mapLayer.end())
					{
						MSG_BOX("Not Find");
					}

					else
					{
						CLayer* pLayer = iter->second;

						map<const _tchar*, CGameObject*>& mapObject = pLayer->GetLayerGameObjectPtr();

						auto iter = mapObject.find(CurClickObject);
						
								
						if(iter == mapObject.end())
						{
							MSG_BOX("Not_Find_Object");
						}

						else
						{
							
							// 메인 랜더 끝이나면 이벤트 매니저같은거 만들어서 삭제처리하기. 
							mapObject.erase(CurClickObject);		
							
						}
					}

				}

				ImGui::EndChild();
				
			
				ImGui::End();
			}

}

void CImgui_ObjectTool::InstallObject(wstring _TextureKey, wstring _CurTextureData, wstring _forConvert)
{

	static int index = m_vecObjectList.size();

	Engine::CLayer* pLayer = CLayer::Create();	
	if (pLayer == nullptr)	
	{
		MSG_BOX("pLayer nullptr Error");	
	}
	
	
	Engine::CGameObject* pGameObject = nullptr;	
	
	pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
	if (pGameObject == nullptr)
	{
		MSG_BOX("CObject nullptr Error");
	}
	
	auto iter = find(m_vecObjectList.begin(), m_vecObjectList.end(), m_strCurObjectName.c_str());
	
	map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();	

	// 새로운 객체를 만들어야하네 계속
	wstring* dynamicWstring = new wstring(_forConvert + to_wstring(index));	
	//dynamicWstring = &_forConvert;		
	m_vecObjectList.push_back((*dynamicWstring).c_str());		
	pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*dynamicWstring).c_str(), pGameObject);	
	pGameObject->SetObjectKey(*dynamicWstring);		// imgui 창에 나올 이름				
	pGameObject->SetTextureKey(m_sCurTextureKey);       // imgui 설정한 텍스처
	index++;	
}

void CImgui_ObjectTool::Save()
{
	// ㅇㅋ 세이브 기능 구현완료 
	OPENFILENAME ofn;       // OPENFILENAME 구조체 선언
	wchar_t szFile[260];       // 파일 이름을 저장할 버퍼 (최대 260자)

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들 (여기선 콘솔 창을 사용)
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0'; // 초기 파일 이름 (빈 문자열)
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";  // 파일 형식 필터 (텍스트 파일과 모든 파일)
	ofn.nFilterIndex = 1;   // 기본 선택 파일 형식 (1 = Text Files)
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:\\Users\\tjsgh\\Desktop\\SR_Team_Project3_Real\\09_12_FrameWork\\Client\\Map";  // 초기 디렉토리 (NULL이면 기본 디렉토리 사용)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// 파일 저장 대화 상자 표시
	if (GetSaveFileName(&ofn) == TRUE) {
		// 파일 경로가 선택되면 메시지 박스로 파일 경로를 표시
		 
		HANDLE hFile = CreateFile(ofn.lpstrFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MSG_BOX("hFile not open");
		}

		// 여기서 오브젝트의 SetTextureKey와 object의 월드 매트릭스 값을 저장하면될듯 


		map<const _tchar*, CLayer*>& pLayerMap = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
		
		ObjectData SaveStruct;
		ZeroMemory(&SaveStruct, sizeof(ObjectData));	

		for (auto iter = pLayerMap.begin(); iter != pLayerMap.end(); ++iter)
		{
			const _tchar* layerKey = iter->first;

			

		
			if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
			{
				// 여기서 작업 파일내용에 들어갈것들
			
				
				//이게 해당 레이어 포인터 
				map<const _tchar*, CGameObject*>& pMapObject = iter->second->GetLayerGameObjectPtr();
				
				for(auto iter2 = pMapObject.begin(); iter2 != pMapObject.end(); ++iter2)
				{

					DWORD bytesWritten = 0;	
						
					wcscpy_s(SaveStruct.objectName, iter2->second->GetObjectKey().c_str());	
					wcscpy_s(SaveStruct.textureKey, iter2->second->GetTextureKey().c_str());
					
					
					CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", SaveStruct.objectName, L"Com_Transform"));
					SaveStruct.worldMatrix = *(pTransform->Get_WorldMatrix());	
					
					SaveStruct.rotation = { pTransform->Rotation_x,pTransform->Rotation_y,pTransform->Rotation_z };
					SaveStruct.scale = { pTransform->Scale_x,pTransform->Scale_y,pTransform->Scale_z };


					if (!WriteFile(hFile, &SaveStruct, sizeof(ObjectData), &bytesWritten, NULL))
					{
						MSG_BOX("쓰기 오류!");
					}
				}
			}
		}
		MSG_BOX("SUCCESS");
	}
	}

void CImgui_ObjectTool::Read()
{
	OPENFILENAME ofn;       // OPENFILENAME 구조체 선언
	wchar_t szFile[260];   // 파일 이름을 저장할 버퍼 (최대 260자)

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // 초기 파일 이름 (빈 문자열)
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";  // 파일 형식 필터
	ofn.nFilterIndex = 1;      // 기본 선택 파일 형식
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:\\Users\\tjsgh\\Desktop\\SR_Team_Project3_Real\\09_12_FrameWork\\Client\\Map";  // 초기 디렉토리
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// 파일 열기 대화상자 표시
	if (GetOpenFileName(&ofn)) {
		// 파일 열기
		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
		if (hFile != INVALID_HANDLE_VALUE) {
	
			Engine::CLayer* pLayer = CLayer::Create();

			DWORD bytesRead = 1;

			while (bytesRead > 0)
			{
				ObjectData* objData = new ObjectData;
			
				if(!ReadFile(hFile, objData, sizeof(ObjectData), &bytesRead, NULL))
				{
					MSG_BOX("읽기 오류");
				};

				if(bytesRead ==0)
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
				
				m_vecObjectList.push_back(objData->objectName);	
				
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
		else {
			MSG_BOX("파일을 열 수 없습니다.");
			//std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	MSG_BOX("SUCCESS");
	

}


_vec3 CImgui_ObjectTool::PickingOnTerrain()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표화

	_vec3 vMousePos;

	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

	// 뷰 포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	// 투영 -> 뷰 스페이스 
	_matrix matProj;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3 vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	// 뷰스페이스 -> 월드 
	_matrix matView;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView); // 여기서 현재 뷰가 업데이트가안됨
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView); // 위치 관련 이동
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);


	CMapTex* pTerrainBufferCom = dynamic_cast<CMapTex*>(Engine::Get_Component(ID_STATIC, L"Layer_Environment", L"Map", L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
	{
		MSG_BOX("pTerrainBufferCom is nullptr");
	}

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_Environment", L"Map", L"Com_Transform"));
	if (pTerrainBufferCom == nullptr)
	{
		MSG_BOX("pTerrainTransCom is nullptr");
	}


	//WorldHeartMap;
	//WorldHeartMap;
	//Proto_WorldHeartMap


	const _vec3* pTerrainTexPos = pTerrainBufferCom->Get_VtxPos(); // 여기도 문제가 생김( 다 0,0,0)을 가져오네
	_vec3* pWorldTerrainTexPos = new _vec3[VTXCNTX2 * VTXCNTZ2];

	// 터레인의 월드 매트릭스를 가져와야하는데
	_matrix matWorld;
	//Engine::CTransform* pTerrainTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", L"Terrain", L"Com_Transform"));
	pTerrainTransCom->Get_WorldMatrix(&matWorld);


	for (int i = 0; i < VTXCNTX2 * VTXCNTZ2; i++)
	{
		D3DXVec3TransformCoord(&pWorldTerrainTexPos[i], &pTerrainTexPos[i], &matWorld);
	}

	//제대로 계산되는지 ( 지금 항등행렬이 월드행렬이므로 그대로의 값이 나와야함)
	// if(*pWorldTerrainTexPos[1] == *pTerrainTexPos[1])
	//{
	//    int a = 4; 
	//}

	_ulong dwVtxId[3]{};
	_ulong dwVtexId_LeftUnder[3]{};
	_float fU, fV, fDist;

	for (_ulong i = 0; i < VTXCNTZ2 - 1; ++i)
	{
		for (_ulong j = 0; j < VTXCNTX2 - 1; ++j)
		{
			_ulong	dwIndex = i * VTXCNTX2 + j;

			// 오른쪽 위
			dwVtxId[0] = dwIndex + VTXCNTX2;
			dwVtxId[1] = dwIndex + VTXCNTX2 + 1;
			dwVtxId[2] = dwIndex + 1;




			if (D3DXIntersectTri(&pWorldTerrainTexPos[dwVtxId[1]],
				&pWorldTerrainTexPos[dwVtxId[2]],
				&pWorldTerrainTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{

				return _vec3(
					pWorldTerrainTexPos[dwVtxId[0]].x + fU * (pWorldTerrainTexPos[dwVtxId[1]].x - pWorldTerrainTexPos[dwVtxId[0]].x) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[0]].x),
					0.f, // Z 좌표는 필요에 따라 계산
					pWorldTerrainTexPos[dwVtxId[0]].z + fU * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[0]].z) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].z - pWorldTerrainTexPos[dwVtxId[0]].z)
				);


				// 해당 사각형의 중점 이니깐 삼각형 2개모여서 만든 사각형의 중점을 리턴하면될듯

				//return _vec3(pWorldTerrainTexPos[dwVtxId[1]].x + fU * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[1]].x),
				//	0.f,
				//	pWorldTerrainTexPos[dwVtxId[1]].z + fV * (pWorldTerrainTexPos[dwVtxId[0]].z - pWorldTerrainTexPos[dwVtxId[1]].z));


				//Test (사각형의 중점 좌표 리턴) 
				//return _vec3(pWorldTerrainTexPos[dwVtxId[0]].x+0.5f, 0.f, pWorldTerrainTexPos[dwVtxId[0]].z-0.5f);
			}

			// 왼쪽 아래
			dwVtxId[0] = dwIndex + VTXCNTX2;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;




			if (D3DXIntersectTri(&pWorldTerrainTexPos[dwVtxId[2]],
				&pWorldTerrainTexPos[dwVtxId[0]],
				&pWorldTerrainTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)
				return _vec3(
					pWorldTerrainTexPos[dwVtxId[0]].x + fU * (pWorldTerrainTexPos[dwVtxId[1]].x - pWorldTerrainTexPos[dwVtxId[0]].x) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[0]].x),
					0.f, // Z 좌표는 필요에 따라 계산
					pWorldTerrainTexPos[dwVtxId[0]].z + fU * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[0]].z) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].z - pWorldTerrainTexPos[dwVtxId[0]].z)
				);



				//return _vec3(pWorldTerrainTexPos[dwVtxId[2]].x + fU * (pWorldTerrainTexPos[dwVtxId[0]].x - pWorldTerrainTexPos[dwVtxId[2]].x),
				//	0.f,
				//	pWorldTerrainTexPos[dwVtxId[2]].z + fV * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[2]].z));
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);


}