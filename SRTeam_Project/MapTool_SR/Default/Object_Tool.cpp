// Object_Tool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "Object_Tool.h"
#include "afxdialogex.h"

#include "VIBuffer.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Scene_Logo.h"
#include "Back_Object.h"

// CObject_Tool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObject_Tool, CDialog)

_int CObject_Tool::m_iObjNum = 0;

CObject_Tool::CObject_Tool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECT, pParent)
	, m_fHeight(0.9f)
	, m_index(0)
	, m_iObjID(0)
{

}

CObject_Tool::~CObject_Tool()
{
}

void CObject_Tool::AddObjComboBox(void)
{
	for (size_t i = 0; i < iObjIndex; ++i)
	{
		CString tempObjName;
		tempObjName.Format(L"Object_%d", i);
		m_comboObjList->AddString(tempObjName);
	}
}

void CObject_Tool::AddObjStaticPictures(void)
{
	UpdateData(TRUE);

	size_t iFileCount = iObjIndex;

	TCHAR szFullPath[255] = L"";

	for (size_t i = 0; i < iFileCount; ++i)
	{
		wsprintf(szFullPath, L"../Bin/Resources/Textures/BackLogo/Object/Object_%d.png", i);
		std::wstring wstrRelativePath = szFullPath;

		CString strImageName;

		strImageName = PathFindFileName(wstrRelativePath.c_str());

		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		auto& iter = m_mapPngImage.find(strImageName);

		if (iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;

			pPngImage->Load(wstrRelativePath.c_str());

			m_mapPngImage.insert(make_pair(strImageName, pPngImage));
		}
	}

	OnCbnSelchangeComboObj();
	UpdateData(FALSE);
}

void CObject_Tool::AddObj_On_ObjTool(const _vec3 * vPos)
{
	if (true == g_bObjAutoNameCheck)
	{
		AddObj_On_ObjTool_AutoName(vPos);
	}
	else
	{
		AddObj_On_ObjTool_WithName(vPos);
	}
}

void CObject_Tool::AddObj_On_ObjTool_AutoName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//자동으로 들어가는 이름
	CString strName;
	strName.Format(L"Object%d", m_iObjNum);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Obj(tempName, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	strName = tempName;

	SetDlgItemText(IDC_EDIT_OBJNAME, strName);

	m_iObjNum++;

	UpdateData(TRUE);
}

void CObject_Tool::AddObj_On_ObjTool_WithName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//사용자가 넣어주는 이름
	CString strName;
	GetDlgItemText(IDC_EDIT_OBJNAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Obj(tempName, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CObject_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_OBJNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_OBJHEIGHT, m_fHeight);
	DDX_Control(pDX, IDC_CHECK_OBJAUTO, m_CheckBox_AutoName);
	DDX_CBIndex(pDX, IDC_COMBO_OBJ, m_index);
	DDX_Control(pDX, IDC_IMAGE_OBJECT, m_pctrObjType);
}


BEGIN_MESSAGE_MAP(CObject_Tool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OBJUNDO, &CObject_Tool::OnBnClickedButtonObjundo)
	ON_BN_CLICKED(IDC_BUTTON_OBJDEL, &CObject_Tool::OnBnClickedButtonObjdel)
	ON_BN_CLICKED(IDC_BUTTON_OBJSAVE, &CObject_Tool::OnBnClickedButtonObjsave)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOAD, &CObject_Tool::OnBnClickedButtonObjload)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ, &CObject_Tool::OnCbnSelchangeComboObj)
	ON_BN_CLICKED(IDC_CHECK_OBJAUTO, &CObject_Tool::OnBnClickedCheckObjauto)
END_MESSAGE_MAP()


// CObject_Tool 메시지 처리기입니다.


void CObject_Tool::OnBnClickedButtonObjundo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strName;

	GetDlgItemText(IDC_EDIT_OBJNAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Obj(tempName);

	delete[] tempName;

	--m_iObjNum;

	_int FixNum = m_iObjNum - 1;

	if (0 > FixNum)
	{
		m_iObjNum = 0;
		FixNum = 0;
	}

	strName.Format(L"Object%d", FixNum);

	SetDlgItemText(IDC_EDIT_OBJNAME, strName);

	Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

}


void CObject_Tool::OnBnClickedButtonObjdel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	GetDlgItemText(IDC_EDIT_OBJNAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Obj(tempName);

	delete[] tempName;


	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(TRUE);
}


