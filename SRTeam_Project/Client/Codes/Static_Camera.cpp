#include "stdafx.h"
#include "..\Headers\Static_Camera.h"

#include "Object_Manager.h"
#include "Transform.h"

CStatic_Camera::CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
	, m_CameraOn(true)
{
}

HRESULT CStatic_Camera::Ready_GameObject(const _vec3 * vEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNaer, const _float & fFar)
{
	return Engine::CCamera::Ready_GameObject(vEye, pAt, pUp, fFovY, fAspect, fNaer, fFar);
}

_int CStatic_Camera::Update_GameObject(const _float & fTimeDelta)
{
	Fixed_MousePointer(WINCX >> 1, WINCY >> 1);
	return 0;
}

_int CStatic_Camera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	PlayerCamera();
	_int iResult = Engine::CCamera::Update_GameObject(fTimeDelta);
	return iResult;
}

void CStatic_Camera::PlayerCamera(void)
{
	/*Engine::CComponent*		pComponent = nullptr;

	const _vec3*		vPlayerVec;

	_vec3		vPos, vLook;

	pComponent = Engine::CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE1, L"Layer_Player", 0, L"Transform_com");

	vPlayerVec = dynamic_cast<CTransform*>(pComponent)->Get_Infomation(Engine::CTransform::INFO_POSITION);
	vPos = *vPlayerVec;

	m_vAt = vPos;


	vPlayerVec = dynamic_cast<CTransform*>(pComponent)->Get_Infomation(Engine::CTransform::INFO_LOOK);
	vLook = *vPlayerVec;

	m_vEye = vLook;
	D3DXVec3Normalize(&m_vEye, &m_vEye);
	m_vEye = vPos - m_vEye * 0.4f;*/

	CComponent* pPlayerCom = CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");

	const _vec3* vPlayerPos = dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);
	m_vAt = *vPlayerPos;

	const _vec3* vPlayerLook = dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_LOOK);
	m_vEye = *vPlayerLook;
	D3DXVec3Normalize(&m_vEye, &m_vEye);
	m_vEye = *vPlayerPos - m_vEye * 0.3f;

}

void CStatic_Camera::Fixed_MousePointer(const _int & iConterBackSizeX, const _int & iConterBackSizeY)
{
	POINT		pt = { iConterBackSizeX , iConterBackSizeY };

	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);

	ShowCursor(false);
}



CStatic_Camera * CStatic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CStatic_Camera* pInstance = new CStatic_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
	{
		MSG_BOX("CStatic_Camera Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CStatic_Camera::Free()
{
	return Engine::CCamera::Free();
}
