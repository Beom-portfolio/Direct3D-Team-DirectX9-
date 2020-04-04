// Test.cpp : 구현 파일입니다.
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


// CTest 진단입니다.

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


// CTest 메시지 처리기입니다.
