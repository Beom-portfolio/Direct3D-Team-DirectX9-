// Item_Tool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "Item_Tool.h"
#include "afxdialogex.h"

#include "VIBuffer.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Scene_Logo.h"
#include "Back_Item.h"

// CItem_Tool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItem_Tool, CDialog)

_int CItem_Tool::m_iItemNum = 0;

CItem_Tool::CItem_Tool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ITEM, pParent)
	, m_fScaleX(0.1f)
	, m_fScaleY(0.1f)
	, m_fScaleZ(0.1f)
	, m_fHeight(0.6f)
	, m_index(0)
	, m_iItemID(0)

{

}

CItem_Tool::~CItem_Tool()
{
}

void CItem_Tool::AddItemComboBox(void)
{
	for (size_t i = 0; i < iItemIndex; ++i)
	{
		CString tempObjName;
		tempObjName.Format(L"Item_%d", i);
		m_comboItemList->AddString(tempObjName);
	}
}

void CItem_Tool::AddItemStaticPictures(void)
{
	UpdateData(TRUE);

	size_t iFileCount = iItemIndex;

	TCHAR szFullPath[255] = L"";

	for (size_t i = 0; i < iFileCount; ++i)
	{
		wsprintf(szFullPath, L"../Bin/Resources/Textures/BackLogo/Item/Item_%d.png", i);
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

	OnCbnSelchangeComboItem();
	UpdateData(FALSE);
}

void CItem_Tool::AddItem_On_ItemTool(const _vec3 * vPos)
{
	if (true == g_bItemAutoNameCheck)
	{
		AddItem_On_ItemTool_AutoName(vPos);
	}
	else
	{
		AddItem_On_ItemTool_WithName(vPos);
	}
}

void CItem_Tool::AddItem_On_ItemTool_AutoName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//자동으로 들어가는 이름
	CString strName;
	strName.Format(L"Item%d", m_iItemNum);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Item(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	strName = tempName;

	SetDlgItemText(IDC_ITEM_NAME, strName);

	m_iItemNum++;

	UpdateData(TRUE);
}

void CItem_Tool::AddItem_On_ItemTool_WithName(const _vec3 * vPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//사용자가 넣어주는 이름
	CString strName;
	GetDlgItemText(IDC_ITEM_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Item(tempName, m_fScaleX, m_fScaleY, m_fScaleZ, vPos);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CItem_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ITEM_NAME, m_strName);
	DDX_Text(pDX, IDC_ITEM_SCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_ITEM_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_ITEM_SCALEZ, m_fScaleZ);
	DDX_Text(pDX, IDC_ITEM_HEIGHT, m_fHeight);
	DDX_Control(pDX, IDC_CHECK_ITEMAUTO, m_CheckBox_AutoName);
	DDX_CBIndex(pDX, IDC_COMBO_ITEM, m_index);
	DDX_Control(pDX, IDC_IMAGE_ITEM, m_pctrItemType);
}


BEGIN_MESSAGE_MAP(CItem_Tool, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ITEMAUTO, &CItem_Tool::OnBnClickedCheckItemauto)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, &CItem_Tool::OnCbnSelchangeComboItem)
	ON_BN_CLICKED(IDC_BUTTON_ITEMUNDO, &CItem_Tool::OnBnClickedButtonItemundo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CItem_Tool::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVEITEM, &CItem_Tool::OnBnClickedButtonSaveitem)
	ON_BN_CLICKED(IDC_BUTTON_LOADITEM, &CItem_Tool::OnBnClickedButtonLoaditem)
END_MESSAGE_MAP()


// CItem_Tool 메시지 처리기입니다.


