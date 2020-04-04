#pragma once

#include "Engine_Defines.h"

// CObject_Tool 대화 상자입니다.

class CObject_Tool : public CDialog
{
	DECLARE_DYNAMIC(CObject_Tool)

public:
	static _int m_iObjNum;
private:
	map<CString, CImage*>		m_mapPngImage;
private:
	CString m_strName;
	_float	m_fHeight;
	_int	m_index;
private:
	CComboBox* m_comboObjList;
	_int	   m_iObjID;
public:
	CButton    m_CheckBox_AutoName;
public:
	CObject_Tool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObject_Tool();
public:
	CComboBox* GetComboBox(void)
	{
		return m_comboObjList;
	}
	const _int	GetObjType(void)
	{
		return m_iObjID;
	}
	const _float   GetHeight(void)
	{
		return m_fHeight;
	}
public:
	void AddObjComboBox(void);
	void AddObjStaticPictures(void);
public:
	void AddObj_On_ObjTool(const _vec3* vPos);
	void AddObj_On_ObjTool_AutoName(const _vec3* vPos);
	void AddObj_On_ObjTool_WithName(const _vec3* vPos);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonObjundo();
	afx_msg void OnBnClickedButtonObjdel();
	afx_msg void OnBnClickedButtonObjsave();
	afx_msg void OnBnClickedButtonObjload();

	CStatic m_pctrObjType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboObj();
	afx_msg void OnBnClickedCheckObjauto();
};
