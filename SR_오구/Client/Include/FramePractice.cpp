// FramePractice.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

// IMGUI 
#include "pch.h"
#include "framework.h"
#include "FramePractice.h"
#include "../Header/MainApp.h"
#include "ImguiManger.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND  g_hWnd;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Imgui가 메세지를 받을 수 있게 전역변수 추가 

 
// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
   // 누수체크용 
   // _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
  
    // TODO: 여기에 코드를 입력합니다.
   

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAMEPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAMEPRACTICE));

    MSG msg;
    msg.message = WM_NULL;

    CMainApp* pMainApp = CMainApp::Create();


    CImGuiManger::GetInstance()->init();

    if (nullptr == pMainApp)
        return FALSE;


    FAILED_CHECK_RETURN(Engine::Ready_Timer(L"Timer_Immediate"), FALSE);
    FAILED_CHECK_RETURN(Engine::Ready_Timer(L"Timer_FPS60"), FALSE);

    FAILED_CHECK_RETURN(Engine::Ready_Frame(L"Frame60", 60.f), FALSE);

    
    //ImGui::CreateContext(); 
    //ImGuiIO& io = ImGui::GetIO(); (void)io;     
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls   
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls    


   
    // Main loop
    bool done = false;

    // 기본 메시지 루프입니다:
    while(true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }


        else
        {
            // ImGui 프레임 시작

         

            Engine::Set_TimeDelta(L"Timer_Immediate");
            _float	fTimer_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

            if (Engine::IsPermit_Call(L"Frame60", fTimer_Immediate))
            {
                Engine::Set_TimeDelta(L"Timer_FPS60");
                _float	fTimer_FPS60 = Engine::Get_TimeDelta(L"Timer_FPS60");


                pMainApp->Update_MainApp(fTimer_FPS60);
                pMainApp->LateUpdate_MainApp(fTimer_FPS60);
                // ImGui UI 작성

                pMainApp->Render_MainApp();

                
            }
            
        }

        

        //// ImGui 초기화
        //ImGui::CreateContext();
        //ImGui_ImplWin32_Init(g_hWnd);  // 윈도우 핸들을 넘겨야 함
        //ImGui_ImplDX9_Init(CGraphicDev::GetInstance()->Get_GraphicDev());  // Direct3D 장치를 넘겨야 함
        //
        //ImGui_ImplDX9_NewFrame();
        //ImGui_ImplWin32_NewFrame();
        //ImGui::NewFrame();
        //
        //ImGui::Begin("Hello, World!");
        //ImGui::Text("This is some text");
        //ImGui::End();
        //
        //ImGui::Render();
        //ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    
    }   
   

    _ulong	dwRefCnt(0);

    if (dwRefCnt = Safe_Release(pMainApp))
    {
        MSG_BOX("MainApp Release Failed");
        return FALSE;
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRAMEPRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT	rc{ 0, 0, WINCX, WINCY };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0,
       rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(g_hWnd);
            break;
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        break;

   
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
