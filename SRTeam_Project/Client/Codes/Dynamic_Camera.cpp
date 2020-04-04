#include "stdafx.h"
#include "..\Headers\Dynamic_Camera.h"

#include "Input_Device.h"

CDynamic_Camera::CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fCameraSpeed(10.f)
{

}

HRESULT CDynamic_Camera::Ready_GameObject(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	if (FAILED(Engine::CCamera::Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		return E_FAIL;

	return NOERROR;
}

_int CDynamic_Camera::Update_GameObject(const _float & fTimeDelta)
{
	Fixed_MousePointer(WINCX >> 1, WINCY >> 1);

	Camera_Controler(fTimeDelta);

	return Engine::CCamera::Update_GameObject(fTimeDelta);
}

void CDynamic_Camera::Camera_Controler(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(_vec3));

		_vec3		vMove = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta;

		m_vEye += vMove;
		m_vAt += vMove;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(_vec3));

		_vec3		vMove = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta * -1;

		m_vEye += vMove;
		m_vAt += vMove;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		_vec3		vMove = *D3DXVec3Normalize(&vRight, &vRight) * m_fCameraSpeed * fTimeDelta;

		m_vEye += vMove;
		m_vAt += vMove;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		_vec3		vMove = *D3DXVec3Normalize(&vRight, &vRight) * m_fCameraSpeed * fTimeDelta * -1;

		m_vEye += vMove;
		m_vAt += vMove;
	}
	_long Move = 0;

	_vec3		vLook;

	memcpy(&vLook, &m_matWorld.m[2][0], sizeof(_vec3));

	if (Move = m_pInput_Device->Get_DIMouseMove(Engine::CInput_Device::DIMM_X))
	{
		_matrix		matRot;

		_vec3	vUp;

		vUp = _vec3(0.f, 1.f, 0.f);

		memcpy(&vUp, &m_matWorld.m[1][0], sizeof(_vec3));

		D3DXMatrixRotationAxis(&matRot, &vUp, Move * 0.1f * fTimeDelta);
		D3DXVec3TransformCoord(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (Move = m_pInput_Device->Get_DIMouseMove(Engine::CInput_Device::DIMM_Y))
	{
		_matrix		matRot;

		_vec3	vRight;

		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

		D3DXMatrixRotationAxis(&matRot, &vRight, Move * 0.1f * fTimeDelta);
		D3DXVec3TransformCoord(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}



}

void CDynamic_Camera::Fixed_MousePointer(const _int& iConterBackSizeX, const _int& iConterBackSizeY)
{
	POINT		pt = { iConterBackSizeX , iConterBackSizeY };
	
	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);

	ShowCursor(false);
}

CDynamic_Camera * CDynamic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CDynamic_Camera* pInstance = new CDynamic_Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
	{
		MSG_BOX("CDynamic_Camera Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CDynamic_Camera::Free()
{
	return Engine::CCamera::Free();
}
