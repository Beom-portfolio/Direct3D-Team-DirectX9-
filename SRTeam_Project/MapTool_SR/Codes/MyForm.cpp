// ../Codes/MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SRTeam_Project_MapTool.h"
#include "MyForm.h"
#include "afxdialogex.h"


// CMyForm 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMyForm, CDialog)

CMyForm::CMyForm(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYFORM, pParent)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyForm, CDialog)
END_MESSAGE_MAP()


// CMyForm 메시지 처리기입니다.
