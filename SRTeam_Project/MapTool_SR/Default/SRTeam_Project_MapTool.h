
// SRTeam_Project_MapTool.h : SRTeam_Project_MapTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h" // 주 기호입니다.
#include "MainFrm.h"
#include "SRTeam_Project_MapToolView.h"

#include "Management.h"
#include "Scene_Logo.h"

// CSRTeam_Project_MapToolApp:
// 이 클래스의 구현에 대해서는 SRTeam_Project_MapTool.cpp을 참조하십시오.
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
// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CSRTeam_Project_MapToolApp theApp;
