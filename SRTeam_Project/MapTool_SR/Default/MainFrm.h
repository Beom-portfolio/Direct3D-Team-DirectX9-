
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainForm;
class CSRTeam_Project_MapToolView;
class CTest;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
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
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


