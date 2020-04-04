// Cube_Tool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "Cube_Tool.h"
#include "afxdialogex.h"

#include "VIBuffer.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Cube.h"
#include "Scene_Logo.h"


// CCube_Tool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCube_Tool, CDialog)

_int CCube_Tool::m_iCubeNum = 0;

CCube_Tool::CCube_Tool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CUBE, pParent)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_fHeight(1.f)
	, m_index(0)
	, m_iCubeID(0)
{

}

CCube_Tool::~CCube_Tool()
{
}

void CCube_Tool::AddComboBox(void)
{
	for (size_t i = 0; i < iCubeIndex; ++i)
	{
		CString tempCubeName;
		tempCubeName.Format(L"Cube_%d", i);
		m_comboCubeList->AddString(tempCubeName);
	}
}

void CCube_Tool::AddStaticPictures(void)
{
	UpdateData(TRUE);

	size_t iFileCount = iCubeIndex;

	TCHAR szFullPath[255] = L"";


	for (size_t i = 0; i < iFileCount; ++i)
	{
		wsprintf(szFullPath, L"../Bin/Resources/Textures/BackLogo/Wall/Wall_%d.png", i);
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

	OnCbnSelchangeComboCube();
	UpdateData(FALSE);
}

void CCube_Tool::AddCube_On_CubeTool(const _vec3 * vPos)
{
	if (true == g_bCubeAutoNameCheck)
	{
		AddCube_On_CubeTool_AutoName(vPos);
	}
	else
	{
		AddCube_On_CubeTool_WithName(vPos);
	}
}

void CCube_Tool::AddCube_On_CubeTool_AutoName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//자동으로 들어가는 이름
	CString strName;
	strName.Format(L"Cube%d", m_iCubeNum);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Cube(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	strName = tempName;

	SetDlgItemText(IDC_EDIT_NAME_CUBE, strName);

	m_iCubeNum++;

	UpdateData(TRUE);
}

void CCube_Tool::AddCube_On_CubeTool_WithName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//사용자가 넣어주는 이름
	CString strName;
	GetDlgItemText(IDC_EDIT_NAME_CUBE, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Cube(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CCube_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME_CUBE, m_strName);
	DDX_Control(pDX, IDC_CHECK_AUTONAME, m_CheckBox_AutoName);
	DDX_Control(pDX, IDC_CUBE_IMAGE, m_pctrCubeType);
	DDX_Text(pDX, IDC_EDIT_X, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_Y, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_Z, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_fHeight);
	DDX_CBIndex(pDX, IDC_COMBO_CUBE, m_index);
}


BEGIN_MESSAGE_MAP(CCube_Tool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCube_Tool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CCube_Tool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCube_Tool::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_AUTONAME, &CCube_Tool::OnBnClickedCheckAutoname)
	ON_CBN_SELCHANGE(IDC_COMBO_CUBE, &CCube_Tool::OnCbnSelchangeComboCube)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CCube_Tool::OnBnClickedButtonUndo)
END_MESSAGE_MAP()


// CCube_Tool 메시지 처리기입니다.


