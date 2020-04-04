#pragma once


#include "GameObject.h"

// Client상에 필요한 뷰스페이스 변환 and 투영 변환에대한 관리를 수행한다.

BEGIN(Engine)

class CInput_Device;
class DLL_EXPORT CCamera abstract : public CGameObject
{

public:
	enum CAMERA_TYPE { CAMERA_DYNAMIC, CAMERA_STATIC, CAMERA_END };

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void) = default;

public:
	const _matrix* Get_TempView(void) { return &m_TempViewmat; }
public:
	virtual HRESULT Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
									, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);

public:
	void Setting_Camera(void);

protected:
	CInput_Device*		m_pInput_Device = nullptr;
	CAMERA_TYPE			m_Camera_Type = CAMERA_END;
protected:
	_matrix				m_matWorld;
protected:	
	_matrix				m_matView;
	_vec3				m_vEye;
	_vec3				m_vAt;
	_vec3				m_vFixedUp;
protected:
	_matrix				m_matProj;
	_float				m_fFovY;
	_float				m_fAspect;
	_float				m_fNear;
	_float				m_fFar;

protected:
	_matrix				m_TempViewmat;

private:
	void SetUp_ViewMatrix(_matrix* pMatrix);
	void SetUp_ProjMatrix(_matrix* pMatrix);
protected:
	virtual _ulong Free(void);
};

END