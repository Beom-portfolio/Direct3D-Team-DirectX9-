// Monster_Tool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "Monster_Tool.h"
#include "afxdialogex.h"

#include "VIBuffer.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Scene_Logo.h"
#include "Monster.h"



// CMonster_Tool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonster_Tool, CDialog)

_int CMonster_Tool::m_iMonNum = 0;

CMonster_Tool::CMonster_Tool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MONSTER, pParent)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_fHeight(1.f)
	, m_index(0)
	, m_iMonID(0)
{

}

CMonster_Tool::~CMonster_Tool()
{
}

void CMonster_Tool::AddComboBox(void)
{
	for (size_t i = 0; i < iMonIndex; ++i)
	{
		CString tempCubeName;
		tempCubeName.Format(L"Monster_%d", i);
		m_comboMonList->AddString(tempCubeName);
	}
}

void CMonster_Tool::AddStaticPictures(void)
{
	UpdateData(TRUE);

	size_t iFileCount = iMonIndex;

	TCHAR szFullPath[255] = L"";

	for (size_t i = 0; i < iFileCount; ++i)
	{
		wsprintf(szFullPath, L"../Bin/Resources/Textures/BackLogo/Monster/Monster_%d.png", i);
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

	OnCbnSelchangeComboMonster();
	UpdateData(FALSE);
}

void CMonster_Tool::AddMon_On_MonTool(const _vec3 * vPos)
{
	if (true == g_bMonAutoNameCheck)
	{
		AddMon_On_MonTool_AutoName(vPos);
	}
	else
	{
		AddMon_On_MonTool_WithName(vPos);
	}
}

void CMonster_Tool::AddMon_On_MonTool_AutoName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//자동으로 들어가는 이름
	CString strName;
	strName.Format(L"Monster%d", m_iMonNum);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Mon(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	strName = tempName;

	SetDlgItemText(IDC_MON_NAME, strName);

	m_iMonNum++;

	UpdateData(TRUE);
}

void CMonster_Tool::AddMon_On_MonTool_WithName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//사용자가 넣어주는 이름
	CString strName;
	GetDlgItemText(IDC_MON_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Mon(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CMonster_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_MON_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MONSCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_MONSCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_MONSCALEZ, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT_MONHEIGHT, m_fHeight);
	DDX_Control(pDX, IDC_CHECK_MONAUTO, m_CheckBox_AutoName);
	DDX_Control(pDX, IDC_MONSTER_IMAGE, m_pctrMonType);
	DDX_CBIndex(pDX, IDC_COMBO_MONSTER, m_index);
}


BEGIN_MESSAGE_MAP(CMonster_Tool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MONUNDO, &CMonster_Tool::OnBnClickedButtonMonundo)
	ON_BN_CLICKED(IDC_BUTTON_MONDEL, &CMonster_Tool::OnBnClickedButtonMondel)
	ON_BN_CLICKED(IDC_BUTTON_MONSAVE, &CMonster_Tool::OnBnClickedButtonMonsave)
	ON_BN_CLICKED(IDC_BUTTON_MONLOAD, &CMonster_Tool::OnBnClickedButtonMonload)
	ON_BN_CLICKED(IDC_CHECK_MONAUTO, &CMonster_Tool::OnBnClickedCheckMonauto)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER, &CMonster_Tool::OnCbnSelchangeComboMonster)
END_MESSAGE_MAP()


// CMonster_Tool 메시지 처리기입니다.


