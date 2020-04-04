
// SRTeam_Project_MapToolView.cpp : CSRTeam_Project_MapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SRTeam_Project_MapTool.h"
#endif

#include "SRTeam_Project_MapToolDoc.h"
#include "SRTeam_Project_MapToolView.h"

#include "MainFrm.h"
#include "MainForm.h"

#include "Component_Manager.h"
#include "Management.h"
#include "Scene_Logo.h"

#include "Timer.h"
#include "Timer_Manager.h"

#include "Frame.h"
#include "Frame_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSRTeam_Project_MapToolView

IMPLEMENT_DYNCREATE(CSRTeam_Project_MapToolView, CView)

BEGIN_MESSAGE_MAP(CSRTeam_Project_MapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CSRTeam_Project_MapToolView 생성/소멸

CSRTeam_Project_MapToolView::CSRTeam_Project_MapToolView()
	:m_pGraphic_Device(Engine::CGraphic_Device::GetInstance())
	, m_pGraphicDev(nullptr)
	, m_pComponent_Manager(Engine::CComponent_Manager::GetInstance())
	, m_pManagement(Engine::CManagement::GetInstance())
	, m_pCurrentScene(nullptr)
	
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pGraphic_Device->AddRef();
	m_pComponent_Manager->AddRef();
	m_pManagement->AddRef();
}

//승범의 작업
HRESULT CSRTeam_Project_MapToolView::Ready_MainApp(void)
{
	if (FAILED(Ready_DefaultSetting(g_hWnd, CGraphic_Device::MODE_WIN, BACKSIZEX, BACKSIZEY)))
		return E_FAIL;

	if (FAILED(Ready_StaticComponent()))
		return E_FAIL;

	if (FAILED(Ready_StartScene()))
		return E_FAIL;

	return NOERROR;
}

int CSRTeam_Project_MapToolView::Update_MainApp(const float & fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;


	return m_pManagement->Update_Management(fTimeDelta);
}

void CSRTeam_Project_MapToolView::Render_MainApp(void)
{
	if (nullptr == m_pGraphicDev
		|| nullptr == m_pManagement)
		return;

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphicDev->BeginScene();

	// 그려야할객체들을 그린다.
	m_pManagement->Render_Management();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}


CSRTeam_Project_MapToolView::~CSRTeam_Project_MapToolView()
{
}

//
BOOL CSRTeam_Project_MapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSRTeam_Project_MapToolView 그리기

//승범
HRESULT CSRTeam_Project_MapToolView::Ready_DefaultSetting(HWND hWnd, Engine::CGraphic_Device::WINMODE eMode, const _uint & iBackCX, const _uint & iBackCY)
{
	if (nullptr == m_pGraphic_Device
		|| nullptr == m_pManagement)
		goto except;

	if (FAILED(m_pGraphic_Device->Ready_GraphicDev(&m_pGraphicDev, hWnd, eMode, iBackCX, iBackCY)))
		goto except;

	// GARA
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		goto except;

	return NOERROR;

except:
	MFC_BOX("Ready_DefaultSetting Failed");
	return E_FAIL;
}

HRESULT CSRTeam_Project_MapToolView::Ready_StaticComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		return E_FAIL;

	pComponent = Engine::CPicking::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Picking", pComponent)))
		return E_FAIL;

	//--For.Buffers
	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Buffer_RectTex", pComponent)))
		return E_FAIL;

	//--For.Renderer
	pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		return E_FAIL;
	m_pManagement->SetUp_Renderer((CRenderer*)pComponent);

	

	return NOERROR;
}

HRESULT CSRTeam_Project_MapToolView::Ready_StartScene(void)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	Engine::CScene_Tool*			pCurrentScene = nullptr;

	pCurrentScene = CScene_Logo::Create(m_pGraphicDev);
	if (nullptr == pCurrentScene)
		return E_FAIL;

	m_pCurrentScene = pCurrentScene;

	if (FAILED(m_pManagement->SetUp_CurrentScene(pCurrentScene)))
		return E_FAIL;


	return NOERROR;
}

unsigned long CSRTeam_Project_MapToolView::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pGraphic_Device);
	Engine::Safe_Release(m_pComponent_Manager);
	Engine::Safe_Release(m_pManagement); // By.Jang	

	_ulong	dwRefCnt = 0;

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		MFC_BOX("CManagement Released Failed");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		MFC_BOX("CObject_Manager Released Failed");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		MFC_BOX("CComponent_Manager Released Failed");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		MFC_BOX("CGraphic_Device Released Failed");


	return dwRefCnt;
}

void CSRTeam_Project_MapToolView::OnDraw(CDC* /*pDC*/)
{
	CSRTeam_Project_MapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Update_MainApp(0.2f);
	Render_MainApp();

	if (false == g_PopupDelayCheck)
		Invalidate(FALSE);

	//Update_MainApp(0.1f);
	//Render_MainApp();


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CSRTeam_Project_MapToolView 인쇄

BOOL CSRTeam_Project_MapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSRTeam_Project_MapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSRTeam_Project_MapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CSRTeam_Project_MapToolView 진단

#ifdef _DEBUG
void CSRTeam_Project_MapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CSRTeam_Project_MapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSRTeam_Project_MapToolDoc* CSRTeam_Project_MapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSRTeam_Project_MapToolDoc)));
	return (CSRTeam_Project_MapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CSRTeam_Project_MapToolView 메시지 처리기


void CSRTeam_Project_MapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	pMainFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow,
		0,
		0,
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	//가로 프레임의 길이를 구하자.
	float fX = float(rcWindow.right - rcMainView.right);

	//세로 프레임의 길이를 구하자.
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL,		//프레임의 크기를 계산한 상태로 출력할 위치를 결정하는 인자.
		0,
		0,
		int(BACKSIZEX + fX),
		int(BACKSIZEY + fY),
		SWP_NOZORDER
	);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

  	Ready_MainApp();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CSRTeam_Project_MapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	//Update_MainApp(0.1f);
}


void CSRTeam_Project_MapToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
	//Invalidate(FALSE);
}


BOOL CSRTeam_Project_MapToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	/*Update_MainApp(0.1f);
	Invalidate(FALSE);*/
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
	
}




void CSRTeam_Project_MapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnLButtonDown(nFlags, point);



	switch (((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetToolType())
	{
	case TOOL_CUBE:
		DCAST(CScene_Logo*, m_pCurrentScene)->Picking_Cube();
		break;
	case TOOL_MONSTER:
		DCAST(CScene_Logo*, m_pCurrentScene)->Picking_Mon();
		break;
	case TOOL_OBJECT:
		DCAST(CScene_Logo*, m_pCurrentScene)->Picking_Obj();
		break;
	case TOOL_ITEM:
		DCAST(CScene_Logo*, m_pCurrentScene)->Picking_Item();
		break;

	}
	

	Invalidate(FALSE);
}


void CSRTeam_Project_MapToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnRButtonDown(nFlags, point);

	const _vec3 pPos = DCAST(CScene_Logo*, m_pCurrentScene)->GetPickingPos();

	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->SetMousePos(pPos);

	Invalidate(FALSE);
}
