
// SRTeam_Project_MapToolView.h : CSRTeam_Project_MapToolView Ŭ������ �������̽�
//

#pragma once

#include "Engine_Defines.h"
#include "Base.h"
#include "Graphic_Device.h"
#include "Picking.h"


namespace Engine
{
	class CComponent_Manager;
	class CManagement;
	class CScene_Tool;
}

class CSRTeam_Project_MapToolDoc;
class CSRTeam_Project_MapToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSRTeam_Project_MapToolView();
	DECLARE_DYNCREATE(CSRTeam_Project_MapToolView)

// Ư���Դϴ�.
public:
	CSRTeam_Project_MapToolDoc* GetDocument() const;
	Engine::CManagement*		GetManagement(void)
	{
		return m_pManagement;
	}
// �۾��Դϴ�.
public:
	HRESULT Ready_MainApp(void);
	int		Update_MainApp(const float& fTimeDelta);
	void	Render_MainApp(void);
public:
	Engine::CGraphic_Device*	m_pGraphic_Device = nullptr;
	Engine::CComponent_Manager*	m_pComponent_Manager = nullptr;
	Engine::CManagement*		m_pManagement = nullptr;
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
private:
	Engine::CScene_Tool*		m_pCurrentScene = nullptr;
private:
	HRESULT Ready_DefaultSetting(HWND hWnd, Engine::CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);
	HRESULT Ready_StaticComponent(void);
	HRESULT Ready_StartScene(void);
public:
	unsigned long Free(void);
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CSRTeam_Project_MapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // SRTeam_Project_MapToolView.cpp�� ����� ����
inline CSRTeam_Project_MapToolDoc* CSRTeam_Project_MapToolView::GetDocument() const
   { return reinterpret_cast<CSRTeam_Project_MapToolDoc*>(m_pDocument); }
#endif

