#pragma once
#include "afxcmn.h"
#include "Cube_Tool.h"
#include "Object_Tool.h"
#include "Monster_Tool.h"
#include "Item_Tool.h"
// CMainForm 폼 뷰입니다.


class CMainForm : public CFormView
{
	DECLARE_DYNCREATE(CMainForm)

protected:
	CMainForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainForm();
public:
	CCube_Tool*						GetCubeTool(void)
	{
		return	m_pCubeTool;
	}
	CObject_Tool*					GetObjectTool(void)
	{
		return	m_pObjectTool;
	}
	CMonster_Tool*					GetMonsterTool(void)
	{
		return	m_pMonsterTool;
	}
	CItem_Tool*						GetItemTool(void)
	{
		return m_pItemTool;
	}
	TOOLTYPE						GetToolType(void)
	{
		return						m_enumToolType;
	}
private:
	CCube_Tool*						m_pCubeTool;
	CObject_Tool*					m_pObjectTool;
	CMonster_Tool*					m_pMonsterTool;
	CItem_Tool*						m_pItemTool;
private:
	TOOLTYPE						m_enumToolType;
public:
	void	SetMousePos(const _vec3 vPos);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	_int		 m_iTerrainX;
	_int		 m_iTerrainZ;
	_float		 m_fMousePosX;
	_float		 m_fMousePosZ;
public:
	CButton		 m_CheckBox_WireFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl  m_Tab_Tool;
	CDialog*  m_pDialog_Cube_Tool;
	CDialog*  m_pDialog_Object_Tool;
	CDialog*  m_pDialog_Monster_Tool;
	CDialog*  m_pDialog_Item_Tool;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTabMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedCheckWire();
};


