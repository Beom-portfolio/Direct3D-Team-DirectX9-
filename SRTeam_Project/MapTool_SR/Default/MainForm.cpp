// MainForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "MainForm.h"

#include "Engine_Defines.h"

// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)


CMainForm::CMainForm()
	: CFormView(IDD_MAINFORM)
	, m_fMousePosX(0.f)
	, m_fMousePosZ(0.f)
{
	m_pDialog_Cube_Tool = nullptr;
	m_pDialog_Object_Tool = nullptr;
}

CMainForm::~CMainForm()
{
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Free();
	Engine::Safe_Delete(m_pDialog_Cube_Tool);
	Engine::Safe_Delete(m_pDialog_Object_Tool);
}

void CMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MENU, m_Tab_Tool);
	DDX_Control(pDX, IDC_CHECK_WIRE, m_CheckBox_WireFrame);
	DDX_Text(pDX, IDC_EDIT_TERRAINX, m_iTerrainX);
	DDX_Text(pDX, IDC_EDIT_TERRAINZ, m_iTerrainZ);
	DDX_Text(pDX, IDC_EDIT_MOUSEPOSX, m_fMousePosX);
	DDX_Text(pDX, IDC_EDIT_MOUSEPOSZ, m_fMousePosZ);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MENU, &CMainForm::OnTcnSelchangeTabMenu)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CMainForm::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_CHECK_WIRE, &CMainForm::OnBnClickedCheckWire)
END_MESSAGE_MAP()


// CMainForm 진단입니다.

#ifdef _DEBUG



void CMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainForm 메시지 처리기입니다.


void CMainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab_Tool.DeleteAllItems();
	m_Tab_Tool.InsertItem(0, L"Cube");
	m_Tab_Tool.InsertItem(1, L"Objects");
	m_Tab_Tool.InsertItem(2, L"Monster");
	m_Tab_Tool.InsertItem(3, L"Item");

	CRect rect;

	m_pDialog_Cube_Tool = new CCube_Tool;
	m_pDialog_Cube_Tool->Create(IDD_CUBE, &m_Tab_Tool);
	m_pDialog_Cube_Tool->GetWindowRect(&rect);
	m_pDialog_Cube_Tool->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pDialog_Cube_Tool->ShowWindow(SW_SHOW);

	m_pCubeTool = (CCube_Tool*)m_pDialog_Cube_Tool;

	m_pDialog_Object_Tool = new CObject_Tool;
	m_pDialog_Object_Tool->Create(IDD_OBJECT, &m_Tab_Tool);
	m_pDialog_Object_Tool->GetWindowRect(&rect);
	m_pDialog_Object_Tool->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pDialog_Object_Tool->ShowWindow(SW_HIDE);

	m_pObjectTool = (CObject_Tool*)m_pDialog_Object_Tool;

	m_pDialog_Monster_Tool = new CMonster_Tool;
	m_pDialog_Monster_Tool->Create(IDD_MONSTER, &m_Tab_Tool);
	m_pDialog_Monster_Tool->GetWindowRect(&rect);
	m_pDialog_Monster_Tool->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pDialog_Monster_Tool->ShowWindow(SW_HIDE);

	m_pMonsterTool = (CMonster_Tool*)m_pDialog_Monster_Tool;

	m_pDialog_Item_Tool = new CItem_Tool;
	m_pDialog_Item_Tool->Create(IDD_ITEM, &m_Tab_Tool);
	m_pDialog_Item_Tool->GetWindowRect(&rect);
	m_pDialog_Item_Tool->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pDialog_Item_Tool->ShowWindow(SW_HIDE);

	m_pItemTool = (CItem_Tool*)m_pDialog_Item_Tool;

	m_enumToolType = TOOL_CUBE;

	UpdateData(FALSE);
}

void CMainForm::SetMousePos(const _vec3 vPos)
{
	UpdateData(TRUE);

	m_fMousePosX = vPos.x;
	m_fMousePosZ = vPos.z;

	CString MPosX, MPosZ;
	MPosX.Format(L"%f", m_fMousePosX);
	MPosZ.Format(L"%f", m_fMousePosZ);

	SetDlgItemText(IDC_EDIT_MOUSEPOSX, MPosX);
	SetDlgItemText(IDC_EDIT_MOUSEPOSZ, MPosZ);

	UpdateData(TRUE);
}

void CMainForm::OnTcnSelchangeTabMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int select = m_Tab_Tool.GetCurSel();

	switch (select)
	{
	case TOOL_CUBE:
		m_enumToolType = TOOL_CUBE;
		m_pDialog_Monster_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Object_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Item_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Cube_Tool->ShowWindow(SW_SHOW);
		break;
	case TOOL_OBJECT:
		m_enumToolType = TOOL_OBJECT;
		m_pDialog_Monster_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Cube_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Item_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Object_Tool->ShowWindow(SW_SHOW);
		break;
	case TOOL_MONSTER:
		m_enumToolType = TOOL_MONSTER;
		m_pDialog_Cube_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Object_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Item_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Monster_Tool->ShowWindow(SW_SHOW);
		break;
	case TOOL_ITEM:
		m_enumToolType = TOOL_ITEM;
		m_pDialog_Cube_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Object_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Monster_Tool->ShowWindow(SW_HIDE);
		m_pDialog_Item_Tool->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;

	UpdateData(FALSE);
}


void CMainForm::OnBnClickedButtonChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Change_Terrain(m_iTerrainX, m_iTerrainZ);


	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMainForm::OnBnClickedCheckWire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iCheckState;
	iCheckState = m_CheckBox_WireFrame.GetCheck();

	switch (iCheckState)
	{
	case BST_UNCHECKED:
		g_bWireFrameCheck = false;
		break;
	case BST_CHECKED:
		g_bWireFrameCheck = true;
		break;
	case BST_INDETERMINATE:
		break;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
