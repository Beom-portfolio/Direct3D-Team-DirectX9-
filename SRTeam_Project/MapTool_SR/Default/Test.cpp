// Test.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "Test.h"


// CTest

IMPLEMENT_DYNCREATE(CTest, CFormView)

CTest::CTest()
	: CFormView(IDD_TEST)
{

}

CTest::~CTest()
{
}

void CTest::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTest, CFormView)
END_MESSAGE_MAP()


// CTest �����Դϴ�.

#ifdef _DEBUG
void CTest::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTest::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTest �޽��� ó�����Դϴ�.
