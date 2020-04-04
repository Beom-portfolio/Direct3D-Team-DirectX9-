// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"

#include "../../Reference/Headers/Engine_Defines.h"
#include "../Headers/MainGame.h"

#include "Timer.h"
#include "Timer_Manager.h"

#include "Frame.h"
#include "Frame_Manager.h"

#include "Input_Device.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	CMainGame*		pMainGame = nullptr;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pMainGame = CMainGame::Create();

	if (nullptr == pMainGame)
		return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

	CTimer_Manager*		pTimer_Manager = CTimer_Manager::GetInstance();
	pTimer_Manager->AddRef();

	CTimer* pTimer = nullptr;

	// for.TimerDefault
	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return FALSE;
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default", pTimer)))
		return FALSE;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return FALSE;
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60", pTimer)))
		return FALSE;


	CFrame_Manager*	pFrame_Manager = CFrame_Manager::GetInstance();
	pFrame_Manager->AddRef();

	// For.Frame:60	
	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_60", 60.0f)))
		return FALSE;


	Engine::CInput_Device*		pInput_Device = Engine::CInput_Device::GetInstance();
	pInput_Device->AddRef();

	if (nullptr == pInput_Device)
		return FALSE;
	if (FAILED(pInput_Device->Ready_Input_Device(hInst, g_hWnd)))
		return FALSE;



    // 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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
			if (nullptr == pMainGame
				|| nullptr == pTimer_Manager
				|| nullptr == pInput_Device)
				break;



			pTimer_Manager->Compute_Timer(L"Timer_Default");

			_float fTimeDelta_Default = pTimer_Manager->Get_TimeDelta(L"Timer_Default");

			if (pFrame_Manager->Permit_Call(L"Frame_60", fTimeDelta_Default))
			{
				pInput_Device->Check_InputState();
				pTimer_Manager->Compute_Timer(L"Timer_60");

				_float fTimeDelta = pTimer_Manager->Get_TimeDelta(L"Timer_60");

				pMainGame->Update_MainGame(fTimeDelta);
				pMainGame->Render_MainGame();
			}
		}
	}

	Engine::_ulong dwRefCnt = 0;

	Engine::Safe_Release(pTimer_Manager);
	Engine::Safe_Release(pFrame_Manager);
	Engine::Safe_Release(pInput_Device);


	if (dwRefCnt = Engine::Safe_Release(pMainGame))
	{
		MSG_BOX("pMainApp Release Failed");
		return FALSE;
	}

	if (dwRefCnt = Engine::Safe_Release(pInput_Device))
	{
		MSG_BOX("pInput_Device Release Failed");
		return FALSE;
	}




    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
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
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT		rcWindow = { 0, 0, WINCX, WINCY };

   AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, hInstance, nullptr);

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
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