void CObject_Tool::OnBnClickedButtonObjsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(
		FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat",
		this
	);

	TCHAR szDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szDirectory);
	PathRemoveFileSpec(szDirectory);

	lstrcat(szDirectory, L"\\DataFiles");

	Dlg.GetOFN().lpstrInitialDir = szDirectory;

	g_PopupDelayCheck = true;

	if (IDCANCEL == Dlg.DoModal())
	{
		g_PopupDelayCheck = false;
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		return;
	}
	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	HANDLE hFile = CreateFile(
		Dlg.GetPathName(),
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	DWORD dwByte = 0;

	//Object
	const map<const _tchar*, CLayer*>* mapObj = CObject_Manager::GetInstance()->GetMapLayer();

	for (auto& Pair : mapObj[SCENE_LOGO])
	{
		if (L"Layer_Terrain" == Pair.first ||
			L"Layer_Camera" == Pair.first)
			continue;

		///
		//check 
		//Cube
		_tchar CubeCheck[255] = L"";
		size_t NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(CubeCheck, NameSize, Pair.first);
		
		for (size_t i = NameSize; i >= wcslen(L"Cube"); --i)
		{
			CubeCheck[i] = 0;
		}

		if (!wcscmp(CubeCheck, L"Cube"))
			continue;
		//

		//Item
		_tchar ItemCheck[255] = L"";
		NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(ItemCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Item"); --i)
		{
			ItemCheck[i] = 0;
		}

		if (!wcscmp(ItemCheck, L"Item"))
			continue;
		//

		//Monster
		_tchar MonCheck[255] = L"";
		NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(MonCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Monster"); --i)
		{
			MonCheck[i] = 0;
		}

		if (!wcscmp(MonCheck, L"Monster"))
			continue;
		//
		///



		OBJ_DATA tempData_Obj;

		list<CGameObject*>* listTemp = Pair.second->GetGameObjList();

		//Name(따로 저장)
		lstrcpy(tempData_Obj.strName, Pair.first);
		//Pos
		const _vec3 &pObjPos = DCAST(CBack_Object*, *listTemp->begin())->GetObjPos();
		tempData_Obj.vPos = pObjPos;
		//Scale
		const _vec3 &pObjScale = DCAST(CBack_Object*, *listTemp->begin())->GetObjScale();
		tempData_Obj.vScale = pObjScale;
		//CubeType
		const _int &iType = DCAST(CBack_Object*, *listTemp->begin())->GetObjType();
		tempData_Obj.iType = iType;
		

		WriteFile(hFile, &tempData_Obj, sizeof(OBJ_DATA), &dwByte, NULL);
	}


	CloseHandle(hFile);
	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CObject_Tool::OnBnClickedButtonObjload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog		Dlg(
		TRUE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat",
		this
	);

	g_PopupDelayCheck = true;

	if (Dlg.DoModal() == IDCANCEL)
	{
		g_PopupDelayCheck = false;
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		return;
	}

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	Dlg.m_ofn.lpstrInitialDir = L"..\\DataFiles";

	HANDLE hFile = CreateFile(
		Dlg.GetPathName(),
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	DWORD dwByte = 0;

	//Obj

	while (true)
	{
		OBJ_DATA*	tempData_Obj = new OBJ_DATA;

		ReadFile(hFile, tempData_Obj, sizeof(OBJ_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Obj);
			break;
		}

		DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Obj(tempData_Obj->strName,
			&tempData_Obj->vPos, true, tempData_Obj->iType);


		m_iObjNum++;
	}

	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


BOOL CObject_Tool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_CheckBox_AutoName.SetCheck(1);

	m_comboObjList = (CComboBox*)GetDlgItem(IDC_COMBO_OBJ);
	AddObjComboBox();
	m_comboObjList->SetCurSel(0);

	AddObjStaticPictures();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObject_Tool::OnCbnSelchangeComboObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

 	CString strName;

	//m_index = m_comboCubeList->GetCurSel();

	if (m_index < 0)
		return;

	//m_index에 따른 크기 조정을 수행
	if (11 == m_index ||
		12 == m_index ||
		13 == m_index)
	{
		m_fHeight = 1.2f;
	}
	else
	{
		m_fHeight = 0.9f;
	}

	strName.Format(L"Object_%d", m_index);

	auto& iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_iObjID = m_index;

	m_pctrObjType.SetBitmap(*iter->second);

	UpdateData(FALSE);
}


void CObject_Tool::OnBnClickedCheckObjauto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iCheckState;
	iCheckState = m_CheckBox_AutoName.GetCheck();

	switch (iCheckState)
	{
	case BST_UNCHECKED:
		g_bObjAutoNameCheck = false;
		break;
	case BST_CHECKED:
		g_bObjAutoNameCheck = true;
		break;
	case BST_INDETERMINATE:

		break;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
