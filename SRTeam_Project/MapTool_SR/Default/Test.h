#pragma once



// CTest �� ���Դϴ�.

class CTest : public CFormView
{
	DECLARE_DYNCREATE(CTest)

protected:
	CTest();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CTest();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

