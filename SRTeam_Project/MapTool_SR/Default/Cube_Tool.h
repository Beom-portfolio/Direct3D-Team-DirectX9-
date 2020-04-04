#pragma once

#include "Engine_Defines.h"

// CCube_Tool ��ȭ �����Դϴ�.

class CCube_Tool : public CDialog
{
	DECLARE_DYNAMIC(CCube_Tool)

public:
	static _int	m_iCubeNum;
private:
	map<CString, CImage*>		m_mapPngImage;
private:
	CString m_strName;
	_float	m_fScaleX;
	_float	m_fScaleY;
	_float	m_fScaleZ;
	_float  m_fHeight;
	_int	m_index;
private:
	CComboBox*  m_comboCubeList;
	_int		m_iCubeID;
public:
	CButton		m_CheckBox_AutoName;
public:
	CCube_Tool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCube_Tool();
public:
	const _int&		GetCubeType(void)
	{
		return m_iCubeID;
	}
	const _float&	GetHeight(void)
	{
		return m_fHeight;
	}
public:
	void AddComboBox(void);
	void AddStaticPictures(void);
public:
	//���콺 ���� Ŭ���� Cube��� �۾��� �����Ҽ� �ֵ���
	void AddCube_On_CubeTool(const _vec3* vPos);
	void AddCube_On_CubeTool_AutoName(const _vec3* vPos);
	void AddCube_On_CubeTool_WithName(const _vec3* vPos);
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUBE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	CStatic	m_pctrCubeType;
	//��ư�� �̿��� ���
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCheckAutoname();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboCube();
	afx_msg void OnBnClickedButtonUndo();
};