void CItem_Tool::OnBnClickedCheckItemauto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iCheckState;
	iCheckState = m_CheckBox_AutoName.GetCheck();

	switch (iCheckState)
	{
	case BST_UNCHECKED:
		g_bItemAutoNameCheck = false;
		break;
	case BST_CHECKED:
		g_bItemAutoNameCheck = true;
		break;
	case BST_INDETERMINATE:
		break;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CItem_Tool::OnCbnSelchangeComboItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	if (m_index < 0)
		return;

	switch (m_index)
	{
	case 0:
		m_fScaleX = 0.1f;
		m_fScaleY = 0.1f;
		m_fScaleZ = 0.1f;
		m_fHeight = 0.6f;
		break;
	case 1:
		m_fScaleX = 0.2f;
		m_fScaleY = 0.3f;
		m_fScaleZ = 0.2f;
		m_fHeight = 0.7f;
		break;
	case 2:
		m_fScaleX = 0.2f;
		m_fScaleY = 0.2f;
		m_fScaleZ = 0.3f;
		m_fHeight = 0.7f;
		break;
	case 3:
		m_fScaleX = 0.1f;
		m_fScaleY = 0.1f;
		m_fScaleZ = 0.1f;
		m_fHeight = 0.6f;
		break;
	case 4:
	case 5:
	case 6:
		m_fScaleX = 0.15f;
		m_fScaleY = 0.15f;
		m_fScaleZ = 0.15f;
		m_fHeight = 0.7f;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
		m_fScaleX = 0.4f;
		m_fScaleY = 0.1f;
		m_fScaleZ = 0.1f;
		m_fHeight = 0.7f;
		break;
	}

	strName.Format(L"Item_%d", m_index);

	auto& iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_iItemID = m_index;

	m_pctrItemType.SetBitmap(*iter->second);

	UpdateData(FALSE);
}


void CItem_Tool::OnBnClickedButtonItemundo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strName;

	GetDlgItemText(IDC_ITEM_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Item(tempName);

	delete[] tempName;

	--m_iItemNum;

	_int FixNum = m_iItemNum - 1;

	if (0 > FixNum)
	{
		m_iItemNum = 0;
		FixNum = 0;
	}

	strName.Format(L"Item%d", FixNum);

	SetDlgItemText(IDC_ITEM_NAME, strName);

	Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CItem_Tool::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	GetDlgItemText(IDC_ITEM_NAME, strName);
	size_t iSize = strName.GetLength() + 1;
	_tchar* tempName = new _tchar[iSize];
	memcpy(tempName, T2W(strName.GetBuffer(0)), iSize * 2);

	DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Del_Item(tempName);

	delete[] tempName;


	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	UpdateData(TRUE);
}

BOOL CItem_Tool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_CheckBox_AutoName.SetCheck(1);

	m_comboItemList = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM);
	AddItemComboBox();
	m_comboItemList->SetCurSel(0);

	AddItemStaticPictures();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CItem_Tool::OnBnClickedButtonSaveitem()
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
		///

		ITEM_DATA tempData_Item;

		list<CGameObject*>* listTemp = Pair.second->GetGameObjList();

		//Name(따로 저장)
		lstrcpy(tempData_Item.strName, Pair.first);
		//Pos
		const _vec3 &pItemPos = DCAST(CBack_Item*, *listTemp->begin())->GetItemPos();
		tempData_Item.vPos = pItemPos;
		//Scale
		const _vec3 &pItemScale = DCAST(CBack_Item*, *listTemp->begin())->GetItemScale();
		tempData_Item.vScale = pItemScale;
		//CubeType
		const _int &iType = DCAST(CBack_Item*, *listTemp->begin())->GetItemType();
		tempData_Item.iType = iType;


		WriteFile(hFile, &tempData_Item, sizeof(ITEM_DATA), &dwByte, NULL);
	}


	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CItem_Tool::OnBnClickedButtonLoaditem()
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
		ITEM_DATA*	tempData_Item = new ITEM_DATA;

		ReadFile(hFile, tempData_Item, sizeof(ITEM_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Item);
			break;
		}

		DCAST(CScene_Logo*, CManagement::GetInstance()->GetCurrentScene())->Add_Item(
			tempData_Item->strName,
			tempData_Item->vScale.x,
			tempData_Item->vScale.y,
			tempData_Item->vScale.z,
			&tempData_Item->vPos, true, tempData_Item->iType);


		m_iItemNum++;
	}

	CloseHandle(hFile);

	g_PopupDelayCheck = false;
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