void CCube_Tool::OnBnClickedButtonSave()
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

	CVIBuffer* pBufferCom_Terrain = (Engine::CVIBuffer*)Engine::CObject_Manager::
									GetInstance()->Get_Component(SCENE_LOGO, L"Layer_Terrain", 0, L"Com_Buffer");

	//Terrain
	tuple<_int, _int, _float> Tuple = pBufferCom_Terrain->Get_VertexCntXZItv();
	TERRAIN_DATA tempData_Terrain;
	tempData_Terrain.iX = get<0>(Tuple);
	tempData_Terrain.iZ = get<1>(Tuple);
	tempData_Terrain.iItv = get<2>(Tuple);

	WriteFile(hFile, &tempData_Terrain, sizeof(TERRAIN_DATA), &dwByte, NULL);


	//Cube
	const map<const _tchar*, CLayer*>* mapCube = CObject_Manager::GetInstance()->GetMapLayer();

	for (auto& Pair : mapCube[SCENE_LOGO])
	{
		if (L"Layer_Terrain" == Pair.first ||
			L"Layer_Camera" == Pair.first)
			continue;



		///
		//check
		// Obj
		_tchar ObjectCheck[255] = L"";
		size_t NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(ObjectCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Object"); --i)
		{
			ObjectCheck[i] = 0;
		}
		if (!wcscmp(ObjectCheck, L"Object"))
			continue;
		//

		//Mon
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

		CUBE_DATA tempData_Cube;

		list<CGameObject*>* listTemp = Pair.second->GetGameObjList(); 
		
		//Name(따로 저장)
		lstrcpy(tempData_Cube.strName, Pair.first);
		//Pos
		const _vec3 &pCubePos = DCAST(CCube*, *listTemp->begin())->GetCubePos();
		tempData_Cube.vPos = pCubePos;
		//Scale
		const _vec3 &pCubeScale = DCAST(CCube*, *listTemp->begin())->GetCubeScale(); 
		tempData_Cube.vScale = pCubeScale;
		//CubeType
		const _int &iType = DCAST(CCube*, *listTemp->begin())->GetCubeType();
		tempData_Cube.iType = iType;
		//DoorCheck
		if (iType == 14 || iType == 15 || iType == 16)
			tempData_Cube.bDoorCheck = true;
		else 
			tempData_Cube.bDoorCheck = false;

		WriteFile(hFile, &tempData_Cube, sizeof(CUBE_DATA), &dwByte, NULL);
	}


	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CCube_Tool::OnBnClickedButtonLoad()
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

	

	//Terrain

	TERRAIN_DATA* tempData_Terrain = new TERRAIN_DATA;

	ReadFile(hFile, tempData_Terrain, sizeof(TERRAIN_DATA), &dwByte, NULL);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Change_Terrain(tempData_Terrain->iX,
		tempData_Terrain->iZ, tempData_Terrain->iItv);
	
	
	//Cube
	while (true)
	{
		CUBE_DATA*	tempData_Cube = new CUBE_DATA;
	
		ReadFile(hFile, tempData_Cube, sizeof(CUBE_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Cube);
			break;
		}

		DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Cube(tempData_Cube->strName,
			tempData_Cube->vScale.x, tempData_Cube->vScale.y, tempData_Cube->vScale.z,
			&tempData_Cube->vPos, true, tempData_Cube->iType);


		
		m_iCubeNum++;
	}
	
	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


//큐브 삭제
void CCube_Tool::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	GetDlgItemText(IDC_EDIT_NAME_CUBE, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Cube(tempName);

	delete[] tempName;

	

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(TRUE);
}


void CCube_Tool::OnBnClickedCheckAutoname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iCheckState;
	iCheckState = m_CheckBox_AutoName.GetCheck();

	switch (iCheckState)
	{
	case BST_UNCHECKED:
		g_bCubeAutoNameCheck = false;
		break;
	case BST_CHECKED:
		g_bCubeAutoNameCheck = true;
		break;
	case BST_INDETERMINATE:
		break;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


BOOL CCube_Tool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_CheckBox_AutoName.SetCheck(1);

	m_comboCubeList = (CComboBox*)GetDlgItem(IDC_COMBO_CUBE);
	AddComboBox();
	m_comboCubeList->SetCurSel(0);

	AddStaticPictures();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCube_Tool::OnCbnSelchangeComboCube()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	//m_index = m_comboCubeList->GetCurSel();

	if (m_index < 0)
		return;

	strName.Format(L"Wall_%d", m_index);

	auto& iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_iCubeID = m_index;

	m_pctrCubeType.SetBitmap(*iter->second);

	UpdateData(FALSE);
}


void CCube_Tool::OnBnClickedButtonUndo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strName;

	GetDlgItemText(IDC_EDIT_NAME_CUBE, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Cube(tempName);

	delete[] tempName;

	--m_iCubeNum;

	_int FixNum = m_iCubeNum - 1;

	if (0 > FixNum)
	{
		m_iCubeNum = 0;
		FixNum = 0;
	}

	strName.Format(L"Cube%d", FixNum);

	SetDlgItemText(IDC_EDIT_NAME_CUBE, strName);
	
	Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
