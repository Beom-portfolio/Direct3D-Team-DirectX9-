
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

class CMainForm;
class CSRTeam_Project_MapToolView;
class CTest;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
private:
	CSplitterWnd	m_MainSplitter;
	CSplitterWnd	m_SecondSplitter;
private:
	CSRTeam_Project_MapToolView* m_pMainView;
	CMainForm*					 m_pMainForm;
public:
	CSRTeam_Project_MapToolView* GetMainView(void)
	{
		return m_pMainView;
	}
	CMainForm*					 GetMainForm(void)
	{
		return m_pMainForm;
	}
// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CStatusBar        m_wndStatusBar;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