void CMonster_Tool::OnBnClickedButtonMonundo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strName;

	GetDlgItemText(IDC_MON_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Mon(tempName);

	delete[] tempName;

	--m_iMonNum;

	_int FixNum = m_iMonNum - 1;

	if (0 > FixNum)
	{
		m_iMonNum = 0;
		FixNum = 0;
	}

	strName.Format(L"Monster%d", FixNum);

	SetDlgItemText(IDC_MON_NAME, strName);

	Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMonster_Tool::OnBnClickedButtonMondel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	GetDlgItemText(IDC_MON_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Mon(tempName);

	delete[] tempName;


	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(TRUE);
}


void CMonster_Tool::OnBnClickedButtonMonsave()
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

		// Obj
		_tchar ObjectCheck[255] = L"";
		NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(ObjectCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Object"); --i)
		{
			ObjectCheck[i] = 0;
		}
		if (!wcscmp(ObjectCheck, L"Object"))
			continue;
		//

		// Item
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
		///

		MON_DATA tempData_Mon;

		list<CGameObject*>* listTemp = Pair.second->GetGameObjList();

		//Name(따로 저장)
		lstrcpy(tempData_Mon.strName, Pair.first);
		//Pos
		const _vec3 &pMonPos = DCAST(CMonster*, *listTemp->begin())->GetMonPos();
		tempData_Mon.vPos = pMonPos;
		//Scale
		const _vec3 &pMonScale = DCAST(CMonster*, *listTemp->begin())->GetMonScale();
		tempData_Mon.vScale = pMonScale;
		//CubeType
		const _int &iType = DCAST(CMonster*, *listTemp->begin())->GetMonType();
		tempData_Mon.iType = iType;


		WriteFile(hFile, &tempData_Mon, sizeof(MON_DATA), &dwByte, NULL);
	}


	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMonster_Tool::OnBnClickedButtonMonload()
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

	//Mon

	while (true)
	{
		MON_DATA*	tempData_Mon = new MON_DATA;

		ReadFile(hFile, tempData_Mon, sizeof(MON_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Mon);
			break;
		}

		DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Mon(
			tempData_Mon->strName,
			tempData_Mon->vScale.x,
			tempData_Mon->vScale.y,
			tempData_Mon->vScale.z,
			&tempData_Mon->vPos, true, tempData_Mon->iType);


		m_iMonNum++;
	}

	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMonster_Tool::OnBnClickedCheckMonauto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iCheckState;
	iCheckState = m_CheckBox_AutoName.GetCheck();

	switch (iCheckState)
	{
	case BST_UNCHECKED:
		g_bMonAutoNameCheck = false;
		break;
	case BST_CHECKED:
		g_bMonAutoNameCheck = true;
		break;
	case BST_INDETERMINATE:
		break;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMonster_Tool::OnCbnSelchangeComboMonster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	//m_index = m_comboCubeList->GetCurSel();

	if (m_index < 0)
		return;

	switch (m_index)
	{
	case 0:
		m_fScaleX = 0.35f;
		m_fScaleY = 0.35f;
		m_fScaleZ = 0.35f;
		m_fHeight = 0.8f;
		break;
	case 1:
		m_fScaleX = 0.3f;
		m_fScaleY = 0.3f;
		m_fScaleZ = 0.3f;
		m_fHeight = 0.7f;
		break;
	case 2:
		m_fScaleX = 0.65f;
		m_fScaleY = 0.65f;
		m_fScaleZ = 0.65f;
		m_fHeight = 1.05f;
		break;
	case 3:
		m_fScaleX = 0.7f;
		m_fScaleY = 0.7f;
		m_fScaleZ = 0.7f;
		m_fHeight = 1.1f;
		break;
	case 4:
		m_fScaleX = 0.34f;
		m_fScaleY = 0.5f;
		m_fScaleZ = 0.4f;
		m_fHeight = 0.98f;
		break;
	case 5:
		m_fScaleX = 0.37f;
		m_fScaleY = 0.43f;
		m_fScaleZ = 0.35f;
		m_fHeight = 0.9f;
		break;
	case 6:
		m_fScaleX = 0.52f;
		m_fScaleY = 0.45f;
		m_fScaleZ = 0.43f;
		m_fHeight = 1.1f;
		break;
	case 7:
		m_fScaleX = 0.48f;
		m_fScaleY = 0.5f;
		m_fScaleZ = 0.42f;
		m_fHeight = 1.f;
		break;
	}

	strName.Format(L"Monster_%d", m_index);

	auto& iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_iMonID = m_index;

	m_pctrMonType.SetBitmap(*iter->second);

	UpdateData(FALSE);
}


BOOL CMonster_Tool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	m_comboMonList = (CComboBox*)GetDlgItem(IDC_COMBO_MONSTER);
	AddComboBox();
	m_comboMonList->SetCurSel(0);

	AddStaticPictures();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
