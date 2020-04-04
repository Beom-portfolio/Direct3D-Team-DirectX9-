// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
HWND g_hWnd;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	CMainGame*		pMainGame = nullptr;

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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



    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
