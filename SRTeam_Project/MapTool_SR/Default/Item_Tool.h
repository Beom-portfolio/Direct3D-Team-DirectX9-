#pragma once

#include "Engine_Defines.h"
// CItem_Tool 대화 상자입니다.

class CItem_Tool : public CDialog
{
	DECLARE_DYNAMIC(CItem_Tool)
public:
	static _int m_iItemNum;
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
	CComboBox* m_comboItemList;
	_int	   m_iItemID;
public:
	CButton    m_CheckBox_AutoName;
public:
	CItem_Tool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItem_Tool();
public:
	const _int GetItemType(void)
	{
		return m_iItemID;
	}
	const _float GetHeight(void)
	{
		return m_fHeight;
	}
public:
	void AddItemComboBox(void);
	void AddItemStaticPictures(void);
public:
	void AddItem_On_ItemTool(const _vec3* vPos);
	void AddItem_On_ItemTool_AutoName(const _vec3* vPos);
	void AddItem_On_ItemTool_WithName(const _vec3* vPos);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckItemauto();
	afx_msg void OnCbnSelchangeComboItem();
	afx_msg void OnBnClickedButtonItemundo();
	afx_msg void OnBnClickedButtonDelete();
	CStatic m_pctrItemType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSaveitem();
	afx_msg void OnBnClickedButtonLoaditem();
};
