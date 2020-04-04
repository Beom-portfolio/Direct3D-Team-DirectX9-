
// SRTeam_Project_MapTool.h : SRTeam_Project_MapTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h" // �� ��ȣ�Դϴ�.
#include "MainFrm.h"
#include "SRTeam_Project_MapToolView.h"

#include "Management.h"
#include "Scene_Logo.h"

// CSRTeam_Project_MapToolApp:
// �� Ŭ������ ������ ���ؼ��� SRTeam_Project_MapTool.cpp�� �����Ͻʽÿ�.
//

namespace Engine 
{
	class CTimer_Manager;
	class CFrame_Manager;
	class CInput_Device;
}
class CSRTeam_Project_MapToolApp : public CWinApp
{
public:
	CSRTeam_Project_MapToolApp();

private:
	CTimer_Manager*		pTimer_Manager = nullptr;
	CFrame_Manager*     pFrame_Manager = nullptr;
	CInput_Device*		pInput_Device = nullptr;
	CSRTeam_Project_MapToolView*	pMainView = nullptr;
// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CSRTeam_Project_MapToolApp theApp;
