#pragma once

#include "Engine_Defines.h"

// CMonster_Tool 대화 상자입니다.

class CMonster_Tool : public CDialog
{
	DECLARE_DYNAMIC(CMonster_Tool)

public:
	static _int m_iMonNum;
private:
	map<CString, CImage*>		m_mapPngImage;
private:
	CString m_strName;
	_float  m_fScaleX;
	_float  m_fScaleY;
	_float  m_fScaleZ;
	_float  m_fHeight;
	_int    m_index;
private:
	CComboBox*  m_comboMonList;
	_int		m_iMonID;
public:
	CButton		m_CheckBox_AutoName;
public:
	CMonster_Tool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonster_Tool();
public:
	const _int&		GetMonType(void)
	{
		return m_iMonID;
	}
	const _float&   GetHeight(void)
	{
		return m_fHeight;
	}
public:
	void AddComboBox(void);
	void AddStaticPictures(void);
public:
	void AddMon_On_MonTool(const _vec3* vPos);
	void AddMon_On_MonTool_AutoName(const _vec3* vPos);
	void AddMon_On_MonTool_WithName(const _vec3* vPos);
// 대화 상자 데이터입니다
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMonundo();
	afx_msg void OnBnClickedButtonMondel();
	afx_msg void OnBnClickedButtonMonsave();
	afx_msg void OnBnClickedButtonMonload();
	afx_msg void OnBnClickedCheckMonauto();
	afx_msg void OnCbnSelchangeComboMonster();
	virtual BOOL OnInitDialog();

	CStatic m_pctrMonType;
};
